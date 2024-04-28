/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-08
 * @brief       ����ͷ ʵ��
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
#include "./USMART/usmart.h"
#include "./BSP/KEY/key.h"
#include "./BSP/TIMER/btim.h"
#include "./BSP/OV2640/ov2640.h"
#include "./BSP/OV2640/sccb.h"
#include "./BSP/DCMI/dcmi.h"
#include "./BSP/USART2/usart2.h"


uint8_t g_ov_mode = 0;                                      /* bit0: 0, RGB565ģʽ;  1, JPEGģʽ */

#define jpeg_buf_size       29*1024                         /* ����JPEG���ݻ���jpeg_buf�Ĵ�С(*4�ֽ�) */
#define jpeg_line_size      1*1024                          /* ����DMA��������ʱ,һ�����ݵ����ֵ */

__ALIGNED(4) uint32_t g_jpeg_data_buf[jpeg_buf_size];       /* JPEG���ݻ���buf */
__ALIGNED(4) uint32_t g_dcmi_line_buf[2][jpeg_line_size];   /* JPEG���� DMA˫����buf */

volatile uint32_t g_jpeg_data_len = 0;                      /* buf�е�JPEG��Ч���ݳ��� */


/**
 * 0,����û�вɼ���;
 * 1,���ݲɼ�����,���ǻ�û����;
 * 2,�����Ѿ����������,���Կ�ʼ��һ֡����
 */
volatile uint8_t g_jpeg_data_ok = 0;                        /* JPEG���ݲɼ���ɱ�־ */

/* JPEG�ߴ�֧���б� */
const uint16_t jpeg_img_size_tbl[][2] =
{
    160, 120,       /* QQVGA */
    176, 144,       /* QCIF */
    320, 240,       /* QVGA */
    400,240,        /* WGVGA */
    352,288,        /* CIF */
    640, 480,       /* VGA */
    800, 600,       /* SVGA */
    1024, 768,      /* XGA */
    1280, 800,      /* WXGA */
    1280, 960,      /* XVGA */
    1440, 900,      /* WXGA+ */
    1280, 1024,     /* SXGA */
    1600, 1200,     /* UXGA */
};

