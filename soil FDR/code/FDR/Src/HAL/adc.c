#include "adc.h"
extern ADC_HandleTypeDef hadc1;
float tmp;
float temperature;
sensor_stru sensor_usr;
void Get_Adc_Average(unsigned char times)
{

	uint32_t temp_val=0,adcBuf[ADC_COUNT];
	unsigned char i,t;

	for(t=0;t<ADC_COUNT;t++)
	{;			
		adcBuf[t]=0;
	}

	for(i=0;i<times;i++)
	{                               //¿ªÆôADC
		HAL_ADCEx_Calibration_Start(&hadc1);
		for(t=0;t<ADC_COUNT;t++)
		{
			
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1,0xffff);
			temp_val = HAL_ADC_GetValue(&hadc1); 			
			adcBuf[t]=temp_val + adcBuf[t];
			if(temp_val>500000)
				adcBuf[t] = 0;
		}
	}
	HAL_ADC_Stop(&hadc1);
	for(i=0;i<ADC_COUNT;i++)
	{
	  sensor_usr.sensor[i] = adcBuf[i]/times;
	}
	for(i=0;i<ADC_COUNT;i++)
	{
	  sensor_usr.sensor[i] = 3.3*sensor_usr.sensor[i]/4095;
	}



	#if DEBUG_USER 
		printf("  osc vol:   %f        fa vol:   %f           fb vol:   %f        ta vol:          tb vol:   %f\n",sensor_usr.sensor[0],sensor_usr.sensor[1]
	,sensor_usr.sensor[2],sensor_usr.sensor[3],sensor_usr.sensor[4]);
		//printf("  fa vol:   %f",sensor_usr.sensor[1]);
		//printf("  fb vol:   %f",sensor_usr.sensor[2]);
		//printf("  ta vol:   %f",sensor_usr.sensor[3]);
		//printf("  tb vol:   %f\n",sensor_usr.sensor[4]);

	//temperature = sensor_usr.sensor[3]-sensor_usr.sensor[4];
	float tmp2,tmp3;
	tmp2=(sensor_usr.sensor[4]/3.3);
	tmp3= (sensor_usr.sensor[3]-sensor_usr.sensor[4]);
	tmp3= 2.8048*tmp3;	
	temperature =tmp3 /tmp2*1000-287.22;	
	
		#endif
	
	//return &adc_io.adc_result[0];
} 