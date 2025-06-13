/********************************************************************************************************
*
* File                : ws_clock.h
* Hardware Environment:
* Build Environment   : Silicon LABs 3.42.00 / uVision3 V3.80 20100913
* Version             : 
* By                  : Su Wei Feng
*
*                                  (c) Copyright 2005-2010, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

#ifndef _WS_CLOCK_H_
#define _WS_CLOCK_H_

#define uchar unsigned char
#define uint unsigned int

/*Hardware Environment：DVK501 && F320 EX*/
#if defined(_DVK501_F320_EX_)
void clock_external(void)       //external oscillator
{   
    uchar i;
	P0SKIP |= 0x0C;  
	P0MDIN &= ~0x0C;
    OSCXCN |= 0x67; 				
    for(i=0;i<255;i++); 			// Wait 1ms for initialization 
    while((OSCXCN & 0x80) == 0);
    CLKSEL |= 0x01;
    OSCICN = 0x00;
}

void clock_internal(void)		//internal oscillator
{
    OSCICN |= 0x03;
}

/*Hardware Environment：DVK501 && F340+ EX*/
#elif defined(_DVK501_F340_EX_)
void clock_external(void)       //external oscillator
{   
    uchar i;
	P0SKIP |= 0xC0;  
	P0MDIN &= ~0xC0;
    OSCXCN |= 0x67; 
    for(i=0;i<255;i++); 		// Wait 1ms for initialization 
    while((OSCXCN & 0x80) == 0); 
    CLKSEL |= 0x01;
    OSCICN = 0x00;
}

void clock_internal(void)		//internal oscillator
{
    OSCICN |= 0x03;      
}

/*Hardware Environment：DVK501 && F020+ EX*/
#elif defined(_DVK501_F020_EX_)
void clock_external(void)       //external oscillator
{   
    uchar i;                                
   	OSCXCN = 0x67;              // 使能外部振荡器，起动外部晶体振荡器f > 6.7MHz，0x67=0110,0111         
	for (i=255;i>0;i--);        // 等待晶体振荡器启动（>1毫秒）
   	while((OSCXCN & 0x80) == 0);   // Wait 1ms for initialization（等待晶体振荡器稳定）查询XTLVLD(晶体振荡器有效标志) => ’1’ , 0x80=1000,0000       
   	OSCICN = 0x08;                 //将系统时钟切换到外部振荡器,  并禁止时钟丢失检测器     
}

/******暂时敝内部时钟函数,以免报警
void clock_internal(void)		//internal oscillator
{
    OSCICN |= 0x03;      
}
******/
#else
  #warning "No clock!"
#endif

#endif