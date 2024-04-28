/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-15
 * @brief       ͨ�ö�ʱ�����벶�� ʵ��
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
#include "./BSP/TIMER/gtim.h"


extern uint8_t  g_timxchy_cap_sta;              /* ���벶��״̬ */
extern uint16_t g_timxchy_cap_val;              /* ���벶��ֵ */

int main(void)
{
    uint32_t temp = 0;
    uint8_t t = 1;

    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);         /* ����ʱ��,168Mhz */
    delay_init(168);                            /* ��ʱ��ʼ�� */
    usart_init(115200);                         /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                                 /* ��ʼ��LED */
    key_init();                                 /* ��ʼ������ */
    gtim_timx_cap_chy_init(0xFFFF, 84 - 1);     /* ��1Mhz��Ƶ�ʼ��� ���� */

    while (1)
    {
        if (g_timxchy_cap_sta & 0x80)           /* �ɹ�������һ�θߵ�ƽ */
        {
            temp = g_timxchy_cap_sta & 0x3F;
            temp *= 0xFFFF;                     /* ���ʱ���ܺ� */
            temp += g_timxchy_cap_val;          /* �õ��ܵĸߵ�ƽʱ�� */
            printf("HIGH:%d us\r\n", temp);     /* ��ӡ�ܵĸߵ�ƽʱ�� */
            g_timxchy_cap_sta = 0;              /* ������һ�β��� */
        }

        t++;

        if (t > 20)                             /* 200ms����һ�� */
        {
            t = 0;
            LED0_TOGGLE();                      /* LED0��˸ ,��ʾ�������� */
        }
        delay_ms(10);
    }
}
