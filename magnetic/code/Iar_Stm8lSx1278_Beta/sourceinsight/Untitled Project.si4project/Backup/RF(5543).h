#ifndef RF_H
#define RF_H
#include "stm8l15x.h"
#include "mytypedef.h"
//工作模式
#define NormalMode      0x00
#define WakeMode        0x10
#define PowerSavingMode 0x01
#define SleepMode       0x11

#define NORMALCOMMAND     		0x00
#define WRITECOMMAND      		0x01
#define READCOMMAND       		0x02
#define LORA_WRITE   			0x90
#define LORA_READ      			0xf0
#define NORMA_OPERATION      	0x00

//写设置命令：0xFF，0x56，0xAE，0x35，0xA9，0x55，0x90
//频率，空中速率，发射功率，串口速率，串口效验，唤醒时间	
#define INDEX_FREQ	    		0x07
#define INDEX_AIRATE	    	0x0a
#define INDEX_POWER	    		0x0b
#define INDEX_BURDRATE	    	0x0c
#define INDEX_CHECK	    		0x0d
#define INDEX_WAKETIME	    	0x0e
#define INDEX_ADDH	    		0x0f	
#define INDEX_ADDL	    		0x10
#define INDEX_MODE	    		0x11



typedef struct
{
	u8 CurrentMode ;
	u8 ConfigureDone;
	u8 status;//dio0 ira flag 
	u8 length_tmp;
	INT8U length ;  //every payload length 
        INT8U RxLengthSum ; 
	uint32_t receive_timeout; //
	unsigned int scan_timeout;
	u16 wake_up_period;//19------>500ms
	u8 CadDoneFlag;
        u8 RtcWakeUp;

}POWER_MODE_Stru;


typedef struct
{

//stm8控制线以及工作状态线
unsigned char LoraM0Flag;
unsigned char LoraM1Flag ;
unsigned char AUX;

u8 CurrentMode;
u8 LastMode;
u8 ConfigureDone;
}Module_mode_stru;


typedef struct
{
//Flash 存储
unsigned char TypeNo;
unsigned char Version;
/*参数4*/
unsigned char Channel ; //默认通道为23通道 433MHZ
unsigned char AirRate;   //空口速度 默认2.4k
unsigned char  power ;
unsigned char  SerialRate ;
unsigned char  CheckBit;
unsigned int WakeupTime ;
/*参数1*/
unsigned char ADDH ;
/*参数2*/
unsigned char ADDM ;
unsigned char ADDL ;

/*参数3*/
unsigned char TranMode;

unsigned char Flash_Write_Done ;

}Module_Params_stru;


POWER_MODE_Stru *GetPowerMode(void);
 Module_Params_stru *GetModuleParams(void);
Module_mode_stru *GetModuleMode(void);
void RF_Initial(void);
void moduleconfig(void);
void lora_process(void);

#endif

