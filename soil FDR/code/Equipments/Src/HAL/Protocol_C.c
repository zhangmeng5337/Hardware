#include "Protocol_C.h"
#include "crc.h"
#include "modbus.h"
#include "adc.h"
#include "rtc.h"
#include "register.h"
#include "loraHW.h"

/******************************************************************************
数据结构：
        地址    设备地址编码          功能码     起始地址      字节计数      数组0定义         数组0数值 ...crc1       crc2
bytes   16        1              1       2        1          2             2           2      2
*******************************************************************************/
unsigned char array_comp(unsigned char *p1,unsigned char *p2,unsigned char len)
{
	unsigned char i;
	unsigned char result;
	result = 0;
	for(i=0;i<len;i++)
	{
		if(p1[i]!=p2[i])
		{
			result = 1;
			break;
		}
		else
			result = 0;
	}
	return result;
}
Proctocol_C_stru protocolC;
unsigned char dev_code;
extern unsigned char modbus_role;
unsigned char getPackLen(unsigned char  *p)
{
	unsigned int len;
	if(p[17] !=0x17)
	{
		len = p[PAYLOAD_LEN_INDEX];//payload长度
		len = len + 2*ADDR_SIZE+1+1;//
		len = len+START_ADDR_SIZE + 1+4;
	
	}
	else
	{
		len =((u16)(p[PAYLOAD_LEN_INDEX]))<<8+p[PAYLOAD_LEN_INDEX+1];//payload长度
		len = len + 2*ADDR_SIZE+1+1;//
		len = len+START_ADDR_SIZE + 1+1+4;			
	}
	return len;

}
void packAnaly(unsigned char  *p)
{
   unsigned int payload_len;
	protocolC.devCode = p[2*ADDR_SIZE];
	protocolC.funCode = p[2*ADDR_SIZE+1];
	protocolC.startAddr[0]=p[2*ADDR_SIZE+1+1];
	protocolC.startAddr[1]=p[2*ADDR_SIZE+1+1+1];
	if(p[2*ADDR_SIZE+1] !=0x17)
	{
	   
		protocolC.payloadLenL = p[PAYLOAD_LEN_INDEX];
		protocolC.payloadLenH = 0;
		payload_len =((u16)(p[PAYLOAD_LEN_INDEX]))<<8+p[PAYLOAD_LEN_INDEX+1] ;//payload长度
		memcpy(protocolC.payload,&p[ADDR_SIZE+ADDR_SIZE+1+1+2+1],payload_len);	
	}
	else
	{
		protocolC.payloadLenH = p[PAYLOAD_LEN_INDEX];
		protocolC.payloadLenL = p[PAYLOAD_LEN_INDEX+1]; 
		payload_len =((u16)(p[PAYLOAD_LEN_INDEX]))<<8+p[PAYLOAD_LEN_INDEX+1] ;//payload长度	
	}

}
unsigned char protocolCAnaly(unsigned char  *p)
{
    unsigned char calCRC;
	unsigned char recCRC;
	unsigned int len;
	unsigned int payload_len;
	unsigned char result;
	result = 0;
	memcpy(protocolC.DesAddr ,p,ADDR_SIZE);//目标地址
	//memcpy(protocolC.SrcAddr,&(p[ADDR_SIZE]),ADDR_SIZE);//源地址

	if(array_comp(protocolC.DesAddr,protocolC.SrcAddr,ADDR_SIZE) == 0)//判断地址是否一致
	{
		getPackLen(p);

	
		calCRC=CRC_Compute(p,len-2);//计算所接收数据的CRC
		recCRC=p[len-1]|(((u16)p[len-2])<<8);//接收到的CRC(低字节在前，高字节在后)
		if(calCRC==recCRC)//CRC校验正确
		{
		
			calCRC=CRC_Compute(&p[ADDR_SIZE],len-4-ADDR_SIZE-ADDR_SIZE);//计算所接收数据的CRC
			recCRC=p[len-3]|(((u16)p[len-4])<<8);//接收到的CRC(低字节在前，高字节在后)
			if(calCRC==recCRC)
			{
		
				packAnaly(p);

				result = 0;
			}
			else
			{
				result = 1;
			}
		}
		else
		{
			calCRC=CRC_Compute(&p[ADDR_SIZE+ADDR_SIZE],len-4-ADDR_SIZE-ADDR_SIZE);//计算所接收数据的CRC
			recCRC=p[len-3]|(((u16)p[len-4])<<8);//接收到的CRC(低字节在前，高字节在后)
			if(calCRC==recCRC)
			{

			  packAnaly(p);

				result = 2;
			}
			else
			{
				result = 3;
			}
		
		}

	}
	else
		result = 4;

	return result;
}
unsigned char *getSensorData(unsigned char paramsType)
{
	
	unsigned char sensor_params;
    modbus_pack_stru p;
	unsigned char timeout;
	unsigned char i;
	unsigned char *result,*tmp;
	sensor_params = paramsType;
	if(paramsType == rh||sensor_params == temperature)
	{	
	   p.RS485_Addr = 1;
	   while(p.RS485_Addr<=4)
	   	{
	   
		   p.RS485_Addr ++;
   satrt:  p.func = 0x03;
		   p.regnum = 0x0002;
		   p.startaddr = 0x0002;
		   modbus_role = 1;
		   Modbus_Pack(p);
		   timeout = HAL_GetTick();
		   while(modbus_role!=2)
		   	{
				if((HAL_GetTick()-timeout)>=3000)
				{
					if(i<3)
					{
						i++;
						goto satrt;
					}
					else
					{
						i=0;
						result[p.RS485_Addr] = 0;
						result[p.RS485_Addr+1] = 0;	
						result[p.RS485_Addr+2] = 0;
						result[p.RS485_Addr+3] = 0;
						break;
					}
				}
					
		   }
		   tmp=modbusRxResult();
		   result[p.RS485_Addr] =   tmp[5];
		   result[p.RS485_Addr+1] = tmp[6];
		   result[p.RS485_Addr+2] = tmp[3];
		   result[p.RS485_Addr+3] = tmp[4];


		  // memcpy(p,)
		   

	   }

		modbus_role = 0;
	}
	else if(sensor_params== quantity)
	{
		result = getadc(quantity);
	}
	else if(sensor_params >= year_month&&sensor_params <= min_sec)
	{
		result = getRTC();
	}

	else if(sensor_params == auto_report_time)
	{
		result = getRegisterProc(auto_report_time);
	}

	return result;
}

