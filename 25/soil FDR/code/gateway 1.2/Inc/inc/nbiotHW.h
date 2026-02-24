#ifndef NBIOTHW_H
#define NBIOTHW_H
 #include "main.h"
#define GATEWAY_HEART_PERIOD		90000

 #define NBIOT_BUFFER_SIZE		512
typedef struct
{
	unsigned char receivedFlag;
	unsigned char nbiotRxBuffer[NBIOT_BUFFER_SIZE];
	unsigned char nbiotTxBuffer[NBIOT_BUFFER_SIZE];
  uint32_t rxSize;
}nbiotUart_stru;
void NbiotUart_CLR_Buf(void); 
void NbiotUart_SendStr(u8 *str);
void nbiot_HardwareInit(unsigned char flag);
void NbiotUartInit(void);
unsigned char *NbiotFrameBuffer(void);
void Nbiot_SendData(u8 *buff,u8 len);
unsigned char *NbiotFrameStatus(void);
unsigned char *getHeartStatus(unsigned char p); 
void Nbiot_RxCpltCallback(unsigned char flag);
uint32_t *NbiotFrameBufferSize(void);
#endif

