/*===========================================================================
��ַ ��http://yhmcu.taobao.com/
���� ������  ԭ �ں͵��ӹ�����  �� �ڰ��ص��ӿƼ����޹�˾
�ʼ� ��yihe_liyong@126.com
�绰 ��18615799380
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
#include "GSM_BSP.h"
#define ON   1
#define OFF  0
#define DEGUG_SENSOR    1
// SPI���Ŷ��� SCLK(PB5), MOSI(PB6), MISO(PB7)
#define PORT_SPI        GPIOB
#define PIN_SCLK        GPIO_Pin_5
#define PIN_MOSI        GPIO_Pin_6
#define PIN_MISO        GPIO_Pin_7

// LED��KEY���Ŷ��壬LED(PB0), KEY(PC1)
#define PORT_LED        GPIOB
#define PIN_LED         GPIO_Pin_3

#define PORT_KEY        GPIOD
#define PIN_KEY         GPIO_Pin_1

#define PORT_POWER_ON        GPIOC
//#define PIN_POWER_ON         GPIO_Pin_0
#define PIN_POWER_ON         GPIO_Pin_2
#define PORT_PWRKEY_IN       GPIOD
#define PIN_PWRKEY_IN        GPIO_Pin_4

#define PORT_SENSOR_EN       GPIOB
#define PIN_SENSOR_EN       GPIO_Pin_7
// LED����������(ON)��, (OFF)�رգ�(TOG)��ת
#define LED_ON()        GPIO_ResetBits(PORT_LED, PIN_LED)
#define LED_OFF()       GPIO_SetBits(PORT_LED, PIN_LED)
#define LED_TOG()       GPIO_ToggleBits(PORT_LED, PIN_LED)

#define ADC_BAT_CHANNEL         ADC_Channel_1
#define ADC_SENSOR_CHANNEL      ADC_Channel_2

#define samplecount			200


void GSM_HardwareInit(unsigned char flag);

void RTC_Config(uint16_t time,unsigned char flag);

void GPIO_Initial(void);                // ��ʼ��ͨ��IO�˿�

void HardwareInit(void);
void LED_Init(void);
void USART_SendStr(unsigned char *Str);
void USART_SenByte(unsigned char *Str,unsigned char len) ;
uint32_t adcGet(ADC_Channel_TypeDef num);
void delay_ms(uint32_t num);//���Ǻܾ�ȷ
void DMA_START_RX(void);

#endif //_BSP_H_

/*===========================================================================
-----------------------------------�ļ�����----------------------------------
===========================================================================*/
