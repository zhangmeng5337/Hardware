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
	
	if(loraUart.receivedFlag1 == 1)//lora���ݴ���
	{	
	     if(lorahw.mode ==2)//����ģʽ�յ����ݲ���Э�����
	     {
	        loraset(6,&(loraUart.lora1RxBuffer[3]),9);
			ParamsSave(0);
			lorahw.mode = 0;
		 }
		 else//����ģʽ�յ�������Э�����
		 {
			 if(protocolCAnaly(loraUart.lora1RxBuffer)	== 0)//У��ɹ�
			 {	 
			      if(*getDataSrc() == 0)
			      {
					  loraNo.loraNo = 0;
					  setDataSrc(DAT_FROM_LORA);//��������lora
					  loraNo.mode =  TransmitMode;
					  wirelessTimoutStart(1);//�����ϱ�ģʽ��ʱ��ʱ��־λ
					  WrRead_equipment(&loraNo);//��д�Ĵ�������
				  }
			      if(*getDataSrc() == 1&&(ROLE == GATEWAY))//�����ն˷��ص�����
			      {
			          unsigned int len;
					  len = ADDR_SIZE + ADDR_SIZE ;
					  len = len + 5 ;
					  len = len + loraUart.lora1RxBuffer[20];
			          SnedToNbiot(loraUart.lora1RxBuffer,len);//�ն�����ͨ������ת����������
					   setDataSrc(0);
				  }


			 }

		 }

		loraUart.receivedFlag1 = 0;
		  
	}
	 MODE_STAUS = getModeStatus();
	if(MODE_STAUS == 0x02)//��nbiot��nbiot���ݴ���
	{
		SIMCOM_Register_Network();
		//if(ROLE == GATEWAY)//����ģʽ
		{
			unsigned char *tmp,result;
			tmp = NbiotFrameStatus();
			if(tmp[0] == 1&&Get_Network_status()==SIMCOM_NET_OK)
			{
				result = protocolCAnaly(NbiotFrameBuffer());
				if(result	== 0)//У��ɹ�
				{	 
				    pcFLag = 1;
					loraNo.loraNo = 0;
					loraNo.mode =  TransmitMode;
					setDataSrc(DAT_FROM_NBIOT);//��������nbiot
					Gateway_Process();//ͨ��nbiot
					wirelessTimoutStart(1);//�����ϱ�ģʽ��ʱ��ʱ��־λ
					WrRead_equipment(&loraNo);//��д�Ĵ�������
					
					pcFLag =  0;
				}	
				else if(result == 4)//��Ϊ���ؽ�������ת�����ն�
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
			 if(protocolCAnaly(modbusBuffer())	== 0)//У��ɹ�,��������pc��
			 {	 
			      
			      setDataSrc(3);//��������pc
				  loraNo.loraNo = 0;
				  loraNo.mode =  TransmitMode;
				  wirelessTimoutStart(1);//�����ϱ�ģʽ��ʱ��ʱ��־λ
				  WrRead_equipment(&loraNo);//��д�Ĵ�������
				  
			 }
			 else //���������豸
			 {
				PCDevice_Modbus_Process();
			 }
			 
			 Gateway_Process();
			 *fameStatus = 0;
		} 
		
	equipmentProcess(pcFLag);//�豸����
	if(loraUart.receivedFlag2 == 1)
	{
		
	}	
}
