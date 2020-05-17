#include "Protocal.h"
#include "stdlib.h"
#include "bsp.h"
#include "gprs_app.h"
#include "stm8l15x_flash.h"
#include "gps.h"
#include "lcd_hal.h"
extern _SaveData Save_Data;


Data_Stru Data_usr;
Flow_stru Flow;
extern _uart uart1;

uint32_t timeout;
unsigned int rtctime=10;//10:9s，修改休眠时间
//unsigned int RepeatTime=30;//10:9s，修改休眠时间
float warn_setting = 3.5;//3.5m预警值
unsigned int wakeupcount=1;
#define SETTING_COUNT  3*rtctime/60
void module_prams_init()
{
  wakeupcount=SETTING_COUNT;
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
  Data_usr.Warn_Thres =warn_setting;
  
  
  
  if(FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS)==0x5a)
  {
    Data_usr.Warn_Thres = FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+1)+
      FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+2)/10.0;
    Data_usr.calibration_done = FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+3);
    if(Data_usr.calibration_done ==1)
    {
      Data_usr.voltage_calibration_value = FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+4)+
        FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+5)/10.0+
          FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+6)/100.0;                 
    }
    
    
    
  }
  else
    Data_usr.Warn_Thres = 3.5;	
  if(Data_usr.Warn_Thres<=0)
    Data_usr.Warn_Thres = 3.5;
  uart1.isGetData = 0;  
  
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
  tmp=malloc(7);
  
  
  // for(i=0;i<samplecount;i++)
  {
    adc_tmp = (adcGet(ADC_SENSOR_CHANNEL,500));
    // adc_sum = adc_sum + adc_tmp;
  }
  // adc_sum = adc_sum /samplecount;
  Data_usr.voltage_calibration_value = adc_tmp/VOLTAGE_FACTOR;
  Data_usr.calibration_done = 1;
  Data_usr.deepth_calibration = SENSOR_FACTOR ;
  //Data_usr.Warn_Thres = 3.5;//Data_usr.voltage_calibration_value*Data_usr.deepth_calibration;
  // Data_usr.deep_f = adc_tmp*Data_usr.deepth_calibration;
  if(Data_usr.Warn_Thres<=0)
    Data_usr.Warn_Thres = warn_setting;
  tmp[0] = 0x5a;
  //  flash_operation(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS,tmp,1);
  //*(adc_tmp+1)=adc_tmp;
  tmp[1]= ((unsigned char)(Data_usr.Warn_Thres*10))/10;
  tmp[2]= ((unsigned char)(Data_usr.Warn_Thres*10))%10;
  tmp[3]= Data_usr.calibration_done ;
  tmp[4]= (unsigned char)Data_usr.voltage_calibration_value;
  tmp[5]= ((unsigned char)(Data_usr.voltage_calibration_value*10))/10;
  tmp[6]= ((unsigned char)(Data_usr.voltage_calibration_value*10))%10;  
  // tmp[1]=2;  
  // tmp[2]=1;
  flash_operation(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS,tmp,7);
  free(tmp);
  
}




