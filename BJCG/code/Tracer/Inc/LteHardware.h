#ifndef LteHardware_H
#define LteHardware_H
#include "main.h"
#define LTE_4G		0
#define LTE_NBIOT	1
#define GPS     	2
void LteUart_SendByte(unsigned char moduleType,uint8_t *str,uint32_t len);
void LtePowerManage(unsigned char moduleType,unsigned char powerCtrl);
void LteUart_SendStr(unsigned char moduleType,u8 *str);
void USART1_CLR_Buf(void);
void LteUartConfig(void);
unsigned char gps_powerON(void);


#endif 
