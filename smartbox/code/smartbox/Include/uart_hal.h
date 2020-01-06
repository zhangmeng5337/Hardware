#ifndef UART_HAL_H_
#define UART_HAL_H_
#include "stm8l15x.h"

#define BUFFERSIZE                      64

typedef struct{
  unsigned char rxbuffer[BUFFERSIZE];
  unsigned char received_flag;
  unsigned char received_len;
  unsigned char received_index;  
}uart_stru;
#endif
