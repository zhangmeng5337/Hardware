/**
 ****************************************************************************************************
 * @file        usart2.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-09
 * @brief       ���ڳ�ʼ������(����2)
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
 * V1.0 20211109
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/USART2/usart2.h"


UART_HandleTypeDef g_uart2_handle;      /* UART��� */
UART_HandleTypeDef USART2_Handler;  //USART2���

/**
 * @brief       ����2��ʼ������
 * @param       baudrate: ������, �����Լ���Ҫ���ò�����ֵ
 * @note        ע��: ����������ȷ��ʱ��Դ, ���򴮿ڲ����ʾͻ������쳣.
 *              �����USART��ʱ��Դ��sys_stm32_clock_init()�������Ѿ����ù���.
 * @retval      ��
 */
void usart2_init(uint32_t baudrate)
{
    GPIO_InitTypeDef gpio_init_struct;

    USART2_UX_CLK_ENABLE();                                     /* USART2 ʱ��ʹ�� */
    USART2_TX_GPIO_CLK_ENABLE();                                /* ��������ʱ��ʹ�� */
    USART2_RX_GPIO_CLK_ENABLE();                                /* ��������ʱ��ʹ�� */

    gpio_init_struct.Pin = USART2_TX_GPIO_PIN;                  /* TX���� */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                    /* ����������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                        /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    gpio_init_struct.Alternate = USART2_TX_GPIO_AF;             /* ����ΪUSART1 */
    HAL_GPIO_Init(USART2_TX_GPIO_PORT, &gpio_init_struct);      /* ��ʼ���������� */

    gpio_init_struct.Pin = USART2_RX_GPIO_PIN;                  /* RX���� */
    gpio_init_struct.Alternate = USART2_RX_GPIO_AF;             /* ����ΪUSART2 */
    HAL_GPIO_Init(USART2_RX_GPIO_PORT, &gpio_init_struct);      /* ��ʼ���������� */
    
    g_uart2_handle.Instance = USART2_UX;                        /* USART2 */
    g_uart2_handle.Init.BaudRate = baudrate;                    /* ������ */
    g_uart2_handle.Init.WordLength = UART_WORDLENGTH_8B;        /* �ֳ�Ϊ8λ���ݸ�ʽ */
    g_uart2_handle.Init.StopBits = UART_STOPBITS_1;             /* һ��ֹͣλ */
    g_uart2_handle.Init.Parity = UART_PARITY_NONE;              /* ����żУ��λ */
    g_uart2_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;        /* ��Ӳ������ */
    g_uart2_handle.Init.Mode = UART_MODE_TX;                 /* �շ�ģʽ */
    HAL_UART_Init(&g_uart2_handle);                             /* HAL_UART_Init()��ʹ��UART2 */
    
}
 




