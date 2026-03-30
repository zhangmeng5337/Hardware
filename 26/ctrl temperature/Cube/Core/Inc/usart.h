/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stdio.h"
/* USER CODE END Includes */

extern UART_HandleTypeDef huart4;

extern UART_HandleTypeDef huart5;

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart2;

extern UART_HandleTypeDef huart3;

extern UART_HandleTypeDef huart6;

/* USER CODE BEGIN Private defines */

extern uint16_t RS485_RX_CNT;//接收计数器
///////串口1
#define BUFFER_SIZE  128  
#define BUFFER_SIZE1  256  
//FIFO
extern volatile uint8_t usart1_rx_len;               //接收一帧数据的长度
extern uint8_t  usart1_rx_buffer[BUFFER_SIZE];       //接收数据缓存数组

extern  volatile uint8_t rx_len1 ;  //接收一帧数据的长度
extern volatile uint8_t recv_end_flag1; //一帧数据接收完成标志
extern uint8_t rx_buffer1[100];  //接收数据缓存数组
void DMA_Usart_Send1(uint8_t *buf,uint8_t len);//串口发送封装
void DMA_Usart1_Read(uint8_t *Data,uint8_t len);//串口接收封装

///////串口2
extern  volatile uint8_t rx_len2 ;  //接收一帧数据的长度
extern volatile uint8_t recv_end_flag2; //一帧数据接收完成标志
extern uint8_t rx_buffer2[BUFFER_SIZE1];  //接收数据缓存数组
void DMA_Usart_Send2(uint8_t *buf,uint8_t len);//串口发送封装
void DMA_Usart2_Read(uint8_t *Data,uint8_t len);//串口接收封装


///////串口3
extern  volatile uint8_t rx_len3 ;  //接收一帧数据的长度
extern volatile uint8_t recv_end_flag3; //一帧数据接收完成标志
extern uint8_t rx_buffer3[BUFFER_SIZE];  //接收数据缓存数组
void DMA_Usart_Send3(uint8_t *buf,uint8_t len);//串口发送封装
void DMA_Usart3_Read(uint8_t *Data,uint8_t len);//串口接收封装
//////////

///////串口4
extern  volatile uint8_t rx_len4 ;  //接收一帧数据的长度
extern volatile uint8_t recv_end_flag4; //一帧数据接收完成标志
extern uint8_t rx_buffer4[BUFFER_SIZE];  //接收数据缓存数组、
void DMA_Usart_Send4(uint8_t *buf,uint8_t len);//串口发送封装
////////////

///////串口5
extern  volatile uint8_t rx_len5 ;  //接收一帧数据的长度
extern volatile uint8_t recv_end_flag5; //一帧数据接收完成标志
extern uint8_t rx_buffer5[BUFFER_SIZE];  //接收数据缓存数组
void DMA_Usart_Send5(uint8_t *buf,uint8_t len);//串口发送封装
void DMA_Usart5_Read(uint8_t *Data,uint8_t len);//串口接收封装


///////串口6
extern  volatile uint8_t rx_len6 ;  //接收一帧数据的长度
extern volatile uint8_t recv_end_flag6; //一帧数据接收完成标志
extern uint8_t rx_buffer6[BUFFER_SIZE];  //接收数据缓存数组
void DMA_Usart_Send6(uint8_t *buf,uint8_t len);//串口发送封装
void DMA_Usart6_Read(uint8_t *Data,uint8_t len);//串口接收封装
/* USER CODE END Private defines */

void MX_UART4_Init(void);
void MX_UART5_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);
void MX_USART6_UART_Init(void);

/* USER CODE BEGIN Prototypes */
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

