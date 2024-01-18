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
/***********************************************************************************
 * 函数名：delay_us
 * 描述  ：us延时
 * 输入  ：无
 * 输出  ：无
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
unsigned char registerTick(unsigned char TickNo,uint32_t ms)
{


    if(tick_usr.TickTime[TickNo] ==0)
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
	


}
void reset_registerTick(unsigned char TickNo)
{
	tick_usr.TimeEnd[TickNo] = 0;
	tick_usr.TickStart[TickNo] = 0;
	tick_usr.TickNum[TickNo] = 0;
}
unsigned char GetTickNum(unsigned char TickNo)
{
   return  tick_usr.TickStart[TickNo];
}
unsigned char GetTickResult(unsigned char TickNo)
{
    return  tick_usr.TimeEnd[TickNo];
}

void TickPro()
{
    unsigned char i;
    for(i = 0 ; i<TICK_SIZE; i++)
    {
        if(tick_usr.TickNum[i]==1 )
        {
            if((HAL_GetTick()-tick_usr.TickTimeTmp[i])>=tick_usr.TickTime[i])
            {
                tick_usr.TimeEnd[i] = 1;
				tick_usr.TickStart[i] = 0;
            }
        }
    }
}

