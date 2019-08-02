#ifndef RF_H
#define RF_H
u8 GetPowerMode(void);
typedef struct{
u8 CurrentMode ;
u8 ConfigureDone;
u8 lora_mode_satus;//dio0 ira flag 
u8 length_tmp;
INT8U length ;  //every payload length 
unsigned char RtcWakeUp_tmp;
uint32_t receive_timeout; //
unsigned int scan_timeout;
u16 wake_up_period;//19------>500ms
}POWER_MODE_Stru;
#endif
