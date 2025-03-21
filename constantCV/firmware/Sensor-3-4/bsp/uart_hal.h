/*******************************************************************************
*
* 代码许可和免责信�?* 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源�?* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或�?* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件�?* 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失�?* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些�?* 全部上述排除或限制可能并不适用于您�?*
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_HAL_H
#define __UART_HAL_H
#include "main.h"
#define LOG_ENABLE  1

#define TX_BUF_SIZE		128
#define RX_BUF_SIZE		128

typedef struct
{
	unsigned char tx_buf[TX_BUF_SIZE];
	unsigned char rx_buf[RX_BUF_SIZE];	
	unsigned char recv_update;
	unsigned int index;
	unsigned char reconfig;
	unsigned char baudrate;
	unsigned char parity; 
		enum brate_index
		{
			brate_1200 = 0,
			brate_2400 = 1,
			brate_4800 = 2,
			brate_9600 = 3,
			brate_19200 = 4,
			brate_38400 = 5,
			brate_57600 = 6,
			brate_115200 = 7
		} brate_index_index_em; // 
		enum parity_index
		{
			parityNone = 0,
			parityEven = 1,
			parityOdd = 2
		} parity_em; // 

	    enum baudrate_e
    {
        baudrate_1200 = 1200,
		baudrate_2400 = 2400,
		baudrate_4800 = 4800,
		baudrate_9600 = 9600,
		baudrate_19200 = 19200,
		baudrate_38400 = 38400,
		baudrate_57600 = 57600,
		baudrate_115200 = 115200
    } baudrate_em; // 
}uart_stru;
void uart_recv_proc(unsigned char recv_dat,unsigned char irq_flag);
void uart_tx(uint8_t *dat,unsigned char len);
void uart_init(void);
uart_stru  *getuart(void);
void uart_proc(void);

#endif /*__CW32L010_SPI_H */

