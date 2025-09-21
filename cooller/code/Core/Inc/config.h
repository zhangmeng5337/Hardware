#ifndef CONFIG_H
#define CONFIG_H

#include "main.h"
#define MODE_COOLLER    0x00
#define MODE_HEATER		0x01
#define RUN			0x00
#define SLEEP       0x01
#define WARN        0x02

#define SLEEP_TIME  20*60*1000
typedef struct 
{
	float max_T;
	float min_T;
	float warn_T;
	uint32_t addr;
	uint32_t max_T_I;
	uint32_t min_T_I;
	uint32_t warn_T_I;	
	unsigned char sleep_flag;
    unsigned char mode;//0:cooller  1:heater

	unsigned char update_T;//temperature poll
	unsigned char update_fig;
	unsigned char status;
	float record_interval;
	float power_save;
	float record_time;
	float interval_tick;
	unsigned char export_flag;//usb export 
	unsigned char update_params;//params set
	
} config_stru;

config_stru *getConfig(void);
void config_init(void);
#endif

