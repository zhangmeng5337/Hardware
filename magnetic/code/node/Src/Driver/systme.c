#include "main.h"
#include "usystem.h"
uint32_t TimingCount[3];
static unsigned char start_flag[3], tick_buffer[3];

void TimingStop(unsigned char seq)
{
	start_flag[seq] = 4;
}
unsigned char get_tick_end(unsigned char num)
{
	return tick_buffer[num];
}
void set_tick_end(unsigned char num)
{
	 tick_buffer[num]=0;
}
unsigned char TimingStart(unsigned char select,uint32_t tCount1,uint32_t tCount2,uint32_t tCount3)
{
  switch(select)
	{
		case 1: 
		{	
			  if(start_flag[0]==0||start_flag[0]==4)
					{
						TimingCount[0] = HAL_GetTick();
						start_flag[0] = 1;	
					}
					else if(start_flag[0] == 1)
					{
						if((HAL_GetTick() - TimingCount[0]) >=tCount1 )
						{
							start_flag[0] = 0;
							tick_buffer[0]=1;
							return 1;
						}	
					}	
			}break;
		case 2: 
		   {	
			    if(start_flag[1]==0||start_flag[1]==4)
						{
							TimingCount[1] = HAL_GetTick();
							start_flag[1] = 1;	
						}
						else if(start_flag[1] == 1)
						{
							if((HAL_GetTick() - TimingCount[1]) >=tCount2 )
							{
								start_flag[1] = 0;
							tick_buffer[1]=2;
								return 2;
							}	
						}	
					}	break;
		case 3: 
		{
				if(start_flag[2]==0||start_flag[2]==4)
				{
					TimingCount[2] = HAL_GetTick();
					start_flag[2] = 1;	
				}
				else if(start_flag[2] == 1)
				{
					if((HAL_GetTick() - TimingCount[2]) >=tCount3 )
					{
						start_flag[2] = 0;
							tick_buffer[2]=3;
						return 3;
					}	
				}	
		}	break;
	}
	return 0;
}



