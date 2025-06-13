/********************************************************************************************************
*
* File                : ws_delay.h
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

#ifndef _WS_DELAY_H_
#define _WS_DELAY_H_

#include <intrins.h>

/*Hardware Environment£ºDVK501 && F320 EX*/
#if defined(_DVK501_F320_EX_)
#define SYSCLK 12000000   //SYSCLK in 12M

void delay_ms(uint ms)
{
	uint i; 
	TCON &= ~0x30; 
	TMOD &= ~0x0f;
	TMOD |= 0x01;
	CKCON |= 0x04;

	for(i=0;i<ms;i++) 
	{ 
		TR0 = 0;
		TH0 = (-SYSCLK/1000) >> 8; 
		TL0 = -SYSCLK/1000;
		TR0 = 1; 
		while (TF0 == 0);
		TF0 = 0; 
	}
} 

void delay_us(uint us)
{
	while(us--);
}

/*Hardware Environment£ºDVK501 && F340+ EX*/
#elif defined(_DVK501_F340_EX_)
#define SYSCLK 12000000   //SYSCLK in 12M

void delay_ms(uint ms)
{
	uint i; 
	TCON &= ~0x30; 
	TMOD &= ~0x0f;
	TMOD |= 0x01;
	CKCON |= 0x04;

	for(i=0;i<ms;i++) 
	{ 
		TR0 = 0;
		TH0 = (-SYSCLK/1000) >> 8; 
		TL0 = -SYSCLK/1000;
		TR0 = 1; 
		while (TF0 == 0);
		TF0 = 0; 
	}
} 

void delay_us(uint us)
{
	while(us--);
}

/*Hardware Environment£ºDVK501 && F020+ EX*/
#elif defined(_DVK501_F020_EX_)
#define SYSCLK 16000000   //SYSCLK in 16M

void delay_ms(uint ms)
{
	uint i; 
	TCON &= ~0x30; 
	TMOD &= ~0x0f;
	TMOD |= 0x01;
	CKCON |= 0x08;

	for(i=0;i<ms;i++) 
	{ 
		TR0 = 0;
		TH0 = (-SYSCLK/1000) >> 8; 
		TL0 = -SYSCLK/1000;
		TR0 = 1; 
		while (TF0 == 0);
		TF0 = 0; 
	}
} 

void delay_us(uint us)
{
	while(us--);
}

#endif

#endif