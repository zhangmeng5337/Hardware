#include "ulitity.h"

extern volatile  uint32 timer_tick_count;


/**************************************************************************************************************
* ��    �ƣ� delay_ms
* ��    �ܣ� ��ʱn����
* ��ڲ����� n-��ʱʱ��
* ���ڲ����� ��
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
*��    �ƣ� systicket_init
*��    �� �����Ķ�ʱ�� 10ms
*��ڲ����� ��
*���ڲ����� ��
*********************************/
void systicket_init(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    SysTick_Config(48000000/100);	 //һ���ӽ���100���ж�
}
