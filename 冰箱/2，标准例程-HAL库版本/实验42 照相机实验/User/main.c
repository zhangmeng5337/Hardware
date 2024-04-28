/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-10
 * @brief       ����� ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/SRAM/sram.h"
#include "./BSP/SDIO/sdio_sdcard.h"
#include "./BSP/NORFLASH/norflash.h"
#include "./FATFS/exfuns/exfuns.h"
#include "./MALLOC/malloc.h"
#include "./USMART/usmart.h"
#include "./TEXT/text.h"
#include "./PICTURE/piclib.h"
#include "./BSP/TIMER/btim.h"
#include "./BSP/OV2640/ov2640.h"
#include "./BSP/OV2640/sccb.h"
#include "./BSP/DCMI/dcmi.h"
#include "./BSP/BEEP/beep.h"
#include "string.h"
#include "math.h"


uint8_t g_ov_mode = 0;                                  /* bit0: 0, RGB565ģʽ;  1, JPEGģʽ */

#define jpeg_buf_size       300 * 1024                  /* ����JPEG���ݻ���jpeg_buf�Ĵ�С(�ֽ�) */
#define jpeg_line_size      1 * 1024                    /* ����DMA��������ʱ,һ�����ݵ����ֵ */

uint32_t *p_dcmi_line_buf[2];                           /* JPEG���� DMA˫����bufָ�� */
uint32_t *p_jpeg_data_buf;                              /* JPEG���ݻ���bufָ�� */

volatile uint32_t g_jpeg_data_len = 0;                  /* buf�е�JPEG��Ч���ݳ��� */


/**
 * 0,����û�вɼ���;
 * 1,���ݲɼ�����,���ǻ�û����;
 * 2,�����Ѿ����������,���Կ�ʼ��һ֡����
 */
volatile uint8_t g_jpeg_data_ok = 0;                    /* JPEG���ݲɼ���ɱ�־ */

/**
 * @brief       ����JPEG����
 *   @ntoe      ��DCMI_IRQHandler�жϷ��������汻����
 *              ���ɼ���һ֡JPEG���ݺ�,���ô˺���,�л�JPEG BUF.��ʼ��һ֡�ɼ�.
 *
 * @param       ��
 * @retval      ��
 */
void jpeg_data_process(void)
{
    uint16_t i, rlen;
    uint32_t *pbuf;

    if (g_ov_mode)   /* ֻ����JPEG��ʽ��,����Ҫ������. */
    {
        if (g_jpeg_data_ok == 0)    /* jpeg���ݻ�δ�ɼ���? */
        {
            __HAL_DMA_DISABLE(&g_dma_dcmi_handle);      /* �ر�DMA */
            while(DMA2_Stream1->CR & 0x01);             /* �ȴ�DMA2_Stream1������ */
            rlen = jpeg_line_size - __HAL_DMA_GET_COUNTER(&g_dma_dcmi_handle);    /* �õ�ʣ�೤�� */
            pbuf = p_jpeg_data_buf + g_jpeg_data_len;   /* ƫ�Ƶ���Ч����ĩβ��������� */
            
            if (DMA2_Stream1->CR & (1 << 19))
            {
                for(i = 0; i < rlen; i++)
                {
                    pbuf[i] = p_dcmi_line_buf[1][i];    /* ��ȡbuf1�����ʣ������ */
                }
            }
            else
            {
                for(i = 0; i < rlen; i++)
                {
                    pbuf[i] = p_dcmi_line_buf[0][i];    /* ��ȡbuf0�����ʣ������ */
                }
            }
            g_jpeg_data_len += rlen;    /* ����ʣ�೤�� */
            g_jpeg_data_ok = 1;         /* ���JPEG���ݲɼ����,�ȴ������������� */
        }
        
        if (g_jpeg_data_ok == 2)        /* ��һ�ε�jpeg�����Ѿ��������� */
        {
            __HAL_DMA_SET_COUNTER(&g_dma_dcmi_handle, jpeg_line_size);    /* ���䳤��Ϊjpeg_buf_size*4�ֽ� */
            __HAL_DMA_ENABLE(&g_dma_dcmi_handle);                         /* ���´��� */
            g_jpeg_data_ok = 0;         /* �������δ�ɼ� */
            g_jpeg_data_len = 0;        /* �������¿�ʼ */
        }
    }
    else
    {
        lcd_set_cursor(0, 0);
        lcd_write_ram_prepare();        /* ��ʼд��GRAM */
    }
}

