#include "ulitity.h"

extern volatile  uint32 timer_tick_count;


/**************************************************************************************************************
* 名    称： delay_ms
* 功    能： 延时n毫秒
* 入口参数： n-延时时间
* 出口参数： 无
***************************************************************************************************************/
void delay_ms(uint32 delay)
{
    uint32 tick = timer_tick_count;
    while(1)
    {
        if(timer_tick_count-tick>delay/10)
            break;
    }
}
/********************************
*名    称： systicket_init
*功    能 ：节拍定时器 10ms
*入口参数： 无
*出口参数： 无
*********************************/
void systicket_init(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    SysTick_Config(48000000/100);	 //一秒钟进入100次中断
}
