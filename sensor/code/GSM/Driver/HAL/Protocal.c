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
  
  
  
  if(FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS)==0x5a)
  {
    Data_usr.Warn_Thres = FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+1)+
      FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+2)/10.0;	
    
    
    
  }
  else
    Data_usr.Warn_Thres = 3.5;	
  if(Data_usr.Warn_Thres<=0)
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
  
  return FLASH_ReadByte(addr);
}
void OilCalibration()
{
  uint32_t adc_tmp;
  unsigned char *tmp;
  tmp=malloc(4);
  
  
  // for(i=0;i<samplecount;i++)
  {
    adc_tmp = (adcGet(ADC_SENSOR_CHANNEL));
    // adc_sum = adc_sum + adc_tmp;
  }
  // adc_sum = adc_sum /samplecount;
  Data_usr.deepth_calibration = SENSOR_FACTOR ;
  Data_usr.Warn_Thres = adc_tmp/VOLTAGE_FACTOR*Data_usr.deepth_calibration;
  // Data_usr.deep_f = adc_tmp*Data_usr.deepth_calibration;
  if(Data_usr.Warn_Thres<=0)
    Data_usr.Warn_Thres = 3.5;
  tmp[0] = 0x5a;
  //  flash_operation(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS,tmp,1);
  //*(adc_tmp+1)=adc_tmp;
  tmp[1]= ((unsigned char)(Data_usr.Warn_Thres*10))/10;
  tmp[2]= ((unsigned char)(Data_usr.Warn_Thres*10))%10;
  // tmp[1]=2;  
  // tmp[2]=1;
  flash_operation(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS,tmp,3);
  free(tmp);
  
}




unsigned char p[17];
float  adc_tmp,adc_tmp2,adc_tmp3;
extern unsigned char RtcWakeUp;
void sensor_adc()
{
  
  adc_tmp = (adcGet(ADC_BAT_CHANNEL));
  adc_tmp = adc_tmp*2;
  Data_usr.vbatf = adc_tmp;
  Data_usr.vbat[0] = (unsigned char)(adc_tmp/1000/1000);
  Data_usr.vbat[1] =((unsigned char)(adc_tmp/1000/100)%10);
  adc_tmp2 = (adcGet(ADC_SENSOR_CHANNEL));
  
  adc_tmp2 = adc_tmp2/VOLTAGE_FACTOR;
  adc_tmp3 = adc_tmp2;
//  if(adc_tmp2>=vol_offset)
//    adc_tmp2= adc_tmp2 - vol_offset;
  
  adc_tmp2 = adc_tmp2*Data_usr.deepth_calibration;
  Data_usr.deep_f = adc_tmp2;
  
  
  if((Data_usr.deep_f*1.05)>Data_usr.Warn_Thres)
    Data_usr.status = 1;
  else
  {
    Data_usr.status = 0;
    //adc_tmp = Data_usr.Warn_Thres;
    
  }
  if(Data_usr.deep_f>3.5)
    adc_tmp2 = 3.5;
  adc_tmp3=adc_tmp2*10;
  Data_usr.deepth[0] = (unsigned char)(adc_tmp3/10 );
  if(adc_tmp3<10)
  Data_usr.deepth[1] = (((unsigned char)(adc_tmp3))%10 );
  else
   Data_usr.deepth[1] = (((unsigned char)(adc_tmp3))%10 );   
  Data_usr.deepth_percent = (unsigned char)(adc_tmp2/Data_usr.Warn_Thres*100);
}
static uint32_t tmp2;
unsigned int flow_count;
void flow_get()
{
  float tmp;
  
  if(Flow.cal_flag == 1)
  { 
    flow_count = flow_count +1;
    tmp = 1.0/Flow.pulse_period*6250/7.0*10;//123
    tmp2 =( (uint32_t)tmp+tmp2)/flow_count;
    if(flow_count>100)
    {
      flow_count = 0;
      tmp2 = 0;
    }
   // tmp2 =999997;
    Data_usr.flow[0]= (unsigned char)(tmp2/256/256/10);
    Data_usr.flow[1]= (unsigned char)(tmp2/256/10%256);//4294919544
    Data_usr.flow[2]= (unsigned char)(tmp2/10%256);
    Data_usr.flow[3]= (unsigned char)(tmp2%10);
    Flow.cal_flag = 0;
  }
}
void data_tansmmit()
{
  unsigned char len,i;
  
  i=0;
  p[0] = NODE_TO_SERVERH;
  p[1] = NODE_TO_SERVERL;
  len = 2;
  memcpy(p+len,Data_usr.id,2);
  len = len + 2;
  Data_usr.len = 12;
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
  //  Data_usr.flow[0]=0x01;
  //  Data_usr.flow[1]=0x02;
  //  Data_usr.flow[2]=0x05;
  //  Data_usr.flow[3]=0x01;      
  memcpy(p+len,Data_usr.flow  ,4);
  len = len + 4;
  memcpy(p+len,&Data_usr.flow_status  ,1);
  len = len + 1;  
  
  Data_usr.checksum = xorCheck(p+2,len-2);
  memcpy(p+CHECKSUM_INDEX,&Data_usr.checksum ,1);
  
  while(len--)
    UART1_SendByte(p[i++]);
  delay_ms(1000);
}
void module_process()
{
  
  
  //p=malloc(12);
  if(ExeIntFlag)
  {
    ExeIntFlag = 0;
    OilCalibration();
    //  module_prams_init();
    
  }
#if !DEGUG_SENSOR 
  if(Get_Network_status()==SIMCOM_NET_OK) 
#else
    if(RtcWakeUp==1)
#endif
    {
      sensor_adc();
      flow_get();
      data_tansmmit();
#if DEGUG_SENSOR 
      RtcWakeUp = 0;
      Set_Network_status();
      GSM_HardwareInit(ON);
      EnterStopMode();
      disableInterrupts(); 
      RTC_Config(1,OFF);//1:55.2s
      HardwareInit();
      module_prams_init();
      //uart_str.receive_flag =0;
      enableInterrupts();
#endif    
      
    }
  
  //free(p);
  
}
