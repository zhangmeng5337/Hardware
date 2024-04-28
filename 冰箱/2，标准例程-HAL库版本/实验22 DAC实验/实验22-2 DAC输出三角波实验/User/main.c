/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-23
 * @brief       DAC������ǲ� ʵ��
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
#include "./BSP/DAC/dac.h"


extern DAC_HandleTypeDef g_dac1_handle;

int main(void)
{
    uint8_t t = 0;
    uint8_t key;

    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(84);                    /* ��ʼ��USMART */
    led_init();                             /* ��ʼ��LED */
    lcd_init();                             /* ��ʼ��LCD */
    key_init();                             /* ��ʼ������ */
    dac_init(1);                            /* ��ʼ��DAC1_OUT1ͨ�� 1:DAC_OUT_1-PA4  2:DAC_OUT_2-PA5 */

    lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30,  70, 200, 16, 16, "DAC Triangular WAVE TEST", RED);
    lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0:Wave1  KEY1:Wave2", RED);
    lcd_show_string(30, 130, 200, 16, 16, "DAC None", BLUE);    /* ��ʾ����� */

    while (1)
    {
        t++;
        key = key_scan(0);      /* ����ɨ�� */

        if (key == KEY0_PRES)   /* �߲����� , Լ100Hz���� */
        {
            lcd_show_string(30, 130, 200, 16, 16, "DAC Wave1 ", BLUE);
            dac_triangular_wave(4095, 5, 2000, 100);    /* ��ֵ4095, ��������5us, 2000��������, 100������ */
            lcd_show_string(30, 130, 200, 16, 16, "DAC None  ", BLUE);
        }
        else if (key == KEY1_PRES)  /* �Ͳ����� , Լ100Hz���� */
        {
            lcd_show_string(30, 130, 200, 16, 16, "DAC Wave2 ", BLUE);
            dac_triangular_wave(4095, 500, 20, 100);    /* ��ֵ4095, ��������500us, 20��������, 100������ */
            lcd_show_string(30, 130, 200, 16, 16, "DAC None  ", BLUE);
        }

        if (t == 10)        /* ��ʱʱ�䵽�� */
        {
            LED0_TOGGLE();  /* LED0��˸ */
            t = 0;
        }

        delay_ms(10);
    }
}



