/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 CW.
  * All rights reserved.</center></h2>
  *
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

#include "system_cw32l010.h"

#include "cw32l010_gtim.h"
#include "cw32l010_sysctrl.h"
#include "cw32l010_gpio.h"
#include "cw32l010_uart.h"
#include "lin.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

typedef struct
{
	uint8_t Data[8];
	uint8_t Length;
	uint8_t ID;
	uint8_t Protected_ID;
	uint16_t CheckSum;
	uint16_t Response_Time;
	uint8_t Send_Complete;
} LinTxMsg;

typedef struct
{
	uint8_t Data[8];
	uint8_t Length;
	uint8_t ID;
	uint8_t Protected_ID;
	uint16_t CheckSum;
	uint16_t Response_Time;
	uint8_t Index;
	uint8_t Receive_Complete;
} LinRxMsg;


extern LinTxMsg linTxMsg;
extern LinRxMsg linRxMsg;
extern void Lin_Send_Wharf(void);
extern void init_LIN(void);
extern void Lin_Send_Msg(void);
extern void Lin_SendData_Process(void);
extern void Lin_Data_receiving_callback(void);
extern uint8_t Lin_Sending_Flag;
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
/* USER CODE BEGIN EFP */
/* USER CODE END EFP */


/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT CW *****END OF FILE****/
