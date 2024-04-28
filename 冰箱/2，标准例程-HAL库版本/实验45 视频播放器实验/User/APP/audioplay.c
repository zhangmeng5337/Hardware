/**
 ****************************************************************************************************
 * @file        audioplay.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-16
 * @brief       ���ֲ����� Ӧ�ô���
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


__audiodev g_audiodev;          /* ���ֲ��ſ����� */

/**
 * @brief       ��ʼ��Ƶ����
 * @param       ��
 * @retval      ��
 */
void audio_start(void)
{
    g_audiodev.status = 3 << 0; /* ��ʼ����+����ͣ */
    i2s_play_start();
}

/**
 * @brief       ֹͣ��Ƶ����
 * @param       ��
 * @retval      ��
 */
void audio_stop(void)
{
    g_audiodev.status = 0;
    i2s_play_stop();
}

/**
 * @brief       �õ�path·���£�Ŀ���ļ�������
 * @param       path : �ļ�·��
 * @retval      ��Ч�ļ�����
 */
uint16_t audio_get_tnum(char *path)
{
    uint8_t res;
    uint16_t rval = 0;
    DIR tdir;           /* ��ʱĿ¼ */
    FILINFO* tfileinfo; /* ��ʱ�ļ���Ϣ */
    
    tfileinfo = (FILINFO*)mymalloc(SRAMIN, sizeof(FILINFO));    /* �����ڴ� */
    
    res = f_opendir(&tdir, (const TCHAR*)path);                 /* ��Ŀ¼ */
    if ((res == FR_OK) && tfileinfo)
    {
        while (1)       /* ��ѯ�ܵ���Ч�ļ��� */
        {
            res = f_readdir(&tdir, tfileinfo);                  /* ��ȡĿ¼�µ�һ���ļ� */
            if ((res != FR_OK) || (tfileinfo->fname[0] == 0))
            {
                break;  /* ������/��ĩβ��,�˳� */
            }

            res = exfuns_file_type(tfileinfo->fname);
            if ((res & 0xF0) == 0x40)   /* ȡ����λ,�����ǲ��������ļ� */
            {
                rval++; /* ��Ч�ļ�������1 */
            }
        }
    }
    
    myfree(SRAMIN, tfileinfo);    /* �ͷ��ڴ� */
    
    return rval;
}

/**
 * @brief       ��ʾ��Ŀ����
 * @param       index : ��ǰ����
 * @param       total : ���ļ���
 * @retval      ��
 */
void audio_index_show(uint16_t index, uint16_t total)
{
    /* ��ʾ��ǰ��Ŀ������,������Ŀ�� */
    lcd_show_num(30 + 0, 230, index, 3, 16, RED);   /* ���� */
    lcd_show_char(30 + 24, 230, '/', 16, 0, RED);
    lcd_show_num(30 + 32, 230, total, 3, 16, RED);  /* ����Ŀ */
}

/**
 * @brief       ��ʾ����ʱ��,������ ��Ϣ
 * @param       totsec : ��Ƶ�ļ���ʱ�䳤��
 * @param       cursec : ��ǰ����ʱ��
 * @param       bitrate: ������(λ��)
 * @retval      ��
 */
void audio_msg_show(uint32_t totsec, uint32_t cursec, uint32_t bitrate)
{
    static uint16_t playtime = 0xFFFF;  /* ��ʱ���� */
    
    if (playtime != cursec)             /* ��Ҫ������ʾʱ�� */
    {
        playtime = cursec;
        
        /* ��ʾ����ʱ�� */
        lcd_show_num(30, 210, playtime / 60, 2, 16, RED);       /* ���� */
        lcd_show_char(30 + 16, 210, ':', 16, 0, RED);
        lcd_show_num(30 + 24, 210, playtime % 60, 2, 16, RED);  /* ���� */
        lcd_show_char(30 + 40, 210, '/', 16, 0, RED);
        
        /* ��ʾ��ʱ�� */
        lcd_show_num(30 + 48, 210, totsec / 60, 2, 16, RED);    /* ���� */
        lcd_show_char(30 + 64, 210, ':', 16, 0, RED);
        lcd_show_num(30 + 72, 210, totsec % 60, 2, 16, RED);    /* ���� */
        
        /* ��ʾλ�� */
        lcd_show_num(30 + 110, 210, bitrate / 1000, 4, 16, RED);/* ��ʾλ�� */
        lcd_show_string(30 + 110 + 32 , 210, 200, 16, 16, "Kbps", RED);
    }
}

/**
 * @brief       ��������
 * @param       ��
 * @retval      ��
 */
