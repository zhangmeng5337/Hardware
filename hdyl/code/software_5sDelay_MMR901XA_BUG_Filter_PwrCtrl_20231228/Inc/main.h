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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "protocol.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
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
#define LED1_Pin GPIO_PIN_1
#define LED1_GPIO_Port GPIOA
#define BP_TXD_Pin GPIO_PIN_2
#define BP_TXD_GPIO_Port GPIOA
#define BP_RXD_Pin GPIO_PIN_3
#define BP_RXD_GPIO_Port GPIOA
#define POW_ECG_Pin GPIO_PIN_4
#define POW_ECG_GPIO_Port GPIOA
#define PCG_OUT_Pin GPIO_PIN_5
#define PCG_OUT_GPIO_Port GPIOA
#define HEA_OUT_Pin GPIO_PIN_6
#define HEA_OUT_GPIO_Port GPIOA
#define TST_OUT_Pin GPIO_PIN_7
#define TST_OUT_GPIO_Port GPIOA
#define ADS_DRDY_Pin GPIO_PIN_12
#define ADS_DRDY_GPIO_Port GPIOB
#define ADS_SCLK_Pin GPIO_PIN_13
#define ADS_SCLK_GPIO_Port GPIOB
#define ADS_SDO_Pin GPIO_PIN_14
#define ADS_SDO_GPIO_Port GPIOB
#define ADS_SDI_Pin GPIO_PIN_15
#define ADS_SDI_GPIO_Port GPIOB
#define ADS_CSB_Pin GPIO_PIN_6
#define ADS_CSB_GPIO_Port GPIOC
#define ADS_ALARM_Pin GPIO_PIN_7
#define ADS_ALARM_GPIO_Port GPIOC
#define POW_APG_Pin GPIO_PIN_8
#define POW_APG_GPIO_Port GPIOC
#define US1_TX_Pin GPIO_PIN_9
#define US1_TX_GPIO_Port GPIOA
#define US1_RX_Pin GPIO_PIN_10
#define US1_RX_GPIO_Port GPIOA
#define POW_HEA_Pin GPIO_PIN_11
#define POW_HEA_GPIO_Port GPIOA
#define POW_TST_Pin GPIO_PIN_12
#define POW_TST_GPIO_Port GPIOA
#define MMR_CLK_Pin GPIO_PIN_10
#define MMR_CLK_GPIO_Port GPIOC
#define MMR_DO_Pin GPIO_PIN_11
#define MMR_DO_GPIO_Port GPIOC
#define MMR_DI_Pin GPIO_PIN_12
#define MMR_DI_GPIO_Port GPIOC
#define MMR_RD_Pin GPIO_PIN_2
#define MMR_RD_GPIO_Port GPIOD
#define MMR_CS_Pin GPIO_PIN_3
#define MMR_CS_GPIO_Port GPIOB
#define MOTOR_Pin GPIO_PIN_8
#define MOTOR_GPIO_Port GPIOB
#define VALVE_Pin GPIO_PIN_9
#define VALVE_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
//add by zm 2023.12.26

#define ON   0
#define OFF  1

#define BP_POWER_EN_Pin GPIO_PIN_0
#define BP_POWER_EN_GPIO_Port GPIOB

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
