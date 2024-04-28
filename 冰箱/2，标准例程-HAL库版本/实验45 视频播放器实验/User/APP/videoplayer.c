/**
 ****************************************************************************************************
 * @file        videoplay.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-16
 * @brief       ��Ƶ������ Ӧ�ô���
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
 * ��һ�η���
 *
 ****************************************************************************************************
 */
#include "./APP/videoplayer.h"
#include "./FATFS/source/ff.h"
#include "./MALLOC/malloc.h"
#include "./SYSTEM/usart/usart.h"
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
#include "./MJPEG/mjpeg.h"
#include "./MJPEG/avi.h"
#include "./BSP/TIMER/btim.h"
#include "string.h"


extern uint16_t frame;          /* ����֡�� */
extern __IO uint8_t frameup;    /* ��Ƶ����ʱ϶���Ʊ���,������1��ʱ��,���Ը�����һ֡��Ƶ */

volatile uint8_t i2splaybuf;    /* �������ŵ���Ƶ֡������ */
uint8_t *i2sbuf[4];             /* ��Ƶ����֡,��4֡,4*5K=20K */

/**
 * @brief       ��Ƶ����I2S DMA����ص�����
 * @param       ��
 * @retval      ��
 */
void audio_i2s_dma_callback(void)
{
    i2splaybuf++;

    if (i2splaybuf > 3)
    {
        i2splaybuf = 0;
    }

    if (DMA1_Stream4->CR & (1 << 19))
    {
        DMA1_Stream4->M0AR = (uint32_t)i2sbuf[i2splaybuf];  /* ָ����һ��buf */
    }
    else
    {
        DMA1_Stream4->M1AR = (uint32_t)i2sbuf[i2splaybuf];  /* ָ����һ��buf */
    }
}

/**
 * @brief       �õ�path·����,Ŀ���ļ����ܸ���
 * @param       path:·��
 * @retval      ����ֵ:����Ч�ļ���
 */
uint16_t video_get_tnum(char *path)
{
    uint8_t res;
    uint16_t rval = 0;
    
    DIR tdir;           /* ��ʱĿ¼ */
    FILINFO *tfileinfo; /* ��ʱ�ļ���Ϣ */
    
    tfileinfo = (FILINFO *)mymalloc(SRAMIN, sizeof(FILINFO));   /* �����ڴ� */
    res = f_opendir(&tdir, (const TCHAR *)path);    /* ��Ŀ¼ */
    
    if (res == FR_OK && tfileinfo)
    {
        while (1)       /* ��ѯ�ܵ���Ч�ļ��� */
        {
            res = f_readdir(&tdir, tfileinfo);                  /* ��ȡĿ¼�µ�һ���ļ� */
            if (res != FR_OK || tfileinfo->fname[0] == 0)
            {
                break; /* ������/��ĩβ��,�˳� */
            }

            res = exfuns_file_type(tfileinfo->fname);
            if ((res & 0xF0) == 0x60)   /* ȡ����λ,�����ǲ�����Ƶ�ļ� */
            {
                rval++;                 /* ��Ч�ļ�������1 */
            }
        }
    }

    myfree(SRAMIN, tfileinfo);          /* �ͷ��ڴ� */
    
    return rval;
}

/**
 * @brief       ��ʾ��ǰ����ʱ��
 * @param       favi    : ��ǰ���ŵ���Ƶ�ļ�
 * @param       aviinfo : avi���ƽṹ��
 * @retval      ��
 */
void video_time_show(FIL *favi, AVI_INFO *aviinfo)
{
    static uint32_t oldsec; /* ��һ�εĲ���ʱ�� */
    
    uint8_t *buf;
    
    uint32_t totsec = 0;    /* video�ļ���ʱ�� */
    uint32_t cursec;        /* ��ǰ����ʱ�� */
    
    totsec = (aviinfo->SecPerFrame / 1000) * aviinfo->TotalFrame;   /* �����ܳ���(��λ:ms) */
    totsec /= 1000;         /* ������. */
    cursec = ((double)favi->fptr / favi->obj.objsize) * totsec;     /* ��ȡ��ǰ���ŵ������� */
    
    if (oldsec != cursec)   /* ��Ҫ������ʾʱ�� */
    {
        buf = mymalloc(SRAMIN, 100); /* ����100�ֽ��ڴ� */
        oldsec = cursec;
        
        sprintf((char *)buf, "����ʱ��:%02d:%02d:%02d/%02d:%02d:%02d", cursec / 3600, (cursec % 3600) / 60, cursec % 60, totsec / 3600, (totsec % 3600) / 60, totsec % 60);
        text_show_string(10, 90, lcddev.width - 10, 16, (char *)buf, 16, 0, RED);   /* ��ʾ�������� */
        
        myfree(SRAMIN, buf);
    }
}

