#ifndef LTEHAL_H
#define LTEHAL_H
#include "main.h"
#define BUFFERSIZE		128
#define MAX_CONNECT_COUNT  10

#define NODE_HEADER    0xaa
#define SERVER_HEADER  0xab

#define HEADDER_LEN 	1
#define DEVID_LEN 		7
#define CRC_LEN 		2
#define PAYLOAD_LEN 	4

#define DEVID_INDEX 	1
#define LENINDEX 		13
#define CMD_INDEX       16 
#define PARAMS_INDEX    17
//常量
#define Success 1U
#define Failure 0U
#define SIMCOM_TIME_OUT					200

#define	SIMCOM_NET_NOT 						                    0    	//开机未完成
#define	SIMCOM_POWER_ON                                         1
#define	SIMCOM_READY_YES                                        2
#define	SIMCOM_CLOSE_ECHO                                       3
#define	SIMCOM_CARD_DET                                         4
#define	SIMCOM_NET_DONE                                         5
#define	SIMCOM_GPRS_READY                                       6
#define	SIMCOM_NET_CLOSE                                        7
#define	SIMCOM_NET_UPDATE                                       8
#define	SIMCOM_NET_CONFIG                                       11
#define	SIMCOM_NET_RTC                                          9
#define	SIMCOM_NET_PASS_THROUGH                                 10
#define	SIMCOM_Connect_Platform                                 12
#define	SIMCOM_NET_ERROR					0XFF	//错误
#define	SIMCOM_NET_OK						13	//链接成功
  

typedef struct{
unsigned char lterxbuffer[BUFFERSIZE];
unsigned char gpsrxbuffer[BUFFERSIZE];
unsigned char LteReceivedFlag;
unsigned char GpsReceivedFlag;
unsigned char RetryConnectCount;
unsigned int rxSize;
unsigned char NetStatus;
}lte_stru;

void SIMCOM_Register_Network(void);

lte_stru *GetLteStru(void);
unsigned char *GetLteTime(void);

unsigned int sendCommand(unsigned char moduleType,char *Command, char *Response, uint32_t Timeout, unsigned char Retry,unsigned char eflag);
;

#endif

