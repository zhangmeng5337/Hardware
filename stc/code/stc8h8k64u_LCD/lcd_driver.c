#include"main.h"
#include "sys.h"
#include "lcd_driver.h"
uchar code seg_tab[]={0,1,2,3,4,5,13,14,15,16,17,18,19,26,27,28,29,30};//0-5 13-19 26-30
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
	CS = 1;
}
void Write_cmd(uchar cmd)		//写指令
{
	CS = 1;
	delay();
	CS = 0;
	write_byte(0X80,3);				//传3位命令码100
	write_byte(cmd,9);				//传9位指令
	CS = 1;
}


/****************************************/
void Write_dat2(uchar *dat,uchar seg_num)		//写数据
{
	uchar i;
	uchar tmp;
	uchar *p;
	p = dat;
	i=0;
	CS = 1;
	delay();
	CS = 0;										//片选低电平使能
	write_byte(0XA0,3);				//传3位命令码101
	write_byte(0,6);				//传6位地址码，起始地址00H
	

	//write_byte(1,6);				//传6位地址码，起始地址00H
   
		write_byte2(0x5f,4);		//seg1  0-1
		write_byte2(0xf0,4);		//传4位数据。高位先传	
	 
	  i=1;
		write_byte2(0X06,4);		//seg2  2-3
		write_byte2(0X60,4);		//传4位数据。高位先传	

	  i=2;
		write_byte2(0X6b,4);		//seg3 4-5
		write_byte2(0Xb0,4);		//传4位数据。高位先传	

		for(i=0;i<8;i++)//6-12
		{
			write_byte2(0,4);		//传4位数据。高位先传
		}	
	  i=3;
		write_byte2(0X2f,4);		//seg4  13-14
		write_byte2(0Xf0,4);		//传4位数据。高位先传	

	  i=4;
		write_byte2(0X36,4);		//seg5  15-16
		write_byte2(0X60,4);		//传4位数据。高位先传	

	  i=5;
		write_byte2(0X3D,4);		//seg6  17-18
		write_byte2(0XD0,4);		//传4位数据。高位先传	
		
	  i=6;
		write_byte2(0X7D,4);		//seg7 19
		for(i=0;i<6;i++)//20-25
		{
			write_byte2(0,4);		//传4位数据。高位先传
		}	
		write_byte2(0XD0,4);		//26	
		
	  i=7;
		write_byte2(0X07,4);		//seg8  27-28
		write_byte2(0X70,4);		//传4位数据。高位先传			
		
		for(i=0;i<3;i++)//29-31
		{
			write_byte2(0,4);		//传4位数据。高位先传
		}			
		
//	if(seg_num==7)
//	{
//		tmp = seg_num-1;  //13
//		tmp =tmp*2;
//		for(i=0;i<seg_tab[tmp];i++)
//		{
//			write_byte2(0,4);		//传4位数据。高位先传
//		}	
//		write_byte2(dat,4);		//19
//		
//		for(i=20;i<26;i++)
//		{
//			write_byte2(0,4);		//传4位数据。高位先传
//		}	
//		write_byte2(dat<<4,4);		//传4位数据。高位先传	
//		
//		for(i=27;i<32;i++)
//		{
//			write_byte2(0,4);		//传4位数据。高位先传
//		}	
//	}
//	else
//	{

//		tmp = seg_num-1;  //13
//		tmp =tmp*2;
//		for(i=0;i<seg_tab[tmp];i++)
//		{
//			write_byte2(0,4);		//传4位数据。高位先传
//		}	
//		write_byte2(dat,4);		//传4位数据。高位先传
//		write_byte2(dat<<4,4);		//传4位数据。高位先传	
//		
//		for(i=seg_tab[tmp+2];i<32;i++)
//		{
//			write_byte2(0,4);		//传4位数据。高位先传
//		}	

//	}


	
		CS = 1;
	delay();
}

void write_byte2(uchar byte,uchar length)
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


