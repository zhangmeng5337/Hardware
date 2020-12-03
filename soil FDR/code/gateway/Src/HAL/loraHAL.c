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
	if(ROLE != GATEWAY)
	p = ReadRegister(0x1206);
	else
	p = ReadRegister(0x45f0);		
	if(ROLE == GATEWAY||getModeStatus()== 0x02)
	{
		NbiotUartInit();
		ServerIP_Pack(p);
		nbiot_HardwareInit(ON);
	}
	if(ROLE != GATEWAY)
	{
		p = ReadRegister(0xf00c);
		tmp = uchar2uint(p);
		sensorModbusRate(tmp,0);	
	}

}


void EquipGateway_Process()
{  
	LORAHW_stru loraNo;
	unsigned char * datasrcFlag;
	static unsigned char pcFLag =  0;
	/*******************************************************************
                       lora数据处理过程
	********************************************************************/
	datasrcFlag = getDataSrc();
	if(loraUart.receivedFlag1 == 1)//lora数据处理
	{	
			setDataSrc(DAT_FROM_LORA);//数据来自lora
			if(lorahw.mode ==2)//配置模式收到数据不做协议解析
			{
				loraset(6,&(loraUart.lora1RxBuffer[3]),9);
				ParamsSave(0);
				lorahw.mode = 0;
				setDataSrc(0);//数据来自lora
			}
		 else//正常模式收到数据做协议解析
		 {
			  setDataSrc(DAT_FROM_LORA);//数据来自lora
			 if(protocolCAnaly(loraUart.lora1RxBuffer)	== 0)//校验成功
			 {	 
			      if(datasrcFlag[0] == 0||DAT_FROM_LORA)
			      {
					  loraNo.loraNo = 0;
					  setDataSrc(DAT_FROM_LORA);//数据来自lora
					  loraNo.mode =  TransmitMode;
					  if(ROLE != GATEWAY)
					    wirelessTimoutStart(1);//主动上报模式超时计时标志位
					  WrRead_equipment(&loraNo);//读写寄存器操作
					  Gateway_Process();//测试
					 // setDataSrc(0);//数据来自lora
				  }
			      if((datasrcFlag[0] == DAT_FROM_RELAY)&&(ROLE == GATEWAY))//网关转发数据给终端后的响应数据
			      {
			          unsigned int len;
					  len = ADDR_SIZE + ADDR_SIZE ;
					  len = len + 5 ;
					  len = len + loraUart.lora1RxBuffer[20];
					  if(Get_Network_status()==SIMCOM_NET_OK)
			          	SnedToNbiot(loraUart.lora1RxBuffer,len);//终端数据通过网关转发给服务器
					  setDataSrc(0);
				  }

			 }

		 }

		loraUart.receivedFlag1 = 0;
		  
	}
	 /*******************************************************************
						nbiot、pc和推肥系统数据处理过程
	 ********************************************************************/

	 MODE_STAUS = getModeStatus();
	 unsigned char *nbiot_enable,*data_save;
	 if(ROLE == GATEWAY)
	 {
	    nbiot_enable = ReadRegister(0x45f5);//nbiot 485推肥使能
	    data_save = ReadRegister(0x45f6);//节点数据存储与否
	 }	
	 
	if(MODE_STAUS == 0x02||nbiot_enable[1]==1)//带nbiot或者网关数据处理
	{
		SIMCOM_Register_Network();
			unsigned char *tmp,result;
			tmp = NbiotFrameStatus();//nbiot接收数据标志位
		  setDataSrc(DAT_FROM_NBIOT);//数据来自nbiot
			if(tmp[0] == 1&&Get_Network_status()==SIMCOM_NET_OK)
			{
				result = protocolCAnaly(NbiotFrameBuffer());
				if(result	== 0)//校验成功
				{	

				    pcFLag = 1;
					loraNo.loraNo = 0;
					loraNo.mode =  TransmitMode;
					
					Gateway_Process();//读数据帧寄存器操作
					if(ROLE != GATEWAY)
					  wirelessTimoutStart(1);//主动上报模式超时计时标志位
					WrRead_equipment(&loraNo);//读写寄存器操作
					
					pcFLag =  0;
				}	
				else if(result == 4)//作为网关进行数据转发给终端,数据已经转发完成
				{
				   if(ROLE == GATEWAY)
					setDataSrc(DAT_FROM_RELAY);
				}
				tmp[0] = 0;
			}
	}
	else if(nbiot_enable[1] == 2||ROLE != GATEWAY)//485推肥系统使能
	{
	   if(data_save[1]==0&&ROLE == GATEWAY)
	   	{
		   setDataSrc(DAT_FROM_PC);
		   WrRead_equipment(&loraNo);//读写寄存器操作
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
				  if(ROLE != GATEWAY)
				  	wirelessTimoutStart(1);//主动上报模式超时计时标志位
				  WrRead_equipment(&loraNo);//读写寄存器操作
				  
			 }
			 else //数据来自设备
			 {
				if(ROLE != GATEWAY)
				PCDevice_Modbus_Process();
			 }
			 
			 Gateway_Process();//读数据帧寄存器操作
			 *fameStatus = 0;
		} 

	}

	equipmentProcess(pcFLag);//设备调度
	if(loraUart.receivedFlag2 == 1)
	{
		
	}	
}