/**
 * @brief       ��ʾ��ǰ��Ƶ�ļ��������Ϣ
 * @param       aviinfo : avi���ƽṹ��
 * @retval      ��
 */
void video_info_show(AVI_INFO *aviinfo)
{
    uint8_t *buf;
    
    buf = mymalloc(SRAMIN, 100);    /* ����100�ֽ��ڴ� */
    
    sprintf((char *)buf, "������:%d,������:%d", aviinfo->Channels, aviinfo->SampleRate * 10);
    text_show_string(10, 50, lcddev.width - 10, 16, (char *)buf, 16, 0, RED);   /* ��ʾ�������� */
    
    sprintf((char *)buf, "֡��:%d֡", 1000 / (aviinfo->SecPerFrame / 1000));
    text_show_string(10, 70, lcddev.width - 10, 16, (char *)buf, 16, 0, RED);   /* ��ʾ�������� */
    
    myfree(SRAMIN, buf);            /* �ͷ��ڴ� */
}

/**
 * @brief       ��Ƶ������Ϣ��ʾ
 * @param       name    : ��Ƶ����
 * @param       index   : ��ǰ����
 * @param       total   : ���ļ���
 * @retval      ��
 */
void video_bmsg_show(uint8_t *name, uint16_t index, uint16_t total)
{
    uint8_t *buf;
    
    buf = mymalloc(SRAMIN, 100);    /* ����100�ֽ��ڴ� */
    
    sprintf((char *)buf, "�ļ���:%s", name);
    text_show_string(10, 10, lcddev.width - 10, 16, (char *)buf, 16, 0, RED);   /* ��ʾ�ļ��� */
    
    sprintf((char *)buf, "����:%d/%d", index, total);
    text_show_string(10, 30, lcddev.width - 10, 16, (char *)buf, 16, 0, RED);   /* ��ʾ���� */
    
    myfree(SRAMIN, buf);            /* �ͷ��ڴ� */
}

/**
 * @brief       ������Ƶ
 * @param       ��
 * @retval      ��
 */
