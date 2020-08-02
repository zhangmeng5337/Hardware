#ifndef GPRS_H
#define GPRS_H
#include "uart1.h"
#include "stm8l15x.h"
//常量
#define Success 1U
#define Failure 0U
#define SIMCOM_TIME_OUT					1000

#define	SIMCOM_NET_NOT 						0    	//开机未完成
#define	SIMCOM_POWER_ON                                         1
#define	SIMCOM_READY_YES                                        2
#define	SIMCOM_CLOSE_ECHO                                       3
#define	SIMCOM_CARD_DET                                         4
#define	SIMCOM_GPRS_READY                                       5
#define	SIMCOM_NET_CLOSE                                        6
#define	SIMCOM_NET_PASS_THROUGH                                 7
#define	SIMCOM_Connect_Platform                                 8
#define	SIMCOM_NET_ERROR					0XFF	//错误
#define	SIMCOM_NET_OK						9	//链接成功


//#define NULL 0U
//#define USARTX_RX_BUF    uart_str.UsartReceiveData
void errorLog(int num);
void phone(char *number);
unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
unsigned char Get_Network_status(void);
void SIMCOM_Register_Network(void);
void set_NetStatus(unsigned char flag);
#endif
