#ifndef RF_H
#define RF_H
#include "stm8l15x.h"
#include "mytypedef.h"
//����ģʽ
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

//д�������0xFF��0x56��0xAE��0x35��0xA9��0x55��0x90
//Ƶ�ʣ��������ʣ����书�ʣ��������ʣ�����Ч�飬����ʱ��	
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

//stm8�������Լ�����״̬��
unsigned char LoraM0Flag;
unsigned char LoraM1Flag ;
unsigned char AUX;

u8 CurrentMode;
u8 LastMode;
u8 ConfigureDone;
}Module_mode_stru;


typedef struct
{
//Flash �洢
unsigned char TypeNo;
unsigned char Version;
/*����4*/
unsigned char Channel ; //Ĭ��ͨ��Ϊ23ͨ�� 433MHZ
unsigned char AirRate;   //�տ��ٶ� Ĭ��2.4k
unsigned char  power ;
unsigned char  SerialRate ;
unsigned char  CheckBit;
unsigned int WakeupTime ;
/*����1*/
unsigned char ADDH ;
/*����2*/
unsigned char ADDM ;
unsigned char ADDL ;

/*����3*/
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

