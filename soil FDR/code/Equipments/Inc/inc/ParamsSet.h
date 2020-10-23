#ifndef PARAMSSET_H_
#define PARAMSSET_H_
#include "main.h"

#define LORA_MASTER  0
#define LORA_SLAVE   1
#define NBIOT        2
#define GPS          3
#define RS485        4

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
