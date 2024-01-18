
#include "stm8l15x.h"
#include "sysclock.h"
#include "uart1.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_spi.h"
#include "bsp.h"
#include "stm8l15x_it.h"
#include "stm8l15x_tim3.h"
#include "stm8l15x_rtc.h"
#include "stm8l15x_flash.h"
#include "RF.h"
#include "stm8l15x_flash.h"
#include "uartParase.h"
#include "uart1.h"
#include "radio.h"

unsigned char InitFlag = 0;
  Module_mode_stru Module_mode;
extern Module_Params_stru Module_Params;



void flash_operation(uint32_t addr,unsigned char *p,unsigned char size)
{
  unsigned char i;
  i =0;
  FLASH_Unlock(FLASH_MemType_Data);	 
  for(i=0;i<size;i++)
  {
    FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + i));
  }
  for(i=0;i<size;i++)
  {
    FLASH_ProgramByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+i, p[i]);
    FLASH_WaitForLastOperation(FLASH_MemType_Data);
  }
  FLASH_Lock(FLASH_MemType_Data);   
  
}
void ModuleInit()
{
  Module_mode.AUX = 0;
  
  Module_mode.ConfigureDone = 0;
  Module_mode.CurrentMode = 0;
  Module_mode.LastMode = 0;
  Module_mode.LoraM0Flag = 0;
  Module_mode.LoraM1Flag = 0;
  Module_Params.CheckBit = 0;  
  Module_Params.ADDH = 0x01;
  Module_Params.ADDM = 0x04;
  
  Module_Params.ADDL = 0x03;
  
  Module_Params.Flash_Write_Done = 1;  
  Module_Params.AirRate = 0x03;
  Module_Params.Channel =0x17;
  Module_Params.SerialRate = 0x07;
  Module_Params.WakeupTime = 250;
}
void RF_Initial( )
{
  
  FLASH_SetProgrammingTime(FLASH_ProgramTime_TProg);  //Flash 初始化
  
  if(FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 9)) == 1)  //判断Flash中是否有数据
  {
    
    Module_Params.Channel = FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS);
    Module_Params.AirRate = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 1));
    Module_Params.power = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 2));
    Module_Params.SerialRate = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 3));
    Module_Params.CheckBit = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 4));
    Module_Params.WakeupTime = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 5));	
    Module_Params.ADDH = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 6));	
    Module_Params.ADDL = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 7));	
    Module_Params.TranMode = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 8));	
  } 
  else
    ModuleInit();
 
}



void moduleconfig()
{
  /*?ж?M0 M1 ????????????
  *M0 = 0  M1 = 1 ?????
  *M0 = 1  M1 = 0 ??????
  *M0 = 0  M1 = 1 ?????
  *M0 = 1  M1 = 1 ??????
  */
//  Module_mode.LoraM1Flag = GPIO_ReadInputDataBit(PORT_SX127X_M1, PIN_SX127X_M1);
//  Module_mode.LoraM0Flag = GPIO_ReadInputDataBit(PORT_SX127X_M0, PIN_SX127X_M0);
//  
//  if((Module_mode.LoraM0Flag == 0) && (Module_mode.LoraM1Flag == 0))
//    Module_mode.CurrentMode = NormalMode;
//  else if((Module_mode.LoraM0Flag != 0) && (Module_mode.LoraM1Flag == 0))
//    Module_mode.CurrentMode = WakeMode;
//  else if((Module_mode.LoraM0Flag == 0) && (Module_mode.LoraM1Flag != 0))			  //??0 ??0x02
//    Module_mode.CurrentMode = PowerSavingMode;
//  else Module_mode.CurrentMode = SleepMode;
  
//  if(Module_mode.LastMode != Module_mode.CurrentMode) { 							  //?????????仯???
//    AUX_CONFIGURING();
   // Module_mode.ConfigureDone = 0;
    RtcWakeUp = 0;
    RTC_DeInit();											//?????????? 
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, DISABLE); //???RTC??? 
    RTC_WakeUpCmd(DISABLE); 								//???RTC????????
    RTC_ITConfig(RTC_IT_WUT, DISABLE);						//???RTC?ж?
    Module_mode.LastMode = Module_mode.CurrentMode;
//  }
  
}


