/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-19
 * @brief       �߼���ʱ������Ƚ�ģʽ ʵ��
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
    uint8_t t = 0;
    
    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);         /* ����ʱ��,168Mhz */
    delay_init(168);                            /* ��ʱ��ʼ�� */
    usart_init(115200);                         /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                                 /* ��ʼ��LED */
    key_init();                                 /* ��ʼ������ */
    
    atim_timx_comp_pwm_init(1000 - 1, 168 - 1); /* 1Mhz�ļ���Ƶ��, 1Khz������. */

    ATIM_TIMX_COMP_CH1_CCRX = 250 - 1;          /* ͨ��1 ��λ25% */
    ATIM_TIMX_COMP_CH2_CCRX = 500 - 1;          /* ͨ��2 ��λ50% */
    ATIM_TIMX_COMP_CH3_CCRX = 750 - 1;          /* ͨ��3 ��λ75% */
    ATIM_TIMX_COMP_CH4_CCRX = 1000 - 1;         /* ͨ��4 ��λ100% */
    
    while (1)
    {
        t++;
        delay_ms(10);

        if (t > 50)                             /* ����LED0��˸, ��ʾ��������״̬ */
        {
            t = 0;
            LED0_TOGGLE();
        }
    }
}
