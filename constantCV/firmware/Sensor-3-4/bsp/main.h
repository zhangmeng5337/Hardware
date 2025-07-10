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
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "cw32l010.h"
#include "cw32l010_gpio.h"
#include "cw32l010_sysctrl.h"
#include "cw32l010_systick.h"
#include "interrupts_cw32l010.h"
#include "system_cw32l010.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */


/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define MAIN_FREQ   24000000
#define VERSION 0x1b//1.0
#define DISPLAY_DEBUG 0

//UARTx
#define  DEBUG_UARTx                   CW_UART1
#define  DEBUG_UART_CLK                SYSCTRL_APB1_PERIPH_UART1
#define  DEBUG_UART_APBClkENx          SYSCTRL_APBPeriphClk_Enable1
#define  DEBUG_UART_BaudRate           9600
#define  DEBUG_UART_UclkFreq           MAIN_FREQ

//UARTx GPIO
#define  DEBUG_UART_GPIO_CLK           (SYSCTRL_AHB_PERIPH_GPIOA)
#define  DEBUG_UART_TX_GPIO_PORT       CW_GPIOA
#define  DEBUG_UART_TX_GPIO_PIN        GPIO_PIN_1
#define  DEBUG_UART_RX_GPIO_PORT       CW_GPIOA
#define  DEBUG_UART_RX_GPIO_PIN        GPIO_PIN_0
//中断
#define  DEBUG_UART_IRQ                UART1_IRQn



//GPIO AF
#define  DEBUG_UART_AFTX               PA00_AFx_UART1RXD()
#define  DEBUG_UART_AFRX               PA01_AFx_UART1TXD()
#define  Curr_V_AFRX                   PB04_AFx_ATIMCH1()




//KEY  
#ifdef HW_VER1

#define  KEY1_GPIO_PIN                    GPIO_PIN_3
#define  KEY1_GPIO_PORT                   CW_GPIOA
#define  KEY2_GPIO_PIN                    GPIO_PIN_2
#define  KEY2_GPIO_PORT                   CW_GPIOB
#define  KEY3_GPIO_PIN                    GPIO_PIN_4
#define  KEY3_GPIO_PORT                   CW_GPIOA
#endif

#ifdef HW_VER2

#define  KEY1_GPIO_PIN                    GPIO_PIN_3
#define  KEY1_GPIO_PORT                   CW_GPIOA
#define  KEY2_GPIO_PIN                    GPIO_PIN_7
#define  KEY2_GPIO_PORT                   CW_GPIOB
#define  KEY3_GPIO_PIN                    GPIO_PIN_4
#define  KEY3_GPIO_PORT                   CW_GPIOA
#endif

#ifdef HW_VER1

#define  DIAG1_GPIO_PIN                   GPIO_PIN_8
#define  DIAG1_GPIO_PORT                  CW_GPIOA
#define  DIAG2_GPIO_PIN                   GPIO_PIN_1
#define  DIAG2_GPIO_PORT                  CW_GPIOB
#define  DIAG3_GPIO_PIN                   GPIO_PIN_0
#define  DIAG3_GPIO_PORT                  CW_GPIOB
#define  DIAG4_GPIO_PIN                   GPIO_PIN_1
#define  DIAG4_GPIO_PORT                  CW_GPIOB

#endif
#ifdef HW_VER2

#define  DIAG1_GPIO_PIN                   GPIO_PIN_7
#define  DIAG1_GPIO_PORT                  CW_GPIOA
#define  DIAG2_GPIO_PIN                   GPIO_PIN_1
#define  DIAG2_GPIO_PORT                  CW_GPIOB
#define  DIAG3_GPIO_PIN                   GPIO_PIN_0
#define  DIAG3_GPIO_PORT                  CW_GPIOB
#define  DIAG4_GPIO_PIN                   GPIO_PIN_2
#define  DIAG4_GPIO_PORT                  CW_GPIOB

#endif


//display
#define  IN_SI_GPIO_PIN                   GPIO_PIN_3
#define  IN_SI_GPIO_PORT                  CW_GPIOB
#define  IN_SCK_GPIO_PIN                  GPIO_PIN_6
#define  IN_SCK_GPIO_PORT                 CW_GPIOA
#define  IN_RCK_GPIO_PIN                  GPIO_PIN_2
#define  IN_RCKGPIO_PORT                  CW_GPIOA





//dac
#define  Curr_V_PIN                        GPIO_PIN_4
#define  Curr_V_GPIO_PORT                  CW_GPIOB

#define  EN_GPIO_PIN                       GPIO_PIN_5
#define  EN_GPIO_PORT                      CW_GPIOA

//cs1237
#define  CS1237_SCL_Pin                    GPIO_PIN_5
#define  CS1237_SCL_GPIO_Port              CW_GPIOB
#define  CS1237_DOUT_Pin                   GPIO_PIN_6
#define  CS1237_DOUT_GPIO_Port             CW_GPIOB

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
void delay_us(uint32_t a);
void delay1_us(uint32_t a);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT CW *****END OF FILE****/
