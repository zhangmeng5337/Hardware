#include "Protocol_C.h"
#include "crc.h"
#include "register.h"
#include "loraHW.h"
#include "sensor.h"
#include "rtc.h"
#include "nbiotHAL.h"
#include "modbus.h"

extern unsigned char MODE_STAUS;

void SendPayloadPack(LORAHW_stru *loraNo,unsigned char ackMode);
unsigned char autoReportMode(void);
/******************************************************************************
数据结构：
        地址    设备地址编码          功能码     起始地址      字节计数      数组0定义         数组0数值 ...crc1       crc2
bytes   16        1              1       2        1          2             2           2      2
*******************************************************************************/
unsigned char ackmode;
Proctocol_C_stru protocolC;
unsigned char dev_code;
Proctocol_C_stru *protocolCstru(void)
{
	return &protocolC;
}
unsigned char *ackmodeStatus(void)
{
	return &ackmode;
}

unsigned char getPackLen(unsigned char  *p)
{
	unsigned int len;
	if(p[17] !=0x17)
	{
		len = p[PAYLOAD_LEN_INDEX];//payload长度
		len = len + 2*ADDR_SIZE+1+1;//
		len = len+START_ADDR_SIZE + 1+4;
	
	}
	/*else
	{
		len =((u16)(p[PAYLOAD_LEN_INDEX]))<<8+p[PAYLOAD_LEN_INDEX+1];//payload长度
		len = len + 2*ADDR_SIZE+1+1;//
		len = len+START_ADDR_SIZE + 1+1+4;			
	}*/
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
		payload_len =(((u16)(p[PAYLOAD_LEN_INDEX]))<<8) + p[PAYLOAD_LEN_INDEX+1] ;//payload长度
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
	unsigned char result;
	result = 0;
	memcpy(protocolC.DesAddr ,p+ADDR_SIZE,ADDR_SIZE);//目标地址
	//memcpy(protocolC.SrcAddr,&(p[ADDR_SIZE]),ADDR_SIZE);//源地址
	len = getPackLen(p);

	
		calCRC=CRC_Compute(p,len-2);//计算所接收数据的CRC
		recCRC=p[len-1]|(((u16)p[len-2])<<8);//接收到的CRC(低字节在前，高字节在后)
		if(calCRC==recCRC)//CRC校验正确，整个数据包校验正确
		{
		
			calCRC=CRC_Compute(&p[ADDR_SIZE+ADDR_SIZE],len-4-ADDR_SIZE-ADDR_SIZE);
			recCRC=p[len-3]|(((u16)p[len-4])<<8);//接收到的CRC(低字节在前，高字节在后)
			if(calCRC==recCRC)//目的地址正确以及数据校验正确
			{
			 	packAnaly(p);
				if(array_comp(protocolC.DesAddr,protocolC.SrcAddr,ADDR_SIZE) == 0)//判断地址是否一致		  
					result = 0;
				else
					result = 4;
			}
			else
			{
				result = 1;//目的地址校验错误
			}
		}
		/*else
		{
			calCRC=CRC_Compute(&p[ADDR_SIZE+ADDR_SIZE],len-4-ADDR_SIZE-ADDR_SIZE);//计算所接收数据的CRC
			recCRC=p[len-3]|(((u16)p[len-4])<<8);//接收到的CRC(低字节在前，高字节在后)
			if(calCRC==recCRC)//16字节地址校验不正确
			{

			  packAnaly(p);

				result = 2;
			}
			else
			{
				result = 3;
			}
		
		}*/
	if(result == 4)
	{
		if(equip_bind_analy(protocolC.DesAddr)==0)
		{
			LORAHW_stru loraNo;
			loraNo.loraNo = 0;
			loraNo.mode =  TransmitMode;
			SendLoraData(&loraNo,p);

		}

	}
		
    if(result == 1)
    {
       ackmode = 4;
	   LORAHW_stru loraNo;
	   loraNo.loraNo = 0;
	   loraNo.mode =  TransmitMode;
	   SendPayloadPack(&loraNo,4);
	}
	return result;
}

