#include "uartParase.h"
#include "sx1276.h"
#include "RF.h"
#include "stdlib.h"

extern unsigned char UsartReceiveData[BUFFERSIZE] , UsartReceiveFlag,usart_i ;

unsigned char uart_read_index=0;
const unsigned char wrcommand[]={0xff,0x56,0xae,0x35,0xa9,0x55};
void module_process(unsigned char mode)
{    unsigned char *pb,count;
        pb = 0;
	count = 0;		
	if(mode == NORMALCOMMAND)
	{
	      pb[0] = GetModuleParams()->ADDH;
	      pb[1] = GetModuleParams()->ADDL;	  
	      if(uart_read_index<usart_i)
		{
		       count = usart_i-uart_read_index;
			memcpy(pb+2,&UsartReceiveData[uart_read_index],count);
		        uart_read_index = usart_i;
				
		  } 	
		else
		{     
		       count = BUFFERSIZE-uart_read_index;
			memcpy(pb+2,&UsartReceiveData[uart_read_index],count);
			memcpy(pb+2+BUFFERSIZE-uart_read_index,UsartReceiveData,usart_i);
			count = usart_i+count;
		        uart_read_index = usart_i;
		  } 
		SX1276_SendPacket(pb, count);
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
	 j=uart_read_index;
	while(j!=usart_i)
	{
              if(i<sizeof(wrcommand))
              	{
			if(UsartReceiveData[j]!=wrcommand[i++])
			{
				re = NORMALCOMMAND;//normal data
				break;
			}
              }
		else if(i>=6)
		{
			if(UsartReceiveData[j]==LORA_WRITE)
			{
				re = WRITECOMMAND;//write command
			}
			else if(UsartReceiveData[j]==LORA_READ)
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
unsigned char uartParase()
{
  unsigned char result;
  result = 0;
	if(UsartReceiveFlag == 1)
	{
		switch(HeaderIdentify())
		{
			case NORMALCOMMAND:
				//UsartReceiveFlag=0;
				//uart_read_index = usart_i;
				result=NORMA_OPERATION;
				break;
			case WRITECOMMAND:
				//UsartReceiveFlag=0;
			     result=LORA_WRITE;
				break;
			case READCOMMAND:
				//UsartReceiveFlag=0;
				result=LORA_READ;
				break;				
		}
	}
        return result;
}
