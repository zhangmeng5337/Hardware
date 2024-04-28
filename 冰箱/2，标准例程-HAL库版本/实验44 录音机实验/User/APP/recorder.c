/**
 ****************************************************************************************************
 * @file        recoder.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-17
 * @brief       录音机 应用代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211117
 * 第一次发布
 ****************************************************************************************************
 */

#include "./APP/recorder.h"
#include "./APP/audioplay.h"
#include "./FATFS/source/ff.h"
#include "./MALLOC/malloc.h"
#include "./SYSTEM/USART/usart.h"
#include "./BSP/ES8388/es8388.h"
#include "./BSP/I2S/i2s.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/KEY/key.h"
#include "./FATFS/exfuns/exfuns.h"
#include "./TEXT/text.h"
#include "./FATFS/source/ffconf.h"
#include "./FATFS/exfuns/fattester.h"
#include "string.h"


uint8_t *p_i2s_recbuf1;     /* I2S 接收缓冲指针1 */
uint8_t *p_i2s_recbuf2;     /* I2S 接收缓冲指针2 */

/**
 * REC录音FIFO管理参数.
 * 由于FATFS文件写入时间的不确定性,如果直接在接收中断里面写文件,可能导致某次写入时间过长
 * 从而引起数据丢失,故加入FIFO控制,以解决此问题
 */
volatile uint8_t g_i2s_recfifo_rdpos = 0;           /* 录音FIFO 读位置 */
volatile uint8_t g_i2s_recfifo_wrpos = 0;           /* 录音FIFO 写位置 */
uint8_t * p_i2s_recfifo_buf[REC_I2S_RX_FIFO_SIZE];  /* 录音接收FIFO数组指针 */

uint32_t g_wav_size;    /* wav数据大小(字节数,不包括文件头!!) */

uint8_t g_rec_sta = 0;  /**
                         * 录音状态
                         * [7]:0,没有开启录音;1,已经开启录音;
                         * [6:1]:保留
                         * [0]:0,正在录音;1,暂停录音;
                         */

/**
 * @brief       读取一个录音FIFO
 * @param       buf:  数据缓存区首地址
 * @retval      0, 没有数据可读;
 *              1, 读到了1个数据块;
 */
uint8_t recoder_i2s_fifo_read(uint8_t **buf)
{
    if (g_i2s_recfifo_rdpos == g_i2s_recfifo_wrpos)     /* 读位置  =  写位置, 说明没得数据可读 */
    {
        return 0;
    }
    
    g_i2s_recfifo_rdpos++;          /* 读位置加1 */

    if (g_i2s_recfifo_rdpos >= REC_I2S_RX_FIFO_SIZE)    /* 读位置超过了总FIFO数, 归零重新开始 */
    {
        g_i2s_recfifo_rdpos = 0;    /* 归零 */
    }
    
    *buf = p_i2s_recfifo_buf[g_i2s_recfifo_rdpos];      /* 返回对应FIFO BUF的地址 */

    return 1;
}

/**
 * @brief       写一个录音FIFO
 * @param       buf:  数据缓存区首地址
 * @retval      0, 写入成功;
 *              1, 写入失败;
 */
uint8_t recoder_i2s_fifo_write(uint8_t *buf)
{
    uint16_t i;
    
    uint8_t temp = g_i2s_recfifo_wrpos; /* 记录当前写位置 */
    
    g_i2s_recfifo_wrpos++;              /* 写位置加1 */

    if (g_i2s_recfifo_wrpos >= REC_I2S_RX_FIFO_SIZE)    /* 写位置超过了总FIFO数, 归零重新开始 */
    {
        g_i2s_recfifo_wrpos = 0;        /* 归零 */
    }
    
    if (g_i2s_recfifo_wrpos == g_i2s_recfifo_rdpos)     /* 写位置  =  读位置, 说明没得位置可写了 */
    {
        g_i2s_recfifo_wrpos = temp;                     /* 还原原来的写位置,此次写入失败 */
        return 1;
    }

    for (i = 0; i < REC_I2S_RX_DMA_BUF_SIZE; i++)       /* 循环写数据 */
    {
        p_i2s_recfifo_buf[g_i2s_recfifo_wrpos][i] = buf[i];
    }

    return 0;
}

