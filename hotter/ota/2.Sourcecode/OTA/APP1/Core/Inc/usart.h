/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
extern DMA_HandleTypeDef hdma_lpuart1_rx;
#define LPUART1_DMA_REC_SIZE 1200
#define LPUART1_REC_SIZE 2000
	
/*定义串口成员变量的结构体指针*/
//#pack(1)
typedef struct
{
	uint8_t  	Lpuart1RecFlag;
	uint16_t 	Lpuart1DMARecLen;
	uint16_t 	Lpuart1RecLen;
	uint8_t 	Lpuart1DMARecBuff[LPUART1_DMA_REC_SIZE];
	uint8_t		Lpuart1RecBuff[LPUART1_REC_SIZE];
}tsLpuart1type;
//#pack(0)
extern tsLpuart1type Lpuart1type;
/* USER CODE END Private defines */

void MX_LPUART1_UART_Init(void);
void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
extern void EnableUartIT(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
