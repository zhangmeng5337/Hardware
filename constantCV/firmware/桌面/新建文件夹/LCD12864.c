/********************************************************************************************************
*
* File                : LCD12864.c
* Hardware Environment:	DVK501 && F020+ EX
* Build Environment   : Silicon LABs 3.42.00 20100913
* Version             : 
* By                  : Su Wei Feng
*
*                                  (c) Copyright 2005-2010, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

#define _DVK501_F020_EX_

#include <c8051f020.h>
#include <ws_clock.h>
#include <ws_delay.h>
#include <ws_lcd_st7920_port.h>

void main(void)
{
	WDTCN = 0xde;      // Disable watchdog timer
    WDTCN = 0xad;	
	XBR2 = 0x40;
	clock_external();	
	delay_ms(100);
	SPI_Init();
	st7920LcdInit(); 
	showLine(2,1,lcd_buffer,"���ݻ�������"); 			//(X,Y,*lcd_stack,*str,...)��ע�⣬����ʱX����ż�����������롣
	showLine(4,2,lcd_buffer,"HuaLi GZ");		//(X,Y,*lcd_stack,*str,...)��ע�⣬����ʱX����ż�����������롣
	refreshLCD(lcd_buffer); 

	while(1);  	

}
