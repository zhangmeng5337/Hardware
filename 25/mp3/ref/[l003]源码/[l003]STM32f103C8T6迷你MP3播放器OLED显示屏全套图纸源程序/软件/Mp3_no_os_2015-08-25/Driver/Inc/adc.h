
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : adc.h
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : Header for adc.c module
* Description    : --
**********************************************************************/

/* Define to prevent recursive inclusion ----------------------------*/
#ifndef __ADC_H
#define __ADC_H

 

/* Includes  == 文件包含 --------------------------------------------*/
#include "stm32f10x.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/



/* Private function prototypes  ==  私有函数声明 --------------------*/
void ADC_GPIO_Config(void);
void ADC_ADC_Config(void);
void ADC_DMA_Config(void);
void ADC_Enable(void);
void ADC_GetVoltage(u32 *voltage);
void ADC_CheckBattery(void);



/* Define to prevent recursive inclusion ----------------------------*/
#endif /* __ADC_H */




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
