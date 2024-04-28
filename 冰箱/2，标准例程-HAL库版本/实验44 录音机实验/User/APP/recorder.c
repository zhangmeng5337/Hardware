/**
 ****************************************************************************************************
 * @file        recoder.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-17
 * @brief       ¼���� Ӧ�ô���
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211117
 * ��һ�η���
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


uint8_t *p_i2s_recbuf1;     /* I2S ���ջ���ָ��1 */
uint8_t *p_i2s_recbuf2;     /* I2S ���ջ���ָ��2 */

/**
 * REC¼��FIFO�������.
 * ����FATFS�ļ�д��ʱ��Ĳ�ȷ����,���ֱ���ڽ����ж�����д�ļ�,���ܵ���ĳ��д��ʱ�����
 * �Ӷ��������ݶ�ʧ,�ʼ���FIFO����,�Խ��������
 */
volatile uint8_t g_i2s_recfifo_rdpos = 0;           /* ¼��FIFO ��λ�� */
volatile uint8_t g_i2s_recfifo_wrpos = 0;           /* ¼��FIFO дλ�� */
uint8_t * p_i2s_recfifo_buf[REC_I2S_RX_FIFO_SIZE];  /* ¼������FIFO����ָ�� */

uint32_t g_wav_size;    /* wav���ݴ�С(�ֽ���,�������ļ�ͷ!!) */

uint8_t g_rec_sta = 0;  /**
                         * ¼��״̬
                         * [7]:0,û�п���¼��;1,�Ѿ�����¼��;
                         * [6:1]:����
                         * [0]:0,����¼��;1,��ͣ¼��;
                         */

/**
 * @brief       ��ȡһ��¼��FIFO
 * @param       buf:  ���ݻ������׵�ַ
 * @retval      0, û�����ݿɶ�;
 *              1, ������1�����ݿ�;
 */
uint8_t recoder_i2s_fifo_read(uint8_t **buf)
{
    if (g_i2s_recfifo_rdpos == g_i2s_recfifo_wrpos)     /* ��λ��  =  дλ��, ˵��û�����ݿɶ� */
    {
        return 0;
    }
    
    g_i2s_recfifo_rdpos++;          /* ��λ�ü�1 */

    if (g_i2s_recfifo_rdpos >= REC_I2S_RX_FIFO_SIZE)    /* ��λ�ó�������FIFO��, �������¿�ʼ */
    {
        g_i2s_recfifo_rdpos = 0;    /* ���� */
    }
    
    *buf = p_i2s_recfifo_buf[g_i2s_recfifo_rdpos];      /* ���ض�ӦFIFO BUF�ĵ�ַ */

    return 1;
}

/**
 * @brief       дһ��¼��FIFO
 * @param       buf:  ���ݻ������׵�ַ
 * @retval      0, д��ɹ�;
 *              1, д��ʧ��;
 */
uint8_t recoder_i2s_fifo_write(uint8_t *buf)
{
    uint16_t i;
    
    uint8_t temp = g_i2s_recfifo_wrpos; /* ��¼��ǰдλ�� */
    
    g_i2s_recfifo_wrpos++;              /* дλ�ü�1 */

    if (g_i2s_recfifo_wrpos >= REC_I2S_RX_FIFO_SIZE)    /* дλ�ó�������FIFO��, �������¿�ʼ */
    {
        g_i2s_recfifo_wrpos = 0;        /* ���� */
    }
    
    if (g_i2s_recfifo_wrpos == g_i2s_recfifo_rdpos)     /* дλ��  =  ��λ��, ˵��û��λ�ÿ�д�� */
    {
        g_i2s_recfifo_wrpos = temp;                     /* ��ԭԭ����дλ��,�˴�д��ʧ�� */
        return 1;
    }

    for (i = 0; i < REC_I2S_RX_DMA_BUF_SIZE; i++)       /* ѭ��д���� */
    {
        p_i2s_recfifo_buf[g_i2s_recfifo_wrpos][i] = buf[i];
    }

    return 0;
}

/**
 * @brief       ¼�� I2S_DMA�����жϷ�����
 *   @note      ���ж�����д������
 * @param       ��
 * @retval      ��
 */
