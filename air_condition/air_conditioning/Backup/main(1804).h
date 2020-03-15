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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define led_status_Pin GPIO_PIN_13
#define led_status_GPIO_Port GPIOC
#define led_ctrl1_Pin GPIO_PIN_14
#define led_ctrl1_GPIO_Port GPIOC
#define KEY2_Pin GPIO_PIN_15
#define KEY2_GPIO_Port GPIOC
#define KEY2_EXTI_IRQn EXTI15_10_IRQn
#define SW1_Pin GPIO_PIN_0
#define SW1_GPIO_Port GPIOA
#define KEY4_Pin GPIO_PIN_4
#define KEY4_GPIO_Port GPIOA
#define KEY4_EXTI_IRQn EXTI4_IRQn
#define KEY3_Pin GPIO_PIN_5
#define KEY3_GPIO_Port GPIOA
#define KEY3_EXTI_IRQn EXTI9_5_IRQn
#define CTR_OUT1_Pin GPIO_PIN_0
#define CTR_OUT1_GPIO_Port GPIOB
#define KEY1_Pin GPIO_PIN_1
#define KEY1_GPIO_Port GPIOB
#define KEY1_EXTI_IRQn EXTI1_IRQn
#define SW2_Pin GPIO_PIN_2
#define SW2_GPIO_Port GPIOB
#define CTR_OUT2_Pin GPIO_PIN_12
#define CTR_OUT2_GPIO_Port GPIOB
#define CTR_OUT3_Pin GPIO_PIN_13
#define CTR_OUT3_GPIO_Port GPIOB
#define DIS_COM1_Pin GPIO_PIN_14
#define DIS_COM1_GPIO_Port GPIOB
#define DIS_COM2_Pin GPIO_PIN_15
#define DIS_COM2_GPIO_Port GPIOB
#define M74HC595_SI_Pin GPIO_PIN_11
#define M74HC595_SI_GPIO_Port GPIOA
#define M74HC595_SCK_Pin GPIO_PIN_12
#define M74HC595_SCK_GPIO_Port GPIOA
#define IR_Pin GPIO_PIN_15
#define IR_GPIO_Port GPIOA
#define DIS_COM4_Pin GPIO_PIN_3
#define DIS_COM4_GPIO_Port GPIOB
#define DIS_COM3_Pin GPIO_PIN_4
#define DIS_COM3_GPIO_Port GPIOB
#define led_ctrl2_Pin GPIO_PIN_5
#define led_ctrl2_GPIO_Port GPIOB
#define IIC_SCL_Pin GPIO_PIN_6
#define IIC_SCL_GPIO_Port GPIOB
#define IIC_SDA_Pin GPIO_PIN_7
#define IIC_SDA_GPIO_Port GPIOB
#define PWM_AD_OUT_Pin GPIO_PIN_8
#define PWM_AD_OUT_GPIO_Port GPIOB
#define M74HC595_RCK_Pin GPIO_PIN_9
#define M74HC595_RCK_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
