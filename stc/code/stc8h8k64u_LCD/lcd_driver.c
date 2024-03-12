#include"main.h"
#include "sys.h"
#include "lcd_driver.h"

//#define CS		P25					//片选端
//#define	RD		P24					//读数据时钟
//#define WR		P23					//写数据时钟		
//#define DATA	P22					//串行数据端
	
//typedef unsigned char uchar;
//typedef unsigned int	uint;

//uchar code tab[]={
//0xbe,0x06,0x5e,0xc6,0xda,0xfa,0x0e,0xfe,0xde,0xee,0xf2,0xb8,0x76,0xf8,0xe8,0x00
//};


void write_byte(uchar byte,uchar length)
{
	uchar i;
	for(i=0;i<length;i++)
	{
		WR = 0;
		delay();
		DATA = byte & 0X80;			//高位先传	
		byte <<= 1;
		WR = 1;									//WR上升沿锁存
		delay();
	}
}
void Write_dat(uchar dat,uchar addr)		//写数据
{
	uchar i;
	CS = 1;
	delay();
	CS = 0;										//片选低电平使能
	write_byte(0XA0,3);				//传3位命令码101
	write_byte(addr,6);				//传6位地址码，起始地址00H
	write_byte(dat,4);		//传4位数据。高位先传
}
void Write_cmd(uchar cmd)		//写指令
{
	CS = 1;
	delay();
	CS = 0;
	write_byte(0X80,3);				//传3位命令码100
	write_byte(cmd,9);				//传9位指令
}

