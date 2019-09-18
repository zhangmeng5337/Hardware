#ifndef PROTOCOL_H
#include "main.h"
#include "bsp.h"

#define PROTOCOL_H


#define STATIC_MODE			        0X81
#define DYNAMIC_MODE				0x91
#define REGISTER_CODE				0xa8
#define HEART_BIT					0xa2


#define NODE_TO_SERVERH				0xAB
#define NODE_TO_SERVERL				0xAB
#define SERVER_TO_NODEH				0xAA
#define SERVER_TO_NODEL		       	0xAA
#define LORA_ACK    		        0x24

typedef struct{
	unsigned char headerH;
	unsigned char headerL;

	unsigned char id[3];	
	unsigned char func;
	unsigned char len;
	unsigned char checksum;
	unsigned char payload[12];	
	unsigned char seq_num;
	unsigned char server_channel;
	unsigned char tx_period;
	unsigned char car_flag;
	unsigned char car_time;
	unsigned char serverId[2];
	unsigned char serverAirRate;
	unsigned char nodeVersion;
	unsigned char vbat;

}DataPack_stru;
void Transmmit(unsigned char func);
void uart_process(void);
#endif

