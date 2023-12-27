#include "equipment_ctrl.h"
#include "config.h"
#include "di.h"
#include "ai_proc.h"

machine_stru machine_usr;

unsigned char equipment_control()
{
   unsigned char result;
   result = 0;
	if(get_config()->machine)
	{
		result = 1;
	}
   //get_di_data()->di_status
   //get_ai_data()->channel_status


  return result;
}

machine_stru *get_machine()
{
	return &machine_usr;
}


