#include "sys.h"
static tick_stru tick_usr;

unsigned char registerTick(uint32_t TickNo,uint32_t ms)
{


    if(tick_usr.TickNum[TickNo] ==0)
    {
        if(tick_usr.TickStart[TickNo]==0)
        {
            tick_usr.TickTime[TickNo] = ms;
            tick_usr.TickTimeTmp[TickNo] = HAL_GetTick();
            tick_usr.TimeEnd[TickNo] = 0;
            tick_usr.TickStart[TickNo] = 1;
			tick_usr.TickNum[TickNo] = 1;

        }
		else
		{

		}
    }
	

  return 0;
 }
void reset_registerTick(uint32_t TickNo)
{
	tick_usr.TimeEnd[TickNo] = 0;
	tick_usr.TickStart[TickNo] = 0;
	tick_usr.TickNum[TickNo] = 0;
}
unsigned char GetTickNum(uint32_t TickNo)
{
   return  tick_usr.TickStart[TickNo];
}
unsigned char GetTickResult(uint32_t TickNo)
{
    return  tick_usr.TimeEnd[TickNo];
}


/***************************************************************
func:system timer calculate
params:
****************************************************************/
void TickPro()
{
    unsigned char i;
    for(i = 0 ; i<TICK_SIZE; i++)
    {
        if(tick_usr.TickNum[i]==1 )
        {
            if((HAL_GetTick()-tick_usr.TickTimeTmp[i])>=tick_usr.TickTime[i])
            {
                tick_usr.TimeEnd[i] = 1;//time reach
				tick_usr.TickStart[i] = 0;//clear timer set flag
            }
        }
    }
}





