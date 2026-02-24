/**--------------File Info------------------------------------------------------------------------------
** File name:			  delay.c
** Last modified Date:  2017-08-8
** Last Version:		V1.02
** Descriptions:		Delay Config
**------------------------------------------------------------------------------------------------------
** Created date:		2017-08-8
** Version:			  	V1.02
** Descriptions:		Delay Config
**
********************************************************************************************************/
#define _DELAY_MANL
#include "stm32f0xx.h"
#include "delay.h"

/**********************************************************************************************************
delay_init:初始化延时函数
功能：配置延时函数
参数：
	SystemCoreClock = SysTick_CLKSource_HCLK = 系统时钟 = 48MHz
**********************************************************************************************************/
void delay_init(void)
{                                                                         //                                                    
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);                      //  滴嗒时钟匹配
	  fac_us = (SystemCoreClock / 100000);   // 6800000                   //  1us = (SystemCoreClock / 8000000) = 6 次滴答时钟
	  fac_ms = (unsigned int)fac_us * 1000;                                 //  1ms = 6 * 1000  次滴答时钟
}

/**********************************************************************************************************
delay_us:延时nus时钟函数
功能：利用滴答时钟进行延时
参数：nus：n个微秒
**********************************************************************************************************/
void delay_us(unsigned long nus)
{
    unsigned long    temp;
  	SysTick->LOAD  = nus * fac_us;                                        //时间加载
	  SysTick->VAL   = 0x00;                                                //清空计数器
	  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;                             //开始倒计时
  	do {
	      temp = SysTick->CTRL;
	  } while ((temp & 0x01) && !(temp & (1 << 16)));                       //等待时间到达
	  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                            //关闭计数器
	  SysTick->VAL   =  0x00;                                               //清空计数器      
}

/**********************************************************************************************************
delay_ms：延时nms时钟函数
功能：利用滴答时钟进行延时
参数：nms：n个毫秒,MAX=300ms
**********************************************************************************************************/
void delay_ms(unsigned long nms)
{
    unsigned long    tick;
	  SysTick->LOAD  = nms * fac_ms;
  	SysTick->VAL   = 0x00;
  	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	  do {
  		  tick = SysTick->CTRL;
  	} while ((tick & 0x01) && !(tick & (1 << 16)));
  	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  	SysTick->VAL   =  0x00;
}
void delay_10us(uint32_t cnt)
{
	uint32_t i,j;
	for(i=0;i<cnt;i++)
		for(j=0;j<80;j++);
}
