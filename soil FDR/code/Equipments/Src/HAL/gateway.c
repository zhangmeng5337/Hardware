#include "gateway.h"
#include "register.h"
#include "Protocol_C.h"
#include "EEPROM.h"
#include "modbus.h"
#include "stdlib.h"

data_in_flash_stru data_in_flash;

/*************************************************************************
flash����֡�洢�ṹ��
                        ����1 ......����2000
���ȣ��ֽڣ�                    20   ...... 20        �ܹ�2000*20=40004     
**************************************************************************/
unsigned char* readDataFrame(void)
{
	unsigned char sensorDataInFlash[24];

	flash_read_Data(data_in_flash.last_addr,sensorDataInFlash,20);
	data_in_flash.last_addr = data_in_flash.last_addr+20;
	if(data_in_flash.last_addr == DATA_MAX_ADDR)
		data_in_flash.last_addr = 0;
	return NULL;

}
data_in_flash_stru *getDataFrameAddr(void)
{
	return &data_in_flash;
}

void DataFrameSave(void)
{
  unsigned char *p;
  unsigned char *swap;
  
  unsigned int len;
  p = ReadRegister(0x4001);
  len = p[0];
  len = len <<8;
  len = len + p[1];
    if(len<DATA_MAX_COUNT) 
  {
    p = ReadRegister(0x1001);//�����յ�ʪ�����ݼĴ���
	flash_write_Data(len*20,p,20);
	len = len +20;
	p[0] = (unsigned char) (len >> 8);
	p[1] = (unsigned char) (len);	
  }
  else
  {
		swap = malloc(DATA_MAX_ADDR);
		flash_read_Data(0,swap,DATA_MAX_ADDR);
		flash_init(1);
		
		/*********************************************************
         2000   0     2001    1
		*********************************************************/
		unsigned int len2;
		uint32_t addr_offset;
		len2 = (len-DATA_MAX_COUNT);//�洢��ǰ�벿��δ����������֡����д��flash
		len2 = len2*20;		
		flash_write_Data(0,swap,len2);//0-swap
		
		addr_offset = len-DATA_MAX_COUNT;
		addr_offset = addr_offset*20;	//����������֡��ַ		
		len2 = (len-DATA_MAX_COUNT);
		len2 = len2*20;	
		p = ReadRegister(0x1001);//
		flash_write_Data(addr_offset, p, 20);//swap
		                                           
		addr_offset = len-DATA_MAX_COUNT+1;
		addr_offset = addr_offset*20;	//�洢��δ��������������д��flash
		len2 =2*DATA_MAX_COUNT-len - 1;   
		len2 = len2*20;			
		flash_write_Data(addr_offset, &swap[(len-DATA_MAX_COUNT+1)*20], len2);	//sap-end	
		len = len +20;
		p[0] = (unsigned char) (len >> 8);
		p[1] = (unsigned char) (len);
		if(len>=(2*DATA_MAX_COUNT))
			len = DATA_MAX_COUNT;
  }
}
void gatewayDataFrame()
{
	
	unsigned char *ackMode;
	Proctocol_C_stru *proc_tmp;
	LORAHW_stru loraNo;
	proc_tmp = protocolCstru();
	if(proc_tmp->funCode == PC_readFrame)
	{
	    ackMode = ackmodeStatus();
		 if(ackMode == 0)
		 {
  		  dev_params_pack(0,temperature,20,1);
		 }
		loraNo.loraNo = 0;
		loraNo.mode =  TransmitMode;
    SendPayloadPack(&loraNo,*ackMode);

	}
    
}

void Gateway_Process(void)
{
  unsigned char *p;
  unsigned int q;
  p = ReadRegister(0x4001);
  if(p[0] == 0&&p[1]==0) //0x4002����
  {
   WriteRegisteSet(0x4002, p, 2);
  }
	else
	{
		if(readRigsterStatus()->wrStatus ==1)//0x4002д����
		{
		if(readRigsterStatus()->regAddr == 0x4002 )
		{
			p = ReadRegister(0x4002);//��ʹ��
			if(p[1]==1)
			{		
				p = ReadRegister(0x4003);
				q = (((u16)(p[0]))<<8)+p[1];
				if(q>0)//��ȡ֡������1
				{
					q = q -1;
					p[0] = (u8)(q>>8);//������1
					p[1] = (u8)(q); //������1
					gatewayDataFrame();//����֡����

				}
				else
				{
					p[0] = 0;
					p[1] = 0;
					p = ReadRegister(0x4001);//���ݶ��������֡����
					p[0] = 0;
					p[1] = 0;

				}
				
			}
		}
		}	
	}

}
