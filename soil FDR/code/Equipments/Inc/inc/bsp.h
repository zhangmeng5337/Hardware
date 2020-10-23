#ifndef BSP_H_
#define BSP_H_
#include "main.h"
typedef struct{
	unsigned char uart_Num;
	uint32_t uart_rate;
	unsigned char uart_stopbit;

}uart_set_stru;
void loraGpioSet(unsigned char p,unsigned char q);
void loratxdata(unsigned char p,unsigned char *q,unsigned char len);
void uart_init(uart_set_stru uartx);
#endif
