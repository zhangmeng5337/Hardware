#ifndef PUMP_CONFIG_H
#define PUMP_CONFIG_H
#include "main.h"
#define GPH2LH 3.78541
#define MML_H 		0
#define L_H 		1
#define GPH_H 		2

#define MAX_MODE 7
#define MAX_PULSE_ML 386
#define STEP_PULSE_ML 386/10000.0
#define MAX_MANUAL_L 200
#define STEP_MANUAL_L 200/800.0


#define MANNUAL_MODE 	0
#define PULSE_MODE 		1
#define ANALOG_MODE 	2
#define BATCH_MODE     	3
#define CYC_MODE 		4
#define WEEKLY_MODE 	5
#define CAL_MODE 		6
#define SLEEP_MODE 		7
#define ANALOG1_MODE 		8
#define ANALOG2_MODE 		9

typedef struct
{
	unsigned char mode;//0：manual 1:pulse 2:analog 3:batch 
	                   //4:cycle 5:weekly 6:calibration 7:sleep 8:4-20MA
	                   //9:0-20mA
	float set_flow;
	float sum_flow;//批量容积
	float flow_real;//L/H
	float flow_real_conv;

	unsigned char set_unit;//0:ml 1:L 2:gph
	unsigned char now_unit;//0:ml 1:L 

	uint32_t add_time[16][14];//序号，容积，unit,mm,ss,hh,mm,1-7
	uint32_t warn[10][6];
	uint32_t time[6];
}pump_stru;
pump_stru *get_pump_params(void);
unsigned char lcd_strings_convert(void);

#endif

