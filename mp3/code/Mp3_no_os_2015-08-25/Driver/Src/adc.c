
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : adc.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : adc program body
* Description    : --
**********************************************************************/

/* Includes  == 文件包含 --------------------------------------------*/
#include "adc.h"
#include "mp3.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//
#define	ADC_GPIO		GPIOC	
#define	ADC_PIN			GPIO_Pin_0	
#define	ADC_CLK			RCC_APB2Periph_GPIOC

//ADC1外设的数据寄存器
#define ADC1_DR_Address		((uint32_t)0x4001244C)


/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/
volatile u32 ADCConvertedValue;


	
	
/* Private functions  ==  私有函数定义 ------------------------------*/

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

	//使能ADC相关GPIO口时钟
	RCC_APB2PeriphClockCmd(ADC_CLK, ENABLE);

	//配置LED的输出引脚
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
	
	//ADC独立模式	 相对于双重模式
	ADC_InitStructure.ADC_Mode					= ADC_Mode_Independent;
	//扫描模式用于多通道采集
	ADC_InitStructure.ADC_ScanConvMode			= DISABLE;
	//开启连续转换模式：当转换完本组（可能是一个）继续重新开始执行
	//相对于单次模式：转换一次后就结束
	ADC_InitStructure.ADC_ContinuousConvMode	= ENABLE;
	//不使用外部触发转换
	ADC_InitStructure.ADC_ExternalTrigConv		= ADC_ExternalTrigConv_None;
	//采集数据右对齐
	ADC_InitStructure.ADC_DataAlign				= ADC_DataAlign_Right;
	//转换组的通道数目
	ADC_InitStructure.ADC_NbrOfChannel			= 1;
	//执行
	ADC_Init(ADC1, &ADC_InitStructure);
  
	//配置ADC时钟，为PCLK2的8分频，即9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	/* ADC1 regular channel11 configuration */ 
	//配置ADC1的通道10为55.5个采样周期
	//默认组，adc1 ，通道10，排序为1,55.5周期
	//ADC1,ch10,序号1,55.5.。。
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);

	/* Enable ADC1 DMA */
	//使能ADC_DMA
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	//使能ADC
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibration register */ 
	//使能ADC1的复位校准寄存器  
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	//等待校准完成
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibration */
	//使能ADC1的开始校准寄存器
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	//等待完成
	while(ADC_GetCalibrationStatus(ADC1));
	 
	/* Start ADC1 Software Conversion */ 
	//使用软件触发，由于没有采用外部触发
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
	DMA_DeInit(DMA1_Channel1); //选择DMA的通道1
	//设定从ADC外设的数据寄存器（ADC1_DR_Address）转移到内存（ADCConcertedValue）
	//每次传输大小16位，使用DMA循环传输模式
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	//数据缓冲区的地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;
	//外设为数据源
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	//数据缓冲区，大小2半字
	DMA_InitStructure.DMA_BufferSize = 1;
	// 外设地址固定
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//内存地址增加，多组adc时，使能，数据传输时，内存增加
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	//半字
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	//DMA循环传输
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	//优先级高
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	//??
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	//执行
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
* Output         : voltage：32位的电压数据地址，电压值以毫伏（mv）为单位
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
