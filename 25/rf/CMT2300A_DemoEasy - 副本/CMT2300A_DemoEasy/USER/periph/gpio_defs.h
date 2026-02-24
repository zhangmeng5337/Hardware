#ifndef __GPIO_DEFS_H
#define __GPIO_DEFS_H

#include "stm32f10x_conf.h"

#define CMT_CSB_GPIO                GPIOB
#define CMT_CSB_GPIO_PIN            GPIO_Pin_6

#define CMT_FCSB_GPIO               GPIOD
#define CMT_FCSB_GPIO_PIN           GPIO_Pin_7

#define CMT_SCLK_GPIO               GPIOD
#define CMT_SCLK_GPIO_PIN           GPIO_Pin_4

#define CMT_SDIO_GPIO               GPIOD
#define CMT_SDIO_GPIO_PIN           GPIO_Pin_2

#define CMT_GPIO1_GPIO              GPIOD
#define CMT_GPIO1_GPIO_PIN          GPIO_Pin_5

#define CMT_GPIO2_GPIO              GPIOD
#define CMT_GPIO2_GPIO_PIN          GPIO_Pin_3

#define CMT_GPIO3_GPIO              GPIOD
#define CMT_GPIO3_GPIO_PIN          GPIO_Pin_0

#define CMT_GPIO4_GPIO              GPIOD
#define CMT_GPIO4_GPIO_PIN          GPIO_Pin_1



#define SET_GPIO_OUT(x)             GPIO_Pin_Setting(x, x##_PIN, GPIO_Speed_50MHz, GPIO_Mode_Out_PP)
#define SET_GPIO_IN(x)              GPIO_Pin_Setting(x, x##_PIN, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING)
#define SET_GPIO_OD(x)              GPIO_Pin_Setting(x, x##_PIN, GPIO_Speed_50MHz, GPIO_Mode_Out_OD)
#define SET_GPIO_AIN(x)             GPIO_Pin_Setting(x, x##_PIN, GPIO_Speed_50MHz, GPIO_Mode_AIN)
#define SET_GPIO_AFOUT(x)           GPIO_Pin_Setting(x, x##_PIN, GPIO_Speed_50MHz, GPIO_Mode_AF_PP)
#define SET_GPIO_AFOD(x)            GPIO_Pin_Setting(x, x##_PIN, GPIO_Speed_50MHz, GPIO_Mode_AF_OD)
#define SET_GPIO_H(x)               (x->BSRR = x##_PIN) //GPIO_SetBits(x, x##_PIN)
#define SET_GPIO_L(x)               (x->BRR  = x##_PIN) //GPIO_ResetBits(x, x##_PIN)
#define READ_GPIO_PIN(x)            (((x->IDR & x##_PIN)!=Bit_RESET) ?1 :0) //GPIO_ReadInputDataBit(x, x##_PIN) 

#endif
