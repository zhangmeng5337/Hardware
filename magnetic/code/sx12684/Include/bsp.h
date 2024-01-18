/*===========================================================================
网址 ：http://yhmcu.taobao.com/
作者 ：李勇  原 亿和电子工作室  现 亿佰特电子科技有限公司
邮件 ：yihe_liyong@126.com
电话 ：18615799380
===========================================================================*/

#ifndef _BSP_H_
#define _BSP_H_
#include "stm8l15x.h"
#include "STM8l15x_conf.h"
#include "mytypedef.h"
#include "STM8l15x_spi.h"
#include "stm8l15x_tim3.h"
#include "stm8l15x_rtc.h"
#include "sysclock.h"
#include "uart1.h"
#include "stm8l15x_clk.h"
#define MODULE          0
#define DEBUG           0
#define EnableMaster false
//#define EnableMaster true
#if MODULE
#define RADIO_NSS_PORT     	   GPIOB
#define RADIO_NSS_PIN             GPIO_Pin_4

#define RADIO_nRESET_PORT            GPIOA
#define RADIO_nRESET_PIN             GPIO_Pin_5


#define RADIO_BUSY_PORT     	   GPIOB
#define RADIO_BUSY_PIN            GPIO_Pin_3

#define RADIO_DIO1_PORT     	   GPIOC
#define RADIO_DIO1_PIN            GPIO_Pin_2

#else

#define RADIO_NSS_PORT     	   GPIOB
#define RADIO_NSS_PIN             GPIO_Pin_4

#define RADIO_nRESET_PORT            GPIOB
#define RADIO_nRESET_PIN             GPIO_Pin_0

#define RADIO_TX_PORT        GPIOA
#define RADIO_TX_PIN         GPIO_Pin_5

#define RADIO_RX_PORT        GPIOA
#define RADIO_RX_PIN         GPIO_Pin_4

#define RADIO_BUSY_PORT     	   GPIOC
#define RADIO_BUSY_PIN            GPIO_Pin_6

#define RADIO_DIO1_PORT     	   GPIOC
#define RADIO_DIO1_PIN            GPIO_Pin_5

#endif





#define RADIO_DIO2_PORT     	   GPIOC
#define RADIO_DIO2_PIN            GPIO_Pin_4

#define RADIO_DIO3_PORT     	   GPIOC
#define RADIO_DIO3_PIN            GPIO_Pin_3

#define PORT_SX127X_DIO4     	   GPIOC
#define PIN_SX127X_DIO4            GPIO_Pin_2

#define PORT_SX127X_DIO5     	   GPIOC
#define PIN_SX127X_DIO5            GPIO_Pin_1

#define PORT_SX127X_M0             GPIOB
#define PIN_SX127X_M0              GPIO_Pin_1

#define PORT_SX127X_M1             GPIOB
#define PIN_SX127X_M1              GPIO_Pin_3

#define PORT_SX127X_AUX            GPIOB
#define PIN_SX127X_AUX             GPIO_Pin_2


#define SX126x_NSS_LOW( )          GPIO_ResetBits( RADIO_NSS_PORT, RADIO_NSS_PIN )
#define SX126x_NSS_HIGH( )         GPIO_SetBits( RADIO_NSS_PORT, RADIO_NSS_PIN )

#define SX1276_RESET_LOW( )        GPIO_ResetBits( RADIO_nRESET_PORT, RADIO_nRESET_PIN )
#define SX1276_RESET_HIGH( )       GPIO_SetBits( RADIO_nRESET_PORT, RADIO_nRESET_PIN )

#define SX1276_OSEN_LOW( )         GPIO_ResetBits( PORT_SX127X_OSEN, PIN_SX127X_OSEN )
#define SX1276_OSEN_HIGH( )        GPIO_SetBits( PORT_SX127X_OSEN, PIN_SX127X_OSEN )

#define SX126x_TX_CTRL_LOW( )      GPIO_ResetBits( RADIO_TX_PORT, RADIO_TX_PIN )
#define SX126x_TX_CTRL_HIGH( )     GPIO_SetBits( RADIO_TX_PORT, RADIO_TX_PIN )

#define SX126x_RX_CTRL_LOW( )      GPIO_ResetBits( RADIO_RX_PORT, RADIO_RX_PIN )
#define SX126x_RX_CTRL_HIGH( )     GPIO_SetBits( RADIO_RX_PORT, RADIO_RX_PIN )

//#define SX126x_TX_CTRL_HIGH( )      GPIO_ResetBits( RADIO_TX_PORT, RADIO_TX_PIN )
//#define SX126x_TX_CTRL_LOW( )     GPIO_SetBits( RADIO_TX_PORT, RADIO_TX_PIN )
//
//#define SX126x_RX_CTRL_HIGH( )      GPIO_ResetBits( RADIO_RX_PORT, RADIO_RX_PIN )
//#define SX126x_RX_CTRL_LOW( )     GPIO_SetBits( RADIO_RX_PORT, RADIO_RX_PIN )

//#define SX1276_READ_DIO0( )    GPIO_ReadInputDataBit( PORT_SX127X_BUSY, PIN_SX127X_BUSY )
#define SX1276_READ_DIO1( )    GPIO_ReadInputDataBit( RADIO_DIO1_PORT, RADIO_DIO1_PIN )
//#define SX1276_READ_DIO2( )    GPIO_ReadInputDataBit( PORT_SX127X_DIO2, PIN_SX127X_DIO2 )
#define SX1276_READ_DIO3( )    GPIO_ReadInputDataBit( RADIO_DIO3_PORT, RADIO_DIO3_PIN )
//#define SX1276_READ_DIO4( )    GPIO_ReadInputDataBit( PORT_SX127X_DIO4, PIN_SX127X_DIO4 )
//#define SX1276_READ_DIO5( )    GPIO_ReadInputDataBit( PORT_SX127X_DIO5, PIN_SX127X_DIO5 )



// SPI引脚定义 SCLK(PB5), MOSI(PB6), MISO(PB7)
#define PORT_SPI        GPIOB
#define PIN_SCLK        GPIO_Pin_5
#define PIN_MOSI        GPIO_Pin_6
#define PIN_MISO        GPIO_Pin_7

// LED和KEY引脚定义，LED(PB0), KEY(PC1)
#define PORT_LED        GPIOD
#define PIN_LED         GPIO_Pin_0

#define PORT_KEY        GPIOC
#define PIN_KEY         GPIO_Pin_1

// LED操作函数，(ON)打开, (OFF)关闭，(TOG)翻转
#define LED_ON()        GPIO_ResetBits(PORT_LED, PIN_LED)
#define LED_OFF()       GPIO_SetBits(PORT_LED, PIN_LED)
#define LED_TOG()       GPIO_ToggleBits(PORT_LED, PIN_LED)

#define AUX_CONFIGURING()       GPIO_ResetBits(PORT_SX127X_AUX, PIN_SX127X_AUX)
#define AUX_CONFIGURED()        GPIO_SetBits(PORT_SX127X_AUX, PIN_SX127X_AUX)

void SClK_Initial(void);                // 初始化系统时钟，系统时钟 = 16MHZ
void GPIO_int(void);                // 初始化通用IO端口
void SPI_Initial(void);                 // 初始化SPI
INT8U SPI_ExchangeByte(INT8U input);    // 通过SPI进行数据交换
void IWDG_Config(void);
void LED_Init(void);
void USART_SendStr(unsigned char *Str);
void IWDG_Config(void);
void LedToggle(void);
void HW_int(void);
#endif //_BSP_H_

/*===========================================================================
-----------------------------------文件结束----------------------------------
===========================================================================*/
