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
	
//#define CS		P10					//片选端    6
//#define WR		P11					//写数据时钟7		
//#define DATA	P37					//串行数据端8
//HT1621控制位（液晶模块接口定义，根据自已的需要更改）
#define HT1621_DAT P37 //HT1621数据引脚
#define HT1621_CS P10 //HT1621使能引脚
#define HT1621_WR P11 //HT1621时钟引脚

#endif

