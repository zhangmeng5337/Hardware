/**
 ****************************************************************************************************
 * @file        usart.h
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

#ifndef _USART2_H
#define _USART2_H

#include "stdio.h"
#include "./SYSTEM/sys/sys.h"

/*******************************************************************************************************/
/* 引脚 和 串口 定义 
 * 默认是针对USART2的.
 */
 
#define USART2_TX_GPIO_PORT              GPIOA
#define USART2_TX_GPIO_PIN               GPIO_PIN_2
#define USART2_TX_GPIO_AF                GPIO_AF7_USART2
#define USART2_TX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* 发送引脚时钟使能 */

#define USART2_RX_GPIO_PORT              GPIOA
#define USART2_RX_GPIO_PIN               GPIO_PIN_3
#define USART2_RX_GPIO_AF                GPIO_AF7_USART2
#define USART2_RX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* 接收引脚时钟使能 */

#define USART2_UX                        USART2
#define USART2_UX_IRQn                   USART2_IRQn
#define USART2_UX_IRQHandler             USART2_IRQHandler
#define USART2_UX_CLK_ENABLE()           do{ __HAL_RCC_USART2_CLK_ENABLE(); }while(0)  /* USART1 时钟使能 */

/*******************************************************************************************************/

extern UART_HandleTypeDef g_uart2_handle;       /* UART句柄 */

void usart2_init(uint32_t baudrate);            /* 串口初始化函数 */

#endif







