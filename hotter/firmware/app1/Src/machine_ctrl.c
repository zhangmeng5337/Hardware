#include "machine_ctrl.h"
#include "do.h"
#include "config.h"

dev_ctrl_stru dev_ctrl_usr;
/*
dq1-15--->do14...0--->
*/
void dev_int(void)
{
    dev_ctrl_usr.fault = 0;
	unsigned char i;
	for(i=0;i<get_config()->dev_size;i++)
	 do_ctrl_proc(i,RELAY_ON);
}
void dev_ctrl()
{
	do_off();

//    if(dev_ctrl_usr.fault == 0)//no machine fault
//    {
//        if(dev_ctrl_usr.mode == AUTO) //manual switch no select
//        {
//            do_ctrl_proc(MACHINE_PWR,RELAY_ON);
//            do_ctrl_proc(SUPPLE_PUMP_PWR,RELAY_ON);
//            do_ctrl_proc(BASIC_PUMP_PWR,RELAY_ON);
//
//        }
//        else
//        {
//
//        }
//    }
//    else
//    {
//
//    }


}

