#include "uartParase.h"
#include "radio.h"
#include "lora_params.h"
#include "stdlib.h"

extern const struct Radio_s Radio;
extern unsigned char TX_Buffer[BUFFERSIZE]; 
extern unsigned char UsartReceiveFlag,usart_i ;
 Module_Params_stru Module_Params;
unsigned char uart_read_index=0;
const unsigned char wrcommand[]={0xff,0x56,0xae,0x35,0xa9,0x55};

void module_process(unsigned char mode)
{    
    unsigned char *pb,count;
    pb = 0;
	count = 0;		
	if(mode == NORMALCOMMAND)
	{
	      pb[0] = Module_Params.ADDH;
	      pb[1] = Module_Params.ADDL;	  
	      if(uart_read_index!=usart_i)
		{
		       count = usart_i-uart_read_index;
		       memmove(pb+2,&TX_Buffer[uart_read_index],count);
		       uart_read_index = 0;
				
		  } 	
         Radio.Send( pb, count);
		
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
	 //j=uart_read_index;
	while(j!=usart_i)
	{
              if(i<sizeof(wrcommand))
              	{
			if(TX_Buffer[j]!=wrcommand[i++])
			{
				re = NORMALCOMMAND;//normal data
				break;
			}
              }
		else if(i>=6)
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
	}
        
}
