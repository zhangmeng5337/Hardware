#ifndef VERIFIER_H
#define VERIFIER_H

#include "main.h"
#define STEADY_TIME  5000
#define STEADY_TIME2  10000

#define KT    273.15

typedef struct
{
	float pressure_vac_thValue;
	float mfc_flow;
	float pressure_steady_val;
	uint32_t vac_sTime;//all relay open except mfc 
	uint32_t vac_eTime;//vacuum pressure steady
	uint32_t vac_tTime;


	uint32_t flow_sTime;  //  mfc relay open
	uint32_t flow_steady_eTime;// flow steady down relay close 
	uint32_t flow_steady_tTime;


	uint32_t pressure_rise_sTime;//down relay close  
	uint32_t pressure_steady_eTime;//pressure steady up relay close
	uint32_t pressure_steady_tTime;
	unsigned char state;


	float pressure_flow_steady;
	float pressure_flow_start;
	float pressure_steady;
	float pressure_steady_peak;
	float flow_start_temp[3];
	float flow_steady_temp[3];
	float pressure_steady_temp[3];
	float pressure_steady_averT;
	float flow_start_temp_averT;
    float error;
}VERIFIER_STRU;
//typedef struct
//{
//	float p1;
//	float v1;
//	float p20;
//	float p21;
//
//	float v2;
//	float R;
//	float T;
//	float n0;
//	float n1;
//	float detalN;
//	float vm;
//	float F;
//	
//
//}calibration_stru;

VERIFIER_STRU *get_verifier(void);
void verifier_proc(void);
//calibration_stru *get_calib(void);
void verifier_init(void);
void calibration_init(void);
void pressure_time_proc(void);

#endif
