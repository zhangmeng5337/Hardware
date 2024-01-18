
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

 

/* Includes  == �ļ����� --------------------------------------------*/
#include "stm32f10x.h"



/* Private typedef    ==  ˽������ ----------------------------------*/
/* Private define     ==  ˽�ж��� ----------------------------------*/

//-----------------------------------------//



/* Private macro      ==  ˽�к� ------------------------------------*/
/* Private variables  ==  ˽�б��� ----------------------------------*/



/* Private function prototypes  ==  ˽�к������� --------------------*/
void ADC_GPIO_Config(void);
void ADC_ADC_Config(void);
void ADC_DMA_Config(void);
void ADC_Enable(void);
void ADC_GetVoltage(u32 *voltage);
void ADC_CheckBattery(void);



/* Define to prevent recursive inclusion ----------------------------*/
#endif /* __ADC_H */




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
