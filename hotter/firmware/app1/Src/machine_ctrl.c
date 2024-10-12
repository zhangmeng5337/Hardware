#include "machine_ctrl.h"
#include "do.h"
dev_ctrl_stru dev_ctrl_usr;

void dev_int(void)
{
    dev_ctrl_usr.fault = 0;
}
void dev_ctrl()
{

    if(dev_ctrl_usr.fault == 0)//no machine fault
    {
        if(dev_ctrl_usr.mode == AUTO) //manual switch no select
        {
            do_ctrl_proc(MACHINE_PWR,RELAY_ON);
            do_ctrl_proc(SUPPLE_PUMP_PWR,RELAY_ON);
            do_ctrl_proc(BASIC_PUMP_PWR,RELAY_ON);

        }
        else
        {

        }
    }
    else
    {

    }


}

