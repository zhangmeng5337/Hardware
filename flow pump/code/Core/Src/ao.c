#include "ao.h"
ao_stru ao_usr;

void ao_init()
{
    ao_usr.range = 1;//Ä¬ÈÏ4-20ma
	ao_usr.current = 0;

    AD5410xInit(ao_usr.range);
}
void ao_ModeChange()
{
	if(ao_usr.reconfig == 1)//ÖØÅäÖÃ
	{
		AD5410xInit(ao_usr.range);
	}
}
void ao_proc()
{
    ao_ModeChange();
    if (ao_usr.range == 1)
    {
        if (ao_usr.current >= 4)
        {
            ao_usr.current = (ao_usr.current - 4) * 256;
            ao_usr.current_dac = ao_usr.current;
            ao_usr.current_dac = ao_usr.current_dac << 4;
		        AD5410xWriteDataReg( ao_usr.current_dac);

        }
		else
		{

		}
			
        

    }
    else
    {
        if (ao_usr.current >= 0)
        {
            ao_usr.current = (ao_usr.current) * 204.8;
            ao_usr.current_dac = ao_usr.current;
            ao_usr.current_dac = ao_usr.current_dac << 4;
            AD5410xWriteDataReg( ao_usr.current_dac);  
        }

    }
	

}