void video_play(void)
{
    uint8_t res;
    
    DIR vdir;               /* Ŀ¼ */
    FILINFO *vfileinfo;     /* �ļ���Ϣ */
    uint8_t *pname;         /* ��·�����ļ��� */
    
    uint16_t totavinum;     /* ��Ƶ�ļ����� */
    uint16_t curindex;      /* ��Ƶ�ļ���ǰ���� */
    uint32_t *voffsettbl;   /* ��Ƶ�ļ�off block������ */

    uint8_t key;            /* ��ֵ */
    uint32_t temp;
    
    while (f_opendir(&vdir, "0:/VIDEO"))    /* ����Ƶ�ļ��� */
    {
        text_show_string(60, 190, 240, 16, "VIDEO�ļ��д���!", 16, 0, RED);
        delay_ms(200);
        lcd_fill(60, 190, 240, 206, WHITE); /* �����ʾ */
        delay_ms(200);
    }

    totavinum = video_get_tnum("0:/VIDEO"); /* �õ�����Ч�ļ��� */
    while (totavinum == NULL)               /* ��Ƶ�ļ�����Ϊ0 */
    {
        text_show_string(60, 190, 240, 16, "û����Ƶ�ļ�!", 16, 0, RED);
        delay_ms(200);
        lcd_fill(60, 190, 240, 146, WHITE); /* �����ʾ */
        delay_ms(200);
    }

    vfileinfo = (FILINFO *)mymalloc(SRAMIN, sizeof(FILINFO));   /* Ϊ���ļ������������ڴ� */
    pname = mymalloc(SRAMIN, 2 * FF_MAX_LFN + 1);               /* Ϊ��·�����ļ��������ڴ� */
    
    voffsettbl = mymalloc(SRAMIN, 4 * totavinum);               /* ����4*totavinum���ֽڵ��ڴ�,���ڴ����Ƶ�ļ����� */
    
    while (vfileinfo == NULL || pname == NULL || voffsettbl == NULL)    /* �ڴ������� */
    {
        text_show_string(60, 190, 240, 16, "�ڴ����ʧ��!", 16, 0, RED);
        delay_ms(200);
        lcd_fill(60, 190, 240, 146, WHITE);     /* �����ʾ */
        delay_ms(200);
    }

    /* ��¼���� */
    res = f_opendir(&vdir, "0:/VIDEO");         /* ��Ŀ¼ */
    if (res == FR_OK)
    {
        curindex = 0;   /* ��ǰ����Ϊ0 */
        while (1)       /* ȫ����ѯһ�� */
        {
            temp = vdir.dptr;                   /* ��¼��ǰoffset */
            res = f_readdir(&vdir, vfileinfo);  /* ��ȡĿ¼�µ�һ���ļ� */
            if (res != FR_OK || vfileinfo->fname[0] == 0)
            {
                break;                          /* ������/��ĩβ��,�˳� */
            }

            res = exfuns_file_type(vfileinfo->fname);
            if ((res & 0xF0) == 0x60)           /* ȡ����λ,�����ǲ��������ļ� */
            {
                voffsettbl[curindex] = temp;    /* ��¼���� */
                curindex++;
            }
        }
    }

    curindex = 0;           /* ��0��ʼ��ʾ */
    res = f_opendir(&vdir, (const TCHAR *)"0:/VIDEO");  /* ��Ŀ¼ */
    while (res == FR_OK)    /* �򿪳ɹ� */
    {
        dir_sdi(&vdir, voffsettbl[curindex]);   /* �ı䵱ǰĿ¼���� */
        res = f_readdir(&vdir, vfileinfo);      /* ��ȡĿ¼�µ�һ���ļ� */

        if (res != FR_OK || vfileinfo->fname[0] == 0)
        {
            break;          /* ������/��ĩβ��,�˳� */
        }

        strcpy((char *)pname, "0:/VIDEO/");                     /* ����·��(Ŀ¼) */
        strcat((char *)pname, (const char *)vfileinfo->fname);  /* ���ļ������ں��� */
        
        lcd_clear(WHITE);   /* ������ */
        video_bmsg_show((uint8_t *)vfileinfo->fname, curindex + 1, totavinum);  /* ��ʾ����,��������Ϣ */
        
        key = video_play_mjpeg(pname);  /* ���������Ƶ�ļ� */
        if (key == KEY2_PRES)           /* ��һ����Ƶ */
        {
            if (curindex)
            {
                curindex--;
            }
            else
            {
                curindex = totavinum - 1;
            }
        }
        else if (key == KEY0_PRES)  /* ��һ����Ƶ */
        {
            curindex++;
            if (curindex >= totavinum)
            {
                curindex = 0;     /* ��ĩβ��ʱ��,�Զ���ͷ��ʼ */
            }
        }
        else
        {
            break;     /* �����˴��� */
        }
    }

    myfree(SRAMIN, vfileinfo);      /* �ͷ��ڴ� */
    myfree(SRAMIN, pname);          /* �ͷ��ڴ� */
    myfree(SRAMIN, voffsettbl);     /* �ͷ��ڴ� */
}

/**
 * @brief       ����һ��mjpeg�ļ�
 * @param       pname : �ļ���
 * @retval      res
 *   @arg       KEY0_PRES , ��һ��.
 *   @arg       KEY1_PRES , ��һ��.
 *   @arg       ���� , ����
 */
