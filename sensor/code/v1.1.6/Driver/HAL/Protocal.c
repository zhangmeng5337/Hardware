#include "Protocal.h"
#include "stdlib.h"
#include "bsp.h"
#include "gprs_app.h"
#include "stm8l15x_flash.h"
#include "gps.h"
#include "lcd_hal.h"
extern _uart uart1;
extern _SaveData Save_Data;
extern  unsigned char ExeIntFlag;
extern unsigned char RtcWakeUp;
Data_Stru Data_usr;
Flow_stru Flow;

unsigned char wakeup_flag;
uint32_t timeout;
#define SETTING_THRES    0.2//���μ����ֵ��λ��m
unsigned int rtctime=100;//10:9s���޸�����ʱ��
float warn_setting = 0.5;//��ֵ����
unsigned int wakeupcount=1;
unsigned int SETTING_COUNT;
void reapte_time()
{ 
  RtcWakeUp = 1;
  rtctime=100;//���ڼ��ʱ��
  SETTING_COUNT=100; 
  wakeupcount = SETTING_COUNT;//��ʱ��������ʱ�䣬wakeupcount=n*rtctime;
}
void module_prams_init()
{
  
  wakeup_flag = 1;
  
  
  Data_usr.id[0] = 0x01;  //id
  Data_usr.id[1] = 0x02;	//id
  Data_usr.len = 0x07;		//data pack length not include header and id
  Data_usr.checksum = 0x00;
  Data_usr.status = 0;
  Data_usr.deepth_calibration = SENSOR_FACTOR;
  Data_usr.Warn_Thres =warn_setting;
  
  if(motor_ctrl(CLOSE)==0)
    Data_usr.gate_status = 0xa6;
  else
    Data_usr.gate_status = 0x6a;		
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
    Data_usr.Warn_Thres = HEIGHT_RANGE;	
  if(Data_usr.Warn_Thres<=0)
    Data_usr.Warn_Thres = HEIGHT_RANGE;
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
    adc_tmp = (adcGet(ADC_SENSOR_CHANNEL,100));
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


void sensor_adc()
{
  float  adc_tmp,adc_tmp2,adc_tmp3;
  
  adc_tmp = (adcGet(ADC_BAT_CHANNEL,50));
  adc_tmp = adc_tmp*2.0;
  Data_usr.vbatf = adc_tmp;
  Data_usr.vbat[0] = (unsigned char)(adc_tmp/1000/1000);
  Data_usr.vbat[1] =((unsigned char)(adc_tmp/1000/100)%10);
  
  
  if(Data_usr.calibration_done ==0||ExeIntFlag==2)
  {
    ExeIntFlag = 0;
    OilCalibration();
  }
  else
  {
    adc_tmp2 = (adcGet(ADC_SENSOR_CHANNEL,50));
    adc_tmp2 = adc_tmp2/ VOLTAGE_FACTOR;
    
  }
  
  adc_tmp2 = adc_tmp2-Data_usr.voltage_calibration_value; 
  adc_tmp2 = adc_tmp2*Data_usr.deepth_calibration;
  Data_usr.deep_f = adc_tmp2;
  if(Data_usr.deep_f<=MIN_OFFSET)
    
    Data_usr.deep_f = 0;
  
  if((Data_usr.deep_f*1.05)>Data_usr.Warn_Thres)
    Data_usr.status = 1;
  else
  {
    Data_usr.status = 0;
  }
  if((Data_usr.deep_f_last-Data_usr.deep_f)>=SETTING_THRES||
     (Data_usr.deep_f-Data_usr.deep_f_last)>=SETTING_THRES)
  {
    Data_usr.status = 1;
  }
  Data_usr.deep_f_last=Data_usr.deep_f;
  if(Data_usr.deep_f>HEIGHT_RANGE)
    adc_tmp2 = HEIGHT_RANGE;
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
  unsigned char deepth_mm;
  unsigned int tmp;
  
  tmp=(uint32_t)(Data_usr.deep_f*1000.0);
  deepth_mm = (unsigned char)(tmp/1000);
  memcpy(p+len,&deepth_mm ,1);//deepth unit:m
  len = len + 1;
  deepth_mm = tmp%1000/100;
  memcpy(p+len,&deepth_mm ,1);//deepth unit:m
  len = len + 1;
  deepth_mm = tmp%100/10;
  memcpy(p+len,&deepth_mm ,1);//deepth unit:m
  len = len + 1;
  deepth_mm = tmp%10;
  memcpy(p+len,&deepth_mm,1);//deepth unit:m
  len = len + 1;
  
  //memcpy(p+len,Data_usr.deepth  ,2);//deepth unit:m
  // len = len + 2;
  memcpy(p+len,&Data_usr.deepth_percent  ,1);//deepth unit:percent
  len = len + 1;
  memcpy(p+len,Data_usr.vbat  ,2);//vbat 
  len = len + 2;
  memcpy(p+len,&Data_usr.status  ,1);
  len = len + 1;     
  memcpy(p+len,Data_usr.flow  ,4);
  len = len + 4;
  memcpy(p+len,&Data_usr.gate_status  ,1);//gate
  len = len + 1;
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
  i=0;
  while(len--)
    UART1_SendByte(p[i++]);  
  delay_ms(1000);
}


void module_process()
{
  //#define VBAT_THRES    4900000          
  static unsigned char lcd_flag=1;
process:
  if(wakeupcount>=SETTING_COUNT)//����ʱ�䵽����ע��
  {
    SIMCOM_Register_Network();
  }
  else
  {
    RtcWakeUp==0;
    if(lcd_flag==0)
      goto stop;
  }
  if(lcd_flag==1||Get_Network_status()==SIMCOM_NET_OK)//�ɼ�����
  {
    sensor_adc();
  }
  if(lcd_flag==1)//�ɼ�����
  {
    //sensor_adc();  
    if(lcd_flag==1)//��Ļ���ѱ�־λ
    {
      if(wakeupcount>=SETTING_COUNT)
      {
        if(Get_Network_status()==SIMCOM_NET_OK)
          lcd_process(3);
        else
          lcd_process(4);    
      }
      else
      {
        lcd_process(5); 
      }
      
      lcd_process(1);  // 
      if(RtcWakeUp==0)
      {
        delay_ms(5000);
        goto stop;
      }
      
    }
    else
    {
      lcd_process(0);
      if(RtcWakeUp==0)
      {
        delay_ms(5000);
        goto stop;
      }
    }
  }
  else
    lcd_process(0);
  
  
  
  if(RtcWakeUp==1||Data_usr.status == 1)
  {
    
    if(Get_Network_status()==SIMCOM_NET_OK) 
    {
       static unsigned char status_reapte;
      if(status_reapte == 1)
      {
        if(Data_usr.status == 1)
          Data_usr.status = 0;
      }
      if(timeout == 0||Data_usr.status == 1)
      {
          status_reapte = 1;
          data_tansmmit(); 
      } 
      Data_usr.status = 2;
      if(timeout<10)
      {
        if(ExeIntFlag)
        {
          timeout = 0;
          goto process2;
        }
        if(uart1.isGetData == 1)
        {
          
          if(uart1.Uart_Buffer[0] == SERVER_TO_NODEH && uart1.Uart_Buffer[1] == SERVER_TO_NODEH && uart1.Uart_Buffer[4] == 0x01)	
          {
            uart1.isGetData =0;
            if(uart1.Uart_Buffer[6] == 0x6a)//������
            {
              if(motor_ctrl(OPEN)==0)
                Data_usr.gate_status=0x6a;
              else
                Data_usr.gate_status=0x6b;                
              
            }
            else if(uart1.Uart_Buffer[6] == 0xa6)//�ط���
              if(motor_ctrl(CLOSE)==0)
                Data_usr.gate_status = 0xa6;
              else
                Data_usr.gate_status=0x6b; 
          }
        }
        timeout++;
        goto  process;
      }
      status_reapte = 0;
      RtcWakeUp = 0;
      timeout = 0;
      wakeupcount = 0;
      set_NetStatus(SIMCOM_POWER_ON);
      GSM_HardwareInit(OFF);
    stop:   lcd_process(0);
    lcd_flag=0;
    RtcWakeUp = 0;
    GPIO_SetBits(PORT_LED, PIN_LED);
    EnterStopMode();
    RTC_Config(rtctime,ON);//10:9s 
    enableInterrupts();
    halt();	//enter stop mode
    
    
    disableInterrupts(); 
    RTC_Config(1,OFF);//10:9s
    HardwareInit();
    LCD_Congfig();
    module_prams_init();
    enableInterrupts();
    
    if(ExeIntFlag)//wake up by key
    {
      wakeup_flag=1;
      ExeIntFlag = 0;
      if(lcd_flag==1)
        lcd_flag = 0;
      else 
        lcd_flag=1;
    }
    else//wake up by rtc
    {
      wakeup_flag = 1;
      wakeupcount++;
      
      if(wakeupcount>=SETTING_COUNT)//��ʱ��������
      {
        set_NetStatus(SIMCOM_POWER_ON);
        GSMInit(); 
        delay_ms(1500);
      }
      else //���ڼ��
      {
        
        sensor_adc(); 
        if(Data_usr.status == 1)
        {
          set_NetStatus(SIMCOM_POWER_ON);
          GSMInit();
          wakeupcount=SETTING_COUNT;
          delay_ms(1500);
        }
        else
        {
          if(lcd_flag==0)
            goto stop;
        } 
      }   
    }
    
    
    }

    
  }
process2: if(ExeIntFlag)//wake up by key
{
  ExeIntFlag = 0;
  if(lcd_flag==1)
    lcd_flag = 0;
  else 
    lcd_flag=1;
}
}

