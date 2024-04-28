/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-20
 * @brief       FPU����(Julia����) ʵ��
 *   @note      45_1, ���汾Ϊ����Ӳ��FPU�汾
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
#include "./BSP/TIMER/btim.h"


/* FPUģʽ��ʾ */
#if __FPU_USED==1
#define SCORE_FPU_MODE                  "FPU On"
#else
#define SCORE_FPU_MODE                  "FPU Off"
#endif

#define     ITERATION           128     /* �������� */
#define     REAL_CONSTANT       0.285f  /* ʵ������ */
#define     IMG_CONSTANT        0.01f   /* �鲿���� */

/* ��ɫ�� */
uint16_t g_color_map[ITERATION];

/* ���������б� */
const uint16_t zoom_ratio[] =
{
    120, 110, 100, 150, 200, 275, 350, 450,
    600, 800, 1000, 1200, 1500, 2000, 1500,
    1200, 1000, 800, 600, 450, 350, 275, 200,
    150, 100, 110,
};

/**
 * @brief       ��ʼ����ɫ��
 * @param       clut     : ��ɫ��ָ��
 * @retval      ��
 */
void julia_clut_init(uint16_t *clut)
{
    uint32_t i = 0x00;
    uint16_t  red = 0, green = 0, blue = 0;

    for (i = 0; i < ITERATION; i++) /* ������ɫ�� */
    {
        /* ����RGB��ɫֵ */
        red = (i * 8 * 256 / ITERATION) % 256;
        green = (i * 6 * 256 / ITERATION) % 256;
        blue = (i * 4 * 256 / ITERATION) % 256;
        
        /* ��RGB888,ת��ΪRGB565 */
        red = red >> 3;
        red = red << 11;
        green = green >> 2;
        green = green << 5;
        blue = blue >> 3;
        clut[i] = red + green + blue;
    }
}

/* RGB LCD ����*/
uint16_t g_lcdbuf[800];

/**
 * @brief        ����Julia����ͼ��
 * @param       size_x   : ��Ļx����ĳߴ�
 * @param       size_y   : ��Ļy����ĳߴ�
 * @param       offset_x : ��Ļx�����ƫ��
 * @param       offset_y : ��Ļy�����ƫ��
 * @param       zoom     : ��������
 * @retval      ��
 */
void julia_generate_fpu(uint16_t size_x, uint16_t size_y, uint16_t offset_x, uint16_t offset_y, uint16_t zoom)
{
    uint8_t i;
    uint16_t x, y;
    float tmp1, tmp2;
    float num_real, num_img;
    float radius;

    for (y = 0; y < size_y; y++)
    {
        for (x = 0; x < size_x; x++)
        {
            num_real = y - offset_y;
            num_real = num_real / zoom;
            num_img = x - offset_x;
            num_img = num_img / zoom;
            i = 0;
            radius = 0;

            while ((i < ITERATION - 1) && (radius < 4))
            {
                tmp1 = num_real * num_real;
                tmp2 = num_img * num_img;
                num_img = 2 * num_real * num_img + IMG_CONSTANT;
                num_real = tmp1 - tmp2 + REAL_CONSTANT;
                radius = tmp1 + tmp2;
                i++;
            }

            LCD->LCD_RAM = g_color_map[i]; /* ���Ƶ���Ļ */
        }
    }
}

uint8_t g_timeout;

int main(void)
{
    uint8_t key;
    uint8_t i = 0;
    uint8_t autorun = 0;
    float time;
    char buf[50];
    
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                             /* ��ʼ��LED */
    key_init();                             /* ��ʼ������ */
    lcd_init();                             /* ��ʼ��LCD */

    btim_timx_int_init(65535, 8400 - 1);    /* 10Khz����Ƶ��,����ʱ6.5�볬�� */

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "FPU TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0:+    KEY1:-", RED);     /* ��ʾ��ʾ��Ϣ */
    lcd_show_string(30, 130, 200, 16, 16, "KEY_UP:AUTO/MANUL", RED);    /* ��ʾ��ʾ��Ϣ */
    delay_ms(500);
    julia_clut_init(g_color_map);           /* ��ʼ����ɫ�� */
    
    while (1)
    {
       key = key_scan(0);

        switch (key)
        {
            case KEY0_PRES:
                i++;

                if (i > sizeof(zoom_ratio) / 2 - 1)i = 0; /* ���Ʒ�Χ */

                break;

            case KEY1_PRES:
                if (i)i--;
                else i = sizeof(zoom_ratio) / 2 - 1;

                break;

            case WKUP_PRES:
                autorun = !autorun; /* �Զ�/�ֶ� */
                break;
            default:break;
        }

        if (autorun == 1)   /* �Զ�ʱ,�Զ������������� */
        {
            i++;
            LED1(0);        /* ����LED1���Զ����� */
            if (i > sizeof(zoom_ratio) / 2 - 1)
            {
                i = 0;      /* ���Ʒ�Χ */
            }
        }
        else {
            LED1(1);        /* Ϩ��LED1���ֶ����� */
        }
        lcd_set_window(0, 0, lcddev.width, lcddev.height);  /* ���ô��� */
        lcd_write_ram_prepare();
        
        BTIM_TIMX_INT->CNT = 0;  /* ����TIM3��ʱ���ļ�����ֵ */
        g_timeout = 0;
        
        julia_generate_fpu(lcddev.width, lcddev.height, lcddev.width / 2, lcddev.height / 2, zoom_ratio[i]);
        
        time = BTIM_TIMX_INT->CNT + (uint32_t)g_timeout * 65536;
        
        sprintf(buf, "%s: zoom:%d  runtime:%0.1fms\r\n", SCORE_FPU_MODE, zoom_ratio[i], time / 10);
        lcd_show_string(5, lcddev.height - 5 - 12, lcddev.width - 5, 12, 12, buf, RED); /* ��ʾ��ǰ������� */
        printf("%s", buf); /* ��������� */
        LED0_TOGGLE();
    }
}



