/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-18
 * @brief       PVD��ѹ��� ʵ��
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
#include "./BSP/PWR/pwr.h"


int main(void)
{
    uint8_t t = 0;
    
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                             /* ��ʼ��LED */
    lcd_init();                             /* ��ʼ��LCD */
    pwr_pvd_init(PWR_PVDLEVEL_7);           /* PVD 2.9V��� */
    
    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "PVD TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    /* Ĭ��LCD��ʾ��ѹ���� */
    lcd_show_string(30, 110, 200, 16, 16, "PVD Voltage OK! ", BLUE);
    
    while (1)
    {
        if ((t % 20) == 0)
        {
            LED0_TOGGLE();      /* ÿ200ms,��תһ��LED0 */
        }

        delay_ms(10);
        t++;
    }
}
