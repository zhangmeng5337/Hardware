
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : microsec.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : microsec program body
* Description    : --
//V1.1������ȡʱ�亯���ķ���ʱ��ֵ��32λ��Ϊ64λ��32λϵͳ����71���Ӿͻ������0��ʼ��64λ�������
//TIM2����ʱ��Ϊ1us��������1������ֵΪ0xffff�� TIM8�ļ���ʱ��ΪTIM2�ļ������ʱ�ӣ������ֵҲΪ0xffff��TIM8������������TIM8���жϣ�
//���ж��н�ȫ�ֱ���G_SystemTime_high32bit��1������ʱ��ֵΪ (G_SystemTime_high32bit << 32) + (TIM8->CNT << 16) + TIM8->CNT
//��us��΢�Ϊ��λ
**********************************************************************/

/* Includes  == �ļ����� --------------------------------------------*/
#include "microsec.h"



/* Private typedef    ==  ˽������ ----------------------------------*/
/* Private define     ==  ˽�ж��� ----------------------------------*/

//-----------------------------------------//
//����64λ�Ķ�ʱ��ʱ��Դ���õ�������ʱ��
#define	D_MICROSEC_LowTimeSource	 		TIM2
#define	D_MICROSEC_LowTimeClk	    		RCC_APB1Periph_TIM2
#define	D_MICROSEC_HighTimeSource	 		TIM3
#define	D_MICROSEC_HighTimeClk	    		RCC_APB1Periph_TIM3

#define	D_MICROSEC_HighTimeIRQn	    		TIM3_IRQn
#define	D_MICROSEC_HighTimeIRQHandler		TIM3_IRQHandler



/* Private macro      ==  ˽�к� ------------------------------------*/
/* Private variables  ==  ˽�б��� ----------------------------------*/
volatile	s64 G_MICROSEC_High32Bit;



/* Private functions  ==  ˽�к������� ------------------------------*/

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

	//ʹ�ܶ�ʱ��2�Ͷ�ʱ��8��ʱ��
	RCC_APB1PeriphClockCmd(D_MICROSEC_LowTimeClk, ENABLE);
	RCC_APB1PeriphClockCmd(D_MICROSEC_HighTimeClk, ENABLE);

	/* TIM2 configuration*/ 
	/* Time Base configuration �������� ���ö�ʱ����ʱ����Ԫ*/
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
	TIM_TimeBaseStructure.TIM_Period = 0xffff; //�Զ���װֵ         
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
	/* ��ʱ������:
	1.���ö�ʱ��������ֵ 50000
	2.����ʱ�ӷ�Ƶϵ����TIM_CKD_DIV1
	3. ����Ԥ��Ƶ��  1Mhz/50000= 1hz 
	4.��ʱ������ģʽ  ���ϼ���ģʽ
	*/
	
	PrescalerValue = (uint16_t) (SystemCoreClock / 1000000) - 1;   //��ʱ����ʱ�ӻ�׼Ϊ1MHz/

	TIM_TimeBaseStructure.TIM_Period = 0xffff;     
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;	         //1M ��ʱ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	//Ӧ�����õ�TIM2 
	TIM_TimeBaseInit(D_MICROSEC_LowTimeSource, &TIM_TimeBaseStructure);
	// ʹ��TIM2���ؼĴ���ARR
	TIM_ARRPreloadConfig(D_MICROSEC_LowTimeSource, ENABLE);	

	TIM_SelectSlaveMode(D_MICROSEC_LowTimeSource, TIM_SlaveMode_Reset);
	TIM_UpdateRequestConfig(D_MICROSEC_LowTimeSource, TIM_UpdateSource_Regular);
	/* ----------------------TIM2 Configuration as Master for the TIM8 -----------*/
	/* Use the TIM2 Update event  as TIM2 Trigger Output(TRGO) */
	TIM_SelectOutputTrigger(D_MICROSEC_LowTimeSource, TIM_TRGOSource_Update);
	/* Enable the TIM2 Master Slave Mode */
	TIM_SelectMasterSlaveMode(D_MICROSEC_LowTimeSource, TIM_MasterSlaveMode_Enable);

	//������ʱ��
	TIM_UpdateDisableConfig(D_MICROSEC_HighTimeSource, DISABLE);
	TIM_ClearITPendingBit(D_MICROSEC_HighTimeSource, TIM_IT_Update);  //������±�־λ���粻�����һ����ʱ���ͻ�����жϣ���Ϊ�ڳ�ʼ���Ĵ���ʱ�ᴥ��һ�θ����¼�
	
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
	//���TIM8�����ж�
	if(TIM_GetITStatus(D_MICROSEC_HighTimeSource, TIM_IT_Update) != RESET)
	{      
		TIM_ClearITPendingBit(D_MICROSEC_HighTimeSource, TIM_IT_Update);
		G_MICROSEC_High32Bit = G_MICROSEC_High32Bit + 1;                                               		
	}
}




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
