/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       �������� ʵ��
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
#include "./BSP/BEEP/beep.h"
#include "./BSP/KEY/key.h"


int main(void)
{
    uint8_t key;
    
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    led_init();                             /* ��ʼ��LED */
    beep_init();                            /* ��ʼ�������� */
    key_init();                             /* ��ʼ������ */
    LED0(0);                                /* �ȵ������ */

    while(1)
    {
        key = key_scan(0);                  /* �õ���ֵ */

        if (key)
        {
            switch (key)
            {
                case WKUP_PRES:             /* ���Ʒ����� */
                    BEEP_TOGGLE();          /* BEEP״̬ȡ�� */
                    break;

                case KEY0_PRES:             /* ����LED0(RED)��ת */
                    LED0_TOGGLE();          /* LED0״̬ȡ�� */
                    break;

                case KEY1_PRES:             /* ����LED1(GREEN)��ת */
                    LED1_TOGGLE();          /* LED1״̬ȡ�� */
                    break;

                case KEY2_PRES:             /* ͬʱ����LED0, LED1��ת */
                    LED0_TOGGLE();          /* LED0״̬ȡ�� */
                    LED1_TOGGLE();          /* LED1״̬ȡ�� */
                    break;

                default : break;
                }
        }
        else
        {
            delay_ms(10);
        }
    }
}

