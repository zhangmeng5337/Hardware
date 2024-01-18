#ifndef ADC_H
#define ADC_H
#include "main.h"
#define ADC_COUNT  1
#define ADC_REF   3.295
typedef struct{
	float sensor[5];
	unsigned int rh;
	unsigned int temperature;
}sensor_stru;
void Get_Adc_Average(unsigned char times);
#endif
