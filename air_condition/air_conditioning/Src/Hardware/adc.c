#include "main.h"
extern ADC_HandleTypeDef hadc1;


//获得ADC值
//ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//返回值:转换结果
unsigned int Get_Adc(uint32_t ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //通道
    ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_41CYCLES_5;        //采样时间
    //ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        //通道配置
	
    HAL_ADC_Start(&hadc1);                               //开启ADC
	
    HAL_ADC_PollForConversion(&hadc1,10);                //轮询转换
 
	return (unsigned int)HAL_ADC_GetValue(&hadc1);	        //返回最近一次ADC1规则组的转换结果
}
//获取指定通道的转换值，取times次,然后平均 
//times:获取次数
//返回值:通道ch的times次转换结果平均值
unsigned int Get_Adc_Average(uint32_t ch,unsigned char times)
{
	uint32_t temp_val=0,adcBuf[5];
	unsigned char i,t;
	for(i=0;i<5;i++)
	{
		HAL_ADC_Start(&hadc1);
		for(t=0;t<times;t++)
		{
			//temp_val+=Get_Adc(ch);	
			HAL_ADC_PollForConversion(&hadc1,0xffff);
			adcBuf[i]=HAL_ADC_GetValue(&hadc1);
			//delay_ms(5);
		}
		HAL_ADC_Stop(&hadc1);
	}
	return temp_val/times;
} 
