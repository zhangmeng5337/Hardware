#ifndef LORAHAL_H_
#define LORAHAL_H_

#include "main.h"
#define LORA_BUFFER_SIZE		512

#define DAT_FROM_RELAY			1
#define DAT_FROM_NBIOT      2
#define DAT_FROM_PC				  3
#define DAT_FROM_LORA			  4
typedef struct
{
	unsigned char receivedFlag1;
	unsigned char receivedFlag2;
	unsigned char ack;
	unsigned char lora1RxBuffer[LORA_BUFFER_SIZE];
	unsigned char lora2RxBuffer[LORA_BUFFER_SIZE];	
}loraUart_stru;

void EquipGateway_Process(void);
void ParamsInit(void);
loraUart_stru *getloraProcStatus(void);


//void Lora_RxCpltCallback(unsigned char uartNo);
#endif
