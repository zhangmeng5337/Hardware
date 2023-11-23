#include "equipment_ctrl.h"
#include "config.h"
#include "di.h"
#include "ai_proc.h"


unsigned char equipment_control()
{
   unsigned char result;
   result = 0;
	if(get_congfig()->machine)
	{
		result = 1;
	}
   //get_di_data()->di_status
   //get_ai_data()->channel_status


  return result;
}

