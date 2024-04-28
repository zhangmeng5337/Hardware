/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-19
 * @brief       �߼���ʱ�����ָ������PWM ʵ��
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
#include "./BSP/KEY/key.h"
#include "./BSP/TIMER/atim.h"


int main(void)
{
    uint8_t key = 0;
    uint8_t t = 0;
    GPIO_InitTypeDef gpio_init_struct;
    
    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);         /* ����ʱ��,168Mhz */
    delay_init(168);                            /* ��ʱ��ʼ�� */
    usart_init(115200);                         /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                                 /* ��ʼ��LED */
    key_init();                                 /* ��ʼ������ */
    
    /* �� LED1 ��������Ϊ����ģʽ, �����PC6��ͻ */
    gpio_init_struct.Pin = LED1_GPIO_PIN;                   /* LED1���� */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                /* ����Ϊ���� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* ����ģʽ */
    HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��LED1���� */
    
    atim_timx_npwm_chy_init(10000 - 1, 8400 - 1);           /* 20Khz�ļ���Ƶ��,2Hz��PWMƵ��. */

    ATIM_TIMX_NPWM_CHY_CCRX = 5000; /* ����PWMռ�ձ�,50%,�������Կ���ÿһ��PWM����,LED1(GREEN)
                                     * ��һ��ʱ��������,һ��ʱ�������,LED1����һ��,��ʾһ��PWM��
                                     */
    atim_timx_npwm_chy_set(5);      /* ���5��PWM��(����LED1(GREEN)��˸5��) */
    
    while (1)
    {
       key = key_scan(0);

        if (key == KEY0_PRES)           /* KEY0���� */
        {
            atim_timx_npwm_chy_set(5);  /* ���5��PWM��(����TIM8_CH1, ��PC6���5������) */
        }

        t++;
        delay_ms(10);

        if (t > 50)                     /* ����LED0��˸, ��ʾ��������״̬ */
        {
            t = 0;
            LED0_TOGGLE();
        }
    }
}
