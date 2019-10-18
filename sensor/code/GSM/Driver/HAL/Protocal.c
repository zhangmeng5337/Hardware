#include "Protocal.h"
#include "GSM.h"
#include "stdlib.h"
#include "bsp.h"
#include "stm8l15x_flash.h"
Data_Stru Data_usr;
Flow_stru Flow;
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
  //Data_usr.deepth [0] = 0x01;
  //Data_usr.deepth[1] = 0x02;	
 // Data_usr.deepth_percent = 0;
 // Data_usr.vbat[0] = 0;
 // Data_usr.vbat[1] = 0;
  Data_usr.status = 0;
  Data_usr.deepth_calibration = SENSOR_FACTOR;
  Data_usr.Warn_Thres = 3.5;
  
  uint32_t tmp;
  unsigned char i;
  if(FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS)==0x5a)
  {
  	Data_usr.Warn_Thres = FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+1)+
		                  FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+2)/10.0;		
  }
  else
    Data_usr.Warn_Thres = 3.5;	

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
extern  unsigned char ExeIntFlag;
void flash_operation(uint32_t addr,unsigned char *p,unsigned char size)
{
  unsigned char i;
  i =0;
  FLASH_Unlock(FLASH_MemType_Data);  
//  for(i=0;i<size;i++)
//  {
//    FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + i));
//  }
  for(i=0;i<size;i++)
  {
    FLASH_ProgramByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+i, p[i]);
    //FLASH_WaitForLastOperation(FLASH_MemType_Data);
  }
  FLASH_Lock(FLASH_MemType_Data);   
  
}

uint32_t flash_read_operation(uint32_t addr)
{
  unsigned char i;
  return FLASH_ReadByte(addr);
}
void OilCalibration()
{
  uint32_t adc_tmp,adc_sum,i;
  unsigned char *tmp;
  tmp=malloc(4);
  adc_sum = 0;
  
 // for(i=0;i<samplecount;i++)
  {
    adc_tmp = (adcGet(ADC_SENSOR_CHANNEL));
   // adc_sum = adc_sum + adc_tmp;
  }
 // adc_sum = adc_sum /samplecount;
  Data_usr.deepth_calibration = SENSOR_FACTOR ;
  Data_usr.Warn_Thres = adc_tmp/VOLTAGE_FACTOR/1000.0*Data_usr.deepth_calibration;
 // Data_usr.deep_f = adc_tmp*Data_usr.deepth_calibration;

  tmp[0] = 0x5a;
//  flash_operation(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS,tmp,1);
  //*(adc_tmp+1)=adc_tmp;
  tmp[1]= ((unsigned char)(Data_usr.Warn_Thres*10))/10;
  tmp[2]= ((unsigned char)(Data_usr.Warn_Thres*10))%10;
  flash_operation(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS,tmp,3);
  free(tmp);
  
}




unsigned char p[16];

extern unsigned char RtcWakeUp;

void module_process()
{
  
  unsigned char len,i;
  float  adc_tmp;
  i=0;
  //p=malloc(12);
  if(ExeIntFlag)
  {
    ExeIntFlag = 0;
    OilCalibration();
  //  module_prams_init();
    
  }
    if(Get_Network_status()==SIMCOM_NET_OK)  
 // if(RtcWakeUp==1)
    {
      adc_tmp = (adcGet(ADC_BAT_CHANNEL));
      adc_tmp = adc_tmp*2;
	  Data_usr.vbatf = adc_tmp;
      Data_usr.vbat[0] = (unsigned char)(adc_tmp/1000/1000);
      Data_usr.vbat[1] =((unsigned char)(adc_tmp/1000/100)%10);
      adc_tmp = (adcGet(ADC_SENSOR_CHANNEL));
      adc_tmp = adc_tmp/50000*Data_usr.deepth_calibration;
	  Data_usr.deep_f = adc_tmp;
      if((adc_tmp*1.05)>Data_usr.Warn_Thres)
        Data_usr.status = 1;
      else
      	{
		  Data_usr.status = 0;
		  //adc_tmp = Data_usr.Warn_Thres;

	  }
      if(adc_tmp>3.5)
        adc_tmp = 3.5;
      Data_usr.deepth[0] = (unsigned char)(adc_tmp*10/10 );
      Data_usr.deepth[1] = (((unsigned char)(adc_tmp*10))%10 );
	  Data_usr.deepth_percent = (unsigned char)(adc_tmp/Data_usr.Warn_Thres*100);
      p[0] = NODE_TO_SERVERH;
      p[1] = NODE_TO_SERVERL;
      len = 2;
      memcpy(p+len,Data_usr.id,2);
      len = len + 2;
      Data_usr.len = 17;
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
      Data_usr.flow[0]=0x01;
      Data_usr.flow[1]=0x02;
      Data_usr.flow[2]=0x05;
      Data_usr.flow[3]=0x01;      
       memcpy(p+len,Data_usr.flow  ,4);
      len = len + 4;
       memcpy(p+len,&Data_usr.flow_status  ,1);
      len = len + 1;  

      Data_usr.checksum = xorCheck(p+2,len-2);
      memcpy(p+CHECKSUM_INDEX,&Data_usr.checksum ,1);
      
      while(len--)
        UART1_SendByte(p[i++]);
      delay_ms(1000);
//      RtcWakeUp = 0;
//      EnterStopMode();
//      disableInterrupts(); 
//      RTC_Config(196,OFF);//1:55.2s
//      HardwareInit();
//      enableInterrupts();
      
      
    }
  
  free(p);
  
}
