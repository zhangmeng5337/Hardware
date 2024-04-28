/**
 ****************************************************************************************************
 * @file        wavplay.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-16
 * @brief       wav���� ����
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
 * V1.0 20211116
 * 1,֧��16λ/24λWAV�ļ�����
 * 2,��߿���֧�ֵ�192K/24bit��WAV��ʽ.
 *
 ****************************************************************************************************
 */
 
#include "./AUDIOCODEC/wav/wavplay.h"
#include "./APP/audioplay.h"
#include "./SYSTEM/USART/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./MALLOC/malloc.h"
#include "./FATFS/source/ff.h"
#include "./BSP/I2S/i2s.h"
#include "./BSP/ES8388/es8388.h"
#include "./BSP/KEY/key.h"
#include "./BSP/LED/led.h"


__wavctrl wavctrl;                      /* WAV���ƽṹ�� */

volatile uint8_t wavtransferend = 0;    /* i2s������ɱ�־ */
volatile uint8_t wavwitchbuf = 0;       /* i2sbufxָʾ��־ */

/**
 * @brief       WAV������ʼ��
 * @param       fname : �ļ�·��+�ļ���
 * @param       wavx  : ��Ϣ��Žṹ��ָ��
 * @retval      0,���ļ��ɹ�
 *              1,���ļ�ʧ��
 *              2,��WAV�ļ�
 *              3,DATA����δ�ҵ�
 */
uint8_t wav_decode_init(char* fname, __wavctrl* wavx)
{
    FIL *ftemp;
    uint8_t *buf; 
    uint32_t br = 0;
    uint8_t res = 0;

    ChunkRIFF *riff;
    ChunkFMT *fmt;
    ChunkFACT *fact;
    ChunkDATA *data;
    
    ftemp = (FIL*)mymalloc(SRAMIN, sizeof(FIL));
    buf = mymalloc(SRAMIN, 512);
    
    if (ftemp && buf)    /* �ڴ�����ɹ� */
    {
        res = f_open(ftemp, (TCHAR*)fname, FA_READ);    /* ���ļ� */
        
        if (res == FR_OK)
        {
            f_read(ftemp, buf, 512, &br);               /* ��ȡ512�ֽ������� */
            riff = (ChunkRIFF *)buf;                    /* ��ȡRIFF�� */
            if (riff->Format == 0x45564157)             /* ��WAV�ļ� */
            {
                fmt = (ChunkFMT *)(buf + 12);           /* ��ȡFMT�� */
                fact = (ChunkFACT *)(buf + 12 + 8 + fmt->ChunkSize);                    /* ��ȡFACT�� */
                
                if (fact->ChunkID == 0x74636166 || fact->ChunkID == 0x5453494C)
                {
                    wavx->datastart = 12 + 8 + fmt->ChunkSize + 8 + fact->ChunkSize;    /* ����fact/LIST���ʱ��(δ����) */
                }
                else
                {
                    wavx->datastart = 12 + 8 + fmt->ChunkSize;
                }
                
                data = (ChunkDATA *)(buf + wavx->datastart);    /* ��ȡDATA�� */
                
                if (data->ChunkID == 0x61746164)                /* �����ɹ�! */
                {
                    wavx->audioformat = fmt->AudioFormat;       /* ��Ƶ��ʽ */
                    wavx->nchannels = fmt->NumOfChannels;       /* ͨ���� */
                    wavx->samplerate = fmt->SampleRate;         /* ������ */
                    wavx->bitrate = fmt->ByteRate * 8;          /* �õ�λ�� */
                    wavx->blockalign = fmt->BlockAlign;         /* ����� */
                    wavx->bps = fmt->BitsPerSample;             /* λ��,16/24/32λ */
                    
                    wavx->datasize = data->ChunkSize;           /* ���ݿ��С */
                    wavx->datastart = wavx->datastart + 8;      /* ��������ʼ�ĵط�. */
                     
                    printf("wavx->audioformat:%d\r\n", wavx->audioformat);
                    printf("wavx->nchannels:%d\r\n", wavx->nchannels);
                    printf("wavx->samplerate:%d\r\n", wavx->samplerate);
                    printf("wavx->bitrate:%d\r\n", wavx->bitrate);
                    printf("wavx->blockalign:%d\r\n", wavx->blockalign);
                    printf("wavx->bps:%d\r\n", wavx->bps);
                    printf("wavx->datasize:%d\r\n", wavx->datasize);
                    printf("wavx->datastart:%d\r\n", wavx->datastart);  
                }
                else
                {
                    res = 3;    /* data����δ�ҵ�. */
                }
            }
            else
            {
                res = 2;        /* ��wav�ļ� */
            }
        }
        else
        {
            res = 1;            /* ���ļ����� */
        }
    }
    
    f_close(ftemp);
    
    myfree(SRAMIN, ftemp);      /* �ͷ��ڴ� */
    myfree(SRAMIN, buf); 
    
    return 0;
}

