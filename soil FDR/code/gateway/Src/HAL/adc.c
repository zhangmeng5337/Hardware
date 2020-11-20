#include "adc.h"
extern ADC_HandleTypeDef hadc1;
float adc_value;

unsigned char *Get_Adc_Average(unsigned char times)
{

	uint32_t temp_val=0;
	unsigned char t;
//	unsigned int adcVbat;	//µçÑ¹»º³åÇø
	

	for(t=0;t<times;t++)
	{
		
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1,0xffff);
		temp_val = temp_val+HAL_ADC_GetValue(&hadc1); 	
	}
	HAL_ADC_Stop(&hadc1);
	temp_val = temp_val/times;
	adc_value = 3.3*temp_val/4096;
	adc_value = adc_value*38.1/5.1;
	adc_value = adc_value/VBAT_CAPACITY;
	adc_value = adc_value*100;	
	t = (unsigned char)(adc_value);
	return (&t);
	
	
} 
unsigned char *getadc()
{
	return Get_Adc_Average(20);

}

