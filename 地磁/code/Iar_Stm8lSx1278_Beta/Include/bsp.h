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
#include "sx1276.h"
#include "mytypedef.h"
#include "STM8l15x_spi.h"
#include "stm8l15x_tim3.h"
#include "stm8l15x_rtc.h"
#include "sysclock.h"
#include "uart1.h"
#include "stm8l15x_clk.h"
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
void GPIO_Initial(void);                // 初始化通用IO端口
void SPI_Initial(void);                 // 初始化SPI
INT8U SPI_ExchangeByte(INT8U input);    // 通过SPI进行数据交换
void HardwareInit(void);
void LED_Init(void);
void USART_SendStr(unsigned char *Str);

#endif //_BSP_H_

/*===========================================================================
-----------------------------------文件结束----------------------------------
===========================================================================*/
