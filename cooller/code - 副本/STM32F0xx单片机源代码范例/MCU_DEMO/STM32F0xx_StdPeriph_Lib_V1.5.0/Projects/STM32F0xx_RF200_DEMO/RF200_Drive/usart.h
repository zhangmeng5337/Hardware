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
#define USART_REC_LEN    200          //  ����ͨ�Ž������ݵ�����������С
#define EN_USART1_RX 		 1             //  ʹ�ܣ�1��/��ֹ��0�����ڽ���

__EXT_USART unsigned char    USART_RXD_Buf[USART_REC_LEN];      //  ����ͨ�Ž��ձ�������Ĵ���
__EXT_USART unsigned int     USART_RXD_Cnt;                     //  ����ͨ�Ž��ռ�����
                              //  ����ͨ�Ž������ݸ�������

 void USART1_INIT(unsigned long baud);
 unsigned char Putchar(unsigned char ch);
 void PutString(unsigned char *dat, unsigned char len);
 void USART_RXD_Data_Process(void);
 unsigned long BoundRemanCode(unsigned char BaudCode);

#endif
