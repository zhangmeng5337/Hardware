#include "pump_config.h"
pump_stru pump_usr;
pump_stru *get_pump_params(void)
{
    return &pump_usr;
}
float unitconv(float flow)
{
    float tmp;
    switch (pump_usr.set_unit)
    {
        case MML_H:
            tmp = flow * 1000;
            break;
        case L_H:
            tmp = flow;
            break;
        case GPH_H:
            tmp =flow / GPH2LH;
            break;
        default:
            tmp = flow;
			break;
    }
	pump_usr.now_unit = pump_usr.set_unit;
	return tmp;
}
float unitconvLH(float flow)
{
    float tmp;
    switch (pump_usr.now_unit)
    {
        case MML_H:
            tmp = flow/1000;
            break;
        case L_H:
            tmp =flow;
            break;
        case GPH_H:
            tmp = flow*GPH2LH;
            break;
        default:
            tmp = flow;
            break;
    }
	return tmp;
}

void pump_set_mode(unsigned char mode)
{
	if(mode <= MAX_MODE)
		pump_usr.mode = mode;
	switch(pump_usr.mode)
	 {
	  case MANNUAL_MODE:
	  case PULSE_MODE: 
		  pump_usr.now_unit = MML_H;
		 
		  break; 
	}

}
/************************************
dat:encoder pulse count;
*************************************/
void pump_set_flow(float dat)
{
       switch(pump_usr.mode)
       	{
		 case MANNUAL_MODE:
			 pump_usr.set_flow = unitconv(dat *STEP_MANUAL_L);break;
	     case PULSE_MODE: 
             pump_usr.now_unit = MML_H;
			 pump_usr.set_flow = unitconv(unitconvLH(dat*STEP_PULSE_ML));
		     break; 
	   case ANALOG1_MODE: 
		   break; 
	   case ANALOG2_MODE: 
		   break; 

	   }
        
}




