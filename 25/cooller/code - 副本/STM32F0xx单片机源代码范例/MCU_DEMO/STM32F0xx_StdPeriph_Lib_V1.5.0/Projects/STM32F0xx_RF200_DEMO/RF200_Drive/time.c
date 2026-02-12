/**--------------File Info------------------------------------------------------------------------------
** File name:			time.c
** Last modified Date:  2017-08-5
** Last Version:		V1.02
** Descriptions:		Time Config
**------------------------------------------------------------------------------------------------------
** Created date:		2017-08-5
** Version:				  V1.02
** Descriptions:		Time Config
**
********************************************************************************************************/
#define __TIME_MANL
#include "stm32f0xx.h"
#include "time.h"

/**********************************************************************************************************
TIM3_INIT:定时器3初始化函数
功能：定时器3配置
	    Tout=((arr+1)*(psc+1))/Tclk
	    Tclk:TIM3的输入时钟频率（单位：MHz）
	    Tout:TIM的溢出时间（单位：us）
参数：psc:预分频系数(prescaler)
	    arr:自动重装寄存器值（Auto relode register value）
**********************************************************************************************************/
void TIM3_INIT(unsigned int arr,unsigned int psc)
{
	  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	  
	  TIM_TimeBaseInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
	  TIM_TimeBaseInitStructure.TIM_CounterMode       = TIM_CounterMode_Up;
	  TIM_TimeBaseInitStructure.TIM_Period            = arr;
	  TIM_TimeBaseInitStructure.TIM_Prescaler         = psc;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	  NVIC_InitStructure.NVIC_IRQChannel         = TIM3_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;
	  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
	
	  NVIC_Init(&NVIC_InitStructure);
	  TIM_Cmd(TIM3,ENABLE);
}

/**********************************************************************************************************
TIM3_IRQHandler：定时器3中断
功能：
参数：
**********************************************************************************************************/
void TIM3_IRQHandler(void)
{

    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != DISABLE) {

		    if(++tmr1s >= 1000) {//1000ms
				    tmr1s  = 0;
					  flag1s = 1;
				}
		}
	  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);	
}