/**
 * @brief       JPEG���ݽ��ջص�����
 * @param       ��
 * @retval      ��
 */
void jpeg_dcmi_rx_callback(void)
{
    uint16_t i;
    uint32_t *pbuf;

    pbuf = p_jpeg_data_buf + g_jpeg_data_len;   /* ƫ�Ƶ���Ч����ĩβ */

    if (DMA2_Stream1->CR & (1 << 19))           /* buf0����,��������buf1 */
    {
        for (i = 0; i < jpeg_line_size; i++)
        {
            pbuf[i] = p_dcmi_line_buf[0][i];    /* ��ȡbuf0��������� */
        }
        
        g_jpeg_data_len += jpeg_line_size;      /* ƫ�� */
    }
    else    /* buf1����,��������buf0 */
    {
        for (i = 0; i < jpeg_line_size; i++)
        {
            pbuf[i] = p_dcmi_line_buf[1][i];    /* ��ȡbuf1��������� */
        }
        
        g_jpeg_data_len += jpeg_line_size;      /* ƫ�� */
    }
}

/**
 * @brief       �л�ΪOV2640ģʽ
 *   @note      �л�PC8/PC9/PC11ΪDCMI���ù���(AF13)
 * @param       ��
 * @retval      ��
 */
void sw_ov2640_mode(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    OV2640_PWDN(0);    /* OV2640 power up */
    
    /* GPIOC8/9/11�л�Ϊ DCMI�ӿ� */
    gpio_init_struct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_11;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* ���츴�� */
    gpio_init_struct.Pull = GPIO_PULLUP;                /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; /* ���� */
    gpio_init_struct.Alternate = GPIO_AF13_DCMI;        /* ����ΪDCMI */
    HAL_GPIO_Init(GPIOC, &gpio_init_struct);            /* ��ʼ��PC8��9, 11���� */
}

/**
 * @brief       �л�ΪSD��ģʽ
 *   @note      �л�PC8/PC9/PC11ΪSDMMC���ù���(AF12)
 * @param       ��
 * @retval      ��
 */
void sw_sdcard_mode(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    OV2640_PWDN(1);     /* OV2640 power down */
    
    /* GPIOC8/9/11�л�Ϊ SDIO�ӿ� */
    gpio_init_struct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_11;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* ���츴�� */
    gpio_init_struct.Pull = GPIO_PULLUP;                /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; /* ���� */
    gpio_init_struct.Alternate = GPIO_AF12_SDIO;        /* ����ΪSDIO */
    HAL_GPIO_Init(GPIOC, &gpio_init_struct);            /* ��ʼ��PC8��9, 11���� */
}

/**
 * @brief       �ļ������������⸲�ǣ�
 *   @note      bmp��ϳ�: ���� "0:PHOTO/PIC13141.bmp" ���ļ���
 *              jpg��ϳ�: ���� "0:PHOTO/PIC13141.jpg" ���ļ���
 * @param       pname : ��Ч���ļ���
 * @param       mode  : 0, ����.bmp�ļ�;  1, ����.jpg�ļ�;
 * @retval      ��
 */
void camera_new_pathname(uint8_t *pname, uint8_t mode)
{
    uint8_t res;
    uint16_t index = 0;
    FIL *ftemp;
    
    ftemp = (FIL *)mymalloc(SRAMIN, sizeof(FIL));   /* ����FIL�ֽڵ��ڴ����� */

    if (ftemp == NULL) return;  /* �ڴ�����ʧ�� */

    while (index < 0XFFFF)
    {
        if (mode == 0)  /* ����.bmp�ļ��� */
        {
            sprintf((char *)pname, "0:PHOTO/PIC%05d.bmp", index);
        }
        else  /* ����.jpg�ļ��� */
        {
            sprintf((char *)pname, "0:PHOTO/PIC%05d.jpg", index);
        }
        
        res = f_open(ftemp, (const TCHAR *)pname, FA_READ); /* ���Դ�����ļ� */

        if (res == FR_NO_FILE) break;   /* ���ļ���������, ����������Ҫ�� */

        index++;
    }
    myfree(SRAMIN, ftemp);
}
/**
 * @brief       OV2640����jpgͼƬ
 * @param       pname : Ҫ������jpg�ļ���(��·��)
 * @retval      0, �ɹ�; ����,�������;
 */
