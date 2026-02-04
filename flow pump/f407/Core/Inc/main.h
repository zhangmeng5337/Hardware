/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ENABLE_Pin GPIO_PIN_0
#define ENABLE_GPIO_Port GPIOC
#define MFAULT_Pin GPIO_PIN_1
#define MFAULT_GPIO_Port GPIOC
#define DIR_Pin GPIO_PIN_2
#define DIR_GPIO_Port GPIOC
#define nBREAK_Pin GPIO_PIN_1
#define nBREAK_GPIO_Port GPIOA
#define LATCH_AO_Pin GPIO_PIN_4
#define LATCH_AO_GPIO_Port GPIOC
#define BL_Pin GPIO_PIN_5
#define BL_GPIO_Port GPIOC
#define FLASH_nCS1_Pin GPIO_PIN_0
#define FLASH_nCS1_GPIO_Port GPIOB
#define FLASH_nW_Pin GPIO_PIN_1
#define FLASH_nW_GPIO_Port GPIOB
#define DC_Pin GPIO_PIN_2
#define DC_GPIO_Port GPIOB
#define TE_Pin GPIO_PIN_7
#define TE_GPIO_Port GPIOE
#define RESX_Pin GPIO_PIN_8
#define RESX_GPIO_Port GPIOE
#define FAULT_Pin GPIO_PIN_10
#define FAULT_GPIO_Port GPIOE
#define WARN_Pin GPIO_PIN_11
#define WARN_GPIO_Port GPIOE
#define RUN_Pin GPIO_PIN_12
#define RUN_GPIO_Port GPIOE
#define SW2_Pin GPIO_PIN_13
#define SW2_GPIO_Port GPIOE
#define SW1_Pin GPIO_PIN_14
#define SW1_GPIO_Port GPIOE
#define KEYSW_Pin GPIO_PIN_15
#define KEYSW_GPIO_Port GPIOE
#define EXTPROC_SPI2_ADC_CS3_Pin GPIO_PIN_15
#define EXTPROC_SPI2_ADC_CS3_GPIO_Port GPIOB
#define CS_Pin GPIO_PIN_9
#define CS_GPIO_Port GPIOC
#define DI_IN0_Pin GPIO_PIN_0
#define DI_IN0_GPIO_Port GPIOD
#define DI_IN1_Pin GPIO_PIN_1
#define DI_IN1_GPIO_Port GPIOD
#define DI_IN2_Pin GPIO_PIN_2
#define DI_IN2_GPIO_Port GPIOD
#define DI_IN3_Pin GPIO_PIN_3
#define DI_IN3_GPIO_Port GPIOD
#define DI_IN4_Pin GPIO_PIN_4
#define DI_IN4_GPIO_Port GPIOD
#define MEZ_Pin GPIO_PIN_5
#define MEZ_GPIO_Port GPIOD
#define DI_IN5_Pin GPIO_PIN_6
#define DI_IN5_GPIO_Port GPIOD
#define DI_IN6_Pin GPIO_PIN_7
#define DI_IN6_GPIO_Port GPIOD
#define DO_ctrl_1_Pin GPIO_PIN_3
#define DO_ctrl_1_GPIO_Port GPIOB
#define DO_ctrl_0_Pin GPIO_PIN_4
#define DO_ctrl_0_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define TIM4PERIOD 65535
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
