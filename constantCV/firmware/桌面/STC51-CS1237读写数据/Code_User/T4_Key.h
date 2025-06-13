//功能 ：用于测试定时器T4的文件
#ifndef __T4_Key_H__
#define __T4_Key_H__
#include<STC15W.h>

sbit KEY1 = P3^2;//根据原理图，KEY1对应的IO口
sbit KEY2 = P3^3;//根据原理图，KEY2对应的IO口
sbit KEY3 = P3^6;//根据原理图，KEY3对应的IO口
sbit LED2 = P4^6;//看原理图，LED2对应的IO口
sbit LED1 = P4^7;//看原理图，LED1对应的IO口


void Init_T4(void);//初始化定时器T4




#endif






