/*===========================================================================
��ַ ��http://yhmcu.taobao.com/
���� ������  ԭ �ں͵��ӹ�����  �� �ڰ��ص��ӿƼ����޹�˾
�ʼ� ��yihe_liyong@126.com
�绰 ��18615799380
===========================================================================*/

#ifndef _BSP_H_
#define _BSP_H_

#include "STM8l15x_conf.h"
#include "sx1276.h"
//#include "MyTypeDef.h"

// SPI���Ŷ��� SCLK(PB5), MOSI(PB6), MISO(PB7)
#define PORT_SPI        GPIOB
#define PIN_SCLK        GPIO_Pin_5
#define PIN_MOSI        GPIO_Pin_6
#define PIN_MISO        GPIO_Pin_7

// LED��KEY���Ŷ��壬LED(PB0), KEY(PC1)
#define version_a	0

//#if 0
#define PORT_LED        GPIOD
#define PIN_LED         GPIO_Pin_0
//#if 1
//#define PORT_LED        GPIOC
//#define PIN_LED         GPIO_Pin_0
//#endif 


#define LORA_AUX_PORT       GPIOB
#define LORA_AUX            GPIO_Pin_0

#define LORA_M0_PORT       GPIOB
#define LORA_M0            GPIO_Pin_2

#define LORA_M1_PORT       GPIOB
#define LORA_M1            GPIO_Pin_1

// LED����������(ON)��, (OFF)�رգ�(TOG)��ת
#define LED_ON()        GPIO_ResetBits(PORT_LED, PIN_LED)
#define LED_OFF()       GPIO_SetBits(PORT_LED, PIN_LED)
#define LED_TOG()       GPIO_ToggleBits(PORT_LED, PIN_LED)
#define debug   1
#define NormalMode      0x00
#define WakeMode        0x10
#define PowerSavingMode 0x01

typedef unsigned char  INT8U;
typedef signed   char  INT8S;
typedef unsigned int   INT16U;
typedef signed   int   INT16S;
typedef unsigned long  INT32U;
typedef signed   long  INT32S;


//void SClK_Initial(void);                // ��ʼ��ϵͳʱ�ӣ�ϵͳʱ�� = 16MHZ
//void GPIO_Initial(void);                // ��ʼ��ͨ��IO�˿�
//void SPI_Initial(void);                 // ��ʼ��SPI
//INT8U SPI_ExchangeByte(INT8U input);    // ͨ��SPI�������ݽ���
void HardwareInit(void);
INT8U SPI_ExchangeByte(INT8U input);
#endif //_BSP_H_

/*===========================================================================
-----------------------------------�ļ�����----------------------------------
===========================================================================*/
