/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define lte_pwr_ctrl_Pin GPIO_PIN_0
#define lte_pwr_ctrl_GPIO_Port GPIOC
#define led_Pin GPIO_PIN_2
#define led_GPIO_Port GPIOC
#define SPI1_WP_Pin GPIO_PIN_4
#define SPI1_WP_GPIO_Port GPIOC
#define Q7_Pin GPIO_PIN_15
#define Q7_GPIO_Port GPIOB
#define CP_Pin GPIO_PIN_6
#define CP_GPIO_Port GPIOC
#define PL_Pin GPIO_PIN_7
#define PL_GPIO_Port GPIOC
#define IN_SCK_Pin GPIO_PIN_8
#define IN_SCK_GPIO_Port GPIOC
#define IN_RCK_Pin GPIO_PIN_9
#define IN_RCK_GPIO_Port GPIOC
#define IN_SI_Pin GPIO_PIN_8
#define IN_SI_GPIO_Port GPIOA
#define RS485A_EN_Pin GPIO_PIN_15
#define RS485A_EN_GPIO_Port GPIOA
#define RS485B_EN_Pin GPIO_PIN_10
#define RS485B_EN_GPIO_Port GPIOC
#define SPI1_CS_Pin GPIO_PIN_2
#define SPI1_CS_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
