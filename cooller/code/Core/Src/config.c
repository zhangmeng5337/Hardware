#include "config.h"
#include "flash.h"


config_stru config_usr;

void config_init()
{
     flash_init();
//	 	 getConfig()->max_T = 10;
//		 getConfig()->min_T = 5;
//		 getConfig()->record_interval = 60;//s
//		 getConfig()->warn_T = -35;
		 getConfig()->power_save = 15;//s
		 getConfig()->update_T = 0;
		 getConfig()->record_time = 0;
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
