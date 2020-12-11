#ifndef LORAHW_H_
#define LORAHW_H_

#include "main.h"
#define DAT_FROM_LORA			4
#define DAT_FROM_NBIOT          2
#define DAT_FROM_PC				3
#define DAT_FROM_RELAY			1


#define DEBUG			1
#define TransmitMode	0
#define WorMOde			1	
#define ConfigMOde		2
#define SleepMode		3

typedef struct
{
	unsigned char addrH;
	unsigned char addrL;
	unsigned char netid;
	unsigned char reg0;
	unsigned char reg1;	
	unsigned char reg2;
	unsigned char reg3;
	unsigned char cryptH;
	unsigned char cryptL;
	unsigned char PID[7];
}LORA_Params_stru;

typedef struct
{
	unsigned char mode; // TransmitMode	0;WorMOde 1;ConfigMOde2;SleepMode 3
	unsigned char loraNo;//0:M1,1:M2
	uint32_t baudrate;
	unsigned char Parity;
	unsigned char enable;
}LORAHW_stru;
unsigned char loraSend(LORAHW_stru *p,unsigned char *buffer,unsigned int len);
unsigned char loraGpioset(LORAHW_stru *p);
void loraset(unsigned char num,unsigned char *p,unsigned char len);
void LoraSetPayloadPackTx(unsigned cmd,unsigned char startaddr,unsigned char len);
void ParamsRead(void);
void loraModuleInit(void);
void LoraTest(void);
void Lora_RxCpltCallback(unsigned char uartNo);
void LoraUartInit(void);
void LoraSendPayloadPackTx(unsigned char *buffer,unsigned char len);
unsigned char EnterStop(void);
void ParamsSave(unsigned char datasrc);//²ÎÊý±£´æ


#endif
