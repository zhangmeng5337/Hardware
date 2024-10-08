/***********************************************************************************************************************
* file           : timer.h
* author         : --
* version        : --
* date           : 2014-9-1
* brief          : Header for timer.c module
***********************************************************************************************************************/

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef __TIMER_H
#define __TIMER_H

 

/* Includes  == 文件包含 ---------------------------------------------------------------------------------------------*/
#include "stm32f10x.h"



/* Private typedef    ==  私有类型 -----------------------------------------------------------------------------------*/
/* Private define     ==  私有定义------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------*/



/* Private macro      ==  私有宏 -------------------------------------------------------------------------------------*/
/* Private variables  ==  私有变量 -----------------------------------------------------------------------------------*/
extern u32 TIME4_ADC_Data[10];
extern u32 TIME4_BackLightTime;
extern u8 TIME4_BackLightFlag;



/* Private function prototypes  ==  私有函数声明 ---------------------------------------------------------------------*/
void	TIMER_Enble(void);
void TIMER_Disble(void);
void	TIMER4_Config(u16 period);
void	TIMER4_NVIC_Config(void);
void	D_TIMER4_IRQHandler(void);

void	TIMER5_Config(u16 period);
void	TIMER5_NVIC_Config(void);
void	D_TIMER5_IRQHandler(void);



/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#endif /* __TIMER_H */




/*********************************  COPYRIGHT 2014 --------  *********  END OF FILE  **********************************/
