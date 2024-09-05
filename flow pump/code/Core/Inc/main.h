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
#define PWM_COUNTER  4999
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
#define ON   1
#define OFF  0
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DO_ctrl_1_Pin GPIO_PIN_13
#define DO_ctrl_1_GPIO_Port GPIOC
#define RUN_Pin GPIO_PIN_3
#define RUN_GPIO_Port GPIOC
#define DO_ctrl_0_Pin GPIO_PIN_2
#define DO_ctrl_0_GPIO_Port GPIOA
#define EXTPROC_RS485_DIRECTION_Pin GPIO_PIN_3
#define EXTPROC_RS485_DIRECTION_GPIO_Port GPIOA
#define MHV_Pin GPIO_PIN_4
#define MHV_GPIO_Port GPIOA
#define MHV_EXTI_IRQn EXTI4_IRQn
#define WARN_Pin GPIO_PIN_6
#define WARN_GPIO_Port GPIOA
#define LCD_SID_Pin GPIO_PIN_7
#define LCD_SID_GPIO_Port GPIOA
#define DI_IN3_Pin GPIO_PIN_4
#define DI_IN3_GPIO_Port GPIOC
#define DI_IN4_Pin GPIO_PIN_0
#define DI_IN4_GPIO_Port GPIOB
#define DI_IN5_Pin GPIO_PIN_1
#define DI_IN5_GPIO_Port GPIOB
#define DI_IN6_Pin GPIO_PIN_2
#define DI_IN6_GPIO_Port GPIOB
#define FAULT_Pin GPIO_PIN_10
#define FAULT_GPIO_Port GPIOB
#define LCD_CLK_Pin GPIO_PIN_11
#define LCD_CLK_GPIO_Port GPIOB
#define MHU_Pin GPIO_PIN_6
#define MHU_GPIO_Port GPIOC
#define MHU_EXTI_IRQn EXTI9_5_IRQn
#define LCD_CS_Pin GPIO_PIN_7
#define LCD_CS_GPIO_Port GPIOC
#define DI_IN1_Pin GPIO_PIN_8
#define DI_IN1_GPIO_Port GPIOC
#define DI_IN0_Pin GPIO_PIN_9
#define DI_IN0_GPIO_Port GPIOC
#define DI_IN0_EXTI_IRQn EXTI9_5_IRQn
#define DI_IN2_Pin GPIO_PIN_8
#define DI_IN2_GPIO_Port GPIOA
#define KEYSW_Pin GPIO_PIN_10
#define KEYSW_GPIO_Port GPIOA
#define MHW_Pin GPIO_PIN_2
#define MHW_GPIO_Port GPIOD
#define MHW_EXTI_IRQn EXTI2_IRQn
#define LATCH_AO_Pin GPIO_PIN_3
#define LATCH_AO_GPIO_Port GPIOB
#define MCU_WL_Pin GPIO_PIN_4
#define MCU_WL_GPIO_Port GPIOB
#define MCU_VL_Pin GPIO_PIN_5
#define MCU_VL_GPIO_Port GPIOB
#define MCU_UL_Pin GPIO_PIN_9
#define MCU_UL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
