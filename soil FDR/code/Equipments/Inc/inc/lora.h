#ifndef LORA_H_
#define LORA_H_

#include "main.h"

#define LORA_MASTER  0
#define LORA_SLAVE   1
#define TX_MODE   		0
#define WOR_MODE   		1
#define CONFIG_MODE   2
#define SLEEP_MODE   	3

typedef struct
{
  unsigned char addH;
	unsigned char addL;
	unsigned char netid;
	unsigned char reg0;
	unsigned char reg1;
	unsigned char reg2;	
	unsigned char reg3;
	unsigned char crptH;
	unsigned char crptL;
  unsigned char *pb;	
}LORA_Params_stru;

typedef struct{
	unsigned char mode;
	unsigned char status;
	uint32_t uart_rate;
	uint32_t air_rate;
	unsigned char payloadSize;
	unsigned char rssiNoise;
	unsigned char power;
	unsigned char channel;
	unsigned char rssi;
	unsigned char txMode;
	unsigned char relay;
	unsigned char LBT;
	unsigned char wor;
	unsigned int worPeriod;
	unsigned char crptH;
	unsigned char crptL;
}lora_usr_stru;

#endif
