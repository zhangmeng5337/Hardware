
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : timer.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : timer program body
* Description    : --
**********************************************************************/

/* Includes  == 文件包含 --------------------------------------------*/
#include "timer.h"
#include "adc.h"
#include "display.h"
#include "playsong.h"
#include "mp3.h"

#include "ff.h"
#include "diskio.h"
#include <string.h>



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//
#define	D_TIMER4_Source	 			TIM4
#define	D_TIMER4_Clk	    		RCC_APB1Periph_TIM4
#define	D_TIMER4_IRQn	    		TIM4_IRQn
#define	D_TIMER4_IRQHandler			TIM4_IRQHandler
#define	D_TIMER4_p					TIM4_IRQHandler

#define	D_TIMER5_Source	 			TIM5
#define	D_TIMER5_Clk	    		RCC_APB1Periph_TIM5
#define	D_TIMER5_IRQn	    		TIM5_IRQn
#define	D_TIMER5_IRQHandler			TIM5_IRQHandler
#define	D_TIMER5_p					TIM5_IRQHandler

u32 TIME4_ADC_Data[10];
u32 TIME4_BackLightTime;
u8 TIME4_BackLightFlag;



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/



/* Private functions  ==  私有函数定义 ------------------------------*/

/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	TIMER_Enble(void)
{
	TIME4_BackLightTime = 0;
	TIME4_BackLightFlag = 0;
	TIMER4_Config(10000);        //每100ms进一次中断
	TIMER4_NVIC_Config();
	TIM_ITConfig(D_TIMER4_Source, TIM_IT_Update, ENABLE);
	
//	TIMER5_Config(2000);
//	TIMER5_NVIC_Config();
//	TIM_ITConfig(D_TIMER5_Source, TIM_IT_Update, ENABLE);
}	



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void TIMER_Disble(void)
{
	TIM_Cmd(D_TIMER4_Source, DISABLE);
	TIM_Cmd(D_TIMER5_Source, DISABLE);
}	



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : period：周期，以10乘以微妙（10 * us）为单位
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	TIMER4_Config(u16 period)
{
	u16 PrescalerValue;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	//使能定时器4的时钟
	RCC_APB1PeriphClockCmd(D_TIMER4_Clk, ENABLE);

	/* 定时器配置:
	1.设置定时器最大计数值 50000
	2.设置时钟分频系数：TIM_CKD_DIV1
	3. 设置预分频：  1Mhz/50000= 1hz 
	4.定时器计数模式  向上计数模式
	*/
	
	PrescalerValue = (uint16_t) (SystemCoreClock / 10000) - 1;   //定时器的时钟基准为100KHz/

	TIM_TimeBaseStructure.TIM_Period = period;     
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;	         //100K 的时钟  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	//应用配置到TIM2 
	TIM_TimeBaseInit(D_TIMER4_Source, &TIM_TimeBaseStructure);
	// 使能TIM2重载寄存器ARR
	TIM_ARRPreloadConfig(D_TIMER4_Source, ENABLE);	

	TIM_UpdateRequestConfig(D_TIMER4_Source, TIM_UpdateSource_Regular);

	//启动定时器
	TIM_UpdateDisableConfig(D_TIMER4_Source, DISABLE);
	TIM_ClearITPendingBit(D_TIMER4_Source, TIM_IT_Update);  //清除更新标志位，如不清楚则一开定时器就会进入中断，因为在初始化寄存器时会触发一次更新事件
	
	TIM_Cmd(D_TIMER4_Source, ENABLE);                  
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	TIMER4_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* Configure and enable I2C DMA RX Channel interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = D_TIMER4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure); 		
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	D_TIMER4_IRQHandler(void)
{	
	u32 power_vol;
	u8 power;
    u8 i;
	u32 little, big, sum;
	//如果TIM4更新中断
	if(TIM_GetITStatus(D_TIMER4_Source, TIM_IT_Update) != RESET)
	{      
		TIM_ClearITPendingBit(D_TIMER4_Source, TIM_IT_Update);
		for(i=0; i<9; i++)
		{
			TIME4_ADC_Data[i] = TIME4_ADC_Data[i + 1];
		}
		ADC_GetVoltage(TIME4_ADC_Data + 9);
		little = 100000;
		big = 0;
		sum = 0;
		for(i=0; i<10; i++)
		{
			if(TIME4_ADC_Data[i] > big)
			{
				big = TIME4_ADC_Data[i];
			}
			if(TIME4_ADC_Data[i] < little)
			{
				little = TIME4_ADC_Data[i];
			}
			sum += TIME4_ADC_Data[i];
		}
		sum -= big;
		sum -= little;
		power_vol = sum >> 3;
		
		if(power_vol < 1700)
		{
			power = 0;
		}
		else if(power_vol > 2050)
		{
			power = 100;
		}
		else
		{
			power = (power_vol - 1700) / 35;
			power = power * 10;
		}

		if(power == 0)
		{
			MP3_LowBatteryWithoutSD();
		}
		else
		{
			Display_Power(power);
		}
		
		TIME4_BackLightTime += 1;
		if(TIME4_BackLightTime > 20)
		{
			OLED_SetDisplayOnOff(0x00);
			TIME4_BackLightFlag = 1;
		}
	}
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : period：周期，以10乘以微妙（10 * us）为单位
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	TIMER5_Config(u16 period)
{
	u16 PrescalerValue;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	//使能定时器4的时钟
	RCC_APB1PeriphClockCmd(D_TIMER5_Clk, ENABLE);

	/* 定时器配置:
	1.设置定时器最大计数值 50000
	2.设置时钟分频系数：TIM_CKD_DIV1
	3. 设置预分频：  1Mhz/50000= 1hz 
	4.定时器计数模式  向上计数模式
	*/
	
	PrescalerValue = (uint16_t) (SystemCoreClock / 100000) - 1;   //定时器的时钟基准为100KHz/

	TIM_TimeBaseStructure.TIM_Period = period;     
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;	         //100K 的时钟  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	//应用配置到TIM2 
	TIM_TimeBaseInit(D_TIMER5_Source, &TIM_TimeBaseStructure);
	// 使能TIM2重载寄存器ARR
	TIM_ARRPreloadConfig(D_TIMER5_Source, ENABLE);	

	TIM_UpdateRequestConfig(D_TIMER5_Source, TIM_UpdateSource_Regular);

	//启动定时器
	TIM_UpdateDisableConfig(D_TIMER5_Source, DISABLE);
	TIM_ClearITPendingBit(D_TIMER5_Source, TIM_IT_Update);  //清除更新标志位，如不清楚则一开定时器就会进入中断，因为在初始化寄存器时会触发一次更新事件
	
	TIM_Cmd(D_TIMER5_Source, ENABLE);                  
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	TIMER5_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* Configure and enable I2C DMA RX Channel interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = D_TIMER5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure); 		
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	D_TIMER5_IRQHandler(void)
{

}
 



/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
