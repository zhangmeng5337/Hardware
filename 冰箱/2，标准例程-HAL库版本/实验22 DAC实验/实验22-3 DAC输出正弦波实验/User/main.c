/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-03
 * @brief       DAC�����Ҳ� ʵ��
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
#include "math.h"


uint16_t g_dac_sin_buf[4096];           /* �������ݻ����� */

/**
 * @brief       �������Ҳ�������
 *   @note      �豣֤ : maxval > samples/2
 * @param       maxval : ���ֵ(0 < maxval < 2048)
 * @param       samples: ������ĸ���
 * @retval      ��
 */
void dac_creat_sin_buf(uint16_t maxval, uint16_t samples)
{
    uint8_t i;
    float inc = (2 * 3.1415962) / samples;      /* ����������һ������DAC_SIN_BUF���㣩 */
    float outdata = 0;

    for (i = 0; i < samples; i++)
    {
        outdata = maxval * (1 + sin(inc * i));  /* ������dots����Ϊ���ڵ�ÿ�����ֵ���Ŵ�maxval������ƫ�Ƶ��������� */
        if (outdata > 4095)
            outdata = 4095;                     /* �����޶� */
        //printf("%f\r\n", outdata);
        g_dac_sin_buf[i] = outdata;
    }
}

/**
 * @brief       ͨ��USMART�������Ҳ��������,�����޸����Ƶ��.
 * @param       arr : TIM7���Զ���װ��ֵ
 * @param       psc : TIM7�ķ�Ƶϵ��
 * @retval      ��
 */
void dac_dma_sin_set(uint16_t arr, uint16_t psc)
{
    dac_dma_wave_enable(1, 100, arr, psc);
}

int main(void)
{
    uint8_t t = 0;
    uint8_t key;
    float temp;
    uint16_t adcx;
    
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(84);                    /* ��ʼ��USMART */
    led_init();                             /* ��ʼ��LED */
    lcd_init();                             /* ��ʼ��LCD */
    key_init();                             /* ��ʼ������ */
    adc_init();                             /* ��ʼ��ADC */
    dac_dma_wave_init(1);                   /* ��ʼ��DACͨ��1 DMA������� */

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "DAC DMA Sine WAVE TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0:3Khz  KEY1:30Khz", RED);

    lcd_show_string(30, 130, 200, 16, 16, "DAC VAL:", BLUE);
    lcd_show_string(30, 150, 200, 16, 16, "DAC VOL:0.000V", BLUE);
    lcd_show_string(30, 170, 200, 16, 16, "ADC VOL:0.000V", BLUE);

    dac_creat_sin_buf(2048, 100);

    dac_dma_wave_enable(1, 100, 10 - 1, 84 - 1);

    while (1)
    {
        t++;
        key = key_scan(0);  /* ����ɨ�� */

        if (key == KEY0_PRES)                               /* �߲����� , Լ3Khz���� */
        {
            dac_creat_sin_buf(2048, 100);                   /* �������Ҳ������� */
            dac_dma_wave_enable(1, 100, 10 - 1, 28 - 1);    /* 300Khz����Ƶ��, 100����, ���Եõ����3KHz�����Ҳ�. */
        }
        else if (key == KEY1_PRES)                          /* �Ͳ����� , Լ30Khz���� */
        {
            dac_creat_sin_buf(2048, 10);                    /* �������Ҳ������� */
            dac_dma_wave_enable(1, 10, 10 - 1, 28 - 1);     /* 300Khz����Ƶ��, 10����, ���Եõ����30KHz�����Ҳ�. */
        }

        adcx = DAC1->DHR12R1;                               /* ��ȡDAC1_OUT1�����״̬ */
        lcd_show_xnum(94, 130, adcx, 4, 16, 0, BLUE);       /* ��ʾDAC�Ĵ���ֵ */

        temp = (float)adcx * (3.3 / 4096);                  /* �õ�DAC��ѹֵ */
        adcx = temp;
        lcd_show_xnum(94, 150, temp, 1, 16, 0, BLUE);       /* ��ʾ��ѹֵ�������� */

        temp -= adcx;
        temp *= 1000;
        lcd_show_xnum(110, 150, temp, 3, 16, 0x80, BLUE);   /* ��ʾ��ѹֵ��С������ */

        adcx = adc_get_result_average(ADC_ADCX_CHY, 20);    /* �õ�ADCͨ��5��ת����� */
        temp = (float)adcx * (3.3 / 4096);                  /* �õ�ADC��ѹֵ(adc��162it��) */
        adcx = temp;
        lcd_show_xnum(94, 170, temp, 1, 16, 0, BLUE);       /* ��ʾ��ѹֵ�������� */

        temp -= adcx;
        temp *= 1000;
        lcd_show_xnum(110, 170, temp, 3, 16, 0x80, BLUE);   /* ��ʾ��ѹֵ��С������ */

        if (t == 10)        /* ��ʱʱ�䵽�� */
        {
            LED0_TOGGLE();  /* LED0��˸ */
            t = 0;
        }

        delay_ms(5);
    }
}



