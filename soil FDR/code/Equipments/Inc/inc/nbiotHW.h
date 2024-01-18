#ifndef NBIOTHW_H
#define NBIOTHW_H
 #include "main.h"

 #define NBIOT_BUFFER_SIZE		1024
typedef struct
{
	unsigned char receivedFlag;
	unsigned char nbiotRxBuffer[NBIOT_BUFFER_SIZE];
	unsigned char nbiotTxBuffer[NBIOT_BUFFER_SIZE];

}nbiotUart_stru;
void NbiotUart_CLR_Buf(void); 
void NbiotUart_SendStr(u8 *str);
void nbiot_HardwareInit(unsigned char flag);
void NbiotUartInit(void);
void Nbiot_RxCpltCallback(void);
unsigned char *NbiotFrameBuffer(void);
void Nbiot_SendData(u8 *buff,u8 len);
unsigned char *NbiotFrameStatus(void);

#endif

