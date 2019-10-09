#include "Protocal.h"
#include "GSM.h"
#include "stdlib.h"
#include "bsp.h"
#include "stm8l15x_flash.h"
Data_Stru Data_usr;
//typedef struct{
//  unsigned char id[2];
//
//  unsigned char len;
//  unsigned char checksum;
//  unsigned char deepth[2];
//  unsigned char deepth_percent;
//  unsigned char vbat[2];
//  unsigned char status;
//  unsigned int deepth_calibration;
//}Data_Stru;

void module_prams_init()
{
  Data_usr.id[0] = 0x01;  //id
  Data_usr.id[1] = 0x02;	//id
  Data_usr.len = 0x07;		//data pack length not include header and id
  Data_usr.checksum = 0x00;
  Data_usr.deepth [0] = 0x01;
  Data_usr.deepth[1] = 0x02;	
  Data_usr.deepth_percent = 0;
  Data_usr.vbat[0] = 0;
  Data_usr.vbat[1] = 0;
  Data_usr.status = 0;
  Data_usr.deepth_calibration = 1;
  Data_usr.Warn_Thres = 0x25;
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
 void flash_operation(uint32_t addr,unsigned char *p,unsigned char size)
  {
	unsigned char i;
	i =0;
	FLASH_Unlock(FLASH_MemType_Data);  
	for(i=0;i<size*4;i++)
	{
	  FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + i));
	}
	for(i=0;i<size;i++)
	{
	  FLASH_ProgramWord(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+i, p[i]);
	  //FLASH_WaitForLastOperation(FLASH_MemType_Data);
	}
	FLASH_Lock(FLASH_MemType_Data);   
	
  }

 void flash_read_operation(uint32_t addr)
 {
		unsigned char i;
	    return FLASH_ReadByte(addr);
 }
  void OilCalibration()
{
   uint32_t adc_tmp,adc_sum,i;
   adc_sum = 0;
	
	for(i=0;i<100;i++)
	{
	    adc_tmp = (adcGet(ADC_SENSOR_CHANNEL));
		adc_sum = adc_sum + adc_tmp;
	}
	adc_sum = adc_sum /100;
    Data_usr.Warn_Thres = adc_sum;
	adc_tmp = 0x5aa55a;
	flash_operation(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS,&adc_tmp,1);

	flash_operation(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS,&adc_sum,1);
	
}



  uint32_t adc_tmp;
unsigned char p[16];
extern  unsigned char ExeIntFlag;

void params_init()
{
	      uint32_t tmp;
		  unsigned char i;
		  i= FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS);
		  tmp = i;
		  tmp = tmp<<8; 	  
		  i= FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+1);
		  tmp = i;
		  tmp = tmp<<8;
		  i= FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+2);
		  tmp = i;
		  tmp = tmp<<8;
		  i= FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+3);
		  tmp = i;
		  tmp = tmp<<8;
	
		  if(tmp==0x5aa55a)
			  Data_usr.Warn_Thres = tmp;
	}

}
void module_process()
{
 
  unsigned char len,i;

i=0;
  //p=malloc(12);
  if(ExeIntFlag)
  {
		ExeIntFlag = 0;
		OilCalibration();
		params_init();

  }
  
  if(Get_Network_status()==SIMCOM_NET_OK)
  {
    adc_tmp = (adcGet(ADC_BAT_CHANNEL));
    Data_usr.vbat[0] = (unsigned char)(adc_tmp/1000/1000)*2;
    Data_usr.vbat[1] = (unsigned char)(adc_tmp/1000/100%10)*2;
     adc_tmp = (adcGet(ADC_SENSOR_CHANNEL));
	 if(adc_tmp>Data_usr.Warn_Thres)
	 	Data_usr.status = 1;
	 else
	 	Data_usr.status = 0;
    Data_usr.deepth[0] = (unsigned char)(adc_tmp/Data_usr.deepth_calibration );
    Data_usr.deepth[0] = (unsigned char)(adc_tmp/Data_usr.deepth_calibration%256 );
    p[0] = NODE_TO_SERVERH;
    p[1] = NODE_TO_SERVERL;
    len = 2;
    memcpy(p+len,Data_usr.id,2);
    len = len + 2;
    Data_usr.len = 7;
    memcpy(p+len,&Data_usr.len ,1);
      
    

    len = len + 1;
    Data_usr.checksum = 0;
    memcpy(p+len,&Data_usr.checksum ,1);
    len = len + 1;
    memcpy(p+len,Data_usr.deepth  ,2);
    len = len + 2;
    memcpy(p+len,&Data_usr.deepth_percent  ,1);
    len = len + 1;
    memcpy(p+len,Data_usr.vbat  ,2);
    len = len + 2;
    memcpy(p+len,&Data_usr.status  ,1);
    len = len + 1;
	
	Data_usr.checksum = xorCheck(p+2,len-2);
    memcpy(p+CHECKSUM_INDEX,&Data_usr.checksum ,1);

    while(len--)
      UART1_SendByte(p[i++]);
    
  }
  
  free(p);
  
}
