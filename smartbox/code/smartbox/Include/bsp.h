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

#define ON   1
#define OFF  0


#define BATEERY_QUANTITY_PORT           GPIOA           //BATTERY  quantity level
#define BATEERY_QUANTITY_LEVEL1_PIN     GPIO_Pin_5
#define BATEERY_QUANTITY_LEVEL2_PIN     GPIO_Pin_6
#define BATEERY_QUANTITY_LEVEL3_PIN     GPIO_Pin_7

#define MODULE_STATUS_PORT              GPIOA      //module status led
#define MODULE_STATUS_PIN               GPIO_Pin_7

#define LOCK_CTRL_PORT                  GPIOE        // lock io
#define LOCK_CTRL_PIN                   GPIO_Pin_1
#define LOCK_FB_PORT                    GPIOB
#define LOCK_FB_PIN                     GPIO_Pin_0

#define GNSS_ENABLE_PORT                GPIOB          //gsm related ios
#define GNSS_ENABLE_PIN                 GPIO_Pin_1
#define GSM_PWR_PORT                    GPIOB
#define GSM_PWR_PIN                     GPIO_Pin_2


#define VBAT_SENSE_CHANNEL              ADC_Channel_23

void HardwareInit(void);
void LED_Init(void);



#endif //_BSP_H_

/*===========================================================================
-----------------------------------文件结束----------------------------------
===========================================================================*/
