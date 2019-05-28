#include "uartParse.h"
#include "ddl.h"
#include "eeprom.h"
#include "bsp_uart.h"
params_typedef params;
extern unsigned char UsartReceiveData[BUFFERSIZE];
unsigned char uartReceiveLen;
extern unsigned char uartReceiveriteIndex;
unsigned char uartReadIndex;

params_typedef system_params_get()
{
  return params;
}

boolean_t anlyz_uart_data(CommandInfo_typedef *CommandInfo)
{
  
  params.KEY_H8 = CommandInfo->funcode[KEY_UD_EW_SN_RS_INDEX];
  params.KEY_L8 = CommandInfo->funcode[KEY_O1_O8_INDEX];	
  params.inhibition = CommandInfo->funcode[KEY_INCH_INDEX];
  params.freq = CommandInfo->funcode[FREQ_INDEX];
  memcopy(&params.sn,CommandInfo->funcode+FREQ_INDEX,SN_LEN);
  return(eeprom_write( params)) ;
  
  
  
}

unsigned char uartPrase()
{
  CommandInfo_typedef CommandInfo;
  unsigned char TmpuartReceiveriteIndex,i;
  TmpuartReceiveriteIndex = uartReceiveLen;
  if(uartReadIndex!=uartReceiveriteIndex&&((uartReceiveLen%COMMANDINFO_SIZE)==0||
                                           uartReceiveLen>=BUFFERSIZE))
  {   
    for(i=0;i<BUFFERSIZE;i++)
    {
      if(UsartReceiveData[i]!=PAYLOAD_HEAD)
        break;
    }
    memcopy(&CommandInfo,UsartReceiveData+i-1,COMMANDINFO_SIZE);
    unsigned char crc=crc_cal(&CommandInfo,COMMANDINFO_SIZE);
    if(crc!=CommandInfo.crc)
    {
      uartReadIndex = uartReadIndex +COMMANDINFO_SIZE;
      if(uartReadIndex>=BUFFERSIZE)
        uartReadIndex = 0;
      
    }
    else 
    {
      unsigned char res= anlyz_uart_data(&CommandInfo);
      switch(res)
      {
      //case :break;	
      }
    }
    uartReceiveLen = 0;
  }
}
