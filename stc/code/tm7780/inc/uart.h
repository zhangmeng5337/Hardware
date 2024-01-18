#ifndef UART_H_
#define UART_H_
#include "hardware.h"



#define BUFFER_SIZE    16
#define CD4052_A0   	P10
#define CD4052_A1   	P37

#define TM7780_SET   	P34
#define TM7780_CAF   	P35
#define TM7780_CF    	P36


#define CURRENT_M		0
#define VOL_M		    1

typedef struct
{
	unsigned char recv_buf[BUFFER_SIZE];
	unsigned char recv_flag;
	unsigned char wptr;
	
}uart_stru;



void UartInit();
void UartSend(char dat);
void UartSendStr(char *p);
unsigned char uart_proc(void);
uart_stru *get_uart(void);
void UartSendbyte(char *p,unsigned int len);
uart_stru *get_uart(void);

#endif

