#include "main.h"
#include "uart.h"
#include "stdio.h"
extern UART_HandleTypeDef huart1;
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x)
{ 
	x = x; 
}  
//�ض���fputc���� 
//֧�ִ���1
int fputc(int ch, FILE *f)
{
	 unsigned char i;
	i = ch;
	 HAL_UART_Transmit(&huart1, &i, 1, 10);

//	while((USART1->SR&0X40)==0){};//ѭ������,ֱ���������   
//    USART1->DR = (u8) ch;      
	return i;
}