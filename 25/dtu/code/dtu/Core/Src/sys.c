#include "sys.h"
#include "stdio.h"
static tick_stru tick_usr;

extern UART_HandleTypeDef huart1;
struct __FILE
{
    int handle;
};
FILE __stdout;

int fputc(int ch, FILE *f)
{
    unsigned char tmp;
    tmp =(unsigned char )ch;

    HAL_UART_Transmit(&huart1, &tmp, 1, 500);

    return ch;
}
int strrindex(char s[], char t[])
{
    int i,j,k,r = -1;
    for(i = 0; s[i] != '\0'; i++)
    {
        k = 0;
        for(j = i; t[k] !='\0';j++,k++)
        {
            if(t[k] == s[j])
            {
                continue;
            }
            else
            {
                break;
            }
        }
        if(t[k] == '\0')
         {
		 			r = i;
				   break;

		 }  
    }
	return r;
}

/***********************************************************************************
 * o‘那y??㏒odelay_us
 * ?豕那?  ㏒ous?車那㊣
 * 那?豕?  ㏒o?T
 * 那?3?  ㏒o?T
************************************************************************************/
void delay_us(uint32_t nCount)
{
	uint32_t i,j;
	for(i=0;i<nCount;i++)
	{
  	for(j=0;j<10;j++)
		{
			__ASM("NOP");
		}
	}
}
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


