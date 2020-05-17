#ifndef __UART3_H
#define __UART3_H

/* 包含系统头文件 */
#include <stdio.h>
#include <stdarg.h>
#include "stm8l15x.h"
#include "stm8l15x_syscfg.h"

//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
#define false 0
#define true 1

u8 UART3_ReceiveByte(void);
void Uart3_Init(u32 boundrate);
void UART3_SendByte(u8 data);
void UART3_SendStr(u8 *str);
u8 UART3_ReceiveByte(void);
void uart3_interrupt_handler(void);
#endif

