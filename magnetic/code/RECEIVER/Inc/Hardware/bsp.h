#ifndef BSP_H
#define BSP_H
#include "main.h"
#define ON	0
#define OFF    1
#define BLINK	2
#define UARTBUFFERSIZE		255
/* ?????§ÕSCL??SDA??? */


typedef struct{
unsigned char receive_flag;
unsigned int receive_len;
unsigned int read_len;
unsigned int index;
unsigned int read_index;
unsigned char receive_buffer[UARTBUFFERSIZE];

}uart_stru;
void UsartReceive_IDLE(unsigned char uart_num);
#endif
