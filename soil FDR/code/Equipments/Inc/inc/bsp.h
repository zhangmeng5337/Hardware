#ifndef BSP_H_
#define BSP_H_
#include "main.h"



#define BIG_SIZE   1024
#define SMALL_SIZE   256
#define NBIOT        2
#define GPS          3
#define RS485        4
typedef struct{
	unsigned char uart_Num;
	uint32_t uart_rate;
	unsigned char uart_stopbit;

}uart_set_stru;
typedef struct{
	unsigned char uart_Num;
	unsigned char receivedFlag;
	unsigned char lora_master_len;
	unsigned char lora_slave_len;
	unsigned char nbiot_len;
	unsigned char gps_len;
	unsigned char rs485_len;	
}uart_data_stru;

void loraGpioSet(unsigned char p,unsigned char q);
void lora_uart_tx(unsigned char lora_num,unsigned char *p,unsigned char len);
void uart_init(uart_set_stru uartx);
#endif
