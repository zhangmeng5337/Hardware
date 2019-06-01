

/* Includes ------------------------------------------------------------------*/
#include "flash.h"
#include "main.h"
#include "eeprom.h"
#include "uartParse.h"
#include "bsp_i2c_ee.h"
/******************************************************************************
* Include files
******************************************************************************/
#include "ddl.h"
#include "flash.h"




unsigned char FLASH_ReadByte(uint32_t readadd)
{
  return *((volatile uint32_t*)readadd);
}
params_typedef *eeprom_data_read()
{
  params_typedef params2;
  
  uint32_t  add;
  add =   0x3ff0;  
  if(*((volatile uint32_t*)add)==0x5a)
  {
    params2.KEY_H8= FLASH_ReadByte(add++);
    params2.KEY_L8= FLASH_ReadByte(add++);
    params2.inhibition= FLASH_ReadByte(add++);
    for(unsigned char i=0;i<SN_LEN;i++)
      params2.sn[i]=FLASH_ReadByte(add++);
    params2.freq=FLASH_ReadByte(add++);	
    params2.CommandStatusToggleFlag =FLASH_ReadByte(add++);	
  }
  else
  {
    
    params2.KEY_H8= KEYFUNC_DEFAULT;
    params2.KEY_L8= KEYFUNC_DEFAULT;
    params2.inhibition= FLASH_ReadByte(add++);
    for(unsigned char i=0;i<SN_LEN;i++)
      params2.sn[i]=SN_DEFAULT;
    params2.freq= CHANNEL_DEFAULT ;
    params2.CommandStatusToggleFlag =0;		
    
  }
  return &params2;  
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
  // uint8_t           u8Data   = 0x5a;
  
  u32Addr  = 0x3ff0;
  Flash_Init(FlashInt, 0);
  
  Flash_SectorErase(u32Addr);
  
  enResult = Flash_WriteByte(u32Addr++, 0x5a);
  enResult = Flash_WriteByte(u32Addr++,  params.KEY_H8);
  enResult = Flash_WriteByte(u32Addr++,  params.KEY_L8);
  enResult = Flash_WriteByte(u32Addr++,  params.inhibition);
  for(unsigned char i=0;i<SN_LEN;i++)
  enResult = Flash_WriteByte(u32Addr++,  params.sn[i]);
  enResult = Flash_WriteByte(u32Addr++,  (uint8_t)(params.freq>>8));
  enResult = Flash_WriteByte(u32Addr++,  (uint8_t)params.freq);
  enResult = Flash_WriteByte(u32Addr++,  (uint8_t)params.CommandStatusToggleFlag>>8);   
  enResult = Flash_WriteByte(u32Addr++,  (uint8_t)params.CommandStatusToggleFlag);  
  uint8_t *pReadBuf;
  if(ee_ReadBytes(pReadBuf, 0, 1)==1)
  {   
    if(pReadBuf[0]!=0x5a)
    { 
     pReadBuf[0]= 0x5a;
      memcpy(&pReadBuf[1],&params,12);
      if (ee_WriteBytes( pReadBuf, 0, 12) == 1)
        return 0;
      else 
        return 1;
    }
  }
  
  //  return   enResult;
}





