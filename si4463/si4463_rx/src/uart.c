#include "main.h"
#include "uart.h"
#include "stdio.h"
extern UART_HandleTypeDef huart1;
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x)
{ 
	x = x; 
}  
//重定义fputc函数 
//支持串口1
int fputc(int ch, FILE *f)
{
	 unsigned char i;
	i = ch;
	 HAL_UART_Transmit(&huart1, &i, 1, 10);

//	while((USART1->SR&0X40)==0){};//循环发送,直到发送完毕   
//    USART1->DR = (u8) ch;      
	return i;
}