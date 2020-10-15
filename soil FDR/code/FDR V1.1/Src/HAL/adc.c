#include "adc.h"
#include "app.h"
#include "filter.h"
#include "FdrAlgorithm.h"
extern ADC_HandleTypeDef hadc1;


sensor_stru sensor_usr;
extern uint32_t	freq_count;
float adc_value;
unsigned int adcBuf_ref[N];	//参考电压缓冲区
unsigned int adcBuf_humid[N];	//湿度电压缓冲区
unsigned int adcBuf_ta[N];//温度a电压缓冲区
unsigned int adcBuf_tb[N];//温度b电压缓冲区
void Get_Adc_Average(unsigned char times)
{

	uint32_t temp_val=0;
	unsigned char i,t;
 


	for(i=0;i<N;i++)
	{                               //开启ADC
		HAL_ADCEx_Calibration_Start(&hadc1);
		for(t=0;t<4;t++)
		{
			
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1,0xffff);
			temp_val = HAL_ADC_GetValue(&hadc1); 	
			
			switch(t)
			{
				case 0: adcBuf_ref[i]=temp_val;break;
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
		
		sensor_usr.ADC_REF=sensor_usr.CalibrationVref*sensor_usr.ADC_REF/sensor_usr.sensor[1];
		sensor_usr.CalibrationVref=0;
					unsigned char addr;
			addr = 0;	
     params_save();
	}
	else if(sensor_usr.CalibrationVref==0&&sensor_usr.ADC_REF==0)
	{
		adc_value = filter(0);
		if(tmp_ref==0)
			tmp_ref = adc_value;
		if(tmp_ref>adc_value)
			tmp_ref = adc_value;
		 adc_value = 4096*1.2/adc_value;
		adc_value = DigitRound(adc_value,4);	
		sensor_usr.ADC_REF = adc_value;	
	}
	
	adc_value = filter(1);
	sensor_usr.sensor[0] = sensor_usr.ADC_REF*adc_value/4096;
  sensor_usr.sensor[0]=DigitRound(sensor_usr.sensor[0],2);

	adc_value = filter(2);
	//free(adcBuf_ta);
	sensor_usr.sensor[1] = sensor_usr.ADC_REF*adc_value/4096;	
  sensor_usr.sensor[1]=DigitRound(sensor_usr.sensor[1],4);
	
	
	adc_value = filter(3);
	//	free(adcBuf_tb);
	sensor_usr.sensor[2] = sensor_usr.ADC_REF*adc_value/4096;
  sensor_usr.sensor[2]=DigitRound(sensor_usr.sensor[2],4);

	/****************************calculabute humid and temperature*********************/
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
			
	

//	for(i=0;i<ADC_COUNT;i++)
//	{
//	  sensor_usr.sensor[i] = 3.3*sensor_usr.sensor[i]/4095;
//	}
//  if(sensor_usr.sensor[0]>value)
//	{
//		value=sensor_usr.sensor[0];
//		freq_last = freq_count;
//		stop_flag = 0;
//	}
//	else
//		stop_flag = 1;


//	#if DEBUG_USER 
	//	printf("  osc vol:   %f        fa vol:   %f           fb vol:   %f        ta vol:          tb vol:   %f\n",sensor_usr.sensor[0],sensor_usr.sensor[1]
	//,sensor_usr.sensor[2],sensor_usr.sensor[3],sensor_usr.sensor[4]);
	//	printf("  fa vol:   %f\n",sensor_usr.sensor[0]);
		//printf("  fb vol:   %f",sensor_usr.sensor[2]);
		//printf("  ta vol:   %f",sensor_usr.sensor[3]);
		//printf("  tb vol:   %f\n",sensor_usr.sensor[4]);

	//temperature = sensor_usr.sensor[3]-sensor_usr.sensor[4];
/*	float tmp2,tmp3;
	tmp2=(sensor_usr.sensor[4]/3.3);
	tmp3= (sensor_usr.sensor[3]-sensor_usr.sensor[4]);
	tmp3= 2.8048*tmp3;	
	temperature =tmp3 /tmp2*1000-287.22;	
	
		#endif*/
	
	//return &adc_io.adc_result[0];
} 
