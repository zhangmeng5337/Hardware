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
#include "gd32f4xx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
typedef unsigned int u16;
typedef unsigned char u8;
typedef uint32_t u32;
#include "string.h"
/* USER CODE END Includes */
#define USART5_DMA  DMA_CH2
#define ADC0_DMA	DMA_CH0

#define WATER_L_IGNORE  1
#define FLOW_FAULT_ENABLE 0

<<<<<<< HEAD
#define LCD_ORI 0      //1:原屏幕 0:修改屏
#define FLOW_SIZE	4 //排水量1-4L
#define DEV_TYPE    0//0:消杀类     1:医疗类
#define SW_NOCHANGE 0//0：管路修改过；1：管路未修改
#define LW_ENABLE 0  //低压开关使能 1：使能 0：关闭
#define TDS_DEBUG 0
#if LW_ENABLE ==0  //低压开关使能 1：使能 0：关闭
#define HW_LEVEL 1


#else


#define HW_LEVEL 0
#endif

=======
#define LCD_ORI 0      //1:ԭ��Ļ 0:�޸���
#define FLOW_SIZE	4 //��ˮ��1-4L
#define DEV_TYPE    0//0:��ɱ��     1:ҽ����
#define SW_NOCHANGE 0//0����·�޸Ĺ���1����·δ�޸�
#define LW_ENABLE 0  //��ѹ����ʹ�� 1��ʹ�� 0���ر�
#define TDS_DEBUG 0
#if LW_ENABLE ==0  //��ѹ����ʹ�� 1��ʹ�� 0���ر�
#define HW_LEVEL 1
#else
#define HW_LEVEL 0
#endif
#define PUMP_3MIN 1
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define VER 1
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */
#if CPU == ST
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
#endif
typedef enum
{
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET
}GPIO_PinState;


/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void adc_config(void);
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#if VER == 0
#define Ele_ConB_Pin GPIO_PIN_2
#define Ele_ConB_GPIO_Port GPIOE
#define Ele_ConA_Pin GPIO_PIN_3
#define Ele_ConA_GPIO_Port GPIOE
#define CVALUE1_Pin GPIO_PIN_0
#define CVALUE1_GPIO_Port GPIOC
#define CVALUE2_Pin GPIO_PIN_1
#define CVALUE2_GPIO_Port GPIOC
#define CVALUE3_Pin GPIO_PIN_2
#define CVALUE3_GPIO_Port GPIOC
#define CVALUE4_Pin GPIO_PIN_3
#define CVALUE4_GPIO_Port GPIOC
#define TEST_VA_Pin GPIO_PIN_5
#define TEST_VA_GPIO_Port GPIOA
#define TDS_P_Pin GPIO_PIN_7
#define TDS_P_GPIO_Port GPIOA
#define SensorA_Pin GPIO_PIN_5
#define SensorA_GPIO_Port GPIOC
#define TDS_N_Pin GPIO_PIN_1
#define TDS_N_GPIO_Port GPIOB
#define CVALUE5_Pin GPIO_PIN_2
#define CVALUE5_GPIO_Port GPIOB
//#define LCD_BL_Pin GPIO_PIN_11
//#define LCD_BL_GPIO_Port GPIOE
#define LCD_IRQ_Pin GPIO_PIN_12
#define LCD_IRQ_GPIO_Port GPIOE
#define SensorB_Pin GPIO_PIN_13
#define SensorB_GPIO_Port GPIOE
#define LCD_RST_Pin GPIO_PIN_14
#define LCD_RST_GPIO_Port GPIOE
#define LCD_READY_Pin GPIO_PIN_15
#define LCD_READY_GPIO_Port GPIOE
#define scl_Pin GPIO_PIN_12
#define scl_GPIO_Port GPIOB
#define int_Pin GPIO_PIN_13
#define int_GPIO_Port GPIOB
#define int_EXTI_IRQn EXTI15_10_IRQn
#define sda_Pin GPIO_PIN_15
#define sda_GPIO_Port GPIOB
#define EN1_485_Pin GPIO_PIN_9
#define EN1_485_GPIO_Port GPIOD
#define EN2_485_Pin GPIO_PIN_10
#define EN2_485_GPIO_Port GPIOD
#define LCD_BUSY_Pin GPIO_PIN_12
#define LCD_BUSY_GPIO_Port GPIOD
#define SPI3_CS_Pin GPIO_PIN_12
#define SPI3_CS_GPIO_Port GPIOA
#define WIFI_EN_Pin GPIO_PIN_6
#define WIFI_EN_GPIO_Port GPIOD
#else
#define Ele_ConB_Pin GPIO_PIN_2
#define Ele_ConB_GPIO_Port GPIOE
#define Ele_ConA_Pin GPIO_PIN_3
#define Ele_ConA_GPIO_Port GPIOE
#define CVALUE1_Pin GPIO_PIN_6
#define CVALUE1_GPIO_Port GPIOE
#define CVALUE2_Pin GPIO_PIN_2
#define CVALUE2_GPIO_Port GPIOA
#define CVALUE3_Pin GPIO_PIN_3
#define CVALUE3_GPIO_Port GPIOA
#define CVALUE4_Pin GPIO_PIN_9
#define CVALUE4_GPIO_Port GPIOE
<<<<<<< HEAD
#define TEST_VA_Pin GPIO_PIN_5
#define TEST_VA_GPIO_Port GPIOA
#define TEST_VAL_Pin GPIO_PIN_2
#define TEST_VAL_GPIO_Port GPIOB

