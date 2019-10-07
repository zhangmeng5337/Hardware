<<<<<<< HEAD
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
typedef unsigned char BYTE;
typedef _Bool BOOL;
#define TRUE  1
#define FALSE  0

#define SDA_Pin         RM_MOSI_Pin
#define SDA_GPIO_Port   RM_MOSI_GPIO_Port
#define SCL_Pin         RM_SCLK_Pin
#define SCL_GPIO_Port   RM_SCLK_GPIO_Port


#define I2C_ADD1_Pin         RM_MISO_Pin
#define I2C_ADD1_GPIO_Port   RM_MISO_GPIO_Port
#define I2C_ADD0_Pin         RM_CS_Pin
#define I2C_ADD0_GPIO_Port   RM_CS_GPIO_Port

#define I2C_MODE_Pin         RM3100_MODE_EN_Pin
#define I2C_MODE_GPIO_Port   RM3100_MODE_EN_GPIO_Port
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define VBAT_ADC_Pin GPIO_PIN_0
#define VBAT_ADC_GPIO_Port GPIOC
#define RM_POWER_Pin GPIO_PIN_0
#define RM_POWER_GPIO_Port GPIOA
#define RM_SCLK_Pin GPIO_PIN_1
#define RM_SCLK_GPIO_Port GPIOA
#define LORA_TXD_Pin GPIO_PIN_2
#define LORA_TXD_GPIO_Port GPIOA
#define LORA_RXD_Pin GPIO_PIN_3
#define LORA_RXD_GPIO_Port GPIOA
#define RM_CS_Pin GPIO_PIN_4
#define RM_CS_GPIO_Port GPIOA
#define RM_MISO_Pin GPIO_PIN_6
#define RM_MISO_GPIO_Port GPIOA
#define RM_MOSI_Pin GPIO_PIN_7
#define RM_MOSI_GPIO_Port GPIOA
#define RM_DRY_Pin GPIO_PIN_4
#define RM_DRY_GPIO_Port GPIOC
#define RM_DRY_EXTI_IRQn EXTI4_IRQn
#define RM3100_MODE_EN_Pin GPIO_PIN_5
#define RM3100_MODE_EN_GPIO_Port GPIOC
#define LORA_A_Pin GPIO_PIN_0
#define LORA_A_GPIO_Port GPIOB
#define LORA_AUX_Pin GPIO_PIN_2
#define LORA_AUX_GPIO_Port GPIOB
#define LORA_B_Pin GPIO_PIN_8
#define LORA_B_GPIO_Port GPIOE
#define LED_Pin GPIO_PIN_15
#define LED_GPIO_Port GPIOE
#define TXD_Pin GPIO_PIN_9
#define TXD_GPIO_Port GPIOA
#define RXD_Pin GPIO_PIN_10
#define RXD_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
