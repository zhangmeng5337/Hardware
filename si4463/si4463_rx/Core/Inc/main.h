/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SDN_Pin GPIO_PIN_2
#define SDN_GPIO_Port GPIOA
#define nSEL_Pin GPIO_PIN_3
#define nSEL_GPIO_Port GPIOA
#define nIRQ_Pin GPIO_PIN_4
#define nIRQ_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
#define 	tx_normal 		0
#define 	rx_normal 		1
#define 	dr_1p2			0
#define 	dr_2p4			1
#define 	dr_4p8			2	
#define 	dr_9p6			3
#define		dr_19p2			4
#define		dr_38p4			5
#define		dr_76p8			6
#define 	dr_115p2		7
#define 	dr_256k			8
#define 	dr_500k			9
#define 	dr_500			10

#define SDN_HIGH()	HAL_GPIO_WritePin(SDN_GPIO_Port, SDN_Pin, GPIO_PIN_SET)
#define SDN_LOW()	HAL_GPIO_WritePin(SDN_GPIO_Port, SDN_Pin, GPIO_PIN_RESET)


#define nSEL_HIGH()	HAL_GPIO_WritePin(nSEL_GPIO_Port, nSEL_Pin, GPIO_PIN_SET)
#define nSEL_LOW()	    HAL_GPIO_WritePin(nSEL_GPIO_Port, nSEL_Pin, GPIO_PIN_RESET)
#define nIRQ 		HAL_GPIO_ReadPin(nIRQ_GPIO_Port, nIRQ_Pin)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