/**
 * @brief       ���buf
 * @param       buf  : �����
 * @param       size : ���������
 * @param       bits : λ��(16/24)
 * @retval      ��ȡ�������ݳ���
 */
uint32_t wav_buffill(uint8_t *buf, uint16_t size, uint8_t bits)
{
    uint16_t readlen = 0;
    uint32_t bread;
    uint16_t i;
    uint8_t *p;
    
    if (bits == 24)                 /* 24bit��Ƶ,��Ҫ����һ�� */
    {
        readlen = (size / 4) * 3;   /* �˴�Ҫ��ȡ���ֽ��� */
        
        f_read(g_audiodev.file, g_audiodev.tbuf, readlen, (UINT*)&bread);   /* ��ȡ���� */
        p = g_audiodev.tbuf;
        
        for (i = 0; i < size;)
        {
            buf[i++] = p[1];
            buf[i] = p[2]; 
            i += 2;
            buf[i++] = p[0];
            p += 3;
        }
        
        bread = (bread * 4) / 3;    /* ����Ĵ�С. */
    }
    else
    {
        f_read(g_audiodev.file, buf, size, (UINT*)&bread);  /* 16bit��Ƶ,ֱ�Ӷ�ȡ���� */
        
        if (bread < size)           /* ����������,����0 */
        {
            for (i = bread; i < size - bread; i++)
            {
                buf[i] = 0;
            }
        }
    }
    
    return bread;
}

/**
 * @brief       WAV����ʱ,I2S DMA����ص�����
 * @param       ��
 * @retval      ��
 */
void wav_i2s_dma_tx_callback(void) 
{
    uint16_t i;

    if (DMA1_Stream4->CR & (1 << 19))
    {
        wavwitchbuf = 0;
        
        if ((g_audiodev.status & 0x01) == 0)
        {
            for (i = 0; i < WAV_I2S_TX_DMA_BUFSIZE; i++)    /* ��ͣ */
            {
                g_audiodev.i2sbuf1[i] = 0;                  /* ���0 */
            }
        }
    }
    else 
    {
        wavwitchbuf = 1;

        if ((g_audiodev.status & 0x01) == 0)
        {
            for (i = 0; i < WAV_I2S_TX_DMA_BUFSIZE; i++)    /* ��ͣ */
            {
                g_audiodev.i2sbuf2[i] = 0;                  /* ���0 */
            }
        }
    }
    
    wavtransferend = 1;
}

/**
 * @brief       ��ȡ��ǰ����ʱ��
 * @param       fname : �ļ�ָ��
 * @param       wavx  : wavx���ſ�����
 * @retval      ��
 */
void wav_get_curtime(FIL *fx, __wavctrl *wavx)
{
    long long fpos;

    wavx->totsec = wavx->datasize / (wavx->bitrate / 8);    /* �����ܳ���(��λ:��) */
    fpos = fx->fptr-wavx->datastart;                        /* �õ���ǰ�ļ����ŵ��ĵط� */
    wavx->cursec = fpos * wavx->totsec / wavx->datasize;    /* ��ǰ���ŵ��ڶ�������? */
}

/**
 * @brief       ����ĳ��wav�ļ�
 * @param       fname : �ļ�·��+�ļ���
 * @retval      KEY0_PRES,����
 *              KEY1_PRES,���ļ�ʧ��
 *              ����,��WAV�ļ�
 */
