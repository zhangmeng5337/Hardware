

/* Includes ------------------------------------------------------------------*/
#include "flash.h"
#include "main.h"
#include "eeprom.h"
#include "uartParse.h"

/******************************************************************************
* Include files
******************************************************************************/
#include "ddl.h"
#include "flash.h"




unsigned char FLASH_ReadByte(uint32_t readadd)
{
  return *((volatile uint32_t*)readadd);
}
params_typedef * eeprom_data_read()
{
  params_typedef params;
  
  uint32_t  add;
  add =   0x3ff0;  
  if(*((volatile uint32_t*)add)==0x5a)
  {
    params.KEY_H8= FLASH_ReadByte(add++);
    params.KEY_L8= FLASH_ReadByte(add++);
    params.inhibition= FLASH_ReadByte(add++);
    for(unsigned char i=0;i<SN_LEN;i++)
      params.sn[i]=FLASH_ReadByte(add++);
    params.freq=FLASH_ReadByte(add++);				
  }
  else
  {
    
    params.KEY_H8= KEYFUNC_DEFAULT;
    params.KEY_L8= KEYFUNC_DEFAULT;
    params.inhibition= FLASH_ReadByte(add++);
    for(unsigned char i=0;i<SN_LEN;i++)
      params.sn[i]=SN_DEFAULT;
    params.freq= CHANNEL_DEFAULT ;
    return &params;
  }
  
}

/*******************************************************************************
* FLASH 中断服务函数
******************************************************************************/
void FlashInt(void)
{
  if (TRUE == Flash_GetIntFlag(flash_int0))
  {
    Flash_ClearIntFlag(flash_int0);
    
    Flash_DisableIrq(flash_int0);
  }
  if (TRUE == Flash_GetIntFlag(flash_int1))
  {
    Flash_ClearIntFlag(flash_int1);
    Flash_DisableIrq(flash_int1);
  }
  
}

en_result_t eeprom_write(params_typedef params)
{
  en_result_t       enResult = Error;
  uint32_t          u32Addr  = 0x3ff0;
  uint8_t           u8Data   = 0x5a;
  
  u32Addr  = 0x3ff0;
  Flash_Init(FlashInt, 0);
  
  Flash_SectorErase(u32Addr);
  
  enResult = Flash_WriteByte(u32Addr++, 0x5a);
  enResult = Flash_WriteByte(u32Addr++,  params.KEY_H8);
  enResult = Flash_WriteByte(u32Addr++,  params.KEY_L8);
  enResult = Flash_WriteByte(u32Addr++,  params.inhibition);
  for(unsigned char i=0;i<SN_LEN;i++)
    enResult = Flash_WriteByte(u32Addr++,  params.sn[i]);
  enResult = Flash_WriteByte(u32Addr++,  params.freq);	
  
  return   enResult;
}





