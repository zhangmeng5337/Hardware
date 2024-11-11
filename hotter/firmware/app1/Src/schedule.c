#include "schedule.h"
#include "stmflash.h"


schedule_stru schedule_u;


void pwr_schedul_init(void)
{
	;//restore paln
}
schedule_stru *get_schedule(void)
{
	return &schedule_u;
}
//void pwr_schedul_set(unsigned char init,schedule_stru dat)
//{
//	unsigned char i,j;
//	if(init == 0)
//	{
//		j = sizeof(schedule_u.buf);
//		for(i=0;i<SCHEDULE_SIZE;i++)
//		{
//		memset(schedule_u.buf,0,j);
//		
//		}
//
//
//	}
//	else
//	{
//		memcpy(schedule_u.buf[dat.buf.index],PLAN_SIZE-1);
//	}
//
//}

unsigned char boot_params_proc(unsigned char oper)
{
    unsigned char buf[512];
    uint32_t  i;

    if (oper == WRITE)
    {	__disable_irq(); 
        memset(buf, 0, 512);
        // ReadFlash(FACTORY_ADDR, buf, 2048);//read original data
        Erase_page(SYS_PARAMS_Addr, 1);
        memset(buf, 0, 512);
        buf[0] = 0x5a;
        i = sizeof(schedule_u.buf);
		memcpy(buf+1,schedule_u.buf,i);
        	
        WriteFlashBytes(SYS_PARAMS_Addr, buf, i);
	__enable_irq();

    }
    else
    {
        memset(buf, 0, 512);
        ReadFlash(SYS_PARAMS_Addr, buf, 512);
        i = sizeof(schedule_u.buf);
        if (buf[0]  == 0x5a)
        {
			memcpy(schedule_u.buf,buf+1,i);
        }
		else
		{
			pwr_schedul_init();
		}
    }
}




