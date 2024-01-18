#include "node_Protocol.h"
#include "filter.h"
#include "bsp.h"
#include "usystem.h"
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern uart_stru uart1;
extern uart_stru uart2;
extern uart_stru uart3;
DataPack_stru DataPack;

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
			
				if(uart2.receive_buffer[i+5] == func&&uart2.receive_buffer[i+2]==DataPack.id[0]&&
					uart2.receive_buffer[i+3]==DataPack.id[1]&&uart2.receive_buffer[i+4]==DataPack.id[2])
				{
				    
					return uart2.receive_buffer[i+5];
				}
			}
				
	   }
	   return 0;
}
unsigned char HeaderIdentify2()
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
	   for(i=0;i<uart1.receive_len;i++)
	   	{
			if(uart1.receive_buffer[i] == NODE_TO_SERVERH && 
				uart1.receive_buffer[i+1] == NODE_TO_SERVERL)
			{
			
				if(uart1.receive_buffer[i+9]==DataPack.serverId[0]&&
					uart1.receive_buffer[i+10]==DataPack.serverId[1])
				{
				    DataPack.id[0] = uart1.receive_buffer[2];
				    DataPack.id[1] = uart1.receive_buffer[3];
				    DataPack.id[2] = uart1.receive_buffer[4];

					return uart1.receive_buffer[i+5];
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

unsigned char uartparase2(unsigned char uartNo)
{
	unsigned char res;
	res=0;
	if(uartNo == 2)
	{
	    //if( uart2.receive_flag==1)
	    	{
	    	    res = HeaderIdentify2();
				if(res)
					return res;
				else
					return 0;
				 //  res = 1;
			//	else
				//	res = 0;
		}
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
	
	memcpy(pbuffer+uTmpLen,&DataPack.serverId[0] ,1);
	uTmpLen = uTmpLen+1;
	
	memcpy(pbuffer+uTmpLen,&DataPack.serverId[0] ,2);
	uTmpLen = uTmpLen+2;
	
	memcpy(pbuffer+uTmpLen,&DataPack.func ,1);
	uTmpLen = uTmpLen+1;
	
	memcpy(pbuffer+uTmpLen,&DataPack.nodeVersion ,1);
	uTmpLen = uTmpLen+1;	 
	
	memcpy(pbuffer+uTmpLen,&DataPack.len ,1);
	uTmpLen = uTmpLen+1;
	
	memcpy(pbuffer+uTmpLen,&DataPack.checksum ,1);
	uTmpLen = uTmpLen+1;
	
	memcpy(pbuffer+uTmpLen,&DataPack.payload ,DataPack.len);
	uTmpLen = uTmpLen+DataPack.len;

	DataPack.checksum = xorCheck(pbuffer+2,uTmpLen-2);
	memcpy(pbuffer+8,&DataPack.checksum ,1);
	//uTmpLen = uTmpLen+DataPack.len;
	
	HAL_UART_Transmit(&huart1,pbuffer,uTmpLen,10);
}



void lora_test()
{
	NodeToServer();
}
void nodeParamsInit()
{
  DataPack.headerH = SERVER_TO_NODEH;
  DataPack.headerL = SERVER_TO_NODEL;
  DataPack.id[0] = 0;
  DataPack.id[1] = 0;
  DataPack.id[2] = 0;
  DataPack.serverId[0] = 0x00;
  DataPack.serverId[1] = 0x01; 
  DataPack.server_channelH=2;
  DataPack.server_channelL=2;	
  DataPack.serverAirRate=3;
  DataPack.nodeVersion = 1;

}
void led_OFF()

{
	HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_SET);
	 HAL_Delay(100);

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
			   DataPack.payload[i++] = DataPack.id[0];//channel	
			   DataPack.payload[i++] = DataPack.id[1];//channel
			   DataPack.payload[i++] = DataPack.id[2];//channel	


			   DataPack.payload[i++] = DataPack.tx_period;//tx period
			   DataPack.payload[i++] = DataPack.car_flag;//car status	
			   DataPack.payload[i++] = DataPack.car_time;//time on the node 
			   DataPack.len = i;
			   DataPack.seq_num  = DataPack.seq_num+ 1;

			   NodeToServer();					
              led_OFF()
;
				
			  // while(TimingStart(2,0,TIME_OUT,0)!=2)
				;
//			   if(uartparase(2,func)==1)
//			   	{
//					DataPack.seq_num = 0;
//					break;			  
			   //}
			  // else 
			   //	goto loop1;
				
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
				   
				   DataPack.payload[i++] = DataPack.seq_num;//tx number
				   DataPack.payload[i++] = DataPack.id[0];//channel 
				   DataPack.payload[i++] = DataPack.id[1];//channel
				   DataPack.payload[i++] = DataPack.id[2];//channel 
				   DataPack.payload[i++] = DataPack.vbat;//					
				   DataPack.payload[i++] = DataPack.serverAirRate;// 
				   DataPack.len = i;
				   DataPack.seq_num  = DataPack.seq_num+ 1;
				   NodeToServer();
				   led_OFF()
;

				}
				else
				{
					DataPack.seq_num = 0;
				}

		     break;
		case HEART_BIT:
		{	uint32_t vbat_tmp;
			vbat_tmp = 0;

			loop3:	if(DataPack.seq_num <= 3)
				{  
				   i = 0;
				   DataPack.func = func;
			
				   DataPack.checksum = 0;
				   DataPack.vbat =(unsigned char )(vbat_tmp*10);

				   DataPack.payload[i++] = DataPack.seq_num;//tx number
				   DataPack.payload[i++] = DataPack.id[0];//channel 
				   DataPack.payload[i++] = DataPack.id[1];//channel
				   DataPack.payload[i++] = DataPack.id[2];//channel 
				   DataPack.payload[i++] = DataPack.vbat;//					
				   DataPack.payload[i++] = DataPack.serverAirRate;// 

				   DataPack.len = i;
				   DataPack.seq_num  = DataPack.seq_num+ 1;
				   NodeToServer();					            
				   led_OFF()
;
					
				}
				else
				{
					DataPack.seq_num = 0;
				}
			}
		     break;
		case SET_PARAMS:
		{

			uint32_t vbat_tmp;
			vbat_tmp = 0;

			if(DataPack.seq_num <= 3)
			{  
				i = 0;
				DataPack.func = func;

				DataPack.checksum = 0;
				DataPack.vbat =(unsigned char )(vbat_tmp*10);

				DataPack.payload[i++] = DataPack.seq_num;//tx number
				DataPack.payload[i++] = DataPack.id[0];//channel 
				DataPack.payload[i++] = DataPack.id[1];//channel
				DataPack.payload[i++] = DataPack.id[2];//channel 
				DataPack.payload[i++] = DataPack.server_channelH;// 	
				DataPack.payload[i++] = DataPack.server_channelL;// 	

				DataPack.payload[i++] = DataPack.serverAirRate;// 
				DataPack.payload[i++] = DataPack.vbat;// 

				DataPack.len = i;
				DataPack.seq_num  = DataPack.seq_num+ 1;
				NodeToServer();					             
				led_OFF()
;
			}
			else
			{
				DataPack.seq_num = 0;
			}
			
		}break;
			case SET_PERIOD:
			{
			
			
						if(DataPack.seq_num <= 3)
						{  
							i = 0;
							DataPack.func = func;
			
							DataPack.checksum = 0;
							//DataPack.vbat =(unsigned char )(vbat_tmp*10);
			
							DataPack.payload[i++] = DataPack.seq_num;//tx number
							DataPack.payload[i++] = DataPack.id[0];//channel 
							DataPack.payload[i++] = DataPack.id[1];//channel
							DataPack.payload[i++] = DataPack.id[2];//channel 
							DataPack.payload[i++] = DataPack.server_channelH;// 	
							DataPack.payload[i++] = DataPack.server_channelL;// 	
			
							DataPack.payload[i++] = DataPack.serverAirRate;// 
							DataPack.payload[i++] = DataPack.tx_period;// 
							//DataPack.payload[i++] =0xff;// 			    
							DataPack.len = i;
							DataPack.seq_num  = DataPack.seq_num+ 1;
							NodeToServer();				
							led_OFF()
;

						}
						else
						{
							DataPack.seq_num = 0;
						}

			}break;

			case SET_THRES:
			{

			
						if(DataPack.seq_num <= 3)
						{  
							i = 0;
							DataPack.func = func;
			
							DataPack.checksum = 0;
						//	DataPack.vbat =(unsigned char )(vbat_tmp*10);
			
							DataPack.payload[i++] = DataPack.seq_num;//tx number
							DataPack.payload[i++] = DataPack.id[0];//channel 
							DataPack.payload[i++] = DataPack.id[1];//channel
							DataPack.payload[i++] = DataPack.id[2];//channel 
							DataPack.payload[i++] = DataPack.server_channelH;// 	
							DataPack.payload[i++] = DataPack.server_channelL;//
							
							DataPack.payload[i++] = DataPack.minThres[0];// 			
							DataPack.payload[i++] = DataPack.minThres[1];// 
							DataPack.payload[i++] = DataPack.minThres[2];// 
							DataPack.payload[i++] = DataPack.minThres[3];//
							
							DataPack.payload[i++] = DataPack.maxThres[0];// 
							DataPack.payload[i++] = DataPack.maxThres[1];// 
							DataPack.payload[i++] = DataPack.maxThres[2];// 
							DataPack.payload[i++] = DataPack.maxThres[3];// 

			
							DataPack.len = i;
							DataPack.seq_num  = DataPack.seq_num+ 1;
							NodeToServer();					            
							led_OFF()
;
			
						}
						else
						{
							DataPack.seq_num = 0;
						}

			}break;
	}
    

}


void uart_process()
{
	
	
	unsigned char i=0;
	if(uart3.receive_flag == 1)
	{
		uart3.receive_flag = 0;
		HAL_UART_Transmit(&huart1,uart3.receive_buffer ,uart3.read_len ,10);
		uart3.read_len = 0;
	}
	if(uart2.receive_flag == 1)
	{
		uart2.receive_flag = 0;
		HAL_UART_Transmit(&huart1,uart2.receive_buffer ,uart1.read_len ,10);
		uart1.read_len = 0;
	}	
	if(uart1.receive_flag == 1)
	{
		unsigned char ret;
HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_RESET);
		ret = uartparase2(2);
		HAL_UART_Transmit(&huart2,uart1.receive_buffer ,uart1.receive_len ,10);
		memset(uart1.receive_buffer,0,256);
		uart1.receive_flag = 0;
		if(ret)
		{
			switch(ret)
			{
				case STATIC_MODE:break;
				case DYNAMIC_MODE:
				{
					Transmmit(DYNAMIC_MODE);
				}break;
				case REGISTER_CODE:

					Transmmit(REGISTER_CODE);break;
				case HEART_BIT:
				{	
					Transmmit(HEART_BIT);
				}break;
			}
			uart1.read_len = 0;
		}
	}
}

void test()
{

}


