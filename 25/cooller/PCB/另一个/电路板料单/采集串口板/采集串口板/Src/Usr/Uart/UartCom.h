/*
*/
#ifndef __UART_COM_H
#define __UART_COM_H
#include    <ucos_ii.h>

//-----------------------------------------------------
#define  UART_TASK_COM_TX_STK_SIZE                         128
#define  UART_TASK_COM_TX_START_PRIO                       6
//-----------------------------------------------------
#define UART_TX_IDLE              0x0001
//-----------------------------------------------------
typedef struct
{
	  unsigned  char    Cmd;                //1
	  unsigned  char    CharacterWord[3];   //3
	  unsigned  char    DataLength;         //1
	  unsigned  char    DataBuf[58];
//..		                                  //  
	  unsigned  char    Pdc;                //1
}CUartCom;


#ifdef OS_VERSION
extern OS_EVENT      *pComTxSem;   //
extern OS_FLAG_GRP   *UartIdleFlag;   //´®¿Ú¿ÕÏÐ
#endif


//---------------------------------------------------------------------
void TaskUartComTxCreate(void);
void  UartComProccese(CUartCom *pCUartCom);
unsigned char PdcCheck(unsigned char *pData,unsigned char mDataLength);

void  UartComProccese(CUartCom *pUartCom);

unsigned char QueryState(CUartCom *pUartCom);
unsigned char PowerLed(CUartCom *pUartCom);
unsigned char ReadAD(CUartCom *pUartCom);
unsigned char ReadMultipleAD(CUartCom *pUartCom);
unsigned char StartApt(CUartCom *pUartCom);
#endif
//$
