#include "stc8h1k.h"
#include "intrins.h"
#include "tm7780.h"

void TM1_Isr() interrupt 3
{
    TIM1_UPD_Interrupt();
	get_power()->time_1s ++;
    
}
void Timer1_Init(void)		//1ms@12.000MHz
{
	AUXR |= 0x40;			//定时器时钟1T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x20;				//设置定时初始值
	TH1 = 0xD1;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时

    ET1 = 1;                                    //使能定时器中断
    EA = 1;	
}




