#ifndef UART_H_
#define UART_H_
#include "main.h"
#define BUFFER_SIZE  1024
#define RSBUFFER_SIZE  256


#define LTE_No		0
#define RS485_No	1
#define LCD_No		2


#define LPUART1_DMA_REC_SIZE 1200
#define LPUART1_REC_SIZE 1500
typedef struct
{
  uint32_t timeout;
	uint8_t  	Lpuart1RecFlag;
	uint16_t 	Lpuart1DMARecLen;
	uint16_t 	Lpuart1RecLen;
	uint8_t 	Lpuart1DMARecBuff[LPUART1_DMA_REC_SIZE];
	uint8_t		Lpuart1RecBuff[LPUART1_REC_SIZE];
	uint32_t    timeoutReboot;
}tsLpuart1type;
extern tsLpuart1type Lpuart1type;
/*定义串口成员变量的结构体指针*/

typedef struct 
{
	unsigned char recv_update;
	unsigned char buff[RSBUFFER_SIZE];
	unsigned char recv_buf[RSBUFFER_SIZE];	
	unsigned int recv_len;
	unsigned int recv_dma_len;
}rs485_stru;

rs485_stru *get_uart_recv(unsigned char uart_num);
void uart_transmit(unsigned char uart_num,uint8_t * pData ,uint16_t Size);
void uart_init(void);

void uart_lte(void);
void uart_rs485(void);


tsLpuart1type *get_lte_recv(void);

#endif


