#ifndef LSM6D_H
#define LSM6D_H
#include "main.h"
typedef struct {
	int16_t data_raw_acceleration[3];
	int16_t data_raw_angular_rate[3];
	int16_t data_raw_temperature;
	float acceleration_mg[3];
	float angular_rate_mdps[3];
	float temperature_degC;

}AccAng_stru;


void lsm6ds3tr_c_read_data_polling(void);
unsigned char lsm6ds3tr_init(void);
AccAng_stru *GetAccAngtic(void);
void lsm6dPowerDown(void);


#endif

