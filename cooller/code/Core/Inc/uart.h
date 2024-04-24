#ifndef UART_H_
#define UART_H_
#include "main.h"
#define UART_CDG_DMA_REC_SIZE   9

#define UART_DMA_REC_SIZE   128
#define UART_REC_SIZE       512

#define UART_AIR        0
#define UART_LCD        1
#define UART_RF         2

typedef struct
{
    uint32_t  timeout;
    uint8_t     uartRecFlag;
    uint16_t   uartRecBuff_index;
    uint16_t    uartDMARecLen;
    uint16_t    uartRecLen;
    uint8_t     uartDMARecBuff[UART_DMA_REC_SIZE];
    uint8_t     uartRecBuff[UART_REC_SIZE];
} uart_recv_stru;
uart_recv_stru *get_uart_recv_stru(unsigned char uart_num);
void uart_transmit(unsigned char uart_num, uint8_t *pData, uint16_t Size);
void uart_rf_recv_proc(void);
void uart_lcd_recv_proc(void);
void uart_air_recv_proc(void);
void uart_init(void);
#endif


