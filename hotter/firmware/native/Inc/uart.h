#ifndef UART_H_
#define UART_H_
#include "main.h"
#define BUFFER_SIZE  128


#define LTE_No		0
#define RS485_No	1
#define LCD_No		2


typedef struct 
{
	unsigned char recv_update;
	unsigned char buff[BUFFER_SIZE];
	unsigned char recv_buf[BUFFER_SIZE];	
	unsigned char recv_len;
}rs485_stru;

rs485_stru *get_uart_recv(unsigned char uart_num);
void uart_transmit(unsigned char uart_num,uint8_t * pData ,uint16_t Size);


#endif


