#include "loraHAL.h"
#include "loraHW.h"
#include "Protocol_C.h"
#include "register.h"
#include "nbiotHAL.h"
#include "nbiotHW.h"
#include "modbus.h"
#include "gateway.h"
#include "sensor.h"
unsigned char MODE_STAUS;

loraUart_stru loraUart;
LORAHW_stru lorahw;
LORA_Params_stru loraParams;
loraUart_stru *getloraProcStatus()
{
	return &loraUart;
}

void ParamsInit()
{
    unsigned char *p;
	unsigned int tmp;
	ParamsRead();
	
	p = ReadRegister(0x1205);
	if(ROLE == 1||getModeStatus()== 0x02)
	{
		NbiotUartInit();
		ServerIP_Pack(p);
		nbiot_HardwareInit(ON);
	}
	p = ReadRegister(0xf00c);
	tmp = uchar2uint(p);
	sensorModbusRate(tmp,0);
}


void EquipGateway_Process()
{  
	LORAHW_stru loraNo;
	static unsigned char pcFLag =  0;
	
	if(loraUart.receivedFlag1 == 1)//lora数据处理
	{	
	     if(lorahw.mode ==2)//配置模式收到数据不做协议解析
	     {
	        loraset(6,&(loraUart.lora1RxBuffer[3]),9);
			ParamsSave(0);
			lorahw.mode = 0;
		 }
		 else//正常模式收到数据做协议解析
		 {
			 if(protocolCAnaly(loraUart.lora1RxBuffer)	== 0)//校验成功
			 {	 
			      if(*getDataSrc() == 0)
			      {
					  loraNo.loraNo = 0;
					  setDataSrc(DAT_FROM_LORA);//数据来自lora
					  loraNo.mode =  TransmitMode;
					  wirelessTimoutStart(1);//主动上报模式超时计时标志位
					  WrRead_equipment(&loraNo);//读写寄存器操作
				  }
			      if(*getDataSrc() == 1&&(ROLE == GATEWAY))//处理终端返回的数据
			      {
			          unsigned int len;
					  len = ADDR_SIZE + ADDR_SIZE ;
					  len = len + 5 ;
					  len = len + loraUart.lora1RxBuffer[20];
			          SnedToNbiot(loraUart.lora1RxBuffer,len);//终端数据通过网关转发给服务器
					   setDataSrc(0);
				  }


			 }

		 }

		loraUart.receivedFlag1 = 0;
		  
	}
	 MODE_STAUS = getModeStatus();
	if(MODE_STAUS == 0x02)//带nbiot，nbiot数据处理
	{
		SIMCOM_Register_Network();
		//if(ROLE == GATEWAY)//网关模式
		{
			unsigned char *tmp,result;
			tmp = NbiotFrameStatus();
			if(tmp[0] == 1&&Get_Network_status()==SIMCOM_NET_OK)
			{
				result = protocolCAnaly(NbiotFrameBuffer());
				if(result	== 0)//校验成功
				{	 
				    pcFLag = 1;
					loraNo.loraNo = 0;
					loraNo.mode =  TransmitMode;
					setDataSrc(DAT_FROM_NBIOT);//数据来自nbiot
					Gateway_Process();//通过nbiot
					wirelessTimoutStart(1);//主动上报模式超时计时标志位
					WrRead_equipment(&loraNo);//读写寄存器操作
					
					pcFLag =  0;
				}	
				else if(result == 4)//作为网关进行数据转发给终端
				{
				   if(ROLE == GATEWAY)
					setDataSrc(DAT_FROM_RELAY);
				}
				tmp[0] = 0;
			}
			
		}

	}

         unsigned char *fameStatus;
		 fameStatus = modbusFrameStatus();
		if(*fameStatus== 2)
		{
			 if(protocolCAnaly(modbusBuffer())	== 0)//校验成功,数据来自pc端
			 {	 
			      
			      setDataSrc(3);//数据来自pc
				  loraNo.loraNo = 0;
				  loraNo.mode =  TransmitMode;
				  wirelessTimoutStart(1);//主动上报模式超时计时标志位
				  WrRead_equipment(&loraNo);//读写寄存器操作
				  
			 }
			 else //数据来自设备
			 {
				PCDevice_Modbus_Process();
			 }
			 
			 Gateway_Process();
			 *fameStatus = 0;
		} 
		
	equipmentProcess(pcFLag);//设备调度
	if(loraUart.receivedFlag2 == 1)
	{
		
	}	
}
