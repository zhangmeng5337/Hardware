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
	
//#define CS		P10					//Ƭѡ��    6
//#define WR		P11					//д����ʱ��7		
//#define DATA	P37					//�������ݶ�8
//HT1621����λ��Һ��ģ��ӿڶ��壬�������ѵ���Ҫ���ģ�
#define HT1621_DAT P37 //HT1621��������
#define HT1621_CS P10 //HT1621ʹ������
#define HT1621_WR P11 //HT1621ʱ������

#endif