uint8_t video_play_mjpeg(uint8_t *pname)
{
    uint8_t *framebuf;  /* ��Ƶ����buf */
    uint8_t *pbuf;      /* bufָ�� */
    
    uint8_t  res = 0;
    uint16_t offset = 0;
    uint32_t nr;
    uint8_t key;
    uint8_t i2ssavebuf;
    
    FIL *favi;

    i2sbuf[0] = mymalloc(SRAMIN, AVI_AUDIO_BUF_SIZE);   /* ������Ƶ�ڴ� */
    i2sbuf[1] = mymalloc(SRAMIN, AVI_AUDIO_BUF_SIZE);   /* ������Ƶ�ڴ� */
    i2sbuf[2] = mymalloc(SRAMIN, AVI_AUDIO_BUF_SIZE);   /* ������Ƶ�ڴ� */
    i2sbuf[3] = mymalloc(SRAMIN, AVI_AUDIO_BUF_SIZE);   /* ������Ƶ�ڴ� */
    
    framebuf = mymalloc(SRAMIN, AVI_VIDEO_BUF_SIZE) ;   /* ������Ƶbuf */
    
    favi = (FIL *)mymalloc(SRAMIN, sizeof(FIL));        /* ����favi�ڴ� */
    
    memset(i2sbuf[0], 0, AVI_AUDIO_BUF_SIZE);
    memset(i2sbuf[1], 0, AVI_AUDIO_BUF_SIZE); 
    memset(i2sbuf[2], 0, AVI_AUDIO_BUF_SIZE);
    memset(i2sbuf[3], 0, AVI_AUDIO_BUF_SIZE);

    if (!i2sbuf[3] || !framebuf || !favi)
    {
        printf("memory error!\r\n");
        res = 0xFF;
    }

    while (res == 0)
    {
        res = f_open(favi, (char *)pname, FA_READ);
        if (res == 0)
        {
            pbuf = framebuf;
            res = f_read(favi, pbuf, AVI_VIDEO_BUF_SIZE, &nr);  /* ��ʼ��ȡ */
            if (res)
            {
                printf("fread error:%d\r\n", res);
                break;
            }

            /* ��ʼavi���� */
            res = avi_init(pbuf, AVI_VIDEO_BUF_SIZE);           /* avi���� */
            if (res)
            {
                printf("avi err:%d\r\n", res);
                break;
            }

            video_info_show(&g_avix);
            btim_tim7_int_init(g_avix.SecPerFrame / 100 - 1, 8400 - 1);   /* 10Khz����Ƶ��,��1��100us */
            offset = avi_srarch_id(pbuf, AVI_VIDEO_BUF_SIZE, "movi");     /* Ѱ��movi ID */
            avi_get_streaminfo(pbuf + offset + 4);  /* ��ȡ����Ϣ */
            f_lseek(favi, offset + 12);             /* ������־ID,����ַƫ�Ƶ������ݿ�ʼ�� */
            
            res = mjpegdec_init((lcddev.width - g_avix.Width) / 2, 110 + (lcddev.height - 110 - g_avix.Height) / 2);    /* JPG�����ʼ�� */
            if (g_avix.SampleRate)    /* ����Ƶ��Ϣ,�ų�ʼ�� */
            {
                i2s_init(I2S_STANDARD_PHILIPS, I2S_MODE_MASTER_TX, I2S_CPOL_LOW, I2S_DATAFORMAT_16B_EXTENDED);          /* �����ֱ�׼,��������,ʱ�ӵ͵�ƽ��Ч,16λ֡���� */
                i2s_samplerate_set(g_avix.SampleRate);      /* ���ò����� */
                i2s_tx_dma_init(i2sbuf[1], i2sbuf[2], g_avix.AudioBufSize / 2);     /* ����DMA */
                i2s_tx_callback = audio_i2s_dma_callback;   /* �ص�����ָ��I2S_DMA_Callback */
                i2splaybuf = 0;
                i2ssavebuf = 0;
                i2s_play_start();   /* ����I2S���� */
            }

            while (1)   /* ����ѭ�� */
            {
                if (g_avix.StreamID == AVI_VIDS_FLAG)       /* ��Ƶ�� */
                {
                    pbuf = framebuf;
                    f_read(favi, pbuf, g_avix.StreamSize + 8, &nr);   /* ������֡+��һ������ID��Ϣ */
                    res = mjpegdec_decode(pbuf, g_avix.StreamSize);
                    if (res)
                    {
                        printf("decode error!\r\n");
                    }

                    while (frameup == 0);   /* ��ʱ�䵽��(��TIM7���ж���������Ϊ1) */

                    frameup = 0;            /* ��־���� */
                    frame++;
                }
                else    /* ��Ƶ�� */
                {
                    video_time_show(favi, &g_avix);   /* ��ʾ��ǰ����ʱ�� */
                    i2ssavebuf++;
                    
                    if (i2ssavebuf > 3)
                    {
                        i2ssavebuf = 0;
                    }

                    do
                    {
                        nr = i2splaybuf;

                        if (nr)
                        {
                            nr--;
                        }
                        else
                        {
                            nr = 3;
                        }
                    } while (i2ssavebuf == nr); /* ��ײ�ȴ�. */

                    f_read(favi, i2sbuf[i2ssavebuf], g_avix.StreamSize + 8, &nr); /* ���i2sbuf */
                    pbuf = i2sbuf[i2ssavebuf];
                } 
 
                key = key_scan(0);
                if (key == KEY0_PRES || key == KEY2_PRES) /* KEY0/KEY2����,������һ��/��һ����Ƶ */
                {
                    res = key;
                    break;
                }
                else if (key == KEY1_PRES || key == WKUP_PRES)
                {
                    i2s_play_stop();    /* �ر���Ƶ */
                    video_seek(favi, &g_avix, framebuf);
                    pbuf = framebuf;
                    i2s_play_start();   /* ����DMA���� */
                }

                if (avi_get_streaminfo(pbuf + g_avix.StreamSize))     /* ��ȡ��һ֡ ����־ */
                {
                        printf("g_frame error \r\n");
                        res = KEY0_PRES;
                        break;
                }
            }

            i2s_play_stop();                                    /* �ر���Ƶ */
            TIM7->CR1 &= ~(1 << 0);                             /* �رն�ʱ��6 */
            lcd_set_window(0, 0, lcddev.width, lcddev.height);  /* �ָ����� */
            mjpegdec_free();                                    /* �ͷ��ڴ� */
            f_close(favi);                                      /* �ر��ļ� */
        }
    }

    myfree(SRAMIN, i2sbuf[0]);
    myfree(SRAMIN, i2sbuf[1]);
    myfree(SRAMIN, i2sbuf[2]);
    myfree(SRAMIN, i2sbuf[3]);
    myfree(SRAMIN, framebuf);
    myfree(SRAMIN, favi);

    return res;
}

