/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "cmsis_os.h"
extern int Vfid_flag,Vms_flag,Vheat_flag;
extern int Tfid_flag,Tms_flag,Tcap_flag,Tdil_flag;
	
extern int Vfid_flag_cnt,Vms_flag_cnt,Vheat_flag_cnt,Tfid_flag_cnt,Tms_flag_cnt,Tcap_flag_cnt;
extern int Vfid_flag_W,Vms_flag_W,Vheat_flag_W,Tfid_flag_W,Tms_flag_W,Tcap_flag_W;
extern int Vfid_flag_warning,Vms_flag_warning,Vheat_flag_warning,Tfid_flag_warning,Tms_flag_warning,Tcap_flag_warning;
//extern int vale1_flag,vale2_flag,vale3_flag,vale4_flag,vale5_flag,vale6_flag,valeA_flag,valeB_flag;	
//extern int tx_flag;
//extern int val6_flag,val12_flag;
//extern osSemaphoreId myBinarySem01Handle;
//extern osSemaphoreId bin_uart2Handle;
//extern osSemaphoreId VmeasureBinarySem03Handle;
//extern osSemaphoreId myBinarySem04Handle;
//extern osSemaphoreId bin_uart3Handle;
//extern osSemaphoreId bin_uart3SendedSem;


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define PERIOD  2
#define FREQ  84000000/50000/PERIOD -1
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define H1_Pin GPIO_PIN_0
#define H1_GPIO_Port GPIOC
#define H2_Pin GPIO_PIN_1
#define H2_GPIO_Port GPIOC
#define H3_Pin GPIO_PIN_2
#define H3_GPIO_Port GPIOC
#define H_STBY_Pin GPIO_PIN_3
#define H_STBY_GPIO_Port GPIOC
#define SPI1_CS1_Pin GPIO_PIN_0
#define SPI1_CS1_GPIO_Port GPIOB
#define SPI1_CS2_Pin GPIO_PIN_1
#define SPI1_CS2_GPIO_Port GPIOB
#define SPI1_CS3_Pin GPIO_PIN_2
#define SPI1_CS3_GPIO_Port GPIOB
#define SPI1_CS4_Pin GPIO_PIN_7
#define SPI1_CS4_GPIO_Port GPIOE
#define SPI2_CS1_Pin GPIO_PIN_12
#define SPI2_CS1_GPIO_Port GPIOB
#define SPI2_WP_Pin GPIO_PIN_8
#define SPI2_WP_GPIO_Port GPIOD
#define GCMS_OUT_Pin GPIO_PIN_12
#define GCMS_OUT_GPIO_Port GPIOD
#define C_IN_Pin GPIO_PIN_13
#define C_IN_GPIO_Port GPIOD
#define C_OUT_Pin GPIO_PIN_14
#define C_OUT_GPIO_Port GPIOD
#define V_STBY1_Pin GPIO_PIN_4
#define V_STBY1_GPIO_Port GPIOB
#define V_STBY2_Pin GPIO_PIN_5
#define V_STBY2_GPIO_Port GPIOB
#define V1_Pin GPIO_PIN_6
#define V1_GPIO_Port GPIOB
#define V2_Pin GPIO_PIN_7
#define V2_GPIO_Port GPIOB
#define V3_Pin GPIO_PIN_8
#define V3_GPIO_Port GPIOB
#define V4_Pin GPIO_PIN_9
#define V4_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */
extern TIM_HandleTypeDef htim4;
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