/**
 * @brief       录音 I2S_DMA接收中断服务函数
 *   @note      在中断里面写入数据
 * @param       无
 * @retval      无
 */
void rec_i2s_dma_rx_callback(void)
{
    if (g_rec_sta == 0x80)  /* 录音模式 */
    {
        if (I2SEXT_RX_DMASx->CR & (1 << 19))
        {
            recoder_i2s_fifo_write(p_i2s_recbuf1);  /* p_i2s_recbuf1 写入FIFO */
        }
        else
        {
            recoder_i2s_fifo_write(p_i2s_recbuf2);  /* p_i2s_recbuf2 写入FIFO */
        }
    }
}

const uint16_t I2S_PLAY_BUF[2] = {0x0000, 0x0000};  /* 2个16位数据,用于录音时I2S Master发送.循环发送0. */

/**
 * @brief       进入PCM 录音模式
 * @param       无
 * @retval      无
 */
void recoder_enter_rec_mode(void)
{
    I2S_TX_DMASx->CR &= ~(1 << 4);  /* 关闭传输完成中断(这里不用中断送数据) （如果在这里不关闭dma就会卡在清空数据过程）*/

    es8388_adda_cfg(0, 1);          /* 开启ADC */
    es8388_input_cfg(0);            /* 开启输入通道(通道1,MIC所在通道) */
    es8388_mic_gain(8);             /* MIC增益设置为最大 */
    es8388_alc_ctrl(3, 4, 4);       /* 开启立体声ALC控制,以提高录音音量 */
    es8388_output_cfg(0, 0);        /* 关闭通道1和2的输出 */
    es8388_spkvol_set(0);           /* 关闭喇叭. */
    es8388_i2s_cfg(0, 3);           /* 飞利浦标准,16位数据长度 */
    
    i2s_init(I2S_STANDARD_PHILIPS, I2S_MODE_MASTER_TX, I2S_CPOL_LOW, I2S_DATAFORMAT_16B);   /* 飞利浦标准,主机发送,时钟低电平有效,16位帧长度 */
    i2sext_init(I2S_STANDARD_PHILIPS, I2S_MODE_SLAVE_RX, I2S_CPOL_LOW, I2S_DATAFORMAT_16B); /* 飞利浦标准,从机接收,时钟低电平有效,16位帧长度 */
    i2s_samplerate_set(REC_SAMPLERATE); /* 设置采样率 */
    
    i2s_tx_dma_init((uint8_t *)&I2S_PLAY_BUF[0], (uint8_t *)&I2S_PLAY_BUF[1], 1);           /* 配置TX DMA */
    I2S_TX_DMASx->CR &= ~(1 << 4);      /* 关闭传输完成中断(这里不用中断送数据) */
    
    i2sext_rx_dma_init(p_i2s_recbuf1, p_i2s_recbuf2, REC_I2S_RX_DMA_BUF_SIZE / 2);          /* 配置RX DMA */
    i2s_rx_callback = rec_i2s_dma_rx_callback;  /* 回调函数指wav_i2s_dma_callback */

    i2s_play_start();   /* 开始I2S数据发送(主机) */
    i2s_rec_start();    /* 开始I2S数据接收(从机) */

    recoder_remindmsg_show(0);
}

/**
 * @brief       进入PCM 放音模式
 * @param       无
 * @retval      无
 */
void recoder_enter_play_mode(void)
{
    es8388_adda_cfg(1, 0);      /* 开启DAC */
    es8388_output_cfg(1, 1);    /* 开启通道1和2的输出 */
    es8388_spkvol_set(28);      /* 喇叭音量设置 */
    i2s_play_stop();            /* 停止时钟发送 */
    i2s_rec_stop();             /* 停止录音 */

    recoder_remindmsg_show(1);
}

/**
 * @brief       初始化WAV头
 * @param       无
 * @retval      无
 */
