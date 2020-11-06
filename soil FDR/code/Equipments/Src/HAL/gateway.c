#include "gateway.h"
#include "register.h"
#include "Protocol_C.h"
#include "EEPROM.h"
#include "modbus.h"
uint32_t last_addr;
void gatewayDataFrame()
{
	unsigned char *sensorDataInFlash;
	unsigned char *q,*p,*ackMode;
	unsigned int calCRC;
	unsigned int len,index;
	index = 0;
	Proctocol_C_stru *proc_tmp;
	proc_tmp = protocolCstru();
	if(proc_tmp->funCode == PC_readFrame)
	{
	    ackMode = ackmodeStatus();
		flash_read2(last_addr,sensorDataInFlash,18);
		last_addr = last_addr+18;
		if(last_addr == 3600)
			last_addr = 0;
		
		 memcpy(p,proc_tmp->DesAddr,ADDR_SIZE);
		 index=index+ADDR_SIZE;
		 len = len +ADDR_SIZE;
		 memcpy(p,proc_tmp->SrcAddr,ADDR_SIZE);	
		 index=index+ADDR_SIZE; 
		 len = len +ADDR_SIZE;
		 p[index++] = proc_tmp->devCode;
		 p[index++] = proc_tmp->funCode;
		 len = len +2;
		 if(ackMode == 0)
		 {
		  memcpy(p,proc_tmp->startAddr ,2); 
		  index=index+2;
		  len = len +2;
		  p[index++] =0;
		  p[index++] =20; 
		  len = len +2;
		  
		  unsigned char j;
		  for(j=0;j<5;j++)	
		  { 	 
			p[index++] = temperature;
			p[index++] = SIGN_DOT1;
			 memcpy(p,sensorDataInFlash+j*4,2);
			 index=index+2;
			 p[index++] = rh;
			 p[index++] = SIGN_DOT1;
			 memcpy(p,sensorDataInFlash+j*4+2,2);
			 index=index+2;
	  
		  }   
		  len = len +20;
		 }
		 else if(*ackMode == 1)
		 {
			p[index++]	= 0x01;//功能码错误
			len = len + 1;
		 }
		 else if(*ackMode == 2)
		 {
			p[index++]	= 0x02;//功能码错误
			len = len + 1;
		 }
		 else if(*ackMode == 3)
		 {
			p[index++]	= 0x03;//功能码错误
			len = len + 1;
		 }
		 else if(*ackMode == 4)
		 {
			p[index++]	= 0x04;//功能码错误
			len = len + 1;
		 }
		 else if(*ackMode == 5)
		 {
			p[index++]	= 0x05;//功能码错误
			len = len + 1;
		 }
		  calCRC=CRC_Compute(&p[ADDR_SIZE+ADDR_SIZE],len-4-ADDR_SIZE-ADDR_SIZE);//计算所接收数据的CRC
		   p[index++] =(u8)(calCRC>>8);
		   p[index++] =(u8)(calCRC);
		  calCRC=CRC_Compute(p,len-2);//计算所接收数据的CRC
		   p[index++] =(u8)(calCRC>>8);
		   p[index++] =(u8)(calCRC);	
		   unsigned char *fameStatus;
	       fameStatus = modbusFrameStatus();
		if(*fameStatus== 2)//数据来自串口
		{
			*fameStatus = 0;
			RS485_SendData(p, len+4);
		}
			  
		else
		{
			 Nbiot_SendData(p, len+4);
			;
		}

	}
    
}

void Gateway_Process(void)
{
  unsigned char *p;
  unsigned int q;
  p = ReadRegister(0x4001);
  if(p[0] == 0&&p[1]==0) //0x4002清零
  {
   WriteRegisteSet(0x4002, p, 2);
  }
  if(readRigsterStatus()->wrStatus ==1)//0x4002写操作
  {
	if(readRigsterStatus()->regAddr == 0x4002 )
	{
		p = ReadRegister(0x4002);
		if(p[1]==1)
		{
			p[0] = 0;
			p[1] = 0;
		}
		p = ReadRegister(0x4003);
		q = (((u16)(p[0]))<<8)+p[1];
		if(q>0)
			q = q -1;
		p[0] = (u8)(q>>8);//计数减1
		p[1] = (u8)(q);	//计数减1
		
		p = ReadRegister(0x4001);//计数减1
		if(q>0)
			q = q -1;
		p[0] = (u8)(q>>8);
		p[1] = (u8)(q);	
    gatewayDataFrame();//数据帧发送
		
	}
  }
}