/**
 * @brief       AVI�ļ�����
 * @param       favi    : AVI�ļ�
 * @param       aviinfo : AVI��Ϣ�ṹ��
 * @param       mbuf    : ���ݻ�����
 * @retval      ִ�н��
 *   @arg       0    , �ɹ�
 *   @arg       ���� , ����
 */
uint8_t video_seek(FIL *favi, AVI_INFO *aviinfo, uint8_t *mbuf)
{
    uint32_t fpos = favi->fptr;
    uint8_t *pbuf;
    uint16_t offset;
    uint32_t br;
    uint32_t delta;
    uint32_t totsec;
    uint8_t key;

    totsec = (aviinfo->SecPerFrame / 1000) * aviinfo->TotalFrame;
    totsec /= 1000;     /* ������ */
    delta = (favi->obj.objsize / totsec) * 5;   /* ÿ��ǰ��5���ӵ������� */

    while (1)
    {
        key = key_scan(1);
        if (key == WKUP_PRES)       /* ��� */
        {
            if (fpos < favi->obj.objsize)
            {
                fpos += delta;
            }

            if (fpos > (favi->obj.objsize - AVI_VIDEO_BUF_SIZE))
            {
                fpos = favi->obj.objsize - AVI_VIDEO_BUF_SIZE;
            }
        }
        else if (key == KEY1_PRES)  /* ���� */
        {
            if (fpos > delta)
            {
                fpos -= delta;
            }
            else
            {
                fpos = 0;
            }
        }
        else 
        {
            break;
        }

        f_lseek(favi, fpos);
        f_read(favi, mbuf, AVI_VIDEO_BUF_SIZE, &br);    /* ������֡+��һ������ID��Ϣ */
        pbuf = mbuf;
        
        if (fpos == 0)  /* ��0��ʼ,����Ѱ��movi ID */
        {
            offset = avi_srarch_id(pbuf, AVI_VIDEO_BUF_SIZE, "movi");
        }
        else
        {
            offset = 0;
        }

        offset += avi_srarch_id(pbuf + offset, AVI_VIDEO_BUF_SIZE, g_avix.VideoFLAG); /* Ѱ����Ƶ֡ */
        avi_get_streaminfo(pbuf + offset);  /* ��ȡ����Ϣ */
        f_lseek(favi, fpos + offset + 8);   /* ������־ID,����ַƫ�Ƶ������ݿ�ʼ�� */
        
        if (g_avix.StreamID == AVI_VIDS_FLAG)
        {
            f_read(favi, mbuf, g_avix.StreamSize + 8, &br);   /* ������֡ */
            mjpegdec_decode(mbuf, g_avix.StreamSize);         /* ��ʾ��Ƶ֡ */
        }
        else
        {
            printf("error flag");
        }

        video_time_show(favi, &g_avix);
    }

    return 0;
}










