#ifndef RF_H
#define RF_H
#include "stm8l15x.h"
#define BUFFERSIZE		128
//typedef unsigned char u8;
typedef struct
{
	u8 CurrentMode ;
	u8 ConfigureDone;
	u8 status;//dio0 ira flag 
	u8 length_tmp;
	u8 length ;  //every payload length 
        u8 RxLengthSum ; 
	uint32_t receive_timeout; //
	unsigned int scan_timeout;
	u16 wake_up_period;//19------>500ms
	u8 CadDoneFlag;
        u8 RtcWakeUp;

}POWER_MODE_Stru;

typedef struct
{
	u8 datarate ;
        unsigned int freq;
	unsigned char power;	
        unsigned int id;
	unsigned char  AddrH;
	unsigned char  AddrL;
	unsigned char BurdRate;
}RfParams_stru;


void RF_Initial(void);
void PowerSavingMode_setting(void);
POWER_MODE_Stru GetPowerMode(void);
#endif
