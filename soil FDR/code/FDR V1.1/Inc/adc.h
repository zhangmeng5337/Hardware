#ifndef ADC_H
#define ADC_H
#include "main.h"
#define ADC_COUNT  1

typedef struct{
	float sensor[5];
	float ADC_REF;
	unsigned int rh;
	unsigned int temperature;
	unsigned char CalibrationT;
	float CalibrationVref;
	unsigned char CalibrationR;	
}sensor_stru;
void Get_Adc_Average(unsigned char times);
#endif
