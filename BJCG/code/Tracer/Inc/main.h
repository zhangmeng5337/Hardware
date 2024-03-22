/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#define ON    1
#define OFF   0
#define ROLE 0
#define DEBUG_MODE 0
typedef uint32_t u32;
typedef unsigned int u16;
typedef  unsigned char u8;
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
#define SIM_PWR_Pin GPIO_PIN_0
#define SIM_PWR_GPIO_Port GPIOA
#define SIM_RST1_Pin GPIO_PIN_1
#define SIM_RST1_GPIO_Port GPIOA
#define MDTR_Pin GPIO_PIN_5
#define MDTR_GPIO_Port GPIOA
#define MRI_Pin GPIO_PIN_6
#define MRI_GPIO_Port GPIOA
#define MRI_EXTI_IRQn EXTI9_5_IRQn
#define WP_Pin GPIO_PIN_7
#define WP_GPIO_Port GPIOA
#define SPI_RESET_Pin GPIO_PIN_2
#define SPI_RESET_GPIO_Port GPIOB
#define EN_5V1_Pin GPIO_PIN_14
#define EN_5V1_GPIO_Port GPIOB
#define EN_3V8_Pin GPIO_PIN_15
#define EN_3V8_GPIO_Port GPIOB
#define EN_5V2_Pin GPIO_PIN_6
#define EN_5V2_GPIO_Port GPIOC
#define EN_5V_Pin GPIO_PIN_12
#define EN_5V_GPIO_Port GPIOA
#define CS_Pin GPIO_PIN_15
#define CS_GPIO_Port GPIOA
#define POWER_CTRL_GPS_Pin GPIO_PIN_11
#define POWER_CTRL_GPS_GPIO_Port GPIOC
#define led_Pin GPIO_PIN_9
#define led_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
