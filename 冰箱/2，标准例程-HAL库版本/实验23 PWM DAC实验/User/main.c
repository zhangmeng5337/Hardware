/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-03
 * @brief       PWM DAC ʵ��
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
#include "./BSP/PWMDAC/pwmdac.h"
#include "./BSP/ADC/adc.h"


extern TIM_HandleTypeDef g_tim9_handler;

int main(void)
{
    uint16_t adcx;
    float temp;
    uint8_t t = 0;
    uint8_t key;
    uint16_t pwmval = 0;
    
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(84);                    /* ��ʼ��USMART */
    led_init();                             /* ��ʼ��LED */
    lcd_init();                             /* ��ʼ��LCD */
    key_init();                             /* ��ʼ������ */
    adc_init();                             /* ��ʼ��ADC */
    pwmdac_init(256 - 1, 0);                /* PWM DAC��ʼ��, Fpwm = 84 / 256 =328.125Khz */

    lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30,  70, 200, 16, 16, "PWM DAC TEST", RED);
    lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY_UP:+  KEY1:-", RED);
    lcd_show_string(30, 130, 200, 16, 16, "PWM VAL:", BLUE);
    lcd_show_string(30, 150, 200, 16, 16, "DAC VOL:0.000V", BLUE);
    lcd_show_string(30, 170, 200, 16, 16, "ADC VOL:0.000V", BLUE);
    
    __HAL_TIM_SET_COMPARE(&g_tim9_handler, PWMDAC_TIMX_CHY, pwmval);    /* ����µ�PWMռ�ձ� */
    
    while (1)
    {
        t++;
        key = key_scan(0);          /* ����ɨ�� */
        if (key == WKUP_PRES)       /* PWMռ�ձȵ��� */
        {
            if (pwmval < 250)       /* ��Χ�޶� */
            {
                pwmval += 10;
            }
            __HAL_TIM_SET_COMPARE(&g_tim9_handler, PWMDAC_TIMX_CHY, pwmval);    /* ����µ�PWMռ�ձ� */
        }
        else if (key == KEY1_PRES)  /* PWMռ�ձȵ��� */
        {
            if (pwmval > 10)        /* ��Χ�޶� */
            {
                pwmval -= 10;
            }
            else
            {
                pwmval = 0;
            }
             __HAL_TIM_SET_COMPARE(&g_tim9_handler, PWMDAC_TIMX_CHY, pwmval);   /* ����µ�PWMռ�ձ� */
        }

        if (t == 10 || key == KEY1_PRES || key == WKUP_PRES)                    /* WKUP / KEY1 ������, ���߶�ʱʱ�䵽�� */
        {
            adcx = __HAL_TIM_GET_COMPARE(&g_tim9_handler,PWMDAC_TIMX_CHY);      /* PWM DAC ��ʱ������Ƚ�ֵ */
            lcd_show_xnum(94, 130, adcx, 3, 16, 0, BLUE);                       /* ��ʾCCRX�Ĵ���ֵ */

            temp = (float)adcx * (3.3 / 256);                   /* �õ�DAC��ѹֵ */
            adcx = temp;
            lcd_show_xnum(94, 150, temp, 1, 16, 0, BLUE);       /* ��ʾ��ѹֵ�������� */

            temp -= adcx;
            temp *= 1000;
            lcd_show_xnum(110, 150, temp, 3, 16, 0X80, BLUE);   /* ��ʾ��ѹֵ��С������ */

            adcx = adc_get_result_average(ADC_ADCX_CHY, 10);    /* �õ�ADC1ͨ��5��ת����� */
            temp = (float)adcx * (3.3 / 4096);                  /* �õ�ADC��ѹֵ(adc��12bit��) */
            adcx = temp;
            lcd_show_xnum(94, 170, temp, 1, 16, 0, BLUE);       /* ��ʾ��ѹֵ�������� */

            temp -= adcx;
            temp *= 1000;
            lcd_show_xnum(110, 170, temp, 3, 16, 0X80, BLUE);   /* ��ʾ��ѹֵ��С������ */

            LED0_TOGGLE();  /* LED0��˸ */
            t = 0;
        }

        delay_ms(5);
    }
}



