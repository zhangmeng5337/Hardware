#ifndef UART_H_
#define UART_H_
#include "stm8l15x.h"

#define BUFFERSIZE		128

typedef struct{
  unsigned char rxbuffer[BUFFERSIZE];
  unsigned cahr received_flag;
  unsigned char received_len;
  unsigned char received_index;  
}uart_stru;
#endif
