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
sbit CS   =P3^7;		//ѡͨ 1
sbit SCLK =P3^4;		//ʱ�� 1
sbit DIN  =P3^6;		//���� 1
sbit DOUT =P3^5;		//��� 1   AT7053BU
#endif

