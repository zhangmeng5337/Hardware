#ifndef UART_H_
#define UART_H_
#include "main.h"
#define UART_BUFFER_SIZE  128

typedef struct 
{
	unsigned char recv_update;
	unsigned char recv_buf[UART_BUFFER_SIZE];	
	unsigned char recv_len;
	uint32_t led_ms;
	//unsigned char status;
}uart_stru;

void uart_init(void);
void uart_proc(void);
void uart_rec(void);
void led_ctrl(uint32_t delay_ms);
void led_ctrl_irq(void);
void uart_tx(void);
void app(void);

#endif



