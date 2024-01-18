
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : adc.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : adc program body
* Description    : --
**********************************************************************/

/* Includes  == �ļ����� --------------------------------------------*/
#include "adc.h"
#include "mp3.h"



/* Private typedef    ==  ˽������ ----------------------------------*/
/* Private define     ==  ˽�ж��� ----------------------------------*/

//-----------------------------------------//
#define	ADC_GPIO		GPIOC	
#define	ADC_PIN			GPIO_Pin_0	
#define	ADC_CLK			RCC_APB2Periph_GPIOC

//ADC1��������ݼĴ���
#define ADC1_DR_Address		((uint32_t)0x4001244C)


/* Private macro      ==  ˽�к� ------------------------------------*/
/* Private variables  ==  ˽�б��� ----------------------------------*/
volatile u32 ADCConvertedValue;


	
	
/* Private functions  ==  ˽�к������� ------------------------------*/

/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//ʹ��ADC���GPIO��ʱ��
	RCC_APB2PeriphClockCmd(ADC_CLK, ENABLE);

	//����LED���������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
		
	GPIO_InitStructure.GPIO_Pin = ADC_PIN;
	GPIO_Init(ADC_GPIO, &GPIO_InitStructure);	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void ADC_ADC_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	//ADC����ģʽ	 �����˫��ģʽ
	ADC_InitStructure.ADC_Mode					= ADC_Mode_Independent;
	//ɨ��ģʽ���ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ScanConvMode			= DISABLE;
	//��������ת��ģʽ����ת���걾�飨������һ�����������¿�ʼִ��
	//����ڵ���ģʽ��ת��һ�κ�ͽ���
	ADC_InitStructure.ADC_ContinuousConvMode	= ENABLE;
	//��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_ExternalTrigConv		= ADC_ExternalTrigConv_None;
	//�ɼ������Ҷ���
	ADC_InitStructure.ADC_DataAlign				= ADC_DataAlign_Right;
	//ת�����ͨ����Ŀ
	ADC_InitStructure.ADC_NbrOfChannel			= 1;
	//ִ��
	ADC_Init(ADC1, &ADC_InitStructure);
  
	//����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	/* ADC1 regular channel11 configuration */ 
	//����ADC1��ͨ��10Ϊ55.5����������
	//Ĭ���飬adc1 ��ͨ��10������Ϊ1,55.5����
	//ADC1,ch10,���1,55.5.����
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);

	/* Enable ADC1 DMA */
	//ʹ��ADC_DMA
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	//ʹ��ADC
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibration register */ 
	//ʹ��ADC1�ĸ�λУ׼�Ĵ���  
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	//�ȴ�У׼���
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibration */
	//ʹ��ADC1�Ŀ�ʼУ׼�Ĵ���
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	//�ȴ����
	while(ADC_GetCalibrationStatus(ADC1));
	 
	/* Start ADC1 Software Conversion */ 
	//ʹ���������������û�в����ⲿ����
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void ADC_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1); //ѡ��DMA��ͨ��1
	//�趨��ADC��������ݼĴ�����ADC1_DR_Address��ת�Ƶ��ڴ棨ADCConcertedValue��
	//ÿ�δ����С16λ��ʹ��DMAѭ������ģʽ
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	//���ݻ������ĵ�ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;
	//����Ϊ����Դ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	//���ݻ���������С2����
	DMA_InitStructure.DMA_BufferSize = 1;
	// �����ַ�̶�
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//�ڴ��ַ���ӣ�����adcʱ��ʹ�ܣ����ݴ���ʱ���ڴ�����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	//����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	//DMAѭ������
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	//���ȼ���
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	//??
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	//ִ��
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void ADC_Enable(void)
{
	ADC_GPIO_Config();
	ADC_DMA_Config();
	ADC_ADC_Config();
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : voltage��32λ�ĵ�ѹ���ݵ�ַ����ѹֵ�Ժ�����mv��Ϊ��λ
* Return         : None
* Remark         : None
**********************************************************************/
void ADC_GetVoltage(u32 *voltage)
{
	*voltage = (ADCConvertedValue * 3300) / 4096;
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : 
* Return         : None
* Remark         : None
**********************************************************************/
void ADC_CheckBattery(void)
{
	u32 ADC_Data[10];
	u32 power_vol;
	u8 power;
    u8 i;
	u32 little, big, sum;

	for(i=0; i<10; i++)
	{
		MICROSEC_Delay_ms(100);
		ADC_GetVoltage(TIME4_ADC_Data + i);
		ADC_GetVoltage(ADC_Data + i);
	}

	little = 100000;
	big = 0;
	sum = 0;
	for(i=0; i<10; i++)
	{
		if(ADC_Data[i] > big)
		{
			big = ADC_Data[i];
		}
		if(ADC_Data[i] < little)
		{
			little = ADC_Data[i];
		}
		sum += ADC_Data[i];
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
}




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
