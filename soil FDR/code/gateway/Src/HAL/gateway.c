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

	flash_read_Data(data_in_flash.lastRead_addr,sensorDataInFlash,20);
	data_in_flash.lastRead_addr = data_in_flash.lastRead_addr+20;
	if(data_in_flash.lastRead_addr == DATA_MAX_ADDR)
		data_in_flash.lastRead_addr = 0;
	return sensorDataInFlash;

}
data_in_flash_stru *getDataFrameAddr(void)
{
	return &data_in_flash;
}
static unsigned char *swap;
void DataFrameSave(void)
{
  unsigned char *p;
  
  
  unsigned int len;
  if(readRigsterStatus()->regAddr ==0x1001)
  {
    if(readRigsterStatus()->wrStatus == 1)
    {
		  p = ReadRegister(0x4001);
		  len = p[0];
		  len = len <<8;
		  len = len + p[1];
			
			if(data_in_flash.lastWrite_addr<DATA_MAX_ADDR) 
		  {
			p = ReadRegister(0x1001);//�����յ�ʪ�����ݼĴ���
			flash_write_Data(len*20,p,20);
			len = len +1;//����֡����
			data_in_flash.lastWrite_addr = data_in_flash.lastWrite_addr + 20;
			p = ReadRegister(0x4001);
			p[0] = (unsigned char) (len >> 8);
			p[1] = (unsigned char) (len);
		   // p = ReadRegister(0x4003);
		   // p[0] = (unsigned char) (len >> 8);
		   // p[1] = (unsigned char) (len);

		  }
		  else
		  {
				swap = malloc(DATA_MAX_ADDR);
				memset(swap,0,DATA_MAX_ADDR);
				flash_read_Data(0,swap,DATA_MAX_ADDR);
				flash_init(1);
				
				/*********************************************************
				 2000	0	  2001	  1
				*********************************************************/
				unsigned int len2;
				uint32_t addr_offset;

				
				addr_offset = data_in_flash.lastWrite_addr-DATA_MAX_ADDR;
				//addr_offset = addr_offset*20;	//д�������ݣ�����ԭʼ����֡��ַ		
				p = ReadRegister(0x1001);//
				flash_write_Data(0, swap, addr_offset);//swap	
				flash_write_Data(addr_offset, p, 20);//swap


	
				//p = ReadRegister(0x1001);//�洢��ǰ�벿��δ����������֡����д��flash
			
				///0 20 40 60 1 2 3 4 										   
				
				
				len2 =2*DATA_MAX_ADDR;
				len2 =len2 - data_in_flash.lastWrite_addr;
				len2 =len2 - 20;				
				addr_offset = data_in_flash.lastWrite_addr-DATA_MAX_ADDR+20;
			//	addr_offset = addr_offset*20;	//�洢��δ��������������д��flash  220 
				//len2 = len2*20; 		
				flash_write_Data(addr_offset, &swap[(data_in_flash.lastWrite_addr-DATA_MAX_ADDR+20)], len2);	//sap-end	
				HAL_FLASH_Lock();
				//len = len +20;
				len = len + 1;
				data_in_flash.lastWrite_addr = data_in_flash.lastWrite_addr + 20;
				if(data_in_flash.lastWrite_addr>=(2*DATA_MAX_ADDR))
					data_in_flash.lastWrite_addr = DATA_MAX_ADDR;
				
				if(len>=DATA_MAX_COUNT)
					len = DATA_MAX_COUNT;
			    p = ReadRegister(0x4001);
				p[0] = (unsigned char) (len >> 8);
				p[1] = (unsigned char) (len);
			   //p = ReadRegister(0x4003);
			   //if(len>=DATA_MAX_COUNT)
			   //	len = DATA_MAX_COUNT;
				//p[0] = (unsigned char) (len >> 8);
				//p[1] = (unsigned char) (len);				
				free(swap);
		  }

	}
  }
  
}
void gatewayDataFrame(unsigned char len)
{
	
	unsigned char *ackMode,*p;
	Proctocol_C_stru *proc_tmp;
	LORAHW_stru loraNo;
	unsigned int q;
	proc_tmp = protocolCstru();
	if(proc_tmp->funCode == pc_cmd)
	{   
	    ackMode = ackmodeStatus();
		 if(*ackMode == 0)
		 {
		  unsigned char i;
		  for(i = 0;i<len;i++)
		  {
			   dev_params_pack(0,temperature,40,1);
				protocolCstru()->payloadLenH = 0;
				protocolCstru()->payloadLenL = 40*len;
				loraNo.loraNo = 0;
				loraNo.mode =  TransmitMode;
				proc_tmp->funCode == PC_readFrame;
				SendPayloadPack(&loraNo,*ackMode);
				
				p = ReadRegister(0x4003);//���ݶ��������֡����
				q = (((u16)(p[0]))<<8)+p[1];
				if(q>0)
				{
					q = q - 1;
					p[0] = (unsigned char)(q>>8);
					p[1] = (unsigned char)(q);


				}
				else
				{
					p[0] = 0;
					p[1] = 0;
				  p = ReadRegister(0x4002);//���ݶ��������֡����
//					p[0] = 0;
//					p[1] = 0;					
				}	
				p = ReadRegister(0x4001);//���ݶ��������֡����
				q = (((u16)(p[0]))<<8)+p[1];
				if(q>0)
				{
					q = q - 1;
					p[0] = (unsigned char)(q>>8);
					p[1] = (unsigned char)(q);

				}
				else
				{
					p[0] = 0;
					p[1] = 0;
					break;

				}				

		  }

		 }


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
	    p = ReadRegister(0x4002);
		if(p[1] ==1)//0x4002д����
		{
				p = ReadRegister(0x4002);//��ʹ��
				if(p[1]==1)
				{		
					p = ReadRegister(0x4003);
					q = (((u16)(p[0]))<<8)+p[1];
					if(q>0)//��ȡ֡������1
					{
						gatewayDataFrame(q);//����֡����
						
						ParamsSave(0);
					}
					
				}
		}	
	}

}
