/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

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
#define UART1_BUFFER_SIZE   8
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B_Pin GPIO_PIN_4
#define B_GPIO_Port GPIOA
#define G_Pin GPIO_PIN_5
#define G_GPIO_Port GPIOA
#define R_Pin GPIO_PIN_6
#define R_GPIO_Port GPIOA
#define MOTOR_A1_Pin GPIO_PIN_8
#define MOTOR_A1_GPIO_Port GPIOA
#define MOTOR_A2_Pin GPIO_PIN_9
#define MOTOR_A2_GPIO_Port GPIOA
#define MOTOR_A3_Pin GPIO_PIN_10
#define MOTOR_A3_GPIO_Port GPIOA
#define MOTOR_A4_Pin GPIO_PIN_11
#define MOTOR_A4_GPIO_Port GPIOA
#define MOTOR_B1_Pin GPIO_PIN_12
#define MOTOR_B1_GPIO_Port GPIOA
#define MOTOR_B2_Pin GPIO_PIN_15
#define MOTOR_B2_GPIO_Port GPIOA
#define MOTOR_B3_Pin GPIO_PIN_3
#define MOTOR_B3_GPIO_Port GPIOB
#define MOTOR_B4_Pin GPIO_PIN_4
#define MOTOR_B4_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
