#include "loraHAL.h"
#include "loraHW.h"
#include "Protocol_C.h"
#include "register.h"
#include "nbiotHAL.h"
#include "nbiotHW.h"
#include "modbus.h"
#include "gateway.h"

loraUart_stru loraUart;
LORAHW_stru lorahw;
LORA_Params_stru loraParams;
void ParamsInit()
{
    unsigned char *p;
	unsigned int tmp;
	ParamsSave();//参数保存
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
	static unsigned char dataToDestFLag=0;
	if(loraUart.receivedFlag1 == 1)
	{	
	     if(lorahw.mode ==2)//配置模式收到数据不做协议解析
	     {
	        loraset(6,&(loraUart.lora1RxBuffer[3]),9);
			ParamsSave();
			lorahw.mode = 0;
		 }
		 else//式收到数据做协议解析
		 {
			 if(protocolCAnaly(loraUart.lora1RxBuffer)	== 0)//校验成功
			 {	 
			      if(dataToDestFLag == 0)
			      {
					  loraNo.loraNo = 0;
					  loraNo.mode =  TransmitMode;
					  wirelessTimoutStart(1);//主动上报模式超时计时标志位
					  WrRead_equipment(&loraNo);//读写寄存器操作
				  }
			      if(dataToDestFLag == 1)
			      {
			          SnedToNbiot(loraUart.lora1RxBuffer);
					      dataToDestFLag = 0;
				  }


			 }

		 }

		loraUart.receivedFlag1 = 0;
		  
	}
    if(ROLE == 1)//网关模式
    {
         unsigned char *fameStatus;
		 fameStatus = modbusFrameStatus();
		if(*fameStatus== 2)
		{
			 if(protocolCAnaly(modbusBuffer())	== 0)//校验成功
			 {	 
			      dataToDestFLag = 0;
				  loraNo.loraNo = 0;
				  loraNo.mode =  TransmitMode;
				  wirelessTimoutStart(1);//主动上报模式超时计时标志位
				  WrRead_equipment(&loraNo);//读写寄存器操作
				  
			 }	
			 *fameStatus = 0;
		}
		unsigned char *tmp,result;
		tmp = NbiotFrameStatus();
		if(tmp[0] == 1&&Get_Network_status()==9)
		{
		    
		     result = protocolCAnaly(NbiotFrameBuffer());
			 if(result	== 0)//校验成功
			 {	 
				  loraNo.loraNo = 0;
				  loraNo.mode =  TransmitMode;
				  wirelessTimoutStart(1);//主动上报模式超时计时标志位
				  WrRead_equipment(&loraNo);//读写寄存器操作
				  dataToDestFLag = 0;
			 }	
			 else if(result	== 4)//数据转发
			 {
				dataToDestFLag = 1;
			 }
			 tmp[0] = 0;
		}
		Gateway_Process();
	}
	//equipmentProcess();//设备调度
	if(loraUart.receivedFlag2 == 1)
	{
		
	}	
}
