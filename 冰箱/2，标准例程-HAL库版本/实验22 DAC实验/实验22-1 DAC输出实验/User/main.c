/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-23
 * @brief       DAC��� ʵ��
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
#include "./BSP/ADC/adc.h"


extern DAC_HandleTypeDef g_dac1_handle;

int main(void)
{
    uint16_t adcx;
    float temp;
    uint8_t t = 0;
    uint16_t dacval = 0;
    uint8_t key;

    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7); /* ����ʱ��,168Mhz */
    delay_init(168);                    /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(84);                /* ��ʼ��USMART */
    led_init();                         /* ��ʼ��LED */
    lcd_init();                         /* ��ʼ��LCD */
    key_init();                         /* ��ʼ������ */
    adc_init();                         /* ��ʼ��ADC3 */
    dac_init(1);                        /* ��ʼ��DAC1_OUT1ͨ�� 1:DAC_OUT_1-PA4  2:DAC_OUT_2-PA5 */

    lcd_show_string(30,  50, 200, 16, 16, "STM32F103", RED);
    lcd_show_string(30,  70, 200, 16, 16, "DAC TEST", RED);
    lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "WK_UP:+  KEY1:-", RED);
    
    lcd_show_string(30, 130, 200, 16, 16, "DAC VAL:", BLUE);
    lcd_show_string(30, 150, 200, 16, 16, "DAC VOL:0.000V", BLUE);
    lcd_show_string(30, 170, 200, 16, 16, "ADC VOL:0.000V", BLUE);

    while (1)
    {
        t++;
        key = key_scan(0);  /* ����ɨ�� */

        if (key == WKUP_PRES)
        {
            if (dacval < 4000)dacval += 200;

            HAL_DAC_SetValue(&g_dac1_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacval);   /* �������200 */
        }
        else if (key == KEY1_PRES)
        {
            if (dacval > 200)dacval -= 200;
            else dacval = 0;

            HAL_DAC_SetValue(&g_dac1_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacval);   /* �������200 */
        }

        if (t == 10 || key == KEY1_PRES || key == WKUP_PRES)        /* WKUP/KEY1������,���߶�ʱʱ�䵽�� */
        {
            adcx = HAL_DAC_GetValue(&g_dac1_handle, DAC_CHANNEL_1); /* ��ȡǰ������DAC1_OUT1��ֵ */
            lcd_show_xnum(94, 130, adcx, 4, 16, 0, BLUE);           /* ��ʾDAC�Ĵ���ֵ */
            
            temp = (float)adcx * (3.3 / 4096);                      /* �õ�DAC��ѹֵ */
            adcx = temp;
            lcd_show_xnum(94, 150, temp, 1, 16, 0, BLUE);           /* ��ʾ��ѹֵ�������� */
            
            temp -= adcx;
            temp *= 1000;
            lcd_show_xnum(110, 150, temp, 3, 16, 0X80, BLUE);       /* ��ʾ��ѹֵ��С������ */
            
            adcx = adc_get_result_average(ADC_ADCX_CHY, 20);        /* �õ�ADC1ͨ��5��ת����� */
            temp = (float)adcx * (3.3 / 4096);                      /* �õ�ADC��ѹֵ(adc��12bit��) */
            adcx = temp;
            lcd_show_xnum(94, 170, temp, 1, 16, 0, BLUE);           /* ��ʾ��ѹֵ�������� */
            
            temp -= adcx;
            temp *= 1000;
            lcd_show_xnum(110, 170, temp, 3, 16, 0X80, BLUE);       /* ��ʾ��ѹֵ��С������ */
            
            LED0_TOGGLE();  /* LED0��˸ */
            t = 0;
        }

        delay_ms(10);
    }
}


