#ifndef LORA_H
#define LORA_H

#define NORMAL_MODE				0X00
#define WAKE_MODE					0X01
#define SLEEP_MODE					0X02
#define POWER_SAVING_MODE			0X03

#define ACK_HEADER

typedef struct{
	unsigned int Frequence;
	unsigned int AirRate;
	unsigned int UartBurdRate;
	unsigned int Check;	
	unsigned char WakeUpPeriod;
	unsigned char Power;
	unsigned int RfId;

}ModuleParams_Str;

typedef struct{
	unsigned int     mode;
	unsigned char  status;

}ModuleOperation_Str;
#endif

