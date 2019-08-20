#include "stm8l15x.h"
#include "sysclock.h"
#include "uart1.h"
#include "stm8l15x_clk.h"
#include "sx1276.h"
#include "stm8l15x_spi.h"
#include "bsp.h"
#include "stm8l15x_it.h"
#include "stm8l15x_tim3.h"
#include "stm8l15x_rtc.h"
#include "stm8l15x_flash.h"
#include "RF.h"
#include "stm8l15x_flash.h"

Module_mode_stru Module_mode;
Module_Params_stru Module_Params;
unsigned int pream_long[6] = {7, 30, 61, 244, 488, 976};
POWER_MODE_Stru POWER_MODE;
uint32_t rx_length;  //payload count in debug mode
INT8U recv_buffer[128]={ 0 }; //receive data buffer

/*参数4*/
extern unsigned char Channel ; //默认通道为23通道 433MHZ

POWER_MODE_Stru *GetPowerMode()
{
   POWER_MODE_Stru *s;
  return s;
}
Module_Params_stru *GetModuleParams()
{
    Module_Params_stru *s;
  return s;
}

Module_mode_stru *GetModuleMode()
{
  Module_mode_stru *s;
  return s;
}

void ModuleInit()
{
	Module_mode.AUX = 0;
	Module_mode.CheckBit = 0;
	Module_mode.ConfigureDone = 0;
	Module_mode.CurrentMode = 0;
	Module_mode.LastMode = 0;
	Module_mode.LoraM0Flag = 0;
	Module_mode.LoraM1Flag = 0;

	Module_Params.ADDH = 0;
	Module_Params.ADDL = 0;
	Module_Params.AirRate = 2;
	Module_Params.Channel =0x17;
	//Module_Params.Flash_Data[] =
	Module_Params.Flash_Write_Done = 1;
	Module_Params.SerialRate = 9600;
	Module_Params.TranMode = 0x01;
	Module_Params.WakeupTime = 250;

	

}
void RF_Initial( )
{

    FLASH_SetProgrammingTime(FLASH_ProgramTime_TProg);  //Flash 初始化
    
    if(FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 5)) == 1)  //判断Flash中是否有数据
    {
        Module_Params.Flash_Data[0] = FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS);
        Module_Params.Flash_Data[1] = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 1));
        Module_Params.Flash_Data[2] = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 2));
        Module_Params.Flash_Data[3] = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 3));
        Module_Params.Flash_Data[4] = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 4));
        Module_Params.ADDH = Module_Params.Flash_Data[0];
                                    
        Module_Params.ADDL = Module_Params.Flash_Data[1];

        if (Module_Params.Flash_Data[2] >> 6 == 0x01)
            Module_mode.CheckBit = 0x06;
        else if (Module_Params.Flash_Data[2] >> 6 == 0x02)
            Module_mode.CheckBit = 0x04;
        else Module_mode.CheckBit = 0x00;
        if (((Module_Params.Flash_Data[2]>>3) & 0x7) < 6)
            Module_Params.SerialRate = 1200 * (unsigned int)(pow(2, ((Module_Params.Flash_Data[2]>>3) & 0x7)));
        else if (((Module_Params.Flash_Data[2]>>3) & 0x7) == 6)
            Module_Params.SerialRate = 57600;
        else Module_Params.SerialRate = 115200;
        
        Module_Params.AirRate = Module_Params.Flash_Data[2]& 0x7;
                                    
        Module_Params.Channel =  Module_Params.Flash_Data[3]&0x1F;

        Module_Params.TranMode = Module_Params.Flash_Data[4]>>7;
        Module_Params.WakeupTime = 250*(((Module_Params.Flash_Data[4]>>3) & 0x7) + 1);
    }  
  
    SX1276_Init(MODE_LORA);         //SX127X 模块复位并初始化为LoRa模式
    SX1276_LoRa_SetDataRate(Module_Params.AirRate);
    SX1276_SetFreq(Module_Params.Channel);				//配置信道为23，即433Hz
    SX1276_SetPower(15, 1);         //配置RFO PIN输出，功率20dBm
    SX1276_SetRxMode();  
}


void lora_rx_process()

{
  if(GetPowerMode()->status  == 1)
  {
    POWER_MODE.length = SX1276_ReceivePacket(recv_buffer);			 // 妫�娴嬫槸鍚︽敹鍒颁竴涓暟鎹寘   
    if( POWER_MODE.length )
    {
      #if debug == 1
        POWER_MODE.RxLengthSum =  POWER_MODE.RxLengthSum +  POWER_MODE.length +4;
      #endif
      POWER_MODE.length_tmp = POWER_MODE.length+POWER_MODE.length_tmp;
      POWER_MODE.length = 0; 
    }  
    
  }
  
}