=======
#define TEST_VAL_Pin GPIO_PIN_2
#define TEST_VAL_GPIO_Port GPIOB

#define TEST_VA_Pin GPIO_PIN_5
#define TEST_VA_GPIO_Port GPIOA
>>>>>>> 0f43380df54ffa604548e9f88db78b166dc21643
#define TDS_P_Pin GPIO_PIN_7
#define TDS_P_GPIO_Port GPIOA
#define SensorA_Pin GPIO_PIN_5
#define SensorA_GPIO_Port GPIOC
#define TDS_N_Pin GPIO_PIN_1
#define TDS_N_GPIO_Port GPIOB
#define CVALUE5_Pin GPIO_PIN_11
#define CVALUE5_GPIO_Port GPIOE
//#define LCD_BL_Pin GPIO_PIN_11
//#define LCD_BL_GPIO_Port GPIOE
#define LCD_IRQ_Pin GPIO_PIN_12
#define LCD_IRQ_GPIO_Port GPIOE
#define SensorB_Pin GPIO_PIN_13
#define SensorB_GPIO_Port GPIOE
#define LCD_RST_Pin GPIO_PIN_14
#define LCD_RST_GPIO_Port GPIOE
#define LCD_READY_Pin GPIO_PIN_15
#define LCD_READY_GPIO_Port GPIOE
#define scl_Pin GPIO_PIN_12
#define scl_GPIO_Port GPIOB
#define int_Pin GPIO_PIN_13
#define int_GPIO_Port GPIOB
#define int_EXTI_IRQn EXTI15_10_IRQn
#define sda_Pin GPIO_PIN_15
#define sda_GPIO_Port GPIOB
#define EN1_485_Pin GPIO_PIN_9
#define EN1_485_GPIO_Port GPIOD
#define EN2_485_Pin GPIO_PIN_10
#define EN2_485_GPIO_Port GPIOD
#define LCD_BUSY_Pin GPIO_PIN_12
#define LCD_BUSY_GPIO_Port GPIOD
#define SPI3_CS_Pin GPIO_PIN_12
#define SPI3_CS_GPIO_Port GPIOA
#define WIFI_EN_Pin GPIO_PIN_6
#define WIFI_EN_GPIO_Port GPIOD


#endif
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
