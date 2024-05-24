#ifndef CONFIG_H
#define CONFIG_H

#include "main.h"
typedef struct 
{
	float max_T;
	float min_T;
	float warn_T;
	uint32_t addr;
	uint32_t max_T_I;
	uint32_t min_T_I;
	uint32_t warn_T_I;	
  unsigned char mode;//0:cooller  1:heater

	unsigned char update_T;//temperature poll
	
	uint32_t record_interval;
	uint32_t power_save;
	uint32_t record_time;
	unsigned char export_flag;//usb export 
	unsigned char update_params;//params set
	
} config_stru;

config_stru *getConfig(void);
void config_init(void);
#endif