unsigned char p[64];
float  adc_tmp,adc_tmp2,adc_tmp3;
extern unsigned char RtcWakeUp;
void sensor_adc()
{
 // static unsigned char  init_flag=0;
  adc_tmp = (adcGet(ADC_BAT_CHANNEL,200));
  adc_tmp = adc_tmp*2;
  Data_usr.vbatf = adc_tmp;
  Data_usr.vbat[0] = (unsigned char)(adc_tmp/1000/1000);
  Data_usr.vbat[1] =((unsigned char)(adc_tmp/1000/100)%10);
  
  
  
  
  if(Data_usr.calibration_done ==0)
  {
    OilCalibration();
  }
  else
  {
    adc_tmp2 = (adcGet(ADC_SENSOR_CHANNEL,200));
    adc_tmp2 = adc_tmp2/ VOLTAGE_FACTOR;
    //adc_tmp2 = adc_tmp2 -Data_usr.voltage_calibration_value ;  
  }
  
  // adc_tmp2=adc_tmp2-1.9;
  //adc_tmp3 = adc_tmp2-2;
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
  if((Data_usr.deep_f_last-Data_usr.deep_f)>=SETTING_THRES||
     (Data_usr.deep_f-Data_usr.deep_f_last)>=SETTING_THRES)
  {
    Data_usr.status = 1;
  }
  Data_usr.deep_f_last=Data_usr.deep_f;
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
unsigned char len;
void data_tansmmit()
{
  unsigned char i;
  
  i=0;
  p[0] = NODE_TO_SERVERH;
  p[1] = NODE_TO_SERVERL;
  len = 2;
  memcpy(p+len,Data_usr.id,2);//id
  len = len + 2;
  Data_usr.len = 12;
  memcpy(p+len,&Data_usr.len ,1);//len
  len = len + 1;
  Data_usr.checksum = 0;
  memcpy(p+len,&Data_usr.checksum ,1);//check sum
  len = len + 1;
  memcpy(p+len,Data_usr.deepth  ,2);//deepth unit:m
  len = len + 2;
  memcpy(p+len,&Data_usr.deepth_percent  ,1);//deepth unit:percent
  len = len + 1;
  memcpy(p+len,Data_usr.vbat  ,2);//vbat 
  len = len + 2;
  memcpy(p+len,&Data_usr.status  ,1);
  len = len + 1;     
  /*memcpy(p+len,Data_usr.flow  ,4);
  len = len + 4;
  memcpy(p+len,&Data_usr.flow_status  ,1);
  len = len + 1;  */
  if(Save_Data.isUsefull == true)
  {
    
    memcpy(p+len,Save_Data.latitude ,11);
    len = len +11;
    memcpy(p+len,Save_Data.N_S ,2);
    len = len +2;
    memcpy(p+len,Save_Data.longitude ,12);
    len = len +12;
    memcpy(p+len,Save_Data.E_W ,2);
    len = len +2;
    
  }
  else
  {
    //memcpy(p+len,0 ,11+2+12+2);
    memset(p+len,0,11+2+12+2);
    len = len+11+2+12+2;
    
  }
  memcpy(p+4,&len ,1);//len
  
  Data_usr.checksum = xorCheck(p+2,len-2);
  memcpy(p+CHECKSUM_INDEX,&Data_usr.checksum ,1);
  
  while(len--)
    UART1_SendByte(p[i++]);
// delay_ms(1000);
}


void module_process()
{
#define VBAT_THRES    4900000          
  static unsigned char lcd_flag=1;
  if(wakeupcount)
    SIMCOM_Register_Network();
  
  sensor_adc(); 
  if(ExeIntFlag)//wake up by key
  {
    ExeIntFlag = 0;
    if(lcd_flag==1)
      lcd_flag = 0;
    else 
      lcd_flag=1;
  }
  if(lcd_flag==1)
  {
    lcd_flag = 0;
    lcd_process(1);  //  
  }
  else
  {
    lcd_process(0);
  }
  
  
  if(RtcWakeUp==1||Data_usr.status == 1)
  {
    
    if(Get_Network_status()==SIMCOM_NET_OK) 
    {
      
      RtcWakeUp = 0;
        data_tansmmit(); 
        while(timeout<250000)
        {
          if(uart1.isGetData == 1)
          {
            
            if(uart1.Uart_Buffer[0] == SERVER_TO_NODEH && uart1.Uart_Buffer[1] == SERVER_TO_NODEH && uart1.Uart_Buffer[4] == 0x01)	
            {
              uart1.isGetData =0;
              if(uart1.Uart_Buffer[6] == 0x6a)
              {
                FLOW_Ctrl(ON);
              }
              else if(uart1.Uart_Buffer[6] == 0xa6)
                FLOW_Ctrl(OFF);
            }
          }
          timeout++;
        }
        timeout = 0;
        wakeupcount = 0;
        // delay_ms(3000);
        set_NetStatus(SIMCOM_POWER_ON);
        GSM_HardwareInit(ON);
      stop:   lcd_process(0);
      EnterStopMode();
      RTC_Config(rtctime,ON);//10:9s 
      enableInterrupts();
      halt();	//enter stop mode
      //wakeupcount++;
      if(wakeupcount>=SETTING_COUNT)//定时发送数据
      {
        //wakeupcount = 0;
        disableInterrupts(); 
        RTC_Config(1,OFF);//10:9s
        HardwareInit();
        GSMInit();
        LCD_Congfig();
        module_prams_init();
        enableInterrupts();
        
      }
      else //周期检测
      {
        wakeupcount++;
        disableInterrupts(); 
        RTC_Config(1,OFF);//10:9s
        HardwareInit();
        LCD_Congfig();
        module_prams_init();
        enableInterrupts();
        // sensor_adc(); 
        if(Data_usr.status == 1)
        {
          GSMInit();
          wakeupcount=SETTING_COUNT;
        }
        else
        {
          goto stop;
        } 
      }
    }
    
 }
}
  