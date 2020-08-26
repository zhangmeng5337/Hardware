#ifndef ADC_H
#define ADC_H
#include "main.h"
#define ADC_COUNT  5
typedef struct{
	float sensor[5];
	unsigned int rh;
	unsigned int temperature;
}sensor_stru;
#endif
