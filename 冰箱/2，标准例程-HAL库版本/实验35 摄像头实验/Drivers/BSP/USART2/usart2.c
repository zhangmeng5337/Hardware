/**
 ****************************************************************************************************
 * @file        usart2.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-09
 * @brief       串口初始化代码(串口2)
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211109
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "./BSP/USART2/usart2.h"


UART_HandleTypeDef g_uart2_handle;      /* UART句柄 */
UART_HandleTypeDef USART2_Handler;  //USART2句柄

/**
 * @brief       串口2初始化函数
 * @param       baudrate: 波特率, 根据自己需要设置波特率值
 * @note        注意: 必须设置正确的时钟源, 否则串口波特率就会设置异常.
 *              这里的USART的时钟源在sys_stm32_clock_init()函数中已经设置过了.
 * @retval      无
 */
void usart2_init(uint32_t baudrate)
{
    GPIO_InitTypeDef gpio_init_struct;

    USART2_UX_CLK_ENABLE();                                     /* USART2 时钟使能 */
    USART2_TX_GPIO_CLK_ENABLE();                                /* 发送引脚时钟使能 */
    USART2_RX_GPIO_CLK_ENABLE();                                /* 接收引脚时钟使能 */

    gpio_init_struct.Pin = USART2_TX_GPIO_PIN;                  /* TX引脚 */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                    /* 复用推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                        /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
    gpio_init_struct.Alternate = USART2_TX_GPIO_AF;             /* 复用为USART1 */
    HAL_GPIO_Init(USART2_TX_GPIO_PORT, &gpio_init_struct);      /* 初始化发送引脚 */

    gpio_init_struct.Pin = USART2_RX_GPIO_PIN;                  /* RX引脚 */
    gpio_init_struct.Alternate = USART2_RX_GPIO_AF;             /* 复用为USART2 */
    HAL_GPIO_Init(USART2_RX_GPIO_PORT, &gpio_init_struct);      /* 初始化接收引脚 */
    
    g_uart2_handle.Instance = USART2_UX;                        /* USART2 */
    g_uart2_handle.Init.BaudRate = baudrate;                    /* 波特率 */
    g_uart2_handle.Init.WordLength = UART_WORDLENGTH_8B;        /* 字长为8位数据格式 */
    g_uart2_handle.Init.StopBits = UART_STOPBITS_1;             /* 一个停止位 */
    g_uart2_handle.Init.Parity = UART_PARITY_NONE;              /* 无奇偶校验位 */
    g_uart2_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;        /* 无硬件流控 */
    g_uart2_handle.Init.Mode = UART_MODE_TX;                 /* 收发模式 */
    HAL_UART_Init(&g_uart2_handle);                             /* HAL_UART_Init()会使能UART2 */
    
}
 




