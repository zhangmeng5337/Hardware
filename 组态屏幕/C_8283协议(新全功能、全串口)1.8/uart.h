#ifndef __UART_H__
#define __UART_H__

#include "sys.h"
#include "crc16.h"
//sbit RS485_TX_EN=P0^1;

#define	Uart_Rev_Pre 			0x00
#define Uart_Rev_GetFH1 	0x01
#define	Uart_Rev_GetFH2 	0x02
#define Uart_Rev_GetLen 	0x03
#define Uart_Rev_GetCode	0x04
#define Uart_Rev_GetAddr1 0x05
#define Uart_Rev_GetAddr2 0x06
#define Uart_Rev_Done 		0x07


#define	Uart2_Tx_Lenth	256
#define	Uart2_Rx_Lenth	1024
#define	TimeOutSet2		10

#define	Uart3_Tx_Lenth	256
#define	Uart3_Rx_Lenth	1024

#define	Uart4_Tx_Lenth	256
#define	Uart4_Rx_Lenth	1024

#define	Uart5_Tx_Lenth	256
#define	Uart5_Rx_Lenth	1024

#define Uartx_Frame_Data_Lenth	264

void Uart2_Init(u32 baud_rate);
void Uart2_Tx_write2buff(u8 dat);

void Uart3_Tx_write3buff(u8 dat);

void Uart4_Init(u32 baud_rate);
void Uart4_Tx_write4buff(u8 dat);

void Uart5_Tx_write5buff(u8 dat);

void  uart_send_str(u8 U_number,u8* Str,u8 Len);

void Uart_Handle_all();
void  Read_0xF00();
void uart_init();

extern struct Uartx_Define Uart2;
typedef struct Uartx_Define{ 
	u8	id;					//���ں�
	u16	Tx_Read;		//���Ͷ�ָ��
	u16	Tx_Write;		//����дָ��
	u8	Tx_Busy;		//����æ
	u16	Rx_Read;		//���ն�ָ��
	u16	Rx_Write;		//����дָ��
	u8	Rx_Busy;		//����æ
	u8	Rx_Flag;		//���ձ�־
	u8  Rx_Buffer[Uart2_Rx_Lenth];	//���ջ���
  u8  Tx_Buffer[Uart2_Tx_Lenth];	//���ͻ���
} *P_S_UART; 

typedef struct Uartx_Frame_Data{
	u8	dataLen;		//���յ����ݳ���
	u8	dataCode;		//��������
	u16	varAddr;		//������ַ
	u8	varData[Uartx_Frame_Data_Lenth];		//��������
	u8	varIndex;		//��������ָ��
} *P_D_UART;

#endif