void get_equipmentData()
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
							p=ReadRegister(start_addr);//
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
				 if(sensor_params== quantity)
				{
				    reg_addr = reg_quantity-start_addr;
							p=ReadRegister(start_addr);//
							for(j=0;j<len;j=j+2)	
							{
								protocolC.payload[index++] = sensor_params++;										
								protocolC.payload[index++] = UNSIGNED;
								memcpy(&protocolC.payload[index],p+start_addr-reg_quantity+j,2);
								index =index +2;					
							}		
			}
			 if(sensor_params >=year_month&&sensor_params<=min_sec)
			{
				p=ReadRegister(start_addr);//
				for(j=0;j<len;j=j+2)	
				{
					protocolC.payload[index++] = sensor_params++;										
					protocolC.payload[index++] = UNSIGNED;
					memcpy(&protocolC.payload[index],p+start_addr-reg_clock_ym+j,2);
					index =index +2;					
				}
		
		    }
			 if(sensor_params == auto_report_time)
			{
				p=ReadRegister(start_addr);//
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
void SendPayloadPack(LORAHW_stru *loraNo,unsigned char ackMode)
{
    unsigned char *p;
	unsigned int calCRC;
	unsigned int len;
	 memcpy(p,protocolC.DesAddr,ADDR_SIZE);
	 p=p+ADDR_SIZE;
	 len = len +ADDR_SIZE;
	 memcpy(p,protocolC.SrcAddr,ADDR_SIZE);	
	 p=p+ADDR_SIZE;	
	 len = len +ADDR_SIZE;
	 *(p++) = protocolC.devCode;
	 *(p++) = protocolC.funCode;
	 len = len +2;
	 if(ackMode == 0)
	 {
	  memcpy(p,protocolC.startAddr ,2);	
	  p=p+2;
	  len = len +2;
	  *(p++) =protocolC.payloadLenH;
	  *(p++) =protocolC.payloadLenL; 
	  len = len +2;
	  memcpy(p,protocolC.payload,((u16)(protocolC.payloadLenH))<<8+protocolC.payloadLenL );	
	  len = len +((u16)(protocolC.payloadLenH))<<8+protocolC.payloadLenL;
	 }
	 else if(ackMode == 1)
	 {
		*(p++)  = 0x01;//功能码错误
		len = len + 1;
	 }
	 else if(ackMode == 2)
	 {
		*(p++)  = 0x02;//addr错误
		len = len + 1;
	 }
	 else if(ackMode == 3)
	 {
		*(p++)  = 0x03;//数据个数错误
		len = len + 1;
	 }
	 else if(ackMode == 4)
	 {
		*(p++)  = 0x04;//多个寄存器错误
		len = len + 1;
	 }
	 else if(ackMode == 5)
	 {
		*(p++)  = 0x05;//预留
		len = len + 1;
	 }
	  calCRC=CRC_Compute(&p[ADDR_SIZE+ADDR_SIZE],len-4-ADDR_SIZE-ADDR_SIZE);//计算所接收数据的CRC
	   *(p++) =(u8)(calCRC>>8);
	   *(p++) =(u8)(calCRC);
	  calCRC=CRC_Compute(p,len-2);//计算所接收数据的CRC
	   *(p++) =(u8)(calCRC>>8);
	   *(p++) =(u8)(calCRC);
	   unsigned char *fameStatus;
	   fameStatus = modbusFrameStatus();
    if(*fameStatus== 2)//数据来自串口
    {
       *fameStatus = 0;
		RS485_SendData(p, len+4);

	}
	else
	{
    	while(loraSend(loraNo,p,len+4))
		;

	}

}


void SendLoraData(LORAHW_stru *loraNo,unsigned char *q)
{
    unsigned char *p;
	unsigned int calCRC;
	unsigned int len;
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
	  *(p++) =protocolC.payloadLenH;
	  *(p++) =protocolC.payloadLenL; 
	  len = len +2;
	  memcpy(p,protocolC.payload,((u16)(protocolC.payloadLenH))<<8+protocolC.payloadLenL );	
	  len = len +((u16)(protocolC.payloadLenH))<<8+protocolC.payloadLenL;
	/* else if(ackMode == 1)
	 {
		*(p++)  = 0x01;//功能码错误
		len = len + 1;
	 }
	 else if(ackMode == 2)
	 {
		*(p++)  = 0x02;//功能码错误
		len = len + 1;
	 }
	 else if(ackMode == 3)
	 {
		*(p++)  = 0x03;//功能码错误
		len = len + 1;
	 }
	 else if(ackMode == 4)
	 {
		*(p++)  = 0x04;//功能码错误
		len = len + 1;
	 }
	 else if(ackMode == 5)
	 {
		*(p++)  = 0x05;//功能码错误
		len = len + 1;
	 }*/
	  calCRC=CRC_Compute(&p[ADDR_SIZE+ADDR_SIZE],len-4-ADDR_SIZE-ADDR_SIZE);//计算所接收数据的CRC
	   *(p++) =(u8)(calCRC>>8);
	   *(p++) =(u8)(calCRC);
	  calCRC=CRC_Compute(p,len-2);//计算所接收数据的CRC
	   *(p++) =(u8)(calCRC>>8);
	   *(p++) =(u8)(calCRC);
	   unsigned char *fameStatus;
	   fameStatus = modbusFrameStatus();
    if(*fameStatus== 2)//数据来自串口
    {
       *fameStatus = 0;
		RS485_SendData(p, len+4);

	}
	else
	{
    	while(loraSend(loraNo,p,len+4))
		;

	}

}

void WrRead_equipment(LORAHW_stru *loraNo)
{
	unsigned int len;
	unsigned int addr;
    unsigned char *tmp,i;
	len = 0;
    ackmode = 0;

	if(protocolC.funCode == gateway_Rcmd||protocolC.funCode == pc_cmd||protocolC.funCode == auto_report_cmd)//判断功能码是否正确
	{
		len = ((u16)(protocolC.payloadLenH))<<8+ protocolC.payloadLenL;  //数据长度
		addr = ((u16)(protocolC.startAddr[0]))<<8+protocolC.startAddr[1];//起始寄存器地址
		
		if(*ReadRegister(addr)!=NULL&&((addr+len)<=x1_ADDR_END||(addr+len)<=x12_ADDR_END||
			(addr+len)<=x4_ADDR_END||(addr+len)<=x44_ADDR_END||(addr+len)<=xE_ADDR_END
			||(addr+len)<=xF_ADDR_END))//判断寄存器地址是否正确
			;
		else
			ackmode = 2;
		if(len<1||len>=0x0010)//判断数据长度是否正确
			ackmode = 3;
		
		if(protocolC.funCode == gateway_Rcmd)//读寄存器
		{
		     get_equipmentData();//获取设备数据
	         SendPayloadPack(loraNo, ackmode);//向网关返回数据或者发送到终端
		}
		else if(protocolC.funCode == pc_cmd)//写寄存器
		{
			
			for(i = 2;i<len;i = i+2)//负载数据提取，数据从2开始，因为数组定义中前两个字节分别是名称和格式
			{
				memcpy(tmp+i,&(protocolC.payload[i]),2);
			}
			
			if(WriteRegisteSet(addr,tmp ,len)==0)//写入寄存器数据
			{
				 equip_set();//与硬件相关的写操作
				 
				 SendPayloadPack(loraNo, ackmode);//向网关返回数据或者发送到终端
			}	
			else
			{
				 SendPayloadPack(loraNo, 5);//向网关返回数据或者发送到终端
			}
			ParamsSave();

		}
		else if(protocolC.funCode == auto_report_cmd||autoReportMode()==1)//自动上报
		{
		    get_equipmentData();//获取设备数据
		   // WriteRegisteSet(addr,tmp ,len);
			SendPayloadPack(loraNo, ackmode);//向网关返回数据或者发送到终端
			
		}
		}
	else//功能码不正确
	{
      ackmode = 1;
	  SendPayloadPack(loraNo, ackmode);//向网关返回数据
	}

}
unsigned char autoReportMode()
{
    unsigned char *tmp;
	tmp = ReadRegister(0xf00e);
	return tmp[1];
}
loraModuleTimeout_stru loraModuleTimeout;
void wirelessTimoutStart(unsigned char p)
{
	loraModuleTimeout.tickFlag = 1;
}
unsigned char wirelessTimoutProcess()
{
	static unsigned int timeoutCount;
  unsigned char result;
	result = 0;
	if(loraModuleTimeout.tickFlag == 1)
	{   

		timeoutCount = uchar2uint( ReadRegister(0xf004));
	    loraModuleTimeout.tickFlag = 2;
		loraModuleTimeout.tickCount = HAL_GetTick();
	}
	if(loraModuleTimeout.tickFlag == 2)
	{   
	    if((HAL_GetTick()-loraModuleTimeout.tickCount) >= timeoutCount)
	    {
	        loraModuleTimeout.tickFlag =0;
			result = 0;
		}
		else
		{
			result = 1;
		}
	}
	return result;
}

void equipmentProcess(void)
{  	
	getRH_T();
	getvbat();
	getRTC();
	getGPS();
   
	 MODE_STAUS = getModeStatus();
	if(MODE_STAUS == 0x02||ROLE == 1)//带nbiot
	{
		SIMCOM_Register_Network();
	}
	else
	{  
		if(ROLE == 0)//终端模式应答modbus命令
		{
			RS485_Service();
		}

		if(autoReportMode()==1)
		{
			if(wirelessTimoutProcess() == 0)
			{
					;//休眠
			}
			else
			{
				   ;//等待接收命令
			}
			if(getRtcStatus()->RtcWakeUP == 1)//休眠时间到主动上报
			{

				protocolC.devCode = soilRH_DEV;
				protocolC.startAddr[0] = 0x10;
				protocolC.startAddr[1] = 0x01 ;
				protocolC.payloadLenH =0x00;
				protocolC.payloadLenL = 20;	 
				LORAHW_stru loraNo;
				loraNo.loraNo = 0;
				loraNo.mode =  TransmitMode;
			    get_equipmentData();//获取设备数据
	            SendPayloadPack(&loraNo, 0);//向网关返回数据或者发送到终端
	            getRtcStatus()->RtcWakeUP = 0;
			}
				
				
		}

	}
}
