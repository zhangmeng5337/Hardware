/********************************************************************************************************
*
* File                : ws_lcd_st7920.h
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

#ifndef _WS_LCD_ST7920_H
#define _WS_LCD_ST7920_H

#include <string.h>
#include <stdarg.h>

#define LCD_DELAY() delay_us(75)

void st7920LcdInit(void)
{
	sendCodeST7920(0x30);
	sendCodeST7920(0x0C);
	sendCodeST7920(0x01);
	sendCodeST7920(0x02);
	sendCodeST7920(0x80);
	delay_ms(2);
	memset(lcd_buffer,' ',sizeof(lcd_buffer));
}

void spiSendChar(uchar send_char)
{
	SPIF = 0;
	SPI0DAT = send_char;
	while (!SPIF);
}

/********************************

    Software simulation SPI

**********************************/
/*void spiSendChar(uchar send_char)  
{
	uchar i;
	for(i=8;i>0;i--)
    {		
		if(send_char & (1 << (i-1)))
			RW_SID = 1;
		else
			RW_SID = 0;
		delay_us(10);
		E_CLK = 1;
		delay_us(10);
		E_CLK = 0;
		delay_us(10);
    }
}*/

void sendCodeST7920(uchar Lcd_code)
{
	CS_LCD = 1;
	delay_us(10);

	spiSendChar(0xF8);
	spiSendChar(Lcd_code & 0xF0);
	spiSendChar((Lcd_code << 4));
	LCD_DELAY();

	delay_us(10);
	CS_LCD = 0;
}

void sendDataST7920(uchar Lcd_data)
{
	CS_LCD = 1;
	delay_us(10);
		
	spiSendChar(0xFA);
	spiSendChar(Lcd_data & 0xF0);
	spiSendChar((Lcd_data << 4));
	LCD_DELAY();

	delay_us(10);
	CS_LCD = 0;	
}

void refreshLCD(const uchar *lcd_stack)
{
	uchar addr;
	sendCodeST7920(0x02);
	for(addr=0;addr<16;addr++)
	{
		sendDataST7920(*(lcd_stack + addr));
	}
	for(addr=32;addr<48;addr++)
	{
		sendDataST7920(*(lcd_stack + addr));
	}
	for(addr=16;addr<32;addr++)
	{
		sendDataST7920(*(lcd_stack + addr));
	}
	
	for(addr=48;addr<64;addr++)
	{
		sendDataST7920(*(lcd_stack + addr));
	}
}

void decToAscii(uchar* str,uint dec,uchar width)
{
	while(width)
	{
		switch(width)
		{
			case 5:
			{
				*str = (dec / 10000 + '0');
				dec %= 10000;
				break;
			}			
			case 4:
			{
				*str = (dec / 1000 + '0');
				dec %= 1000;
				break;
			}
			case 3:
			{
				*str = (dec / 100 + '0');
				dec %= 100;
				break;
			}
			case 2:
			{
				*str = (dec / 10 + '0');
				dec %= 10;
				break;
			}
			case 1:
			{
				*str = (dec + '0');
				break;
			}
			default: *str = '0';
		}
		width--;
		str++;
	}
}

void hexToAscii(uchar* str,uint hex,uchar width)
{
	uint tmp;
	while(width)
	{
		switch(width)
		{		
			case 4:
			{
				tmp = (hex >> 12);
				if(tmp > 9) *str = tmp + ('A' - 10);
				else *str = tmp + '0';
				break;
			}
			case 3:
			{
				tmp = (hex >> 8) & 0x000F;
				if(tmp > 9) *str = tmp + ('A' - 10);
				else *str = tmp + '0';
				break;
			}
			case 2:
			{
				tmp = (hex >> 4) & 0x000F;
				if(tmp > 9) *str = tmp + ('A' - 10);
				else *str = tmp + '0';
				break;
			}
			case 1:
			{
				tmp = hex & 0x000F;
				if(tmp > 9) *str = tmp + ('A' - 10);
				else *str = tmp + '0';
				break;
			}
			default: *str = '0';
		}
		width--;
		str++;
	}
}

void showLine(const uchar x,const uchar y,uchar *lcd_stack,char *str,...)
{
	uchar coordinate = 16 * y + x;
	va_list marker;
	va_start(marker,str); // Initialize variable arguments.  
	while(*str != '\0')
	{
		if(coordinate > 64) break;	

		if(*str == '\\')
		{
			str++;
			lcd_stack[coordinate] = *str;
		}
		else if(*str == '%')
		{
			str++;
			if (*str == 'd' || *str == 'D')
			{
				str++;
				decToAscii(&lcd_stack[coordinate],va_arg(marker,uchar),(*str - '0'));
				coordinate += (*str - '0' - 1);
			}
			else if(*str == 'c' || *str == 'C')
			{
				lcd_stack[coordinate] = va_arg(marker,uchar);
			}
			else if(*str == 'x' || *str == 'X')
			{
				str++;
				hexToAscii(&lcd_stack[coordinate],va_arg(marker,uchar),(*str - '0'));
				coordinate += (*str - '0' - 1);
			}
		}
		else
		{
			lcd_stack[coordinate] = *str;
		}
		str++;
		coordinate++;
	}
	va_end( marker ); // Reset variable arguments. 
} 

#endif /*_WS_LCD_ST7920_H*/
