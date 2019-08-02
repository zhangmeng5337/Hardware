#ifndef RF_H
#define RF_H
u8 GetPowerMode(void);
typedef struct
{
	u8 CurrentMode ;
	u8 ConfigureDone;
	u8 satus;//dio0 ira flag 
	u8 length_tmp;
	INT8U length ;  //every payload length 
        INT8U RxLengthSum ; 
	uint32_t receive_timeout; //
	unsigned int scan_timeout;
	u16 wake_up_period;//19------>500ms
	u8 CadDoneFlag;
        u8 RtcWakeUp;
        u8 RtcWakeUp_tmp;
}POWER_MODE_Stru;
void RF_Initial(void);
#endif
