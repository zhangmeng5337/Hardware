#ifndef LIS2MDL_H
#define LIS2MDL_H
#include "main.h"
typedef struct{
	 int16_t data_raw_magnetic[3];
	 int16_t data_raw_temperature;
	 float magnetic_mG[3];
	 float temperature_degC;
}sensors_stru;
unsigned char lis2mdl_init(void);
void lis2mdl_read_data_simple(void);
sensors_stru *GetMagnetic(void);
void lis2mdlPowerDown(void);


#endif

