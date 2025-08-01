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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define ADC_POLL_T		100
#define PID_POLL_T		100
#define RF_POLL_T		100

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define ON 0
#define OFF 1
#define DEBUG_EN  0
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
#define air_pwr_Pin GPIO_PIN_0
#define air_pwr_GPIO_Port GPIOC
#define ok_Pin GPIO_PIN_1
#define ok_GPIO_Port GPIOC
#define ok_EXTI_IRQn EXTI1_IRQn
#define set_Pin GPIO_PIN_2
#define set_GPIO_Port GPIOC
#define set_EXTI_IRQn EXTI2_IRQn
#define inc_Pin GPIO_PIN_3
#define inc_GPIO_Port GPIOC
#define inc_EXTI_IRQn EXTI3_IRQn
#define dec_Pin GPIO_PIN_4
#define dec_GPIO_Port GPIOC
#define dec_EXTI_IRQn EXTI4_IRQn
#define SPEAK_Pin GPIO_PIN_15
#define SPEAK_GPIO_Port GPIOB
#define air_FREQ_Pin GPIO_PIN_6
#define air_FREQ_GPIO_Port GPIOB

<<<<<<< HEAD
/* USER CODE BEGIN Private defines */
=======

/* USER CODE BEGIN Private defines */
#define PWR_EN_Pin GPIO_PIN_12
#define PWR_EN_GPIO_Port GPIOB
>>>>>>> 1ae077bb4952fdb979a8f9608a90611b9cba3ce6

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
