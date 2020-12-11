#ifndef ADC_H
#define ADC_H
#include "main.h"
#define ADC_COUNT  1
#define VPROBE    0.6
typedef struct{
	float sensor[5];
	float ADC_REF;
	unsigned int rh;
	unsigned int temperature;
	unsigned char CalibrationT;
	float CalibrationVref;
	float CalibrationProbeVref;
	unsigned char CalibrationR;	
}sensor_stru;
void Get_Adc_Average(unsigned char times);
sensor_stru *getSensor(void);

#endif