void recoder_wav_init(__WaveHeader *wavhead)
{
    wavhead->riff.ChunkID = 0x46464952;     /* RIFF" */
    wavhead->riff.ChunkSize = 0;            /* 还未确定,最后需要计算 */
    wavhead->riff.Format = 0x45564157;      /* "WAVE" */
    wavhead->fmt.ChunkID = 0x20746D66;      /* "fmt " */
    wavhead->fmt.ChunkSize = 16;            /* 大小为16个字节 */
    wavhead->fmt.AudioFormat = 0x01;        /* 0x01,表示PCM; 0x00,表示IMA ADPCM */
    wavhead->fmt.NumOfChannels = 2;         /* 双声道 */
    wavhead->fmt.SampleRate = REC_SAMPLERATE;               /* 采样速率 */
    wavhead->fmt.ByteRate = wavhead->fmt.SampleRate * 4;    /* 字节速率=采样率*通道数*(ADC位数/8) */
    wavhead->fmt.BlockAlign = 4;            /* 块大小=通道数*(ADC位数/8) */
    wavhead->fmt.BitsPerSample = 16;        /* 16位PCM */
    wavhead->data.ChunkID = 0x61746164;     /* "data" */
    wavhead->data.ChunkSize = 0;            /* 数据大小,还需要计算 */
}

/**
 * @brief       显示录音时间和码率
 * @param       tsec : 时间（单位 : 秒）
 * @param       kbps : 码率
 * @retval      无
 */
void recoder_msg_show(uint32_t tsec, uint32_t kbps)
{
    lcd_show_string(30, 210, 200, 16, 16, "TIME:", RED);    /* 显示录音时间 */
    lcd_show_num(30 + 40, 210, tsec / 60, 2, 16, RED);      /* 分钟 */
    lcd_show_char(30 + 56, 210, ':', 16, 0, RED);
    lcd_show_num(30 + 64, 210, tsec % 60, 2, 16, RED);      /* 秒钟 */

    lcd_show_string(140, 210, 200, 16, 16, "KPBS:", RED);   /* 显示码率 */
    lcd_show_num(140 + 40, 210, kbps / 1000, 4, 16, RED);   /* 码率显示 */
}

/**
 * @brief       提示信息
 * @param       mode : 工作模式
 *   @arg       0, 录音模式
 *   @arg       1, 放音模式
 * @retval      无
 */
void recoder_remindmsg_show(uint8_t mode)
{
    lcd_fill(30, 120, lcddev.width, 180, WHITE);    /* 清除原来的显示 */

    if (mode == 0)  /* 录音模式 */
    {
        lcd_show_string(30, 120, 200, 16, 16, "KEY0:REC/PAUSE", BLUE);
        lcd_show_string(30, 140, 200, 16, 16, "KEY2:STOP&SAVE", BLUE);
        lcd_show_string(30, 160, 200, 16, 16, "WK_UP:PLAY", BLUE);
    }
    else            /* 放音模式 */
    {
        lcd_show_string(30, 120, 200, 16, 16, "KEY0:STOP Play", BLUE);
        lcd_show_string(30, 140, 200, 16, 16, "WK_UP:PLAY/PAUSE", BLUE);
    }
}

/**
 * @brief       通过时间获取文件名
 *   @note      仅限在SD卡保存,不支持FLASH DISK保存
 *   @note      组合成:形如"0:RECORDER/REC00001.wav"的文件名
 * @param       pname : 文件路径
 * @retval      无
 */
void recoder_new_pathname(char *pname)
{
    uint8_t res;
    uint16_t index = 0;
    FIL *ftemp;
    ftemp = (FIL *)mymalloc(SRAMIN, sizeof(FIL));   /* 开辟FIL字节的内存区域 */

    if (ftemp == NULL) return;  /* 内存申请失败 */
    while (index < 0xFFFF)
    {
        sprintf((char *)pname, "0:RECORDER/REC%05d.wav", index);
        res = f_open(ftemp, (const TCHAR *)pname, FA_READ); /* 尝试打开这个文件 */
        if (res == FR_NO_FILE)
        {
            break;              /* 该文件名不存在=正是我们需要的. */
        }
        index++;
    }
    myfree(SRAMIN, ftemp);
}

