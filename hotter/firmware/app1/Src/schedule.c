#include "schedule.h"
#include "stmflash.h"


schedule_stru schedule_u;

void pwr_schedul_reset(void)
{
	;//restore paln
	
}
unsigned char  boot_params_proc(unsigned char oper)
{
    unsigned char buf[512],result;
    uint32_t  i,j;

    if (oper == WRITE)
    {	__disable_irq(); 
        memset(buf, 0, 512);
        // ReadFlash(FACTORY_ADDR, buf, 2048);//read original data
        Erase_page(SYS_PARAMS_Addr, 1);
        memset(buf, 0, 512);
        buf[0] = 0x5a;
        i = sizeof(schedule_u.buf);
		for(i=0;i<SCHEDULE_SIZE;i++)
			memcpy(buf+1,schedule_u.buf[i],PLAN_SIZE);
        	
        WriteFlashBytes(SYS_PARAMS_Addr, buf, i*PLAN_SIZE);
	__enable_irq();
	result = 0;

    }
    else
    {
        memset(buf, 0, 512);
        ReadFlash(SYS_PARAMS_Addr, buf, 512);
        i = sizeof(schedule_u.buf);
        if (buf[0]  == 0x5a)
        {  
         	j = 0;
           	for(i=0;i<SCHEDULE_SIZE;i++)
           	{
           	
			get_schedule()->buf[i].pwr_state =buf[j ++];
			get_schedule()->buf[i].temperature =buf[j ++];
			get_schedule()->buf[i].shour =buf[j ++];
			get_schedule()->buf[i].sminute =buf[j ++];
			get_schedule()->buf[i].sweekday =buf[j ++];
			get_schedule()->buf[i].ehour =buf[j ++];
			get_schedule()->buf[i].eminute =buf[j ++];
			get_schedule()->buf[i].eweekday =buf[j ++];
			get_schedule()->buf[i].enable =buf[j ++];
			get_schedule()->buf[i].index =buf[j ++];

			}
			result = 0;
   

        }
		else
		{
		    result = 1;
			pwr_schedul_reset();
		}
    }
	return result;
}
void pwr_schedul_set(void)
{
    static uint32_t stick;
	if(schedule_u.save_flag == 1)
	{
		if((HAL_GetTick()-stick)>=3000)
		{
		    boot_params_proc(WRITE);
			schedule_u.save_flag = 0;
			
		}
	}
	else
	{
		stick = HAL_GetTick();
	}
}


void pwr_schedul_init(void)
{
	if( boot_params_proc(READ) == 1)
	{
		pwr_schedul_reset();
	}
	
}
schedule_stru *get_schedule(void)
{
	return &schedule_u;
}








