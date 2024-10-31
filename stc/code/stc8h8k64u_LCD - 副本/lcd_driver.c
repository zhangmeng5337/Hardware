#include"main.h"
#include "sys.h"
#include "lcd_driver.h"

//#define CS		P25					//Ƭѡ��
//#define	RD		P24					//������ʱ��
//#define WR		P23					//д����ʱ��		
//#define DATA	P22					//�������ݶ�
	
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
		DATA = byte & 0X80;			//��λ�ȴ�	
		byte <<= 1;
		WR = 1;									//WR����������
		delay();
	}
}

void Write_dat(uchar dat,uchar addr)		//д����
{
	uchar i;
	CS = 1;
	delay();
	CS = 0;										//Ƭѡ�͵�ƽʹ��
	write_byte(0XA0,3);				//��3λ������101
	write_byte(addr,6);				//��6λ��ַ�룬��ʼ��ַ00H
	write_byte(dat,4);		//��4λ���ݡ���λ�ȴ�
	CS = 1;
}
void Write_cmd(uchar cmd)		//дָ��
{
	CS = 1;
	delay();
	CS = 0;
	write_byte(0X80,3);				//��3λ������100
	write_byte(cmd,9);				//��9λָ��
	CS = 1;
}


/****************************************/
void Write_dat2(uchar dat,uchar addr)		//д����
{
	uchar i;
	uchar tmp;

	i=0;
	CS = 1;
	delay();
	CS = 0;										//Ƭѡ�͵�ƽʹ��
	write_byte(0XA0,3);				//��3λ������101
	write_byte(addr,6);				//��6λ��ַ�룬��ʼ��ַ00H
	write_byte2(dat,8);		//seg1  0-1
	//write_byte2(dat<<4,4);		//��4λ���ݡ���λ�ȴ�	

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
		DATA = byte & 0X80;			//��λ�ȴ�	
		byte <<= 1;
		WR = 1;									//WR����������
		delay();
	}
}