uint8_t ov2640_jpg_photo(uint8_t *pname)
{
    FIL *f_jpg;
    uint8_t res = 0, headok = 0;
    uint32_t bwr;
    uint32_t i, jpgstart, jpglen;
    uint8_t *pbuf;
    
    uint16_t datasize = 0;          /* ����д�������� */
    uint32_t datalen = 0;           /* ��д�������� */
    uint8_t  *databuf;              /* ���ݻ��棬����ֱ��д�ⲿSRAM���ݵ�SD��������д��������� */
    
    f_jpg = (FIL *)mymalloc(SRAMIN, sizeof(FIL));   /* ����FIL�ֽڵ��ڴ����� */
    databuf = mymalloc(SRAMIN, 4096);   /* ����4K�ڴ� */
    if (databuf == NULL) return 0XFF;   /* �ڴ�����ʧ�� */

    g_ov_mode = 1;
    g_jpeg_data_ok = 0;
    
    sw_ov2640_mode();               /* �л�ΪOV2640ģʽ */
    ov2640_jpeg_mode();             /* JPEGģʽ */
    
    dcmi_rx_callback = jpeg_dcmi_rx_callback;   /* JPEG�������ݻص����� */
    dcmi_dma_init((uint32_t)p_dcmi_line_buf[0], (uint32_t)p_dcmi_line_buf[1], jpeg_line_size, DMA_MDATAALIGN_WORD, DMA_MINC_ENABLE); /* DCMI DMA���� */
    
    ov2640_image_win_set(0, 0, 1600, 1200);
    ov2640_outsize_set(1600, 1200); /* ��������ߴ�(1600 * 1200) */
    
    dcmi_start();                   /* �������� */
    while (g_jpeg_data_ok != 1);    /* �ȴ���һ֡ͼƬ�ɼ��� */
    g_jpeg_data_ok = 2;             /* ���Ա�֡ͼƬ,������һ֡�ɼ� */
    while (g_jpeg_data_ok != 1);    /* �ȴ��ڶ�֡ͼƬ�ɼ���  */
    g_jpeg_data_ok = 2;             /* ���Ա�֡ͼƬ,������һ֡�ɼ� */
    while (g_jpeg_data_ok != 1);    /* �ȴ�����֡ͼƬ�ɼ���,����֡,�ű��浽SD��ȥ */

    dcmi_stop();                    /* ֹͣDMA���� */
    g_ov_mode = 0;
    sw_sdcard_mode();               /* �л�ΪSD��ģʽ */
    
    printf("jpeg data size:%d\r\n", g_jpeg_data_len * 4);   /* ���ڴ�ӡJPEG�ļ���С */
    pbuf = (uint8_t *)p_jpeg_data_buf;
    jpglen = 0;                     /* ����jpg�ļ���СΪ0 */
    headok = 0;                     /* ���jpgͷ��� */

    for (i = 0; i < g_jpeg_data_len * 4; i++)   /* ����0XFF,0XD8��0XFF,0XD9,��ȡjpg�ļ���С */
    {
        if ((pbuf[i] == 0XFF) && (pbuf[i + 1] == 0XD8))     /* �ҵ�FF D8 */
        {
            jpgstart = i;
            headok = 1;             /* ����ҵ�jpgͷ(FF D8) */
        }

        if ((pbuf[i] == 0XFF) && (pbuf[i + 1] == 0XD9) && headok)   /* �ҵ�ͷ�Ժ�,����FF D9 */
        {
            jpglen = i - jpgstart + 2;
            break;
        }
    }

    if (jpglen)                     /* ������jpeg���� */
    {
        res = f_open(f_jpg, (const TCHAR *)pname, FA_WRITE | FA_CREATE_NEW);    /* ģʽ0,���߳��Դ�ʧ��,�򴴽����ļ� */

        if (res == 0)
        {
            pbuf += jpgstart;       /* ƫ�Ƶ�0XFF,0XD8�� */
            
            while(datalen < jpglen) /* ѭ��д�룡����ֱ��д�ⲿSRAM���ݵ�SDIO�������������FIFO������� */
            {
                if((jpglen - datalen) > 4096)
                {
                    datasize = 4096;
                }else
                {
                    datasize = jpglen - datalen;    /* �������� */
                }

                my_mem_copy(databuf, pbuf, datasize);
                res = f_write(f_jpg, databuf, datasize, (UINT *)&bwr); /* д������ */
                pbuf += datasize;
                jpglen -= datasize;

                if (res) break;
            }
        }

        f_close(f_jpg);
    }
    else
    {
        res = 0XFD;
    }
    
    g_jpeg_data_len = 0;
    sw_ov2640_mode();       /* �л�ΪOV2640ģʽ */
    ov2640_rgb565_mode();   /* RGB565ģʽ */

    dcmi_dma_init((uint32_t)&LCD->LCD_RAM, 0, 1, DMA_MDATAALIGN_HALFWORD, DMA_MINC_DISABLE); /* DCMI DMA����,MCU��,���� */
    myfree(SRAMIN, f_jpg);
    myfree(SRAMIN, databuf);
    
    return res;
}

