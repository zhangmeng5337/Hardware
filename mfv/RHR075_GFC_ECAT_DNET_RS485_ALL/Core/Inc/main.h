/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32g4xx_hal.h"

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
#define ADC_LOG_ENABLE    0
#define DEBUG_LOG1_ENABLE 0
#define DEBUG_LOG2_ENABLE 0
#define P1_T_SEL  0// 0:average 1: 
#define SLOPE_DEBUG  0


#define RS_485	0
#define RS_232  1
#define COM_TYPE	RS_485
//#define DEBUG_ENABLE   1
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define relay_ctrl_1_Pin GPIO_PIN_4
#define relay_ctrl_1_GPIO_Port GPIOA
#define relay_ctrl_2_Pin GPIO_PIN_5
#define relay_ctrl_2_GPIO_Port GPIOA
#define relay_ctrl_3_Pin GPIO_PIN_6
#define relay_ctrl_3_GPIO_Port GPIOA
#define relay_ctrl_5_Pin GPIO_PIN_7
#define relay_ctrl_5_GPIO_Port GPIOA
#define led_Pin GPIO_PIN_0
#define led_GPIO_Port GPIOB
#define ERR_Pin GPIO_PIN_2
#define ERR_GPIO_Port GPIOB
#define RS485_EN_Pin GPIO_PIN_13
#define RS485_EN_GPIO_Port GPIOB
#define relay_ctrl_4_Pin GPIO_PIN_15
#define relay_ctrl_4_GPIO_Port GPIOB
#define ECAT_SPI3_CS_Pin GPIO_PIN_6
#define ECAT_SPI3_CS_GPIO_Port GPIOB
#define PROC_RST_Pin GPIO_PIN_7
#define PROC_RST_GPIO_Port GPIOB
#define CDG_ZERO_Pin GPIO_PIN_8
#define CDG_ZERO_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
