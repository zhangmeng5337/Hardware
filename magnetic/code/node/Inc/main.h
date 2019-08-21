/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
