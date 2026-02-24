/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : main.h
* Author             : MCD Application Team
* Version            : V1.0
* Date               : 10/08/2007
* Description        : Header for main.c module
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"

#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"

//逻辑单元数量在 usb_prop.c中设置

//IO定义 **********************************************************************
#define USB_DISCONNECT                    GPIOC  		  //USB所在端口为PROT B
#define USB_DISCONNECT_PIN                GPIO_Pin_9	  //USB软连接与断开控制IO
#define RCC_APB2Periph_GPIO_DISCONNECT    RCC_APB2Periph_GPIOC		  

#define USB_LED_PORT                      GPIOF		  //LED所在端口为PROT F
#define RCC_APB2Periph_GPIO_LED           RCC_APB2Periph_GPIOF

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SysTick_Config(void);
void Delay(u32 nCount);
void Decrement_TimingDelay(void);
#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
