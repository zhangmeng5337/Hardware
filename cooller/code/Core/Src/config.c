#include "config.h"
config_stru config_usr;

void config_init()
{
	config_usr.max_T = -10;
	config_usr.min_T = -30;
	config_usr.record_interval = 30;//s
	config_usr.warn_T = -35;
	config_usr.power_save = 15;//s
	config_usr.update_T = 0;
	config_usr.record_time = 0;
	config_usr.mode = 0;
}
config_stru *getConfig()
{
  return &config_usr;
}

