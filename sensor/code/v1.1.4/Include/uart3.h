#ifndef __UART3_H
#define __UART3_H

/* ����ϵͳͷ�ļ� */
#include <stdio.h>
#include <stdarg.h>
#include "stm8l15x.h"
#include "stm8l15x_syscfg.h"

//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
#define false 0
#define true 1

u8 UART3_ReceiveByte(void);
void Uart3_Init(u32 boundrate);
void UART3_SendByte(u8 data);
void UART3_SendStr(u8 *str);
u8 UART3_ReceiveByte(void);
void uart3_interrupt_handler(void);
#endif

