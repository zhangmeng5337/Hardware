/**--------------File Info------------------------------------------------------------------------------
** File name:			usart.h
** Last modified Date:  2017-08-8
** Last Version:		V1.00
** Descriptions:		usart1 Config
**------------------------------------------------------------------------------------------------------
** Created by:			ShiHong Hu
** Created date:		2017-08-8
** Version:				  V1.00
** Descriptions:		usart1 Config
**
********************************************************************************************************/
#ifndef __USART_H
#define __USART_H

#ifdef   __USART_MANL
#define  __EXT_USART
#else  
#define  __EXT_USART  extern 
#endif

#define SET      1
#define RESET    0
#define USART_REC_LEN    200          //  串行通信接收数据的数组容量大小
#define EN_USART1_RX 		 1             //  使能（1）/禁止（0）串口接收

__EXT_USART unsigned char    USART_RXD_Buf[USART_REC_LEN];      //  串行通信接收保存数组寄存器
__EXT_USART unsigned int     USART_RXD_Cnt;                     //  串行通信接收计数器
                              //  串行通信接收数据个数计数

 void USART1_INIT(unsigned long baud);
 unsigned char Putchar(unsigned char ch);
 void PutString(unsigned char *dat, unsigned char len);
 void USART_RXD_Data_Process(void);
 unsigned long BoundRemanCode(unsigned char BaudCode);

#endif
