#include "node_protocol.h"
#include "filter.h"
#include "bsp.h"
#include "usystem.h"



extern UART_HandleTypeDef huart2;
extern uart_stru uart1;
extern uart_stru uart2;
volatile DataPack_stru DataPack;
extern float MAX_THRES;
extern float  MIN_THRES;
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
			if(uart2.receive_buffer[i] == SERVER_TO_NODEH && 
				uart2.receive_buffer[i+1] == SERVER_TO_NODEH)
			{
			
				if(uart2.receive_buffer[i+3]==DataPack.serverId[0]&&
					uart2.receive_buffer[i+4]==DataPack.serverId[1])
				{
				   if((uart2.receive_buffer[i+9]==DataPack.id[0]&&uart2.receive_buffer[i+10]==DataPack.id[1]&&
				   	uart2.receive_buffer[i+11]==DataPack.id[2])||
					 (uart2.receive_buffer[i+10]==DataPack.id[0]&&uart2.receive_buffer[i+11]==DataPack.id[1]&&
				   	uart2.receive_buffer[i+12]==DataPack.id[2])
					 )
				   	{
					   if(uart2.receive_buffer[i+5] == func)
						return 1;
					   else
						return uart2.receive_buffer[i+5];

				   }

				}
			}
				
	   }
	   return 0;
}

unsigned char uartparase(unsigned char uartNo,unsigned char func)
{
	unsigned char res;
	res = 0;
	if(uartNo == 2)
	{
	    if( uart2.receive_flag==1)
	    	{   uart2.receive_flag = 0;
	    	   res = HeaderIdentify(func);
					 
				if(res)
				   return res;
				else
					return  0;
		}
		uart2.receive_flag = 0;
	}
	return res;
}


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
void NodeToLORA()
{
	unsigned char wrcommand[6]={0xff,0x56,0xae,0x35,0xa9,0x55};
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

	memcpy(pbuffer,wrcommand ,6);
	uTmpLen = uTmpLen+6;
	memcpy(pbuffer,DataPack.server_channelH,1);
	uTmpLen = uTmpLen+1;
	memcpy(pbuffer,DataPack.server_channelL,1);
	uTmpLen = uTmpLen+1;
	memcpy(pbuffer,DataPack.serverAirRate,1);
	uTmpLen = uTmpLen+1;
	memcpy(pbuffer,DataPack.id,3);
	uTmpLen = uTmpLen+3;


	if(DataPack.register_status == 1||DataPack.func==REGISTER_CODE)
	HAL_UART_Transmit(&huart2,pbuffer,uTmpLen,10);
	//HAL_UART_Transmit(&huart2,pbuffer,uTmpLen,10);

}

void NodeToServer()
{

	unsigned char pbuffer[32],uTmpLen;
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
	
	//memcpy(pbuffer+uTmpLen,&DataPack.serverId[0],2);
//	uTmpLen = uTmpLen+DataPack.len+2;
	
	memcpy(pbuffer+uTmpLen,&DataPack.payload ,DataPack.len);
	uTmpLen = uTmpLen+DataPack.len;

	DataPack.checksum = xorCheck(pbuffer+2,uTmpLen-2);
	memcpy(pbuffer+7,&DataPack.checksum ,1);
	//uTmpLen = uTmpLen+DataPack.len;
	if(DataPack.register_status == 1||DataPack.func==REGISTER_CODE)
	HAL_UART_Transmit(&huart2,pbuffer,uTmpLen,10);
}




