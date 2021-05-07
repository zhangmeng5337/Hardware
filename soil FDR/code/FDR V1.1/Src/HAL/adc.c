#include "adc.h"
#include "app.h"
#include "filter.h"
#include "FdrAlgorithm.h"
#include "calman.h"
extern ADC_HandleTypeDef hadc1;


sensor_stru sensor_usr;
extern uint32_t	freq_count;
float adc_value;
uint32_t adcBuf_ref[N];	//参考电压缓冲区
uint32_t adcBuf_humid[N];	//湿度电压缓冲区
uint32_t adcBuf_ta[N];//温度a电压缓冲区
uint32_t adcBuf_tb[N];//温度b电压缓冲区

sensor_stru *getSensor()
{
	return &sensor_usr;
}

/*********************************************************
            adc采集探针，温度传感器电压
*********************************************************/
void Get_Adc_Average(unsigned char times)
{

	uint32_t temp_val=0;
	unsigned char i,t;
 

	HAL_ADCEx_Calibration_Start(&hadc1);
	for(i=0;i<N;i++)
	{                               //开启ADC
	
		for(t=1;t<4;t++)
		{
			
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1,0xffff);
			temp_val = HAL_ADC_GetValue(&hadc1); 	
			
			switch(t)
			{
				//case 0: adcBuf_ref[i]=temp_val;break;
				case 1: adcBuf_humid[i]=temp_val;break;
				case 2: adcBuf_ta[i]=temp_val;break;
				case 3: adcBuf_tb[i]=temp_val;break;				
			}
		}	
	}
	
	HAL_ADC_Stop(&hadc1);

	/***************************filter adc value*************************************/
	static float tmp_ref;

	if(sensor_usr.CalibrationVref!=0&&sensor_usr.sensor[1]!=0)
	{
		sensor_usr.ADC_REF=sensor_usr.CalibrationVref;		
		//sensor_usr.ADC_REF=sensor_usr.CalibrationVref*sensor_usr.ADC_REF/sensor_usr.sensor[1];
		sensor_usr.CalibrationVref=0;
     params_save();
	}
	else if(sensor_usr.CalibrationVref==0&&sensor_usr.ADC_REF==0)
	{
	//	adc_value = filter(0);
		if(tmp_ref==0)
			tmp_ref = adc_value;
		if(tmp_ref>adc_value)
			tmp_ref = adc_value;
		 adc_value = 4096*3.0/adc_value;
		sensor_usr.ADC_REF = adc_value;	
	}
	sensor_usr.ADC_REF = 3;
	//Claman(adcBuf_ta,0.01,10);
	adc_value = filter(1);
	//sensor_usr.sensor[0] = sensor_usr.ADC_REF*adc_value/4096;
	sensor_usr.sensor[0] = sensor_usr.sensor[0] +sensor_usr.ADC_REF*adc_value/4096;
	sensor_usr.sensor[0] = sensor_usr.sensor[0]/2;
 // sensor_usr.sensor[0]=DigitRound(sensor_usr.sensor[0],2);

	adc_value = filter(2);
	//free(adcBuf_ta);
	//sensor_usr.sensor[1] = sensor_usr.ADC_REF*adc_value/4096;	
		sensor_usr.sensor[1] = sensor_usr.sensor[1] +sensor_usr.ADC_REF*adc_value/4096;
	sensor_usr.sensor[1] = sensor_usr.sensor[1]/2;
 // sensor_usr.sensor[1]=DigitRound(sensor_usr.sensor[1],4);
	
	
	adc_value = filter(3);
	//	free(adcBuf_tb);
	//sensor_usr.sensor[2] = sensor_usr.ADC_REF*adc_value/4096;
	sensor_usr.sensor[2] = sensor_usr.sensor[2] +sensor_usr.ADC_REF*adc_value/4096;
	sensor_usr.sensor[2] = sensor_usr.sensor[2]/2;
  //sensor_usr.sensor[2]=DigitRound(sensor_usr.sensor[2],4);

	/****************************calculabute humid and temperature*********************/
	float vprobeTmp;
	if(sensor_usr.CalibrationProbeVref<=2&&sensor_usr.CalibrationProbeVref>0)
	{
	
		vprobeTmp = VPROBE - sensor_usr.CalibrationProbeVref;
		
	}
	else
		vprobeTmp = 0;	
	sensor_usr.sensor[0] = sensor_usr.sensor[0] + vprobeTmp;
	sensor_usr.rh = SoilHumid(MEASURE,sensor_usr.sensor[0]);
	unsigned char tmp;
	if(sensor_usr.CalibrationT&0x80)
	{
		tmp = ~sensor_usr.CalibrationT+1;
		sensor_usr.temperature = SoilTemperature(MEASURE,sensor_usr.sensor[1],sensor_usr.sensor[2])-tmp;
	}
	else
	{
		tmp = sensor_usr.CalibrationT;	
	  sensor_usr.temperature = SoilTemperature(MEASURE,sensor_usr.sensor[1],sensor_usr.sensor[2])+tmp;
	}	
			
} 
