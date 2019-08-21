#include "Node_Protocol.h"
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
void NodeToServer()
{
	unsigned char *pbuffer,i;
	*pbuffer++ = NODE_TO_SERVER;
	DataPack.header = NODE_TO_SERVER;
	DataPack.id[0] = 0; 	
	DataPack.id[1] = 0; 
	DataPack.id[2] = 0; 
	DataPack.func = DYNAMIC_MODE;
	i = 0;
	DataPack.p[i++] = 0;
	DataPack.checksum = 0;
	i = i + 6;
	memcpy(pbuff,DataPack,i);
	DataPack.checksum = xorCheck(pbuff,i);
	
}
void Transmmit()
{

	if(DataPack.seq <= 3)
	{
		DataPack.seq  = DataPack.seq  + 1;
		NodeToServer();
		LoraTx();
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
	if(uart1.index>=BUFFERSIZE)
		uart1.index = 0;
	//else
	//	uart1.index = 	uart1.index+1 ;		
}
unsigned char HeaderIdentify()
{
       if((uart1.read_index+uart1.read_len)>=BUFFERSIZE)
	  {
		memcpy(pb,&uart1.receive_buffer[uart1.read_index],BUFFERSIZE-uart1.read_index);
	  	memcpy(pb+BUFFERSIZE-uart1.read_index,&uart1.receive_buffer[uart1.read_index],
			      uart1.read_len+uart1.read_index-BUFFERSIZE);
	   }
	   else
	   {
		memcpy(pb,&uart1.receive_buffer[uart1.read_index],uart1.read_len);
	   }
}

unsigned char uartparase(unsigned char uartNo)
{
       unsigned int 
	if(uartNo == 1)
	{
	    if( uart1.receive_flag==1)
	    	{
				if(HeaderIdentify()==1)
				
		}
	}
}

