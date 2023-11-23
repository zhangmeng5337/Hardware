#ifndef UART_H_
#define UART_H_
#include "main.h"
#define BUFFER_SIZE  128


#define LTE_No		0
#define RS485_No	1
#define LCD_No		2


#define LPUART1_DMA_REC_SIZE 1200
#define LPUART1_REC_SIZE 2000
typedef struct
{
	uint8_t  	Lpuart1RecFlag;
	uint16_t 	Lpuart1DMARecLen;
	uint16_t 	Lpuart1RecLen;
	uint8_t 	Lpuart1DMARecBuff[LPUART1_DMA_REC_SIZE];
	uint8_t		Lpuart1RecBuff[LPUART1_REC_SIZE];
}tsLpuart1type;
extern tsLpuart1type Lpuart1type;
/*定义串口成员变量的结构体指针*/

typedef struct 
{
	unsigned char recv_update;
	unsigned char buff[BUFFER_SIZE];
	unsigned char recv_buf[BUFFER_SIZE];	
	unsigned char recv_len;
}rs485_stru;

rs485_stru *get_uart_recv(unsigned char uart_num);
void uart_transmit(unsigned char uart_num,uint8_t * pData ,uint16_t Size);

void uart_lte(void);
void uart_rs485(void);
#endif