unsigned char read_equipment()
{
	//enum FUNC_CODE fun_code;
	unsigned int reg_addr,start_addr;
	unsigned char sensor_params;
	unsigned char i,index,j;
	unsigned char *p;
	unsigned char len;
    index = 0;
	dev_code = protocolC.devCode;
	start_addr =((u16)(protocolC.startAddr[0]))<<8+protocolC.startAddr[1];
    len =  ( (u16)(protocolC.payloadLenH)) <<8+ protocolC.payloadLenL;   
			for(i=0;i<len;i++)
			{
			      if(start_addr == reg_soil_temp_5||start_addr == reg_soil_temp_4||start_addr == reg_soil_temp_3||
				  	start_addr == reg_soil_temp_2||start_addr == reg_soil_temp_1)
				    sensor_params = temperature; 
				  else if(start_addr == reg_soil_rh_5||start_addr == reg_soil_rh_4||start_addr == reg_soil_rh_3||
				  	start_addr == reg_soil_rh_2||start_addr == reg_soil_rh_1)
				  	sensor_params = rh;
				  else if(start_addr == reg_clock_ym||start_addr == reg_clock_dh||start_addr == reg_clock_ms)
				  {   
				     if(start_addr == reg_clock_ym)
					  sensor_params = year_month;
				     if(start_addr == reg_clock_dh)
					  sensor_params = day_hour;
				     if(start_addr == reg_clock_ms)
					  sensor_params = min_sec;
				  }
				  else if(start_addr == reg_quantity)
				  {   
					  sensor_params = quantity;
				  }
				  else if(start_addr >= reg_period_set&&start_addr <= reg_period_sec)
				  {   
					  sensor_params = auto_report_time;
				  }

				if(dev_code ==soilRH_DEV)
                {  
                    reg_addr = start_addr-reg_soil_temp_5;
					if(start_addr >= reg_soil_temp_5&&start_addr <= reg_soil_rh_1)
					{
							p=getSensorData(rh);//
							for(j=0;j<len;j=j+2)  
							{
							    if(j%2 == 0)
									protocolC.payload[index++] = sensor_params;
								else
								{
									if(sensor_params == temperature)
										protocolC.payload[index++] = rh;
									else
										protocolC.payload[index++] = temperature;		
								}
																		
								protocolC.payload[index++] = SIGN_DOT1;
								memcpy(&protocolC.payload[index],p+reg_addr+j,2);
								index =index +2;
								if(j>=8)//0 8 16 24 32
								  break;
									
							}			

					}
					
				}
				else  if(dev_code ==soilCON_DEV)
				;
				else  if(dev_code ==co2_DEV);
				else  if(dev_code ==con_DEV);
				else  if(dev_code ==sunShine_DEV);
				else  if(dev_code ==pressure_DEV);
				else  if(dev_code ==flow_DEV);
				else  if(dev_code ==gateway_DEV);
				else if(sensor_params== quantity)
				{
				    reg_addr = reg_quantity-start_addr;
							p=getSensorData(sensor_params);//
							for(j=0;j<len;j=j+2)	
							{
								protocolC.payload[index++] = sensor_params++;										
								protocolC.payload[index++] = UNSIGNED;
								memcpy(&protocolC.payload[index],p+start_addr-reg_quantity+j,2);
								index =index +2;					
							}		
			}
			else if(sensor_params >=year_month&&sensor_params<=min_sec)
			{
				p=getSensorData(sensor_params);//
				for(j=0;j<len;j=j+2)	
				{
					protocolC.payload[index++] = sensor_params++;										
					protocolC.payload[index++] = UNSIGNED;
					memcpy(&protocolC.payload[index],p+start_addr-reg_clock_ym+j,2);
					index =index +2;					
				}
		
		    }
			else if(sensor_params == auto_report_time)
			{
				p=getSensorData(sensor_params);//
				for(j=0;j<len;j=j+2)	
				{
					protocolC.payload[index++] = sensor_params++;										
					protocolC.payload[index++] = UNSIGNED;
					memcpy(&protocolC.payload[index],p+start_addr-reg_period_set+j,2);
					index =index +2;					
				}			
		}

		}	
   
}
void sendProtocolCData(LORAHW_stru *loraNo)
{
    unsigned char *p;
	unsigned int calCRC;
	unsigned int len;
	len = 0;
	if(protocolC.funCode == gateway_Rcmd)
	{
	     memcpy(p,protocolC.DesAddr,ADDR_SIZE);
		 p=p+ADDR_SIZE;
		 len = len +ADDR_SIZE;
	     memcpy(p,protocolC.SrcAddr,ADDR_SIZE);	
		 p=p+ADDR_SIZE;	
		 len = len +ADDR_SIZE;
		 *(p++) = protocolC.devCode;
		 *(p++) = protocolC.funCode;
		 len = len +2;
		  memcpy(p,protocolC.startAddr ,2);	
		  p=p+2;
		  len = len +2;
		  memcpy(p,protocolC.startAddr ,2);	
		  len = len +2;
		  *(p++) =protocolC.payloadLenH;
		  *(p++) =protocolC.payloadLenL; 
		  len = len +2;
		  memcpy(p,protocolC.payload,((u16)(protocolC.payloadLenH))<<8+protocolC.payloadLenL );	
		  len = len +((u16)(protocolC.payloadLenH))<<8+protocolC.payloadLenL;
		  calCRC=CRC_Compute(&p[ADDR_SIZE+ADDR_SIZE],len-4-ADDR_SIZE-ADDR_SIZE);//计算所接收数据的CRC
		   *(p++) =(u8)(calCRC>>8);
		   *(p++) =(u8)(calCRC);
		  calCRC=CRC_Compute(p,len-2);//计算所接收数据的CRC
		   *(p++) =(u8)(calCRC>>8);
		   *(p++) =(u8)(calCRC);	
		   
		  loraSend(loraNo,p,len+4);
	}
}
