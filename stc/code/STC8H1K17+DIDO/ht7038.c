#include "main.h"
  #include "eeprom.h"
    #include "ht7050.h"
//sbit RESET=P1^3;		//��λ

//д�����ݵ�ATT7053c��ָ����ַ
void write_7053c(uchar add,ulong date)
{
	uchar i;
	SCLK=0;
	if(add<0x7f)add=add+0x80;
	CS=0;delay();
	for(i=0;i<8;i++)
	{
		add=add<<1;
		SCLK=1;delay();
		DIN=CY;
		SCLK=0;delay();
	}
	date=date<<8;delay();
	for(i=0;i<24;i++)
	{
		date=date<<1;
		SCLK=1;delay();
		DIN=CY;
		SCLK=0;delay();
	}
	CS=1;
}
//��ATT7053c��ָ����ַ��������
ulong read_7053c(uchar add)
{
	uchar i;
	ulong dat=0;
	CS=0;delay();
	for(i=0;i<8;i++)
	{
		add=add<<1;
		SCLK=1;delay();
		DIN=CY;
		SCLK=0;delay();
	}
	delay();delay();
	for(i=0;i<24;i++)
	{
		dat=dat<<1;
		SCLK=1;delay();
		if(DOUT==1)dat++;
		SCLK=0;delay();
	}
	CS=1;
	return(dat);
}
//��ʼ��att7053c
void in_att7053c(void)
{
	write_7053c(0x33,0x55);		//�����λ
	delay1(2);
	write_7053c(0x32,0xbc);		//����40-45
//	write_7053c(0x40,0x2000);	//���ܶ�����0
//	write_7053c(0x41,0x88);
//	write_7053c(0x42,0x7e);		//�򿪵���
	write_7053c(0x43,0x07);		//��3·ADC

//	write_7053c(0x32,0xa6);		//����50-7c
//	write_7053c(0x69,0x0380);		//
//	write_7053c(0x6a,0x0380);		//
//	write_7053c(0x6b,0x0380);		//
//	write_7053c(0x59,0x00);		//

//	write_7053c(0x61,0x023a);	//��������
//	write_7053c(0x6d,xjjz);
//	write_7053c(0x59,0x0026);		//����Ŵ�4��
	write_7053c(0x32,0x00);
	delay1(2);

}
