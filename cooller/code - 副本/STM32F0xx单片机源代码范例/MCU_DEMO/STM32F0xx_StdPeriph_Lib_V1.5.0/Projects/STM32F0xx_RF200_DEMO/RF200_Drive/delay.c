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
delay_init:��ʼ����ʱ����
���ܣ�������ʱ����
������
	SystemCoreClock = SysTick_CLKSource_HCLK = ϵͳʱ�� = 48MHz
**********************************************************************************************************/
void delay_init(void)
{                                                                         //                                                    
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);                      //  ���ʱ��ƥ��
	  fac_us = (SystemCoreClock / 100000);   // 6800000                   //  1us = (SystemCoreClock / 8000000) = 6 �εδ�ʱ��
	  fac_ms = (unsigned int)fac_us * 1000;                                 //  1ms = 6 * 1000  �εδ�ʱ��
}

/**********************************************************************************************************
delay_us:��ʱnusʱ�Ӻ���
���ܣ����õδ�ʱ�ӽ�����ʱ
������nus��n��΢��
**********************************************************************************************************/
void delay_us(unsigned long nus)
{
    unsigned long    temp;
  	SysTick->LOAD  = nus * fac_us;                                        //ʱ�����
	  SysTick->VAL   = 0x00;                                                //��ռ�����
	  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;                             //��ʼ����ʱ
  	do {
	      temp = SysTick->CTRL;
	  } while ((temp & 0x01) && !(temp & (1 << 16)));                       //�ȴ�ʱ�䵽��
	  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                            //�رռ�����
	  SysTick->VAL   =  0x00;                                               //��ռ�����      
}

/**********************************************************************************************************
delay_ms����ʱnmsʱ�Ӻ���
���ܣ����õδ�ʱ�ӽ�����ʱ
������nms��n������,MAX=300ms
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