void rec_i2s_dma_rx_callback(void)
{
    if (g_rec_sta == 0x80)  /* ¼��ģʽ */
    {
        if (I2SEXT_RX_DMASx->CR & (1 << 19))
        {
            recoder_i2s_fifo_write(p_i2s_recbuf1);  /* p_i2s_recbuf1 д��FIFO */
        }
        else
        {
            recoder_i2s_fifo_write(p_i2s_recbuf2);  /* p_i2s_recbuf2 д��FIFO */
        }
    }
}

const uint16_t I2S_PLAY_BUF[2] = {0x0000, 0x0000};  /* 2��16λ����,����¼��ʱI2S Master����.ѭ������0. */

/**
 * @brief       ����PCM ¼��ģʽ
 * @param       ��
 * @retval      ��
 */
void recoder_enter_rec_mode(void)
{
    I2S_TX_DMASx->CR &= ~(1 << 4);  /* �رմ�������ж�(���ﲻ���ж�������) ����������ﲻ�ر�dma�ͻῨ��������ݹ��̣�*/

    es8388_adda_cfg(0, 1);          /* ����ADC */
    es8388_input_cfg(0);            /* ��������ͨ��(ͨ��1,MIC����ͨ��) */
    es8388_mic_gain(8);             /* MIC��������Ϊ��� */
    es8388_alc_ctrl(3, 4, 4);       /* ����������ALC����,�����¼������ */
    es8388_output_cfg(0, 0);        /* �ر�ͨ��1��2����� */
    es8388_spkvol_set(0);           /* �ر�����. */
    es8388_i2s_cfg(0, 3);           /* �����ֱ�׼,16λ���ݳ��� */
    
    i2s_init(I2S_STANDARD_PHILIPS, I2S_MODE_MASTER_TX, I2S_CPOL_LOW, I2S_DATAFORMAT_16B);   /* �����ֱ�׼,��������,ʱ�ӵ͵�ƽ��Ч,16λ֡���� */
    i2sext_init(I2S_STANDARD_PHILIPS, I2S_MODE_SLAVE_RX, I2S_CPOL_LOW, I2S_DATAFORMAT_16B); /* �����ֱ�׼,�ӻ�����,ʱ�ӵ͵�ƽ��Ч,16λ֡���� */
    i2s_samplerate_set(REC_SAMPLERATE); /* ���ò����� */
    
    i2s_tx_dma_init((uint8_t *)&I2S_PLAY_BUF[0], (uint8_t *)&I2S_PLAY_BUF[1], 1);           /* ����TX DMA */
    I2S_TX_DMASx->CR &= ~(1 << 4);      /* �رմ�������ж�(���ﲻ���ж�������) */
    
    i2sext_rx_dma_init(p_i2s_recbuf1, p_i2s_recbuf2, REC_I2S_RX_DMA_BUF_SIZE / 2);          /* ����RX DMA */
    i2s_rx_callback = rec_i2s_dma_rx_callback;  /* �ص�����ָwav_i2s_dma_callback */

    i2s_play_start();   /* ��ʼI2S���ݷ���(����) */
    i2s_rec_start();    /* ��ʼI2S���ݽ���(�ӻ�) */

    recoder_remindmsg_show(0);
}

/**
 * @brief       ����PCM ����ģʽ
 * @param       ��
 * @retval      ��
 */
void recoder_enter_play_mode(void)
{
    es8388_adda_cfg(1, 0);      /* ����DAC */
    es8388_output_cfg(1, 1);    /* ����ͨ��1��2����� */
    es8388_spkvol_set(28);      /* ������������ */
    i2s_play_stop();            /* ֹͣʱ�ӷ��� */
    i2s_rec_stop();             /* ֹͣ¼�� */

    recoder_remindmsg_show(1);
}

/**
 * @brief       ��ʼ��WAVͷ
 * @param       ��
 * @retval      ��
 */
