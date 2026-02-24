
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : microsec.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : microsec program body
* Description    : --
//V1.1：将获取时间函数的返回时间值由32位改为64位，32位系统运行71分钟就会溢出从0开始，64位不会溢出
//TIM2计数时钟为1us计数器加1，计数值为0xffff。 TIM8的计数时钟为TIM2的计数溢出时钟，其计数值也为0xffff。TIM8计数溢出会进入TIM8的中断，
//在中断中将全局变量G_SystemTime_high32bit加1。所以时间值为 (G_SystemTime_high32bit << 32) + (TIM8->CNT << 16) + TIM8->CNT
//以us（微妙）为单位
**********************************************************************/

/* Includes  == 文件包含 --------------------------------------------*/
#include "microsec.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//
//定义64位的定时器时钟源，用到两个定时器
#define	D_MICROSEC_LowTimeSource	 		TIM2
#define	D_MICROSEC_LowTimeClk	    		RCC_APB1Periph_TIM2
#define	D_MICROSEC_HighTimeSource	 		TIM3
#define	D_MICROSEC_HighTimeClk	    		RCC_APB1Periph_TIM3

#define	D_MICROSEC_HighTimeIRQn	    		TIM3_IRQn
#define	D_MICROSEC_HighTimeIRQHandler		TIM3_IRQHandler



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/
volatile	s64 G_MICROSEC_High32Bit;



/* Private functions  ==  私有函数定义 ------------------------------*/

/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void MICROSEC_Init(void)
{
	G_MICROSEC_High32Bit = 0;
	MICROSEC_Tim_Config();
	MICROSEC_NVIC_Config();
	TIM_ITConfig(D_MICROSEC_HighTimeSource, TIM_IT_Update, ENABLE);
}	



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void MICROSEC_Tim_Config(void)
{
	u16 PrescalerValue;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	//使能定时器2和定时器8的时钟
	RCC_APB1PeriphClockCmd(D_MICROSEC_LowTimeClk, ENABLE);
	RCC_APB1PeriphClockCmd(D_MICROSEC_HighTimeClk, ENABLE);

	/* TIM2 configuration*/ 
	/* Time Base configuration 基本配置 配置定时器的时基单元*/
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
	TIM_TimeBaseStructure.TIM_Period = 0xffff; //自动重装值         
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;       
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(D_MICROSEC_HighTimeSource, &TIM_TimeBaseStructure); 

	TIM_PrescalerConfig(D_MICROSEC_HighTimeSource, 0, TIM_PSCReloadMode_Update);
	
	/* Disable the TIM8 Update event */
	TIM_UpdateDisableConfig(D_MICROSEC_HighTimeSource, ENABLE);
	
	/* ----------------------TIM2 Configuration as slave for the TIM3 ----------*/
	/* Select the TIM8 Input Trigger: TIM2 TRGO used as Input Trigger for TIM8*/
	TIM_SelectInputTrigger(D_MICROSEC_HighTimeSource, TIM_TS_ITR1);
	/* Use the External Clock as TIM8 Slave Mode */
	TIM_SelectSlaveMode(D_MICROSEC_HighTimeSource, TIM_SlaveMode_External1);
	/* Enable the TIM8 Master Slave Mode */
	TIM_SelectMasterSlaveMode(D_MICROSEC_HighTimeSource, TIM_MasterSlaveMode_Enable);
	TIM_ARRPreloadConfig(D_MICROSEC_HighTimeSource, ENABLE);	
	/* 定时器配置:
	1.设置定时器最大计数值 50000
	2.设置时钟分频系数：TIM_CKD_DIV1
	3. 设置预分频：  1Mhz/50000= 1hz 
	4.定时器计数模式  向上计数模式
	*/
	
	PrescalerValue = (uint16_t) (SystemCoreClock / 1000000) - 1;   //定时器的时钟基准为1MHz/

	TIM_TimeBaseStructure.TIM_Period = 0xffff;     
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;	         //1M 的时钟  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	//应用配置到TIM2 
	TIM_TimeBaseInit(D_MICROSEC_LowTimeSource, &TIM_TimeBaseStructure);
	// 使能TIM2重载寄存器ARR
	TIM_ARRPreloadConfig(D_MICROSEC_LowTimeSource, ENABLE);	

	TIM_SelectSlaveMode(D_MICROSEC_LowTimeSource, TIM_SlaveMode_Reset);
	TIM_UpdateRequestConfig(D_MICROSEC_LowTimeSource, TIM_UpdateSource_Regular);
	/* ----------------------TIM2 Configuration as Master for the TIM8 -----------*/
	/* Use the TIM2 Update event  as TIM2 Trigger Output(TRGO) */
	TIM_SelectOutputTrigger(D_MICROSEC_LowTimeSource, TIM_TRGOSource_Update);
	/* Enable the TIM2 Master Slave Mode */
	TIM_SelectMasterSlaveMode(D_MICROSEC_LowTimeSource, TIM_MasterSlaveMode_Enable);

	//启动定时器
	TIM_UpdateDisableConfig(D_MICROSEC_HighTimeSource, DISABLE);
	TIM_ClearITPendingBit(D_MICROSEC_HighTimeSource, TIM_IT_Update);  //清除更新标志位，如不清楚则一开定时器就会进入中断，因为在初始化寄存器时会触发一次更新事件
	
	TIM_Cmd(D_MICROSEC_LowTimeSource, ENABLE); 
	TIM_Cmd(D_MICROSEC_HighTimeSource, ENABLE);                  
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
s64 MICROSEC_GetTime(void)
{
	s64 time;
	time = 0;

	time = 	((G_MICROSEC_High32Bit << 32) & 0xffffffff00000000) + 
				((D_MICROSEC_HighTimeSource->CNT << 16) & 0x00000000ffff0000) + 
				(D_MICROSEC_LowTimeSource->CNT & 0x000000000000ffff);
	
	return time;
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void MICROSEC_Delay_ms(s64 tim)
{
   s64 now; 
	now = MICROSEC_GetTime();
	while((MICROSEC_GetTime() - now) < (tim * 1000))
	{;}
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void MICROSEC_Delay_us(s64 tim)
{
   s64 now; 
	now = MICROSEC_GetTime();
	while((MICROSEC_GetTime() - now) < tim)
	{;}
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void MICROSEC_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

	/* Configure and enable I2C DMA RX Channel interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = D_MICROSEC_HighTimeIRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
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
void D_MICROSEC_HighTimeIRQHandler(void)
 {
	//如果TIM8更新中断
	if(TIM_GetITStatus(D_MICROSEC_HighTimeSource, TIM_IT_Update) != RESET)
	{      
		TIM_ClearITPendingBit(D_MICROSEC_HighTimeSource, TIM_IT_Update);
		G_MICROSEC_High32Bit = G_MICROSEC_High32Bit + 1;                                               		
	}
}




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