const char *EFFECTS_TBL[7] = {"Normal", "Negative", "B&W", "Redish", "Greenish", "Bluish", "Antique"};     /* 7����Ч */
const char *JPEG_SIZE_TBL[13] = {"QQVGA", "QCIF", "QVGA", "WGVGA", "CIF", "VGA", "SVGA", "XGA", "WXGA", "SVGA", "WXGA+", "SXGA", "UXGA"}; /* JPEGͼƬ 13�ֳߴ� */

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
    uint16_t i;
    uint16_t rlen;      /* ʣ�����ݳ��� */
    uint32_t *pbuf;
    
    if (g_ov_mode)      /* ֻ����JPEG��ʽ��,����Ҫ������. */
    {
        if (g_jpeg_data_ok == 0)                    /* jpeg���ݻ�δ�ɼ���? */
        {
            __HAL_DMA_DISABLE(&g_dma_dcmi_handle);  /* �ر�DMA */
            while(DMA2_Stream1->CR & 0x01);         /* �ȴ�DMA2_Stream1������ */

            rlen = jpeg_line_size - __HAL_DMA_GET_COUNTER(&g_dma_dcmi_handle);    /* �õ�ʣ�೤�� */
            pbuf = g_jpeg_data_buf + g_jpeg_data_len;   /* ƫ�Ƶ���Ч����ĩβ,������� */
            if (DMA2_Stream1->CR & (1 << 19))
            {
                for (i = 0; i < rlen; i++)
                {
                    pbuf[i] = g_dcmi_line_buf[1][i];    /* ��ȡbuf1�����ʣ������ */
                }
            }
            else
            {
                for (i = 0; i < rlen; i++)
                {
                    pbuf[i] = g_dcmi_line_buf[0][i];    /* ��ȡbuf0�����ʣ������ */
                }
            }

            g_jpeg_data_len += rlen;    /* ����ʣ�೤�� */
            g_jpeg_data_ok = 1;         /* ���JPEG���ݲɼ����,�ȴ������������� */
        }
        if (g_jpeg_data_ok == 2)        /* ��һ�ε�jpeg�����Ѿ��������� */
        {
            __HAL_DMA_SET_COUNTER(&g_dma_dcmi_handle, jpeg_line_size);   /* ���䳤��Ϊjpeg_buf_size*4�ֽ� */
            __HAL_DMA_ENABLE(&g_dma_dcmi_handle);   /* ���´��� */
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
 *   @ntoe      ��DMA2_Stream1_IRQHandler�жϷ��������汻����
 *
 * @param       ��
 * @retval      ��
 */
void jpeg_dcmi_rx_callback(void)
{
    uint16_t i;
    volatile uint32_t *pbuf;
    pbuf = g_jpeg_data_buf + g_jpeg_data_len;   /* ƫ�Ƶ���Ч����ĩβ */

    if (DMA2_Stream1->CR & (1 << 19))           /* buf0����,��������buf1 */
    {
        for (i = 0; i < jpeg_line_size; i++)
        {
            pbuf[i] = g_dcmi_line_buf[0][i];    /* ��ȡbuf0��������� */
        }
        
        g_jpeg_data_len += jpeg_line_size;      /* ƫ�� */
    }
    else    /* buf1����,��������buf0 */
    {
        for (i = 0; i < jpeg_line_size; i++)
        {
            pbuf[i] = g_dcmi_line_buf[1][i];    /* ��ȡbuf1��������� */
        }
        
        g_jpeg_data_len += jpeg_line_size;      /* ƫ�� */
    }
}

/**
 * @brief       JPEG����
 *   @ntoe      JPEG����,ͨ������2���͸�����.
 *
 * @param       ��
 * @retval      ��
 */
void jpeg_test(void)
{
    uint32_t i, jpgstart, jpglen;
    uint8_t *p;
    uint8_t key, headok = 0;
    uint8_t effect = 0, saturation = 2, contrast = 2;
    uint8_t size = 2;   /* Ĭ����QVGA 320*240�ߴ� */
    uint8_t msgbuf[15]; /* ��Ϣ������ */
    
    lcd_clear(WHITE);
    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "OV2640 JPEG Mode", RED);
    lcd_show_string(30, 100, 200, 16, 16, "KEY0:Contrast", RED);    /* �Աȶ� */
    lcd_show_string(30, 120, 200, 16, 16, "KEY1:Saturation", RED);  /* ɫ�ʱ��Ͷ� */
    lcd_show_string(30, 140, 200, 16, 16, "KEY2:Effect", RED);      /* ��Ч */
    lcd_show_string(30, 160, 200, 16, 16, "KEY_UP:Size", RED);      /* �ֱ������� */
    sprintf((char *)msgbuf, "JPEG Size:%s", JPEG_SIZE_TBL[size]);
    lcd_show_string(30, 180, 200, 16, 16, (char*)msgbuf, RED);      /* ��ʾ��ǰJPEG�ֱ��� */
    
    ov2640_jpeg_mode(); /* JPEGģʽ */
    dcmi_init();        /* DCMI���� */
    dcmi_rx_callback = jpeg_dcmi_rx_callback;    /* JPEG�������ݻص����� */
    dcmi_dma_init((uint32_t)&g_dcmi_line_buf[0], (uint32_t)&g_dcmi_line_buf[1], jpeg_line_size, DMA_MDATAALIGN_WORD, DMA_MINC_ENABLE); /* DCMI DMA���� */
    ov2640_outsize_set(jpeg_img_size_tbl[size][0], jpeg_img_size_tbl[size][1]);     /* ��������ߴ� */
    dcmi_start();       /* �������� */

    while (1)
    {
        if (g_jpeg_data_ok == 1)    /* �Ѿ��ɼ���һ֡ͼ���� */
        {
            p = (uint8_t *)g_jpeg_data_buf;
            printf("g_jpeg_data_len:%d\r\n", g_jpeg_data_len * 4);  /* ��ӡ���ݳ��� */
            lcd_show_string(30, 210, 210, 16, 16, "Sending JPEG data...", RED); /* ��ʾ���ڴ������� */
            jpglen = 0;             /* ����jpg�ļ���СΪ0 */
            headok = 0;             /* ���jpgͷ��� */
            
            for (i = 0; i < g_jpeg_data_len * 4; i++)       /* ����0XFF,0XD8��0XFF,0XD9,��ȡjpg�ļ���С */
            {
                if ((p[i] == 0XFF) && (p[i + 1] == 0XD8))   /* �ҵ�FF D8 */
                {
                    jpgstart = i;
                    headok = 1; /* ����ҵ�jpgͷ(FF D8) */
                }

                if ((p[i] == 0XFF) && (p[i + 1] == 0XD9) && headok) /* �ҵ�ͷ�Ժ�,����FF D9 */
                {
                    jpglen = i - jpgstart + 2;
                    break;
                }
            }

            if (jpglen)         /* ������jpeg���� */
            {
                p += jpgstart;  /* ƫ�Ƶ�0XFF,0XD8�� */

                for (i = 0; i < jpglen; i++)    /* ��������jpg�ļ� */
                {
                    USART2->DR = p[i];

                    while ((USART2->SR & 0X40) == 0);   /* ѭ������,ֱ��������� */

                    key = key_scan(0);

                    if (key)break;
                }
            }

            if (key != 0)   /* �а�������,��Ҫ���� */
            {
                lcd_show_string(30, 210, 210, 16, 16, "Quit Sending data   ", RED); /* ��ʾ�˳����ݴ��� */
                switch (key)
                {
                    case KEY0_PRES: /* �Աȶ����� */
                        contrast++;
                        if( contrast > 4)contrast = 0;
                        ov2640_contrast(contrast);
                        sprintf((char*)msgbuf, "Constrast:%d", (signed char)contrast - 2);
                        break;

                    case KEY1_PRES: /* ���Ͷ����� */
                        saturation++;
                        if (saturation > 4)saturation = 0;
                        ov2640_color_saturation(saturation);
                        sprintf((char *)msgbuf, "Saturation:%d", saturation);
                        break;
                    
                    case KEY2_PRES: /* ��Ч���� */
                        effect++;
                        if (effect > 6) effect = 0;
                        ov2640_special_effects(effect);
                        sprintf((char *)msgbuf, "Effect:%s", EFFECTS_TBL[effect]);
                        break;
                    
                    case WKUP_PRES: /* ��Ч���� */
                        size++;
                        if (size > 12)size = 0; /* ���ֻ֧��WXGA��jpeg���ݱ��棬�ٴ�ֱ��ʾͲ����ڴ����� */

                        ov2640_outsize_set(jpeg_img_size_tbl[size][0], jpeg_img_size_tbl[size][1]);     /* ��������ߴ� */
                        sprintf((char *)msgbuf, "JPEG Size:%s", JPEG_SIZE_TBL[size]);
                        break;

                    default : break;
                }

                lcd_fill(30, 180, 239, 190 + 16, WHITE);
                lcd_show_string(30, 180, 210, 16, 16, (char*)msgbuf, RED);                              /* ��ʾ��ʾ���� */
                delay_ms(800);
            }
            else
            {
                lcd_show_string(30, 210, 210, 16, 16, "Send data complete!!", RED);                     /* ��ʾ����������� */
            }
            
            g_jpeg_data_ok = 2;     /* ���jpeg���ݴ�������,������DMAȥ�ɼ���һ֡��. */
        }
    }
}

/**
 * @brief       RGB565����
 *   @ntoe      RGB����ֱ����ʾ��LCD����
 *
 * @param       ��
 * @retval      ��
 */
void rgb565_test(void)
{
    uint8_t key;
    uint8_t effect = 0, saturation = 3, contrast = 2;
    uint8_t scale = 1;          /* Ĭ����ȫ�ߴ����� */
    uint8_t msgbuf[15];         /* ��Ϣ������ */

    lcd_clear(WHITE);
    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "OV2640 RGB565 Mode", RED);
    lcd_show_string(30, 100, 200, 16, 16, "KEY0:Contrast", RED);            /* �Աȶ� */
    lcd_show_string(30, 120, 200, 16, 16, "KEY1:Saturation", RED);          /* ִ���Զ��Խ� */
    lcd_show_string(30, 140, 200, 16, 16, "KEY2:Effects", RED);             /* ��Ч */
    lcd_show_string(30, 160, 200, 16, 16, "KEY_UP:FullSize/Scale", RED);    /* 1:1�ߴ�(��ʾ��ʵ�ߴ�)/ȫ�ߴ����� */

    ov2640_rgb565_mode();       /* RGB565ģʽ */
    dcmi_init();                /* DCMI���� */
    dcmi_dma_init((uint32_t)&LCD->LCD_RAM, 0, 1, DMA_MDATAALIGN_HALFWORD, DMA_MINC_DISABLE); /* DCMI DMA����,MCU��,���� */
    ov2640_outsize_set(lcddev.width, lcddev.height);    /* ����������ʾ */
    dcmi_start();               /* �������� */
    
    while (1)
    {
        key = key_scan(0);

        if (key)
        {
            dcmi_stop();        /* ֹͣ��ʾ */
            switch (key)
            {
                case KEY0_PRES: /* �Աȶ����� */
                    contrast++;
                    if (contrast > 4)contrast = 0;
                    ov2640_contrast(contrast);
                    sprintf((char*)msgbuf, "Contrast:%d", (signed char)contrast - 2);
                    break;

                case KEY1_PRES: /* ���Ͷ����� */
                    saturation++;
                    if (saturation > 4)saturation = 0;
                    ov2640_color_saturation(saturation);    /* ���Ͷ����� */
                    sprintf((char *)msgbuf, "Saturation:%d", (signed char)saturation - 2);
                    break;
                
                case KEY2_PRES: /* ��Ч���� */
                    effect++;
                    if (effect > 6)effect = 0;
                    ov2640_special_effects(effect);         /* ������Ч */
                    sprintf((char *)msgbuf, "%s", EFFECTS_TBL[effect]);
                    break;

                case WKUP_PRES: /* 1:1�ߴ�(��ʾ��ʵ�ߴ�)/���� */
                    scale = !scale;

                    if (scale == 0)
                    {
                        ov2640_image_win_set((1600 - lcddev.width) / 2, (1200 - lcddev.height) / 2, lcddev.width, lcddev.height);
                        ov2640_outsize_set(lcddev.width, lcddev.height);
                        sprintf((char *)msgbuf, "Full Size 1:1");
                    }
                    else
                    {
                        ov2640_image_win_set(0, 0, 1600, 1200);
                        ov2640_outsize_set(lcddev.width, lcddev.height);
                        sprintf((char *)msgbuf, "Scale");
                    }
                    
                    break;

                default : break;
            }
            lcd_show_string(30, 50, 210, 16, 16, (char*)msgbuf, RED);   /* ��ʾ��ʾ���� */
            delay_ms(800);
            dcmi_start();   /* ���¿�ʼ���� */
        }
        delay_ms(10);
    }
}

