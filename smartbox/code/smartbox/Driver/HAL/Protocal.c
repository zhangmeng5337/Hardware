#include "Protocal.h"
#include "GSM.h"
#include "stdlib.h"
#include "bsp.h"
#include "stm8l15x_flash.h"
#include "uart_hal.h"
_SaveData Save_Data;
Data_Stru Data_usr;
extern uart_stru uart;
extern uart_stru uart2;

void module_prams_init()
{
  Data_usr.id[0] =      0x03;  //id
  Data_usr.id[1] =      0x05;	//id
  Data_usr.id[2] =      0x01;  //id
  Data_usr.id[3] =      0x02;	//id  
  Data_usr.len =        0x07;		//data pack length not include header and id
  Data_usr.checksum =   0x00;
  Data_usr.status = 0;
  if(FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS)==0x5a)
  {;
  //Data_usr.Warn_Thres = FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+1)+
  //  FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+2)/10.0;	 
  }
  else
    ; 
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
unsigned char p[64]=0;
 unsigned char len_i;
void data_process(unsigned char dataNo)
{
  unsigned char len,i;
  
  len_i=0;
  p[0] = NODE_TO_SERVERH;
  p[1] = NODE_TO_SERVERL;
  len = 2;
  memcpy(p+len,Data_usr.id,4);
  len = len + 4;
  if(dataNo == GET_GNSS)
  {
    if(Save_Data.isUsefull == 1)  //gps数据有效  	
    { 
      memcpy(p+len+1,Save_Data.longitude,longitude_Length);///获取经度信息
      len = len + longitude_Length;
      memcpy(p+len+1,Save_Data.E_W,E_W_Length);//获取E/W
      len = len + E_W_Length;   
      
      memcpy(p+len+1,Save_Data.latitude,latitude_Length);//获取纬度信息
      len = len + latitude_Length;
      memcpy(p+len+1,Save_Data.N_S,N_S_Length);//获取N/S
      len = len + N_S_Length;
      
      memcpy(p+len+1,Save_Data.UTCTime,UTCTime_Length);//获取UTC时间
      len = len + UTCTime_Length;         
      
    }
    else
    { 
      memcpy(p+len+1,0,longitude_Length);
      len = len + longitude_Length;
      memcpy(p+len+1,0,E_W_Length);
      len = len + E_W_Length;   
      
      memcpy(p+len+1,0,latitude_Length);
      len = len + latitude_Length;
      memcpy(p+len+1,0,N_S_Length);
      len = len + N_S_Length;
      
      memcpy(p+len+1,0,UTCTime_Length);
      len = len + UTCTime_Length;         
      
    } 
    
    memcpy(p+len+1,Data_usr.vbat,3);
    len = len +3; 
    if(get_lock_status()==1)
      p[len+1] = 0x31;
    else
      p[len+1] = 0x32;  
    len = len +2;
      p[+6] = len;     
    while(len--)
      UART1_SendByte(p[len_i++]);
  }
}
unsigned char uart_analy()
{
  
  
  if(uart.received_flag == 1)
  {
    if(uart.rxbuffer[0] ==SERVER_TO_NODEH&&uart.rxbuffer[1] ==SERVER_TO_NODEL&&
       uart.rxbuffer[2] ==Data_usr.id[0]&&uart.rxbuffer[3] ==Data_usr.id[1]&& 
         uart.rxbuffer[4] ==Data_usr.id[2]&&uart.rxbuffer[5] ==Data_usr.id[3])
    {
      uart.received_flag = 0;
      
      return uart.rxbuffer[6];
    }
    else
    {
    uart.received_flag = 0;
    return 1;
    }
      
    
  }
  else
    return 1;
}


unsigned char parseGpsBuffer()
{
  char *subString;
  char *subStringNext;
  unsigned char i = 0,res;
  res = 1;
  if (Save_Data.isGetData  == 1)
  {
    Save_Data.isGetData = 0;
    for (i = 0 ; i <= 6 ; i++)
    {
      if (i == 0)
      {
        if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
          res= 1;	//解析错误
      }
      else
      {
        subString++;
        if ((subStringNext = strstr(subString, ",")) != NULL)
        {
          char usefullBuffer[2]; 
          switch(i)
          {
          case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
          case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
          case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
          case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
          case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
          case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W
          
          default:break;
          }
          
          subString = subStringNext;
          Save_Data.isParseData = 1;
          if(usefullBuffer[0] == 'A')
            Save_Data.isUsefull = 1;
          else if(usefullBuffer[0] == 'V')
            Save_Data.isUsefull = 0;
          res=  0;
        }
        else
        {
          res=  1;	//解析错误
        }
      }
    }
    memset(uart2.rxbuffer,0,BUFFERSIZE);
  }
    if (uart2.received_flag  == 2)
    {
      uart2.received_flag  =0;
     memset(uart2.rxbuffer,0,BUFFERSIZE);
    }
  return res;
}
extern uint32_t ADCdata ;
void battery_quantity()
{
  float vbat_tmp;
  adcGet(VBAT_SENSE_CHANNEL);
  ADCdata = ADCdata*BATTERY_FACTOR;
  Data_usr.vbat[0]= ADCdata/1000/1000 + 0x30;
  Data_usr.vbat[1]= '.';  
  Data_usr.vbat[2]= ADCdata/1000/100%10 + 0x30; 
  vbat_tmp = ADCdata/1000/1000.0;
  vbat_tmp = vbat_tmp/BATTERY_FULL;  
  if(vbat_tmp>=1)
  {
    LED_Init(LEVEL3_LED,ON);
  }
  else if(vbat_tmp>=0.3&&vbat_tmp<1)
  {
    LED_Init(LEVEL2_LED,ON);
  }
  else 
  {
    LED_Init(LEVEL1_LED,ON);
  }
}
unsigned char tt,tt1,tt2,t3;
uint32_t time_out;
void module_process()
{

  if(Get_Network_status()==SIMCOM_NET_OK)
  { parseGpsBuffer();
    switch(uart_analy())
    {
    case LOCK_ON: lock_state(ON);tt++;break;
    case LOCK_OFF:lock_state(OFF);tt1++; break;
    case GET_GNSS:data_process(GET_GNSS);tt2++;t3=1; break; 
    }
    memset(uart.rxbuffer,0,BUFFERSIZE);
    if(t3==0)
    {
      if(time_out>=1000)
      {
         time_out=0; 
         data_process(GET_GNSS);    
      }
      else
        time_out++;
      if(time_out%50==0&&time_out>0)
        LED_Init(STATUS_LED,TOGGLE);    
    }

    else
      t3=0;
    
   
  }
  else
  {
    LED_Init(STATUS_LED,ON);  
  }
  battery_quantity();

}

