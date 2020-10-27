#ifndef UART_H_
#define UART_H_
#include "main.h"
typedef struct
{
	unsigned char *buffer;
	unsigned char len;
	unsigned char receivedFlag;
}uart_usr_stru;
#endif
