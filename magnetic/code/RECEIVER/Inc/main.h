/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32l4xx_hal.h"

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
#define TTL_TXD_Pin GPIO_PIN_2
#define TTL_TXD_GPIO_Port GPIOA
#define TTL_RXD_Pin GPIO_PIN_3
#define TTL_RXD_GPIO_Port GPIOA
#define COM_TX_Pin GPIO_PIN_4
#define COM_TX_GPIO_Port GPIOC
#define COM_RX_Pin GPIO_PIN_5
#define COM_RX_GPIO_Port GPIOC
#define LORA_AUX_Pin GPIO_PIN_8
#define LORA_AUX_GPIO_Port GPIOC
#define LORA_AUX_EXTI_IRQn EXTI9_5_IRQn
#define LORA_B_Pin GPIO_PIN_9
#define LORA_B_GPIO_Port GPIOC
#define LORA_A_Pin GPIO_PIN_8
#define LORA_A_GPIO_Port GPIOA
#define LORA_TXD_Pin GPIO_PIN_9
#define LORA_TXD_GPIO_Port GPIOA
#define LORA_RXD_Pin GPIO_PIN_10
#define LORA_RXD_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_11
#define LED_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
