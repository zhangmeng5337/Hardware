/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
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
#define DI_IN14_Pin GPIO_PIN_13
#define DI_IN14_GPIO_Port GPIOC
#define DI_IN13_Pin GPIO_PIN_14
#define DI_IN13_GPIO_Port GPIOC
#define DI_IN12_Pin GPIO_PIN_15
#define DI_IN12_GPIO_Port GPIOC
#define DI_IN8_Pin GPIO_PIN_0
#define DI_IN8_GPIO_Port GPIOC
#define DI_IN9_Pin GPIO_PIN_1
#define DI_IN9_GPIO_Port GPIOC
#define DI_IN10_Pin GPIO_PIN_2
#define DI_IN10_GPIO_Port GPIOC
#define DI_IN11_Pin GPIO_PIN_3
#define DI_IN11_GPIO_Port GPIOC
#define IN_SI_Pin GPIO_PIN_2
#define IN_SI_GPIO_Port GPIOA
#define IN_RCK_Pin GPIO_PIN_3
#define IN_RCK_GPIO_Port GPIOA
#define IN_SCK_Pin GPIO_PIN_4
#define IN_SCK_GPIO_Port GPIOA
#define IO0_Pin GPIO_PIN_4
#define IO0_GPIO_Port GPIOC
#define LATCH_AO_MCU_Pin GPIO_PIN_5
#define LATCH_AO_MCU_GPIO_Port GPIOC
#define MSTART_MCU_Pin GPIO_PIN_0
#define MSTART_MCU_GPIO_Port GPIOB
#define MDRDY_PT2_MCU_Pin GPIO_PIN_10
#define MDRDY_PT2_MCU_GPIO_Port GPIOB
#define MIBSPI1MCS1_Pin GPIO_PIN_11
#define MIBSPI1MCS1_GPIO_Port GPIOB
#define DI_IN4_Pin GPIO_PIN_12
#define DI_IN4_GPIO_Port GPIOB
#define DI_IN5_Pin GPIO_PIN_13
#define DI_IN5_GPIO_Port GPIOB
#define DI_IN6_Pin GPIO_PIN_14
#define DI_IN6_GPIO_Port GPIOB
#define DI_IN7_Pin GPIO_PIN_15
#define DI_IN7_GPIO_Port GPIOB
#define DI_IN0_Pin GPIO_PIN_6
#define DI_IN0_GPIO_Port GPIOC
#define DI_IN1_Pin GPIO_PIN_7
#define DI_IN1_GPIO_Port GPIOC
#define DI_IN2_Pin GPIO_PIN_8
#define DI_IN2_GPIO_Port GPIOC
#define DI_IN3_Pin GPIO_PIN_9
#define DI_IN3_GPIO_Port GPIOC
#define status_Pin GPIO_PIN_8
#define status_GPIO_Port GPIOA
#define MIBSPI1MCS3_Pin GPIO_PIN_11
#define MIBSPI1MCS3_GPIO_Port GPIOA
#define MIBSPI1MCS2_Pin GPIO_PIN_12
#define MIBSPI1MCS2_GPIO_Port GPIOA
#define lte_pwr_ctrl_Pin GPIO_PIN_15
#define lte_pwr_ctrl_GPIO_Port GPIOA
#define lte_rst_Pin GPIO_PIN_3
#define lte_rst_GPIO_Port GPIOB
#define Mb_rxen1_Pin GPIO_PIN_4
#define Mb_rxen1_GPIO_Port GPIOB
#define DI_IN15_Pin GPIO_PIN_6
#define DI_IN15_GPIO_Port GPIOB
#define lte_3_8V_EN_Pin GPIO_PIN_7
#define lte_3_8V_EN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
