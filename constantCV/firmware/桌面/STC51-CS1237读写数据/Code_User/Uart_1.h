//���Դ���0����USBת����ֱ������   9600bps 30MHZ
#ifndef __Uart_1_H__
#define __Uart_1_H__

#include<STC15W.h>
void Init_Uart1(void);// ����: ���ڳ�ʼ������.
void Send_Data1(unsigned char dat);//COM1����һ���ֽ�
void Send_Str1(unsigned char *s);//����: COM1����һ���ַ���
//��ʱ����  ��������ʹ��
void Delay1ms(unsigned int a);	// 1ms @30.000MHz

#endif