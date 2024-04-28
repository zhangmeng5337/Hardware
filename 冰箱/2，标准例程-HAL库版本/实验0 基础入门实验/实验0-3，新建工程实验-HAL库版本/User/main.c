/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       template ʵ��
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


void led_init(void);                    /* LED��ʼ���������� */

int main(void)
{
    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7); /* ����ʱ��,168Mhz */
    delay_init(168);                    /* ��ʱ��ʼ�� */
    led_init();                         /* ��ʼ��LED */
    
    while(1)
    {
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_RESET);     /* LED0 �� */
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_SET);      /* LED1 �� */
        delay_ms(500);
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_SET);       /* LED0 �� */
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_RESET);    /* LED1 �� */
        delay_ms(500);
    }
}

/**
 * @brief       ��ʼ��LED���IO��, ��ʹ��ʱ��
 * @param       ��
 * @retval      ��
 */
void led_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    __HAL_RCC_GPIOF_CLK_ENABLE();                       /* LED0��LED1ʱ��ʹ�� */

    gpio_init_struct.Pin = GPIO_PIN_9;                  /* LED0���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;        /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* ���� */
    HAL_GPIO_Init(GPIOF, &gpio_init_struct);            /* ��ʼ��LED0���� */

    gpio_init_struct.Pin = GPIO_PIN_10;                 /* LED1���� */
    HAL_GPIO_Init(GPIOF, &gpio_init_struct);            /* ��ʼ��LED1���� */

    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET); /* LED0 �� */
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);/* LED1 �� */
}
