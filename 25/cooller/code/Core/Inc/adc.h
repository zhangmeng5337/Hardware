#ifndef ADC_H
#define ADC_H
#include "main.h"
#include "pt100.h"
#include "filter.h"
#define PT_VREF 2.499
#define R2_V 2.0
#define V2_V  0.092

#define R1_V 10.0
#define RF_V 100


#define V1_RATIO 10.0


#define ROM_NUM		1220


#define MAX_TEMP_VOL  100
#define MIN_TEMP_VOL -60


#define BATTERY_V    16.75

typedef struct
{
	uint32_t adc_value[5];
	float    T_value[4];
	float    v_value[4];
	float    R_value[4];
	float    M_value[N];	
    float    V2_V_offset; 
	float    heater_T;
	float	 cooller_T;  
	float ref;
	float average_T;//制冷检测温度，通过rf得到
	unsigned char update;
	unsigned char valid;

}adc_stru;


void adc_init(void);
adc_stru *get_temperature(void) ;
void adc_proc(void);
void set_adc_update(void);

#endif