void nodeParamsInit()
{
  DataPack.headerH = NODE_TO_SERVERH;
  DataPack.headerL = NODE_TO_SERVERL;
  DataPack.id[0] = 0x01;
  DataPack.id[1] = 0x04;
  DataPack.id[2] = 0x03;
  DataPack.serverId[0] = 0;
  DataPack.serverId[1] = 1; 
  DataPack.server_channelH=2;
  DataPack.server_channelL=2;
  DataPack.serverAirRate=3;
  DataPack.register_status = 0;

}
unsigned char time_out = 0;
void Transmmit(unsigned char func)
{
    unsigned char i;
	switch(func)
	{
		case STATIC_MODE:break;
		case DYNAMIC_MODE:
	 	   if(DataPack.seq_num <= REAPTE_COUNT)
			{  
					i = 0;
					//DataPack.headerH = NODE_TO_SERVERH;
					//DataPack.headerL = NODE_TO_SERVERL;
					//DataPack.id[0] = 
					DataPack.func = func;
					DataPack.checksum = 0;
					DataPack.payload[i++] = DataPack.seq_num;//tx number
					DataPack.payload[i++] = DataPack.serverId[0];//channel	
					DataPack.payload[i++] = DataPack.serverId[1];//channel
					DataPack.payload[i++] = DataPack.server_channelH;//channel	
					DataPack.payload[i++] = DataPack.server_channelL;//channel
					DataPack.payload[i++] = DataPack.tx_period;//tx period
					DataPack.payload[i++] = DataPack.car_flag;//car status	
					DataPack.payload[i++] = DataPack.car_time;//time on the node 
					DataPack.len = i;
					//DataPack.seq_num  = DataPack.seq_num+ 1;	
					NodeToServer();			 
				}break;
		case REGISTER_CODE:
			if(DataPack.seq_num < REAPTE_COUNT)
				{  
						i = 0;
						DataPack.func = func;

						DataPack.checksum = 0;
						DataPack.payload[i++] = DataPack.seq_num;//tx number
						DataPack.payload[i++] = DataPack.serverId[0];//	
						DataPack.payload[i++] = DataPack.serverId[1];//
						DataPack.payload[i++] = DataPack.server_channelH;//channel	
						DataPack.payload[i++] = DataPack.server_channelL;//channel
						DataPack.payload[i++] = DataPack.vbat;//
						DataPack.payload[i++] = DataPack.serverAirRate;// 
						DataPack.len = i;
						//DataPack.seq_num  = DataPack.seq_num+ 1;
						NodeToServer();
					}	break;

		case HEART_BIT:
		{	

			uint32_t vbat_tmp;
			
			vbat_tmp = Get_Battery_Gas_adc(50);

			  if(DataPack.seq_num <= REAPTE_COUNT)
				{  
						i = 0;
						DataPack.func = func;

						DataPack.checksum = 0;
						DataPack.vbat =(unsigned char )(vbat_tmp*10);

						DataPack.payload[i++] = DataPack.seq_num;//tx number
						DataPack.payload[i++] = DataPack.serverId[0];//  
						DataPack.payload[i++] = DataPack.serverId[1];//
						DataPack.payload[i++] = DataPack.server_channelH;//channel   
						DataPack.payload[i++] = DataPack.server_channelL;//channel
						DataPack.payload[i++] = DataPack.vbat;// 
						DataPack.payload[i++] = DataPack.serverAirRate;// 
						DataPack.len = i;
						DataPack.seq_num  = DataPack.seq_num+ 1;
						NodeToServer();
					}
		     break;
	  }
 }
}

void ReceiverAnalysis()
{


	switch(uartparase(2,1))
	{
		case STATIC_MODE:
		{

		}break;
		case DYNAMIC_MODE:
		{

		}break;
		case REGISTER_CODE:
		{

		}break;
		case HEART_BIT:
		{

		}break;
		case SET_PARAMS:
		{

            if(uart2.receive_buffer[7]>=6)
            {
				//DataPack.headerH = NODE_TO_SERVERH;
				//DataPack.headerL = NODE_TO_SERVERL;
				DataPack.id[0] = uart2.receive_buffer[10];;
				DataPack.id[1] = uart2.receive_buffer[11];;
				DataPack.id[2] = uart2.receive_buffer[12];;
				DataPack.serverId[0] = uart2.receive_buffer[3];
				DataPack.serverId[1] = uart2.receive_buffer[4]; 
				DataPack.server_channelH=uart2.receive_buffer[13];
				DataPack.server_channelL=uart2.receive_buffer[14];
				DataPack.serverAirRate=uart2.receive_buffer[15];
				HAL_GPIO_WritePin(LORA_A_GPIO_Port, LORA_A_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LORA_B_GPIO_Port, LORA_B_Pin, GPIO_PIN_SET);
				NodeToLORA();
                
				//HAL_NVIC_SystemReset();
			}
		}break;
			case SET_PERIOD:
			{
			
				if(uart2.receive_buffer[7]>=6)
				{
					//DataPack.headerH = NODE_TO_SERVERH;
					//DataPack.headerL = NODE_TO_SERVERL;
					//DataPack.id[0] = uart2.receive_buffer[2];;
					//DataPack.id[1] = uart2.receive_buffer[3];;
					//DataPack.id[2] = uart2.receive_buffer[4];;
					DataPack.tx_period = uart2.receive_buffer[16];
				//	HAL_NVIC_SystemReset();
				}
			}break;

			case SET_THRES:
			{
			
				if(uart2.receive_buffer[7]>=6)
				{
					 
					 MIN_THRES=uart2.receive_buffer[15]<<24+uart2.receive_buffer[16]<<16+
					 	        uart2.receive_buffer[17]<<8+uart2.receive_buffer[18];
					 MAX_THRES=uart2.receive_buffer[19]<<24+uart2.receive_buffer[20]<<16+
					 	        uart2.receive_buffer[21]<<8+uart2.receive_buffer[22];

					//HAL_NVIC_SystemReset();
				}
			}break;

	}		

}