void audio_play(void)
{
    uint8_t res;
    DIR wavdir;             /* Ŀ¼ */
    FILINFO *wavfileinfo;   /* �ļ���Ϣ */
    char *pname;            /* ��·�����ļ��� */
    uint16_t totwavnum;     /* �����ļ����� */
    uint16_t curindex;      /*��ǰ���� */
    uint8_t key;            /* ��ֵ */
    uint32_t temp;
    uint32_t *wavoffsettbl; /* ����offset������ */

    es8388_adda_cfg(1, 0);  /* ����DAC�ر�ADC */
    es8388_output_cfg(1, 1);/* DACѡ��ͨ��1��� */

    while (f_opendir(&wavdir, "0:/MUSIC"))  /* �������ļ��� */
    {
        text_show_string(30, 190, 240, 16, "MUSIC�ļ��д���!", 16, 0, BLUE);
        delay_ms(200);
        lcd_fill(30, 190, 240, 206, WHITE); /* �����ʾ */
        delay_ms(200);
    }

    totwavnum = audio_get_tnum("0:/MUSIC"); /* �õ�����Ч�ļ��� */
    while (totwavnum == NULL)               /* �����ļ�����Ϊ0 */
    {
        text_show_string(30, 190, 240, 16, "û�������ļ�!", 16, 0, BLUE);
        delay_ms(200);
        lcd_fill(30, 190, 240, 146, WHITE); /* �����ʾ */
        delay_ms(200);
    }
    
    wavfileinfo = (FILINFO*)mymalloc(SRAMIN, sizeof(FILINFO));  /* �����ڴ� */
    pname = mymalloc(SRAMIN, FF_MAX_LFN * 2 + 1);               /* Ϊ��·�����ļ��������ڴ� */
    wavoffsettbl = mymalloc(SRAMIN, 4 * totwavnum);             /* ����4*totwavnum���ֽڵ��ڴ�,���ڴ�������ļ�off block���� */
    while (!wavfileinfo || !pname || !wavoffsettbl)             /* �ڴ������� */
    {
        text_show_string(30, 190, 240, 16, "�ڴ����ʧ��!", 16, 0, BLUE);
        delay_ms(200);
        lcd_fill(30, 190, 240, 146, WHITE);                     /* �����ʾ */
        delay_ms(200);
    }
    
    /* ��¼���� */
    res = f_opendir(&wavdir, "0:/MUSIC");   /* ��Ŀ¼ */
    if (res == FR_OK)
    {
        curindex = 0;   /* ��ǰ����Ϊ0 */
        while (1)       /* ȫ����ѯһ�� */
        {
            temp = wavdir.dptr;                     /* ��¼��ǰindex */

            res = f_readdir(&wavdir, wavfileinfo);  /* ��ȡĿ¼�µ�һ���ļ� */
            if ((res != FR_OK) || (wavfileinfo->fname[0] == 0))
            {
                break;  /* ������/��ĩβ��,�˳� */
            }

            res = exfuns_file_type(wavfileinfo->fname);
            if ((res & 0xF0) == 0x40)           /* ȡ����λ,�����ǲ��������ļ� */
            {
                wavoffsettbl[curindex] = temp;  /* ��¼���� */
                curindex++;
            }
        }
    }
    
    curindex = 0;           /* ��0��ʼ��ʾ */
    res = f_opendir(&wavdir, (const TCHAR*)"0:/MUSIC"); /* ��Ŀ¼ */
    while (res == FR_OK)    /* �򿪳ɹ� */
    {
        dir_sdi(&wavdir, wavoffsettbl[curindex]);       /* �ı䵱ǰĿ¼���� */
        res = f_readdir(&wavdir, wavfileinfo);          /* ��ȡĿ¼�µ�һ���ļ� */
        if ((res != FR_OK) || (wavfileinfo->fname[0] == 0))
        {
            break;          /* ������/��ĩβ��,�˳� */
        }
        
        strcpy((char*)pname, "0:/MUSIC/");                      /* ����·��(Ŀ¼) */
        strcat((char*)pname, (const char*)wavfileinfo->fname);  /* ���ļ������ں��� */
        lcd_fill(30, 190, lcddev.width - 1, 190 + 16, WHITE);   /* ���֮ǰ����ʾ */
        text_show_string(30, 190, lcddev.width - 60, 16, (char*)wavfileinfo->fname, 16, 0, BLUE);   /* ��ʾ�������� */
        audio_index_show(curindex + 1, totwavnum);

        key = audio_play_song(pname);                           /* ���������Ƶ�ļ� */
        if (key == KEY2_PRES)       /* ��һ�� */
        {
            if (curindex)
            {
                curindex--;
            }
            else
            {
                curindex = totwavnum - 1;
            }
        }
        else if (key == KEY0_PRES)  /* ��һ�� */
        {
            curindex++;
            if (curindex >= totwavnum)
            {
                curindex = 0;       /* ��ĩβ��ʱ��,�Զ���ͷ��ʼ */
            }
        }
        else
        {
            break;  /* �����˴��� */
        }
    }

    myfree(SRAMIN, wavfileinfo);    /* �ͷ��ڴ� */
    myfree(SRAMIN, pname);          /* �ͷ��ڴ� */
    myfree(SRAMIN, wavoffsettbl);   /* �ͷ��ڴ� */
}

/**
 * @brief       ����ĳ����Ƶ�ļ�
 * @param       fname : �ļ���
 * @retval      ����ֵ
 *   @arg       KEY0_PRES , ��һ��.
 *   @arg       KEY2_PRES , ��һ��.
 *   @arg       ���� , ����
 */
uint8_t audio_play_song(char* fname)
{
    uint8_t res;  
    
    res = exfuns_file_type(fname); 
    switch (res)
    {
        case T_WAV:
            res = wav_play_song(fname);
            break;

        default:            /* �����ļ�,�Զ���ת����һ�� */
            printf("can't play:%s\r\n", fname);
            res = KEY0_PRES;
            break;
    }
    return res;
}













