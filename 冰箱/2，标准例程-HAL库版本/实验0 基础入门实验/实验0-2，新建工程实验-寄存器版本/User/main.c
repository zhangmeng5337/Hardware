/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       �½�����ʵ��-�Ĵ����汾 ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨������ԭ�� ̽���� F407������
 * ������Ƶ��www.yuanzige.com
 * ������̳��http://www.openedv.com/forum.php
 * ��˾��ַ��www.alientek.com
 * �����ַ��zhengdianyuanzi.tmall.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"


int main(void)
{
    uint8_t t = 0;

    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(84, 115200);                 /* ���ڳ�ʼ��Ϊ115200 */

    while (1)
    {
        printf("t:%d\r\n", t);
        delay_ms(500);
        t++;
    }
}
