#ifndef CONFIG_H
#define CONFIG_H
#include "main.h"


#define PT		0	
#define THER		1	

#define SENSOR_TYPE	PT



#define HIGH_FLOW 0
#define LOW_FLOW  1
#define CMD_START  LINE_PURGED_START
#define CMD_STOP   NORMAL_OPERATION

#define VACUUM_STEADY           0
#define FLOW_STEADY             1
#define PRESSURE_STEADY         2


#define LINE_PURGED_START     	1
#define FLOW_SET_POINT_START  	2
#define FLOW_SET_POINT_STEADY  	3
#define PRESSURE_RISE_START  	4
#define PRESSURE_STEADY_START  	5
#define NORMAL_OPERATION  	    6
#define FLOW_SET_POINT_SHOCK  	7
#define PRESSURE_STEADY_SHOCK  	8
#define DEBUG_STEADY		  	9
#define PRESSURE_STEADY_END  	10
#define IDLE_STATE          	11
#define PURGE_START				12
#define PURGE_END				13
#define DRAIN_START				14
#define DRAIN_END				15



#define RE_CMD  		0
#define RE_FLOAT		1
#define RE_CHAR 		2
#define RE_STRING 		3



typedef struct
{
	unsigned char flow_mode;//high flow or low flow
	unsigned char cmd;// start or stop
	float pressure_error;
	float p20_pressure_error;
	float vac_val; //cacuum value
	float flow_setpoint; //steady flow
	float pressure_val; //steady pressure
	unsigned char update;

	float res_float;
	unsigned char res_char;
	unsigned char res_cmd;
	unsigned char debug_cmd;
	unsigned char *string;
	unsigned char debug;
	unsigned char debug_log;

}CONFIG_STRU;


CONFIG_STRU *get_config(void);
void config_proc(void);
void config_init(void);


#endif
