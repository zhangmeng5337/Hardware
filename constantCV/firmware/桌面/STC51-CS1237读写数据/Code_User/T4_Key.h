//���� �����ڲ��Զ�ʱ��T4���ļ�
#ifndef __T4_Key_H__
#define __T4_Key_H__
#include<STC15W.h>

sbit KEY1 = P3^2;//����ԭ��ͼ��KEY1��Ӧ��IO��
sbit KEY2 = P3^3;//����ԭ��ͼ��KEY2��Ӧ��IO��
sbit KEY3 = P3^6;//����ԭ��ͼ��KEY3��Ӧ��IO��
sbit LED2 = P4^6;//��ԭ��ͼ��LED2��Ӧ��IO��
sbit LED1 = P4^7;//��ԭ��ͼ��LED1��Ӧ��IO��


void Init_T4(void);//��ʼ����ʱ��T4




#endif