void recoder_wav_init(__WaveHeader *wavhead)
{
    wavhead->riff.ChunkID = 0x46464952;     /* RIFF" */
    wavhead->riff.ChunkSize = 0;            /* ��δȷ��,�����Ҫ���� */
    wavhead->riff.Format = 0x45564157;      /* "WAVE" */
    wavhead->fmt.ChunkID = 0x20746D66;      /* "fmt " */
    wavhead->fmt.ChunkSize = 16;            /* ��СΪ16���ֽ� */
    wavhead->fmt.AudioFormat = 0x01;        /* 0x01,��ʾPCM; 0x00,��ʾIMA ADPCM */
    wavhead->fmt.NumOfChannels = 2;         /* ˫���� */
    wavhead->fmt.SampleRate = REC_SAMPLERATE;               /* �������� */
    wavhead->fmt.ByteRate = wavhead->fmt.SampleRate * 4;    /* �ֽ�����=������*ͨ����*(ADCλ��/8) */
    wavhead->fmt.BlockAlign = 4;            /* ���С=ͨ����*(ADCλ��/8) */
    wavhead->fmt.BitsPerSample = 16;        /* 16λPCM */
    wavhead->data.ChunkID = 0x61746164;     /* "data" */
    wavhead->data.ChunkSize = 0;            /* ���ݴ�С,����Ҫ���� */
}

/**
 * @brief       ��ʾ¼��ʱ�������
 * @param       tsec : ʱ�䣨��λ : �룩
 * @param       kbps : ����
 * @retval      ��
 */
void recoder_msg_show(uint32_t tsec, uint32_t kbps)
{
    lcd_show_string(30, 210, 200, 16, 16, "TIME:", RED);    /* ��ʾ¼��ʱ�� */
    lcd_show_num(30 + 40, 210, tsec / 60, 2, 16, RED);      /* ���� */
    lcd_show_char(30 + 56, 210, ':', 16, 0, RED);
    lcd_show_num(30 + 64, 210, tsec % 60, 2, 16, RED);      /* ���� */

    lcd_show_string(140, 210, 200, 16, 16, "KPBS:", RED);   /* ��ʾ���� */
    lcd_show_num(140 + 40, 210, kbps / 1000, 4, 16, RED);   /* ������ʾ */
}

/**
 * @brief       ��ʾ��Ϣ
 * @param       mode : ����ģʽ
 *   @arg       0, ¼��ģʽ
 *   @arg       1, ����ģʽ
 * @retval      ��
 */
void recoder_remindmsg_show(uint8_t mode)
{
    lcd_fill(30, 120, lcddev.width, 180, WHITE);    /* ���ԭ������ʾ */

    if (mode == 0)  /* ¼��ģʽ */
    {
        lcd_show_string(30, 120, 200, 16, 16, "KEY0:REC/PAUSE", BLUE);
        lcd_show_string(30, 140, 200, 16, 16, "KEY2:STOP&SAVE", BLUE);
        lcd_show_string(30, 160, 200, 16, 16, "WK_UP:PLAY", BLUE);
    }
    else            /* ����ģʽ */
    {
        lcd_show_string(30, 120, 200, 16, 16, "KEY0:STOP Play", BLUE);
        lcd_show_string(30, 140, 200, 16, 16, "WK_UP:PLAY/PAUSE", BLUE);
    }
}

/**
 * @brief       ͨ��ʱ���ȡ�ļ���
 *   @note      ������SD������,��֧��FLASH DISK����
 *   @note      ��ϳ�:����"0:RECORDER/REC00001.wav"���ļ���
 * @param       pname : �ļ�·��
 * @retval      ��
 */
void recoder_new_pathname(char *pname)
{
    uint8_t res;
    uint16_t index = 0;
    FIL *ftemp;
    ftemp = (FIL *)mymalloc(SRAMIN, sizeof(FIL));   /* ����FIL�ֽڵ��ڴ����� */

    if (ftemp == NULL) return;  /* �ڴ�����ʧ�� */
    while (index < 0xFFFF)
    {
        sprintf((char *)pname, "0:RECORDER/REC%05d.wav", index);
        res = f_open(ftemp, (const TCHAR *)pname, FA_READ); /* ���Դ�����ļ� */
        if (res == FR_NO_FILE)
        {
            break;              /* ���ļ���������=����������Ҫ��. */
        }
        index++;
    }
    myfree(SRAMIN, ftemp);
}