int main(void)
{
    uint8_t res;
    float fac;
    uint8_t *pname;         /* ��·�����ļ��� */
    uint8_t key;            /* ��ֵ */
    uint8_t i;
    uint8_t sd_ok = 1;      /* 0,sd��������;1,SD������ */
    uint8_t scale = 1;      /* Ĭ����ȫ�ߴ����� */
    uint8_t msgbuf[15];     /* ��Ϣ������ */

    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ����1��ʼ��Ϊ115200 */
    usmart_dev.init(84);                    /* ��ʼ��USMART */
    led_init();                             /* ��ʼ��LED */
    beep_init();                            /* ��ʼ�������� */
    lcd_init();                             /* ��ʼ��LCD */
    key_init();                             /* ��ʼ������ */
    sram_init();                            /* ��ʼ��SRAM */
    btim_timx_int_init(10000 - 1, 8400 - 1);/* 10KHz����Ƶ�ʣ�1���ж�һ�� */
    piclib_init();                          /* ��ʼ����ͼ */

    my_mem_init(SRAMIN);                    /* ��ʼ���ڲ�SRAM�ڴ�� */
    my_mem_init(SRAMEX);                    /* ��ʼ���ⲿSRAM�ڴ�� */
    my_mem_init(SRAMCCM);                   /* ��ʼ��CCM�ڴ�� */
    
    exfuns_init();                          /* Ϊfatfs��ر��������ڴ� */
    f_mount(fs[0], "0:", 1);                /* ����SD�� */
    f_mount(fs[1], "1:", 1);                /* ����FLASH */

    while (fonts_init())                    /* ����ֿ� */
    {
        lcd_show_string(30, 90, 200, 16, 16, "Font error!", RED);
        delay_ms(200);
        lcd_fill(20, 90, 200 + 20, 90 + 16, WHITE);
        delay_ms(200);
    }

    text_show_string(30, 50, 200, 16, "����ԭ��STM32������", 16, 0, RED);
    text_show_string(30, 70, 200, 16, "�����ʵ��", 16, 0, RED);
    text_show_string(30, 90, 200, 16, "KEY0:����(bmp��ʽ)", 16, 0, RED);
    text_show_string(30, 110, 200, 16, "KEY1:����(jpg��ʽ)", 16, 0, RED);
    text_show_string(30, 130, 200, 16, "WK_UP:FullSize/Scale", 16, 0, RED);
    
    res = f_mkdir("0:/PHOTO");              /* ����PHOTO�ļ��� */
    
    if (res != FR_EXIST && res != FR_OK)    /* �����˴��� */
    {
        res = f_mkdir("0:/PHOTO");          /* ����PHOTO�ļ��� */
        text_show_string(30, 150, 240, 16, "SD������!", 16, 0, RED);
        delay_ms(200);
        text_show_string(30, 150, 240, 16, "���չ��ܽ�������!", 16, 0, RED);
        delay_ms(200);
        sd_ok = 0;
    }

    p_dcmi_line_buf[0] = mymalloc(SRAMIN, jpeg_line_size * 4);  /* Ϊjpeg dma���������ڴ� */
    p_dcmi_line_buf[1] = mymalloc(SRAMIN, jpeg_line_size * 4);  /* Ϊjpeg dma���������ڴ� */
    p_jpeg_data_buf = mymalloc(SRAMEX, jpeg_buf_size);          /* Ϊjpeg�ļ������ڴ� */
    pname = mymalloc(SRAMIN, 30);                               /* Ϊ��·�����ļ�������30���ֽڵ��ڴ� */

    while (pname == NULL || !p_dcmi_line_buf[0] || !p_dcmi_line_buf[1] || !p_jpeg_data_buf) /* �ڴ������� */
    {
        text_show_string(30, 150, 240, 16, "�ڴ����ʧ��!", 16, 0, RED);
        delay_ms(200);
        lcd_fill(30, 150, 240, 146, WHITE); /* �����ʾ */
        delay_ms(200);
    }

    while (ov2640_init())   /* ��ʼ��OV2640 */
    {
        text_show_string(30, 170, 240, 16, "OV2640 ����!", 16, 0, RED);
        delay_ms(200);
        lcd_fill(30, 150, 239, 206, WHITE);
        delay_ms(200);
    }

    delay_ms(100);
    text_show_string(30, 170, 230, 16, "OV2640 ����", 16, 0, RED);
    
    ov2640_rgb565_mode();       /* RGB565ģʽ */
    dcmi_init();                /* DCMI���� */
    dcmi_dma_init((uint32_t)&LCD->LCD_RAM, 0, 1, DMA_MDATAALIGN_HALFWORD, DMA_MINC_DISABLE);    /* DCMI DMA����,MCU��,���� */
    
    ov2640_outsize_set(lcddev.width, lcddev.height);    /* ����������ʾ */
    dcmi_start();               /* �������� */
    ov2640_flash_intctrl();     /* ����ƿ��� */
    lcd_clear(BLACK);
    
    while (1)
    {
        key = key_scan(0);
        
         if (key && (key != KEY2_PRES))
        {
            dcmi_stop();                /* ֹͣ��ʾ */
            
            if (key == WKUP_PRES)       /* ���Ŵ��� */
            {
                scale = !scale;

                if (scale == 0)
                {
                    ov2640_image_win_set((1600 - lcddev.width) / 2, (1200 - lcddev.height) / 2, lcddev.width, lcddev.height);
                    ov2640_outsize_set(lcddev.width, lcddev.height);
                    sprintf((char *)msgbuf, "Full Size 1:1");
                }
                else
                {
                    ov2640_image_win_set(0,0,1600,1200);    /* ȫ�ߴ����� */
                    ov2640_outsize_set(lcddev.width, lcddev.height);
                    sprintf((char *)msgbuf, "Scale");
                }
                
                lcd_show_string(30, 50, 200, 16, 16, (char *)msgbuf, RED);
                delay_ms(800);
            }
            else if (sd_ok)             /* SD�������ſ������� */
            {
                sw_sdcard_mode();       /* �л�ΪSD��ģʽ */

                if (key == KEY0_PRES)   /* BMP���� */
                {
                    camera_new_pathname(pname, 0);  /* �õ��ļ��� */
                    res = bmp_encode(pname, 0, 0, lcddev.width, lcddev.height, 0);
                    sw_ov2640_mode();               /* �л�ΪOV2640ģʽ */
                }
                else if (key == KEY1_PRES)          /* JPG���� */
                {
                    camera_new_pathname(pname, 1);  /* �õ��ļ��� */
                    res = ov2640_jpg_photo(pname);

                    if (scale == 0)
                    {
                        ov2640_image_win_set((1280 - fac * lcddev.width) / 2, (800 - lcddev.height) / 2, lcddev.width, lcddev.height);
                    }
                    else
                    {
                        ov2640_image_win_set(0, 0, 1600, 1200);     /* ȫ�ߴ����� */
                    }
                    ov2640_outsize_set(lcddev.width, lcddev.height);
                }
                sw_ov2640_mode();   /* �л�ΪOV2640ģʽ */
                
                if (res)            /* �������� */
                {
                    text_show_string(30, 130, 240, 16, "д���ļ�����!", 16, 0, RED);
                }
                else
                {
                    text_show_string(30, 130, 240, 16, "���ճɹ�!", 16, 0, RED);
                    text_show_string(30, 150, 240, 16, "����Ϊ:", 16, 0, RED);
                    text_show_string(30 + 56, 150, 240, 16, (char*)pname, 16, 0, RED);
                    BEEP(1);        /* �������̽У���ʾ������� */
                    delay_ms(100);
                    BEEP(0);        /* �رշ����� */
                }

                delay_ms(1000);     /* �ȴ�1���� */
                dcmi_start();       /* ������ʹ��dcmi,Ȼ�������ر�DCMI,�����ٿ���DCMI,���Է�ֹRGB���Ĳ������� */
                dcmi_stop();
            }
            else    /* ��ʾSD������ */
            {
                text_show_string(30, 130, 240, 16, "SD������!", 16, 0, RED);
                text_show_string(30, 150, 240, 16, "���չ��ܲ�����!", 16, 0, RED);
            }
            
            BEEP(0);
            
            if (key != WKUP_PRES) delay_ms(1800);
            
            dcmi_start();   /* ��ʼ��ʾ */
        }

        delay_ms(10);
        i++;

        if (i == 20)
        {
            i = 0;
            LED0_TOGGLE();
        }
    }
}

