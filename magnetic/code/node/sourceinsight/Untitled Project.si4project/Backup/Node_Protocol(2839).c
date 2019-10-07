#include "Node_Protocol.h"
extern UART_HandleTypeDef huart2;

DataPack_stru DataPack;
unsigned char xorCheck(unsigned char *pbuffer,unsigned char len)
{
    unsigned char result;
    unsigned char i;
	result = 0;
   for(i=0;i<len;i++)
   {
	result = result^pbuffer[i];
   }
  return result;
}
void NodeToServer(unsigned char *p)
{
	unsigned char *pbuffer,i;
	DataPack.headerH = NODE_TO_SERVERH;

	DataPack.headerL = NODE_TO_SERVERL;
		
	DataPack.id[0] = 0; 	
	DataPack.id[1] = 0; 
	DataPack.id[2] = 0; 
	DataPack.func = *p;p++;
	DataPack.len =  *p;p++;
	DataPack.checksum = 0;
	DataPack.server_channel = *p;p++;
	DataPack.tx_period = *p++;p++;
	DataPack.car_flag =  *p++;p++;
	DataPack.car_time =  *p++;p++;
	memcopy(p,&DataPack,12);
	DataPack.checksum = xorCheck(p,12);
	HAL_UART_Transmit(&huart2,p,12,10);
}
void Transmmit()
{
    unsigned char p[7],i;
    
	if(DataPack.seq_num <= 3)
	{  
	   i = 0;
	   p[i++] = DYNAMIC_MODE;
	   p[i++] = 0x04;
	   p[i++] = 0;
       p[i++] = 0;
	   p[i++] = 1;
	   p[i++] = 1;
	   p[i++] = 0;



		DataPack.seq_num  = DataPack.seq_num+ 1;
		NodeToServer(p);
		
	}
	else
	{
		
	}
}
#define NODE_TO_SERVERH		0xAB
#define NODE_TO_SERVERL			0xAB
#define SERVER_TO_NODEH		0xAA
#define SERVER_TO_NODEL		       0xAA
#define LORA_ACK    		              0x24
extern uart_stru uart1;
extern uart_stru uart2;
void index_zero()
{
	if(uart1.index>=UARTBUFFERSIZE)
		uart1.index = 0;
	//else
	//	uart1.index = 	uart1.index+1 ;		
}
unsigned char HeaderIdentify()
{
       if((uart1.read_index+uart1.read_len)>=UARTBUFFERSIZE)
	  {
		memcopy(pb,&uart1.receive_buffer[uart1.read_index],UARTBUFFERSIZE-uart1.read_index);
	  	memcopy(pb+UARTBUFFERSIZE-uart1.read_index,&uart1.receive_buffer[uart1.read_index],
			      uart1.read_len+uart1.read_index-UARTBUFFERSIZE);
	   }
	   else
	   {
		memcpy(pb,&uart1.receive_buffer[uart1.read_index],uart1.read_len);
	   }
}

unsigned char uartparase(unsigned char uartNo)
{
	if(uartNo == 1)
	{
	    if( uart1.receive_flag==1)
	    	{
				if(HeaderIdentify()==1)
				
		}
	}
}

