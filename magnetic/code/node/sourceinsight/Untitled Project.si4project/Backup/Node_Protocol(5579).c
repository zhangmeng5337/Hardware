#include "Node_Protocol.h"
#include "filter.h"
#include "bsp.h"
#include "usystem.h"
extern UART_HandleTypeDef huart2;
extern uart_stru uart1;
extern uart_stru uart2;
void index_zero()
{
	if(uart1.index>=UARTBUFFERSIZE)
		uart1.index = 0;
	//else
	//	uart1.index = 	uart1.index+1 ;		
}
unsigned char HeaderIdentify(unsigned char func)
{



       /*if((uart2.read_index+uart2.read_len)>=UARTBUFFERSIZE)
	  {
		memcopy(pb,&uart2.receive_buffer[uart2.read_index],UARTBUFFERSIZE-uart2.read_index);
	  	memcopy(pb+UARTBUFFERSIZE-uart2.read_index,&uart2.receive_buffer[uart2.read_index],
			      uart1.read_len+uart2.read_index-UARTBUFFERSIZE);
	   }
	   else
	   {
		memcpy(pb,&uart1.receive_buffer[uart1.read_index],uart1.read_len);
	   }*/
	   unsigned char i;
	   for(i=0;i<uart2.receive_len;i++)
	   	{
			if(uart2.receive_buffer[i] == NODE_TO_SERVERH && 
				uart2.receive_buffer[i+1] == NODE_TO_SERVERL)
			{
				if(uart2.receive_buffer[i+5] == func)
				{
					return 1;
				}
			}
				
	   }
	   return 0;
}

unsigned char uartparase(unsigned char uartNo,unsigned char func)
{
	unsigned char res;
	if(uartNo == 2)
	{
	    if( uart2.receive_flag==1)
	    	{
				if(HeaderIdentify(func)==1)
				   res = 1;
				else
					res = 0;
		}
	}
	return res;
}

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

	unsigned char pbuffer[12],uTmpLen;
	/*DataPack.headerH = NODE_TO_SERVERH;

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
	DataPack.car_time =  *p++;p++;*/
	uTmpLen = 0;
	memset(pbuffer, 0, sizeof(pbuffer));

	memcpy(pbuffer,&DataPack.headerH ,1);
	uTmpLen = uTmpLen+1;

	memcpy(pbuffer+uTmpLen,&DataPack.headerL ,1);
	uTmpLen = uTmpLen+1;
	
	memcpy(pbuffer+uTmpLen,&DataPack.id[0] ,3);
	uTmpLen = uTmpLen+3;
	
	memcpy(pbuffer+uTmpLen,&DataPack.func ,1);
	uTmpLen = uTmpLen+1;
	
	memcpy(pbuffer+uTmpLen,&DataPack.len ,1);
	uTmpLen = uTmpLen+1;
	
	memcpy(pbuffer+uTmpLen,&DataPack.checksum ,1);
	uTmpLen = uTmpLen+1;
	
	memcpy(pbuffer+uTmpLen,&DataPack.payload ,DataPack.len);
	uTmpLen = uTmpLen+DataPack.len;

	DataPack.checksum = xorCheck(pbuffer+2,uTmpLen-2);
	memcpy(pbuffer+7,&DataPack.checksum ,1);
	//uTmpLen = uTmpLen+DataPack.len;
	
	HAL_UART_Transmit(&huart2,pbuffer,uTmpLen,10);
}




void nodeParamsInit()
{
  DataPack.headerH = NODE_TO_SERVERH;
  DataPack.headerL = NODE_TO_SERVERL;
  DataPack.id[0] = 0;
  DataPack.id[1] = 0;
  DataPack.id[2] = 0;
  DataPack.serverId[0] = 0;
  DataPack.serverId[1] = 1; 
  DataPack.server_channel=2;
  DataPack.serverAirRate=3;

}
void Transmmit(unsigned char func)
{
    unsigned char i;
	switch(func)
	{
		case STATIC_MODE:break;
		case DYNAMIC_MODE:
		loop1:	if(DataPack.seq_num <= 3)
			{  
			   i = 0;
			   //DataPack.headerH = NODE_TO_SERVERH;
			   //DataPack.headerL = NODE_TO_SERVERL;
			   //DataPack.id[0] = 
			   DataPack.func = func;

			   DataPack.checksum = 0;
			   DataPack.payload[i++] = DataPack.seq_num;//tx number
			   DataPack.payload[i++] = DataPack.server_channel;//channel	
			   DataPack.payload[i++] = DataPack.tx_period;//tx period
			   DataPack.payload[i++] = DataPack.car_flag;//car status	
			   DataPack.payload[i++] = DataPack.car_time;//time on the node 
			   DataPack.len = i;
			   DataPack.seq_num  = DataPack.seq_num+ 1;
			   NodeToServer();
			   while(TimingStart(2,0,TIME_OUT,0)!=2)
			   	{
				   if(uartparase(2,func)==1)
					{
						DataPack.seq_num = 0;
						break;			  
				   }		

			   }
			   if(DataPack.seq_num == 0)
			   	goto loop1;

				
			}
			else
			{
				DataPack.seq_num = 0;
			}

		     break;

		case REGISTER_CODE:

			loop2:	if(DataPack.seq_num <= 3)
				{  
				   i = 0;
				   DataPack.func = func;
			
				   DataPack.checksum = 0;

				   DataPack.payload[i++] = DataPack.vbat;//
				   DataPack.payload[i++] = DataPack.serverId[0];//	
				   DataPack.payload[i++] = DataPack.serverId[1];//
				   DataPack.payload[i++] = DataPack.server_channel;//
				   DataPack.payload[i++] = DataPack.serverAirRate;// 
				   DataPack.len = i;
				   DataPack.seq_num  = DataPack.seq_num+ 1;
				   NodeToServer();
				   while(TimingStart(2,0,TIME_OUT,0)!=2)
					{
					   if(uartparase(2,func)==1)
						{
							DataPack.seq_num = 0;
							break;			  
					   }
							   
				   }
				   if(DataPack.seq_num == 0)
					goto loop1;

					
				}
				else
				{
					DataPack.seq_num = 0;
				}

		     break;
		case HEART_BIT:
		{	uint32_t vbat_tmp;
			vbat_tmp = Get_Battery_Gas_adc(50);

			loop3:	if(DataPack.seq_num <= 3)
				{  
				   i = 0;
				   DataPack.func = func;
			
				   DataPack.checksum = 0;
				   DataPack.vbat =(unsigned char )(vbat_tmp*10);

				   DataPack.payload[i++] = DataPack.vbat;// 
				   DataPack.len = i;
				   DataPack.seq_num  = DataPack.seq_num+ 1;
				   NodeToServer();
				   while(TimingStart(2,0,TIME_OUT,0)!=2)
					{
					   if(uartparase(2,func)==1)
						{
							DataPack.seq_num = 0;
							break;			  
					   }
								   
				   }
				   if(DataPack.seq_num == 0)
					goto loop1;

					
				}
				else
				{
					DataPack.seq_num = 0;
				}
			}
		     break;
	}
    

}






