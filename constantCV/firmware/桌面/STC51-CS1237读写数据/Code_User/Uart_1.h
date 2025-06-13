//测试串口0，与USB转串口直接连接   9600bps 30MHZ
#ifndef __Uart_1_H__
#define __Uart_1_H__

#include<STC15W.h>
void Init_Uart1(void);// 描述: 串口初始化程序.
void Send_Data1(unsigned char dat);//COM1发送一个字节
void Send_Str1(unsigned char *s);//描述: COM1发送一个字符串
//延时函数  主函数中使用
void Delay1ms(unsigned int a);	// 1ms @30.000MHz

#endif