/**
 * @brief       WAV录音
 * @param       无
 * @retval      无
 */
void wav_recorder(void)
{
    uint8_t res, i;
    uint8_t key;
    uint8_t rval = 0;
    uint32_t bw;
    
    __WaveHeader *wavhead = 0;
    DIR recdir;             /* 目录 */
    FIL *f_rec = 0;         /* 录音文件 */
    
    uint8_t *pdatabuf;      /* 数据缓存指针 */
    char *pname = 0;
    uint8_t timecnt = 0;    /* 计时器 */
    uint32_t recsec = 0;    /* 录音时间 */

    while (f_opendir(&recdir, "0:/RECORDER"))   /* 打开录音文件夹 */
    {
        lcd_show_string(30, 230, 240, 16, 16, "RECORDER文件夹错误!", RED);
        delay_ms(200);
        lcd_fill(30, 230, 240, 246, WHITE);     /* 清除显示 */
        delay_ms(200);
        f_mkdir("0:/RECORDER");     /* 创建该目录 */
    }

    /* 申请内存 */
    for (i = 0; i < REC_I2S_RX_FIFO_SIZE; i++)
    {
        p_i2s_recfifo_buf[i] = mymalloc(SRAMIN, REC_I2S_RX_DMA_BUF_SIZE); /* I2S录音FIFO内存申请 */

        if (p_i2s_recfifo_buf[i] == NULL)
        {
            break;  /* 申请失败 */
        }
    }
    
    p_i2s_recbuf1 = mymalloc(SRAMIN, REC_I2S_RX_DMA_BUF_SIZE);          /* I2S录音内存1申请 */
    p_i2s_recbuf2 = mymalloc(SRAMIN, REC_I2S_RX_DMA_BUF_SIZE);          /* I2S录音内存2申请 */ 
    f_rec = (FIL *)mymalloc(SRAMIN, sizeof(FIL));                       /* 开辟FIL字节的内存区域 */
    
    wavhead = (__WaveHeader *)mymalloc(SRAMIN, sizeof(__WaveHeader));   /* 开辟__WaveHeader字节的内存区域 */
    pname = mymalloc(SRAMIN, 30);   /* 申请30个字节内存,文件名类似"0:RECORDER/REC00001.wav" */

    if (!p_i2s_recbuf2 || !f_rec || !wavhead || !pname)rval = 1;

    if(rval==0)
    {
        recoder_enter_rec_mode();   /* 进入录音模式,此时耳机可以听到咪头采集到的音频 */
        pname[0]=0;                 /* pname没有任何文件名 */

        while (rval == 0)
        {
            key = key_scan(0);

            switch (key)
            {
                case KEY2_PRES: /* STOP&SAVE */
                    if (g_rec_sta & 0x80)   /* 有录音 */
                    {
                        g_rec_sta = 0;      /* 关闭录音 */
                        wavhead->riff.ChunkSize = g_wav_size + 36; /* 整个文件的大小-8; */
                        wavhead->data.ChunkSize = g_wav_size;      /* 数据大小 */
                        f_lseek(f_rec, 0);  /* 偏移到文件头. */
                        f_write(f_rec, (const void *)wavhead, sizeof(__WaveHeader), &bw); /* 写入头数据 */
                        f_close(f_rec);
                        g_wav_size = 0;
                    }

                    g_rec_sta = 0;
                    recsec = 0;
                    LED1(1);    /* 关闭DS1 */
                    lcd_fill(30, 190, lcddev.width, lcddev.height, WHITE); /* 清除显示,清除之前显示的录音文件名 */
                    break;
 
                case KEY0_PRES: /* REC/PAUSE */
                    if (g_rec_sta & 0x01)         /* 如果是暂停,继续录音 */
                    {
                        g_rec_sta &= 0xFE;        /* 取消暂停 */
                    }
                    else if (g_rec_sta & 0x80)    /* 已经在录音了,暂停 */
                    {
                        g_rec_sta |= 0x01;        /* 暂停 */
                    }
                    else    /* 还没开始录音 */
                    {
                        recsec = 0;
                        recoder_new_pathname(pname);    /* 得到新的名字 */
                        text_show_string(30, 190, lcddev.width, 16, "录制:", 16, 0, RED);
                        text_show_string(30 + 40, 190, lcddev.width, 16, pname + 11, 16, 0, RED);   /* 显示当前录音文件名字 */
                        recoder_wav_init(wavhead);      /* 初始化wav数据 */
                        res = f_open(f_rec, (const TCHAR *)pname, FA_CREATE_ALWAYS | FA_WRITE);

                        if (res)            /* 文件创建失败 */
                        {
                            g_rec_sta = 0;  /* 创建文件失败,不能录音 */
                            rval = 0xFE;    /* 提示是否存在SD卡 */
                        }
                        else
                        {
                            res = f_write(f_rec, (const void *)wavhead, sizeof(__WaveHeader), &bw); /* 写入头数据 */
                            recoder_msg_show(0, 0);
                            g_rec_sta |= 0x80;    /* 开始录音 */
                        }
                    }

                    if (g_rec_sta & 0x01)
                    {
                        LED1(0);    /* 提示正在暂停 */
                    }
                    else 
                    {
                        LED1(1);
                    }
                    break;

                case WKUP_PRES:     /* 播放最近一段录音 */
                    if (g_rec_sta != 0x80)  /* 没有在录音 */
                    {
                        if (pname[0])       /* 如果按键被按下,且pname不为空 */
                        {
                            text_show_string(30, 190, lcddev.width, 16, "播放:", 16, 0, RED);
                            text_show_string(30 + 40, 190, lcddev.width, 16, (char*)pname + 11, 16, 0, RED); /* 显示当播放的文件名字 */
                            recoder_enter_play_mode();  /* 进入播放模式 */
                            audio_play_song(pname);     /* 播放pname */
                            lcd_fill(30, 190, lcddev.width, lcddev.height, WHITE); /* 清除显示,清除之前显示的录音文件名 */
                            recoder_enter_rec_mode();   /* 重新进入录音模式 */
                        }
                    }
                    break;
            }

            if (recoder_i2s_fifo_read(&pdatabuf))   /* 读取一次数据, 读到数据了, 写入文件 */
            {
                res = f_write(f_rec, pdatabuf, REC_I2S_RX_DMA_BUF_SIZE, &bw); /* 写入文件 */

                if (res)
                {
                    printf("write error:%d\r\n", res);
                }

                g_wav_size += REC_I2S_RX_DMA_BUF_SIZE;  /* WAV数据大小增加 */
            }
            else 
            {
                delay_ms(5);
            }
            
            timecnt++;

            if ((timecnt % 20) == 0)
            {
                LED0_TOGGLE();      /* LED0闪烁 */
            }

            if (recsec != (g_wav_size / wavhead->fmt.ByteRate))    /* 录音时间显示 */
            {
                LED1_TOGGLE();      /* LED1闪烁 */
                recsec = g_wav_size / wavhead->fmt.ByteRate;       /* 录音时间 */
                recoder_msg_show(recsec, wavhead->fmt.SampleRate * wavhead->fmt.NumOfChannels * wavhead->fmt.BitsPerSample); /* 显示码率 */
            }
        }
    }
    
    for (i = 0; i < REC_I2S_RX_FIFO_SIZE; i++)
    {
        myfree(SRAMIN, p_i2s_recfifo_buf[i]);   /* 录音FIFO内存释放 */
    }
    
    myfree(SRAMIN, p_i2s_recbuf1);  /* 释放内存 */
    myfree(SRAMIN, p_i2s_recbuf2);  /* 释放内存 */
    myfree(SRAMIN, f_rec);          /* 释放内存 */
    myfree(SRAMIN, wavhead);        /* 释放内存 */
    myfree(SRAMIN, pname);          /* 释放内存 */
}





