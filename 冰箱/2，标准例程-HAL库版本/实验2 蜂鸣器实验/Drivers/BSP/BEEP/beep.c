/**
 ****************************************************************************************************
 * @file        beep.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       ������ ��������
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
 * �޸�˵��
 * V1.0 20211014
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/BEEP/beep.h"


/**
 * @brief       ��ʼ��BEEP���IO��, ��ʹ��ʱ��
 * @param       ��
 * @retval      ��
 */
void beep_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    BEEP_GPIO_CLK_ENABLE();                             /* BEEPʱ��ʹ�� */

    gpio_init_struct.Pin = BEEP_GPIO_PIN;               /* ���������� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;        /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* ���� */
    HAL_GPIO_Init(BEEP_GPIO_PORT, &gpio_init_struct);   /* ��ʼ������������ */

    BEEP(0);                                            /* �رշ����� */
}






