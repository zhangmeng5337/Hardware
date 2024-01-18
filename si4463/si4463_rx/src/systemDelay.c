#include "systemDelay.h"
static unsigned char  fac_us=0;							   
static unsigned int fac_ms=0;						
//void system_delay_init(void)
//{
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	
//	fac_us=SystemCoreClock/8000000;			
//	fac_ms=(unsigned int)fac_us*1000;				
//}								    
//					   
//void system_delay_us(uint32_t nus)
//{		
//	uint32_t temp;	    	 
//	SysTick->LOAD=nus*fac_us; 					 
//	SysTick->VAL=0x00;        				
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;
//	do
//	{
//		temp=SysTick->CTRL;
//	}while((temp&0x01)&&!(temp&(1<<16)));		
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	
//	SysTick->VAL =0X00;      					
//}
void system_delay_ms(unsigned int nms)
{
	HAL_Delay(nms);    		
} 
/*
*用于超过1000ms的延时
*/
void system_delay_ms_divide10(unsigned int times)
{
	unsigned int i;
	times = times/10;
	for(i = 0;i<10;i++)
	{
		HAL_Delay(times);
	}
}
/*
*函数功能：只用于中断中的延时
*/
void system_mydelayms(unsigned int a)
{
	unsigned int i,j;
	for(i = 0;i<0xccd;i++)
	{
		for(j=0;j<a;j++);
	}
}
unsigned char two_hex_cmp(const unsigned char *str1,const unsigned char *str2,\
	const unsigned char afirstChar,const unsigned char bfirstChar,const unsigned char length)
{
    unsigned char i = 0;
    unsigned char cmpflag = 0;
    for(i = 0;i<length;i++)
    {
        if(str1[i + afirstChar] != str2[i + bfirstChar])
        {
            cmpflag = 0;
            break;
        }
        else
        {
            cmpflag = 1;
        }
    }
    return cmpflag;
}