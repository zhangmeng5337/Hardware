#ifndef CONFIG_H
#define CONFIG_H

#include "main.h"
typedef struct 
{
	float max_T;
	float min_T;
	float warn_T;
	uint32_t record_interval;
	uint32_t power_save;
	
} config_stru;

config_stru *getConfig(void);

#endif