int main(void)
{
    uint8_t key, t;

    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(84);                    /* ��ʼ��USMART */
    usart2_init(921600);                    /* ��ʼ������2������Ϊ921600 */
    led_init();                             /* ��ʼ��LED */
    lcd_init();                             /* ��ʼ��LCD */
    key_init();                             /* ��ʼ������ */
    btim_timx_int_init(10000 - 1, 8400 - 1);/* 10KHz����Ƶ�ʣ�1���ж�һ�� */
    
    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "OV2640 TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);

    while (ov2640_init())       /* ��ʼ��OV2640 */
    {
        lcd_show_string(30, 130, 240, 16, 16, "OV2640 ERROR", RED);
        delay_ms(200);
        lcd_fill(30, 130, 239, 170, WHITE);
        delay_ms(200);
        LED0_TOGGLE();
    }

    lcd_show_string(30, 130, 200, 16, 16, "OV2640 OK", RED);
    ov2640_flash_intctrl();     /* ����ƿ��� */
    
    while (1)
    {
        key = key_scan(0);

        if (key == KEY0_PRES)
        {
            g_ov_mode = 0;      /* RGB565ģʽ */
            break;
        }
        else if (key == KEY1_PRES)
        {
            g_ov_mode = 1;      /* JPEGģʽ */
            break;
        }

        t++;

        if (t == 100)lcd_show_string(30, 150, 230, 16, 16, "KEY0:RGB565  KEY1:JPEG", RED);  /* ��˸��ʾ��ʾ��Ϣ */

        if (t == 200)
        {
            lcd_fill(30, 150, 210, 150 + 16, WHITE);
            t = 0;
            LED0_TOGGLE();
        }

        delay_ms(5);
    }

    if (g_ov_mode == 1)
    {
        jpeg_test();
    }
    else
    {
        rgb565_test();
    }
}


