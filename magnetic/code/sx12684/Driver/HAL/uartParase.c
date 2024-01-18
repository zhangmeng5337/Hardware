#include "uartParase.h"
#include "radio.h"
#include "lora_params.h"
#include "stdlib.h"
#include "crc.h"

extern const struct Radio_s Radio;
extern unsigned char TX_Buffer[BUFFERSIZE]; 
extern unsigned char UsartReceiveFlag,usart_i ;
 Module_Params_stru Module_Params;
unsigned char uart_read_index=0;
const unsigned char wrcommand[]={0xff,0x56,0xae,0x35,0xa9,0x55};
    unsigned char pb[64],count;
void module_process(unsigned char mode)
{    
   // unsigned char *pb,count;
	uint16_t  crc_value;
   // pb = 0;
	count = 0;		
	if(mode == NORMALCOMMAND)
	{
	     // Module_Params.ADDH=TX_Buffer[2];
	     // Module_Params.ADDM=TX_Buffer[3];
	     // Module_Params.ADDL=TX_Buffer[4];
	      if(uart_read_index!=usart_i)
		{
		       count = usart_i-uart_read_index;
		       memmove(pb,&TX_Buffer[uart_read_index],count);
		       uart_read_index = 0;
				
		  } 
		  crc_value=RadioComputeCRC(pb,count,CRC_TYPE_IBM);//????��?3?��a����?����y?Y�㨹CRC?��
		  pb[count]=crc_value>>8;
		  pb[count+1]=crc_value;
         Radio.Standby();         
         Radio.Send( pb, count+2);
		
	}
	else if(mode == WRITECOMMAND)
	{
		
	}
	else if(mode == READCOMMAND)
	{
		
	}
	
	free(pb);
}


unsigned char HeaderIdentify()
{
       unsigned char i,j,re;
	 j=0;
	while(j!=usart_i)
	{
              if(usart_i<sizeof(wrcommand))
              	{
			if(TX_Buffer[j]!=wrcommand[i++])
			{
				re = NORMALCOMMAND;//normal data
				break;
			}
              }
		else if(usart_i>=6)
		{
			if(TX_Buffer[j]==LORA_WRITE)
			{
				re = WRITECOMMAND;//write command
			}
			else if(TX_Buffer[j]==LORA_READ)
			{
				re = READCOMMAND;//read command
			}
			else 
				re = NORMALCOMMAND;
			i = 0;
			break;
		}

	      j++;
		if(j >=BUFFERSIZE)
			j = 0;
	}
	return re;
}
void uartParase()
{
  //unsigned char result;
  //result = 0;
	if(UsartReceiveFlag == 1)
	{
	module_process(HeaderIdentify());
	UsartReceiveFlag = 0;
        usart_i = 0;
	}
        
}
