#include "uartParse.h"
#include "ddl.h"
#include "eeprom.h"
#include "bsp_uart.h"
#include "command.h"
#include "crc.h"
params_typedef params;
extern unsigned char UsartReceiveData[BUFFERSIZE];
unsigned char uartReceiveLen;
extern unsigned char uartReceivewriteIndex;
unsigned char uartReadIndex;

params_typedef *system_params_get()
{
  //params_typedef *params1;
  //params1 = &params;
  
  return &params;
}

boolean_t anlyz_uart_data(CommandInfo_typedef *CommandInfo)
{
  
  params.KEY_H8 = CommandInfo->funcode[KEY_H8_INDEX];
  params.KEY_L8 = CommandInfo->funcode[KEY_L8_INDEX];	
  params.inhibition = CommandInfo->funcode[KEY_INCH_INDEX];
  params.freq = CommandInfo->funcode[FREQ_INDEX]>>8+
    CommandInfo->funcode[FREQ_INDEX+1];
  memcpy(&params.sn,CommandInfo->funcode+SN_INDEX,SN_LEN);
  params.CommandStatusToggleFlag = 0;
  return(eeprom_write( params)) ;
  
}

void uartPrase()
{
  CommandInfo_typedef CommandInfo;
  unsigned char i;
	unsigned char *p;
  //tmpuartReceiveriteIndex = uartReceiveLen;
  if(uartReadIndex!=uartReceivewriteIndex&&((uartReceiveLen%COMMANDINFO_SIZE)==0||
                                            uartReceiveLen>=BUFFERSIZE))
  {   
    for(i=0;i<BUFFERSIZE;i++)
    {
      if(UsartReceiveData[i]!=WAKE_HEAD)
        break;
    }

    memcpy(&CommandInfo,UsartReceiveData+i-1,COMMANDINFO_SIZE);
    memcpy(p,UsartReceiveData+i-1,COMMANDINFO_SIZE);		
    unsigned int crc=CRC16_Get8(p,COMMANDINFO_SIZE);
    if(crc!=CommandInfo.crc)
    {
      uartReadIndex = uartReadIndex +COMMANDINFO_SIZE;
      if(uartReadIndex>=BUFFERSIZE)
        uartReadIndex = 0;
      
    }
    else //У����ȷ������ָ��
    {
      if(CommandInfo.funcode[FUNC_INDEX]==SETTING_CMD&&
         CommandInfo.funcode[FUNC_INDEX+1]==0x0d)//���ò���
        anlyz_uart_data(&CommandInfo);
      else if(CommandInfo.funcode[FUNC_INDEX]==WAKEUP_CMD&&
              CommandInfo.funcode[FUNC_INDEX+1]==0x0d)//���ڲ���
      {
        uart_ack_response(WAKEUP_CMD);
      }
      else if(CommandInfo.funcode[FUNC_INDEX]==FORMAT_CMD&&
              CommandInfo.funcode[FUNC_INDEX+1]==0x0d)//��ʽ������
      {
        uart_ack_response(FORMAT_CMD);
      }
      else if(CommandInfo.funcode[FUNC_INDEX]==READ_CMD&&
              CommandInfo.funcode[FUNC_INDEX+1]==0x0d)//��ʽ������
      {
        uart_ack_response(READ_CMD);
      } 
    }
    uartReceiveLen = 0;
  }
}