/**
 * @brief       WAV¼��
 * @param       ��
 * @retval      ��
 */
void wav_recorder(void)
{
    uint8_t res, i;
    uint8_t key;
    uint8_t rval = 0;
    uint32_t bw;
    
    __WaveHeader *wavhead = 0;
    DIR recdir;             /* Ŀ¼ */
    FIL *f_rec = 0;         /* ¼���ļ� */
    
    uint8_t *pdatabuf;      /* ���ݻ���ָ�� */
    char *pname = 0;
    uint8_t timecnt = 0;    /* ��ʱ�� */
    uint32_t recsec = 0;    /* ¼��ʱ�� */

    while (f_opendir(&recdir, "0:/RECORDER"))   /* ��¼���ļ��� */
    {
        lcd_show_string(30, 230, 240, 16, 16, "RECORDER�ļ��д���!", RED);
        delay_ms(200);
        lcd_fill(30, 230, 240, 246, WHITE);     /* �����ʾ */
        delay_ms(200);
        f_mkdir("0:/RECORDER");     /* ������Ŀ¼ */
    }

    /* �����ڴ� */
    for (i = 0; i < REC_I2S_RX_FIFO_SIZE; i++)
    {
        p_i2s_recfifo_buf[i] = mymalloc(SRAMIN, REC_I2S_RX_DMA_BUF_SIZE); /* I2S¼��FIFO�ڴ����� */

        if (p_i2s_recfifo_buf[i] == NULL)
        {
            break;  /* ����ʧ�� */
        }
    }
    
    p_i2s_recbuf1 = mymalloc(SRAMIN, REC_I2S_RX_DMA_BUF_SIZE);          /* I2S¼���ڴ�1���� */
    p_i2s_recbuf2 = mymalloc(SRAMIN, REC_I2S_RX_DMA_BUF_SIZE);          /* I2S¼���ڴ�2���� */ 
    f_rec = (FIL *)mymalloc(SRAMIN, sizeof(FIL));                       /* ����FIL�ֽڵ��ڴ����� */
    
    wavhead = (__WaveHeader *)mymalloc(SRAMIN, sizeof(__WaveHeader));   /* ����__WaveHeader�ֽڵ��ڴ����� */
    pname = mymalloc(SRAMIN, 30);   /* ����30���ֽ��ڴ�,�ļ�������"0:RECORDER/REC00001.wav" */

    if (!p_i2s_recbuf2 || !f_rec || !wavhead || !pname)rval = 1;

    if(rval==0)
    {
        recoder_enter_rec_mode();   /* ����¼��ģʽ,��ʱ��������������ͷ�ɼ�������Ƶ */
        pname[0]=0;                 /* pnameû���κ��ļ��� */

        while (rval == 0)
        {
            key = key_scan(0);

            switch (key)
            {
                case KEY2_PRES: /* STOP&SAVE */
                    if (g_rec_sta & 0x80)   /* ��¼�� */
                    {
                        g_rec_sta = 0;      /* �ر�¼�� */
                        wavhead->riff.ChunkSize = g_wav_size + 36; /* �����ļ��Ĵ�С-8; */
                        wavhead->data.ChunkSize = g_wav_size;      /* ���ݴ�С */
                        f_lseek(f_rec, 0);  /* ƫ�Ƶ��ļ�ͷ. */
                        f_write(f_rec, (const void *)wavhead, sizeof(__WaveHeader), &bw); /* д��ͷ���� */
                        f_close(f_rec);
                        g_wav_size = 0;
                    }

                    g_rec_sta = 0;
                    recsec = 0;
                    LED1(1);    /* �ر�DS1 */
                    lcd_fill(30, 190, lcddev.width, lcddev.height, WHITE); /* �����ʾ,���֮ǰ��ʾ��¼���ļ��� */
                    break;
 
                case KEY0_PRES: /* REC/PAUSE */
                    if (g_rec_sta & 0x01)         /* �������ͣ,����¼�� */
                    {
                        g_rec_sta &= 0xFE;        /* ȡ����ͣ */
                    }
                    else if (g_rec_sta & 0x80)    /* �Ѿ���¼����,��ͣ */
                    {
                        g_rec_sta |= 0x01;        /* ��ͣ */
                    }
                    else    /* ��û��ʼ¼�� */
                    {
                        recsec = 0;
                        recoder_new_pathname(pname);    /* �õ��µ����� */
                        text_show_string(30, 190, lcddev.width, 16, "¼��:", 16, 0, RED);
                        text_show_string(30 + 40, 190, lcddev.width, 16, pname + 11, 16, 0, RED);   /* ��ʾ��ǰ¼���ļ����� */
                        recoder_wav_init(wavhead);      /* ��ʼ��wav���� */
                        res = f_open(f_rec, (const TCHAR *)pname, FA_CREATE_ALWAYS | FA_WRITE);

                        if (res)            /* �ļ�����ʧ�� */
                        {
                            g_rec_sta = 0;  /* �����ļ�ʧ��,����¼�� */
                            rval = 0xFE;    /* ��ʾ�Ƿ����SD�� */
                        }
                        else
                        {
                            res = f_write(f_rec, (const void *)wavhead, sizeof(__WaveHeader), &bw); /* д��ͷ���� */
                            recoder_msg_show(0, 0);
                            g_rec_sta |= 0x80;    /* ��ʼ¼�� */
                        }
                    }

                    if (g_rec_sta & 0x01)
                    {
                        LED1(0);    /* ��ʾ������ͣ */
                    }
                    else 
                    {
                        LED1(1);
                    }
                    break;

                case WKUP_PRES:     /* �������һ��¼�� */
                    if (g_rec_sta != 0x80)  /* û����¼�� */
                    {
                        if (pname[0])       /* �������������,��pname��Ϊ�� */
                        {
                            text_show_string(30, 190, lcddev.width, 16, "����:", 16, 0, RED);
                            text_show_string(30 + 40, 190, lcddev.width, 16, (char*)pname + 11, 16, 0, RED); /* ��ʾ�����ŵ��ļ����� */
                            recoder_enter_play_mode();  /* ���벥��ģʽ */
                            audio_play_song(pname);     /* ����pname */
                            lcd_fill(30, 190, lcddev.width, lcddev.height, WHITE); /* �����ʾ,���֮ǰ��ʾ��¼���ļ��� */
                            recoder_enter_rec_mode();   /* ���½���¼��ģʽ */
                        }
                    }
                    break;
            }

            if (recoder_i2s_fifo_read(&pdatabuf))   /* ��ȡһ������, ����������, д���ļ� */
            {
                res = f_write(f_rec, pdatabuf, REC_I2S_RX_DMA_BUF_SIZE, &bw); /* д���ļ� */

                if (res)
                {
                    printf("write error:%d\r\n", res);
                }

                g_wav_size += REC_I2S_RX_DMA_BUF_SIZE;  /* WAV���ݴ�С���� */
            }
            else 
            {
                delay_ms(5);
            }
            
            timecnt++;

            if ((timecnt % 20) == 0)
            {
                LED0_TOGGLE();      /* LED0��˸ */
            }

            if (recsec != (g_wav_size / wavhead->fmt.ByteRate))    /* ¼��ʱ����ʾ */
            {
                LED1_TOGGLE();      /* LED1��˸ */
                recsec = g_wav_size / wavhead->fmt.ByteRate;       /* ¼��ʱ�� */
                recoder_msg_show(recsec, wavhead->fmt.SampleRate * wavhead->fmt.NumOfChannels * wavhead->fmt.BitsPerSample); /* ��ʾ���� */
            }
        }
    }
    
    for (i = 0; i < REC_I2S_RX_FIFO_SIZE; i++)
    {
        myfree(SRAMIN, p_i2s_recfifo_buf[i]);   /* ¼��FIFO�ڴ��ͷ� */
    }
    
    myfree(SRAMIN, p_i2s_recbuf1);  /* �ͷ��ڴ� */
    myfree(SRAMIN, p_i2s_recbuf2);  /* �ͷ��ڴ� */
    myfree(SRAMIN, f_rec);          /* �ͷ��ڴ� */
    myfree(SRAMIN, wavhead);        /* �ͷ��ڴ� */
    myfree(SRAMIN, pname);          /* �ͷ��ڴ� */
}





