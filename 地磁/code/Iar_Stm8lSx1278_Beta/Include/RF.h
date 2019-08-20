#ifndef RF_H
#define RF_H
#include "stm8l15x.h"
#include "mytypedef.h"
//工作模式
#define NormalMode      0x00
#define WakeMode        0x10
#define PowerSavingMode 0x01
#define SleepMode       0x11
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
u8 CheckBit;
u8 CurrentMode;
u8 LastMode;
u8 ConfigureDone;
}Module_mode_stru;


typedef struct
{
//Flash 存储
unsigned char Flash_Data[5] ;
unsigned char Flash_Write_Done ;
/*参数1*/
unsigned char ADDH ;
/*参数2*/
unsigned char ADDL ;
/*参数3*/
u32 SerialRate ;
unsigned char AirRate;   //空口速度 默认2.4k
/*参数4*/
unsigned char Channel ; //默认通道为23通道 433MHZ
/*参数5*/
unsigned char TranMode ;
unsigned int WakeupTime ;
}Module_Params_stru;


POWER_MODE_Stru *GetPowerMode(void);
Module_Params_stru *GetModuleParams(void);
Module_mode_stru *GetModuleMode(void);
void RF_Initial(void);
#endif

