/*
*/
#ifndef __UART_H
#define __UART_H
//========PC机通讯有关定义==================

#define  PROTOCOL_485_MOTOR_ENABLE  

#define  UART_TASK_STK_SIZE                         128
#define  UART_TASK_START_PRIO                       5

#define  UART1TXBUFSIZE                   128
#define  UART1RXBUFSIZE                   64

#define  MODBUS_RX_DATA_LIST_NUMBER       16

#define  ADDR_485             0x01
#define  BAND_RATE            9600

typedef struct
{//
    unsigned char ReciveLength;
    unsigned char ReciveBuf[UART1RXBUFSIZE]; 	
}CModBusRecive;
typedef struct
{//
    CModBusRecive mModBusRecive[MODBUS_RX_DATA_LIST_NUMBER];
	  unsigned int  mWriteIn;
	  unsigned int  mReadOut;
}CModBusRxDataList;

extern CModBusRxDataList  mModBusRxDataList;
extern unsigned int  mRxLenght;
extern unsigned char m485LocalAddr;
extern unsigned char Uart1TxdDataLenght;
//////////////////////////////////////////
///////////////////////////////////////////////////////////////
extern unsigned char Uart1ReciveBuf[UART1RXBUFSIZE];
extern unsigned char Uart1TxdDataBuf[UART1TXBUFSIZE];

void UartInital(void);
void UartDmaIntial(void);
void UartDmaTxNString(unsigned char* pData,unsigned char mSendSize);
void TaskUartCreate(void);

#endif
//$
