#ifndef MAIN_H_
#define MAIN_H_
#include <STC8F.h>
#define uchar  unsigned char
#define bchar bdata unsigned char
#define uint   unsigned int
#define ulong  unsigned long
#define xchar  xdata unsigned char
#define xint   xdata unsigned int
#define xlong  unsigned long
	
//SPI
sbit CS   =P5^4;		//ѡͨ 1
sbit SCLK =P3^6;		//ʱ�� 1
sbit SPI_MOSI  =P3^5;		//���� 1
sbit SPI_MISO =P3^4;		//��� 1   AT7053BU

sbit CD4052_A1   =P1^0;		//ѡͨ 1
sbit CD4052_A0   =P1^1;		//ѡͨ 1


#endif

