#include "config.h"
#include "flash.h"


config_stru config_usr;

void config_init()
{
     flash_init();
	config_usr.mode = 0;//0÷∆¿‰£¨1º”»»
}
config_stru *getConfig()
{
  return &config_usr;
}

void config_proc()
{
	if(config_usr.update_params==1)
	{
		flash_save();
		config_usr.update_params = 0;
	}
}
