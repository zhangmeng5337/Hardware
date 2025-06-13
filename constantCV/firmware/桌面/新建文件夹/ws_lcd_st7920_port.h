/********************************************************************************************************
*
* File                : ws_lcd_st7920_port.h
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

#ifndef _WS_LCD_ST7920_PORT_H
#define _WS_LCD_ST7920_PORT_H

void st7920LcdInit(void);
void spiSendChar(uchar send_char);
void sendCodeST7920(uchar Lcd_code);
void sendDataST7920(uchar Lcd_data);
void refreshLCD(const uchar *lcd_stack);
void decToAscii(uchar* str,uint dec,uchar width);
void hexToAscii(uchar* str,uint hex,uchar width);
void showLine(const uchar x,const uchar y,uchar* lcd_stack,char * str,...);

uchar idata lcd_buffer[64];

/*Hardware Environment£ºDVK501 && F320 EX*/
#if defined(_DVK501_F320_EX_)						    
sbit CS_LCD = P0^7;
/*sbit RW_SID = P0^6;
sbit E_CLK = P0^0;*/

void SPI_Init(void)
{	
	XBR0 |= 0x03;
	P0SKIP |= 0x80;
	P0MDOUT |= 0xC1;
	SPI0CFG = 0x40;
	SPI0CN = 0x01;
	SPI0CKR = 0x2F;
}

/*Hardware Environment£ºDVK501 && F340+ EX*/
#elif defined(_DVK501_F340_EX_)
sbit CS_LCD = P1^5;

void SPI_Init(void)
{	
	XBR0 |= 0x02;
	P0SKIP |= 0x08;
	P0MDOUT |= 0x0D;
	SPI0CFG = 0x40;
	SPI0CN = 0x01;
	SPI0CKR = 0x2F;
}

/*Hardware Environment£ºDVK501 && F020+ EX*/
#elif defined(_DVK501_F020_EX_)
sbit CS_LCD = P1^2;

void SPI_Init(void)
{	
	XBR0 |= 0x06;
	P0MDOUT |= 0x14;
	SPI0CN |= 0x02;
	SPI0CN |= 0x01;
	SPI0CKR = 0x7B;
}

#else
  #warning "No SPI!"
#endif

#include <ws_lcd_st7920.h>

#endif /*_WS_LCD_ST7920_PORT_H*/
