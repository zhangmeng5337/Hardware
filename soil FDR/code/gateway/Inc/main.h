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
#include "string.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
typedef unsigned char u8;
typedef unsigned int u16;
typedef uint32_t  u32;
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
#define ROLE				EQUIP//0:ÖÕ¶Ë£»1£ºÍø¹Ø
//#define ROLE				GATEWAY//0:ÖÕ¶Ë£»1£ºÍø¹Ø

#define GATEWAY				1
#define EQUIP				0



void SystemClock_Config(void);
 void MX_GPIO_Init(void);
 void MX_DMA_Init(void);
 void MX_ADC1_Init(void);
 void MX_UART5_Init(void);
 void MX_USART1_UART_Init(void);
 void MX_USART2_UART_Init(void);
 void MX_USART3_UART_Init(void);
 void MX_USART6_UART_Init(void);
 void MX_RTC_Init(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DTR_Pin GPIO_PIN_1
#define DTR_GPIO_Port GPIOC
#define EN_3_3V_Pin GPIO_PIN_2
#define EN_3_3V_GPIO_Port GPIOC
#define SIM_PWR_Pin GPIO_PIN_3
#define SIM_PWR_GPIO_Port GPIOC
#define CTRL1OUT11_Pin GPIO_PIN_1
#define CTRL1OUT11_GPIO_Port GPIOA
#define MODE1_Pin GPIO_PIN_4
#define MODE1_GPIO_Port GPIOA
#define MODE2_Pin GPIO_PIN_5
#define MODE2_GPIO_Port GPIOA
#define CTRL1OUT21_Pin GPIO_PIN_7
#define CTRL1OUT21_GPIO_Port GPIOA
#define CTRL1OUT31_Pin GPIO_PIN_4
#define CTRL1OUT31_GPIO_Port GPIOC
#define CTRL2OUT11_Pin GPIO_PIN_5
#define CTRL2OUT11_GPIO_Port GPIOC
#define CTRL2OUT21_Pin GPIO_PIN_0
#define CTRL2OUT21_GPIO_Port GPIOB
#define CTRL2OUT31_Pin GPIO_PIN_1
#define CTRL2OUT31_GPIO_Port GPIOB
#define CTRL3OUT11_Pin GPIO_PIN_2
#define CTRL3OUT11_GPIO_Port GPIOB
#define RS485_EN1_Pin GPIO_PIN_12
#define RS485_EN1_GPIO_Port GPIOB
#define CTRL3OUT21_Pin GPIO_PIN_13
#define CTRL3OUT21_GPIO_Port GPIOB
#define CTRL3OUT31_Pin GPIO_PIN_14
#define CTRL3OUT31_GPIO_Port GPIOB
#define CTRL2__Pin GPIO_PIN_15
#define CTRL2__GPIO_Port GPIOB
#define CTRL3__Pin GPIO_PIN_8
#define CTRL3__GPIO_Port GPIOC
#define CTRL1__Pin GPIO_PIN_9
#define CTRL1__GPIO_Port GPIOC
#define LORA_AUX_Pin GPIO_PIN_3
#define LORA_AUX_GPIO_Port GPIOB
#define LORA_M1_Pin GPIO_PIN_4
#define LORA_M1_GPIO_Port GPIOB
#define LORA_AUX2_Pin GPIO_PIN_5
#define LORA_AUX2_GPIO_Port GPIOB
#define LORA_M12_Pin GPIO_PIN_6
#define LORA_M12_GPIO_Port GPIOB
#define LORA_M0_Pin GPIO_PIN_7
#define LORA_M0_GPIO_Port GPIOB
#define LORA_M02_Pin GPIO_PIN_8
#define LORA_M02_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define ON   1
#define OFF  0
void MX_RTC_Init(void);
#define LORA_TX	0
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