uint8_t wav_play_song(char* fname)
{
    uint8_t key;
    uint8_t t = 0; 
    uint8_t res;  
    uint32_t fillnum; 
    
    g_audiodev.file = (FIL*)mymalloc(SRAMIN, sizeof(FIL));
    g_audiodev.i2sbuf1 = mymalloc(SRAMIN, WAV_I2S_TX_DMA_BUFSIZE);
    g_audiodev.i2sbuf2 = mymalloc(SRAMIN, WAV_I2S_TX_DMA_BUFSIZE);
    g_audiodev.tbuf = mymalloc(SRAMIN, WAV_I2S_TX_DMA_BUFSIZE);
    
    if (g_audiodev.file && g_audiodev.i2sbuf1 && g_audiodev.i2sbuf2 && g_audiodev.tbuf)
    { 
        res = wav_decode_init(fname, &wavctrl); /* �õ��ļ�����Ϣ */
        
        if (res == 0)   /* �����ļ��ɹ� */
        {
            if (wavctrl.bps == 16)
            {
                es8388_i2s_cfg(0, 3);   /* �����ֱ�׼,16λ���ݳ��� */
                i2s_init(I2S_STANDARD_PHILIPS, I2S_MODE_MASTER_TX, I2S_CPOL_LOW, I2S_DATAFORMAT_16B_EXTENDED); /* �����ֱ�׼,��������,ʱ�ӵ͵�ƽ��Ч,16λ��չ֡���� */
            }
            else if (wavctrl.bps == 24)
            {
                es8388_i2s_cfg(0, 0);   /* �����ֱ�׼,24λ���ݳ��� */
                i2s_init(I2S_STANDARD_PHILIPS, I2S_MODE_MASTER_TX, I2S_CPOL_LOW, I2S_DATAFORMAT_24B);          /* �����ֱ�׼,��������,ʱ�ӵ͵�ƽ��Ч,24λ���� */
            }
            
            i2s_samplerate_set(wavctrl.samplerate);     /* ���ò����� */
            i2s_tx_dma_init(g_audiodev.i2sbuf1, g_audiodev.i2sbuf2, WAV_I2S_TX_DMA_BUFSIZE / 2);   /* ����TX DMA */
            i2s_tx_callback = wav_i2s_dma_tx_callback;  /* �ص�����ָwav_i2s_dma_callback */
            audio_stop();
            
            res = f_open(g_audiodev.file, (TCHAR*)fname, FA_READ);    /* ���ļ� */
            
            if (res == 0)
            {
                f_lseek(g_audiodev.file, wavctrl.datastart);          /* �����ļ�ͷ */
                
                fillnum = wav_buffill(g_audiodev.i2sbuf1, WAV_I2S_TX_DMA_BUFSIZE, wavctrl.bps);
                fillnum = wav_buffill(g_audiodev.i2sbuf2, WAV_I2S_TX_DMA_BUFSIZE, wavctrl.bps);
                
                audio_start();  
                
                while (res == 0)
                { 
                    while(wavtransferend == 0);             /* �ȴ�wav�������; */
                    
                    wavtransferend = 0;
                    
                    if (fillnum != WAV_I2S_TX_DMA_BUFSIZE)  /* ���Ž���? */
                    {
                        res = KEY0_PRES;
                        break;
                    }
                    
                    if (wavwitchbuf)
                    {
                        fillnum = wav_buffill(g_audiodev.i2sbuf2, WAV_I2S_TX_DMA_BUFSIZE, wavctrl.bps);   /* ���buf2 */
                    }
                    else
                    {
                        fillnum = wav_buffill(g_audiodev.i2sbuf1, WAV_I2S_TX_DMA_BUFSIZE, wavctrl.bps);   /* ���buf1 */
                    }
                    
                    while (1)
                    {
                        key = key_scan(0);
                        if (key == WKUP_PRES)   /* ��ͣ */
                        {
                            if (g_audiodev.status & 0x01)
                            {
                                g_audiodev.status &= ~(1 << 0);
                            }
                            else 
                            {
                                g_audiodev.status |= 0x01;
                            }
                        }
                        
                        if (key == KEY2_PRES || key == KEY0_PRES)   /* ��һ��/��һ�� */
                        {
                            res = key;
                            break;
                        }
                        
                        wav_get_curtime(g_audiodev.file, &wavctrl); /* �õ���ʱ��͵�ǰ���ŵ�ʱ�� */
                        audio_msg_show(wavctrl.totsec, wavctrl.cursec, wavctrl.bitrate);
                        
                        t++;
                        if (t == 20)
                        {
                            t = 0 ;
                            LED0_TOGGLE();
                        }
                        
                        if ((g_audiodev.status & 0x01) == 0)
                        {
                            delay_ms(10);
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                audio_stop();
            }
            else
            {
                res = 0xFF;
            }
        }
        else
        {
            res = 0xFF;
        }
    }
    else
    {
        res = 0xFF;
    }
    
    myfree(SRAMIN, g_audiodev.tbuf);      /* �ͷ��ڴ� */
    myfree(SRAMIN, g_audiodev.i2sbuf1);   /* �ͷ��ڴ� */
    myfree(SRAMIN, g_audiodev.i2sbuf2);   /* �ͷ��ڴ� */
    myfree(SRAMIN, g_audiodev.file);      /* �ͷ��ڴ� */
    
    return res;
}



