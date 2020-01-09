#include "Protocal.h"
#include "GSM.h"
#include "stdlib.h"
#include "bsp.h"
#include "stm8l15x_flash.h"
#include "uart_hal.h"
#include "hex_str.h"
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
  //hex2str( Data_usr.id,  p+len, 4 );  
  memcpy(p+len,Data_usr.id,4);
  len = len + 4+1;
  if(dataNo == GET_GNSS)
  {
    if(Save_Data.isUsefull == 1)  //gps数据有效  	
    { 
      memcpy(p+len,Save_Data.longitude,longitude_Length);///获取经度信息
      len = len + longitude_Length;
      memcpy(p+len,Save_Data.E_W,E_W_Length);//获取E/W
      len = len + E_W_Length;   
      
      memcpy(p+len,Save_Data.latitude,latitude_Length);//获取纬度信息
      len = len + latitude_Length;
      memcpy(p+len,Save_Data.N_S,N_S_Length);//获取N/S
      len = len + N_S_Length;
      
      memcpy(p+len,Save_Data.UTCTime,UTCTime_Length);//获取UTC时间
      len = len + UTCTime_Length;         
      
    }
    else
    {
      memset(p+len,0,longitude_Length);
      len = len + longitude_Length;
      memset(p+len,0,E_W_Length);
      len = len + E_W_Length;   
      
      memset(p+len,0,latitude_Length);
      len = len + latitude_Length;
      memset(p+len,0,N_S_Length);
      len = len + N_S_Length;
      
      memset(p+len,0,UTCTime_Length);
      len = len + UTCTime_Length;      
    }
  } 
  else
  { if(dataNo == HEART) 
  {
    p[len] = HEART;
    len = len +1;
  }
  } 
  
  memcpy(p+len,Data_usr.vbat,3);
  len = len +3; 
  if(get_lock_status()==1)
    p[len] = 0x31;
  else
    p[len] = 0x30;  
  len = len +1;
  p[6] = len-7;     
  while(len--)
    UART1_SendByte(p[len_i++]);
  
}
unsigned char uart_analy()
{
  
  
  if(uart.received_flag == 2)
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
unsigned char bat_status=0;
unsigned char battery_status()
{
  return bat_status;
}
void battery_quantity()
{
  float vbat_tmp;
  adcGet(VBAT_SENSE_CHANNEL);
  ADCdata = ADCdata*BATTERY_FACTOR;
  Data_usr.vbat[0]= ADCdata/1000/1000 + 0x30;
  Data_usr.vbat[1]= '.';  
  Data_usr.vbat[2]= ADCdata/1000/100%10 + 0x30; 
  vbat_tmp = ADCdata/1000/1000.0;
  if(vbat_tmp<=3)
    vbat_tmp = 0.05;
  else
    vbat_tmp = (vbat_tmp-BATTERY_REF)/BATTERY_FULL;  
  if(vbat_tmp>=1)
  {
    LED_Init(LEVEL3_LED,ON);
    bat_status =4;
  }
  else if(vbat_tmp>=0.3&&vbat_tmp<1)
  {
    LED_Init(LEVEL2_LED,ON);
    bat_status =3;
  }
  else if(vbat_tmp>=0.1)
  {
    LED_Init(LEVEL1_LED,ON);
    bat_status =2; 
    
  }
  else
  {
    LED_Init(LEVEL_ALL_LED,OFF);
    LED_Init(STATUS_LED,ON);
    bat_status=1;
  }
}

static unsigned char lockOnCount,lockOffCount,heartCount,getGPSCount,t3;

extern unsigned char wake_up_flag;
void gsm_module_pwr()
{
  if(battery_status()>1)
  {
    if(Get_Network_status()==SIMCOM_NET_INIT)  
    {
      gsm_power_state(ON);
      Set_Network_status(SIMCOM_NET_NOT);
      //Set_Network_status(SIMCOM_NET_ERROR);
    }  
  }
  else
  {
    if(Get_Network_status()!=SIMCOM_NET_INIT&&Get_Network_status()!=SIMCOM_NET_ERROR)  
    {
      gsm_power_state(OFF);
      Set_Network_status(SIMCOM_NET_ERROR);
    }
  }
//  if(wake_up_flag)
//  {
//    wake_up_flag = 0;
//    heartCount++;
//    if(heartCount>5)
//    {
//      heartCount =0;
//      gsm_power_state(OFF);
//      Set_Network_status(SIMCOM_NET_ERROR);
//      
//    }
//  }
}

void module_process()
{
  static uint32_t time_out;
  if(Get_Network_status()==SIMCOM_NET_OK)
  { 
    parseGpsBuffer();
    switch(uart_analy())
    {
    case LOCK_ON: lock_state(ON);lockOnCount++;break;
    case LOCK_OFF:lock_state(OFF);lockOffCount++; break;
    case HEART:data_process(HEART);heartCount++;t3=1; break; 
    case GET_GNSS:data_process(GET_GNSS);getGPSCount++;t3=1; break;
    default:; break;      
    }
    
    if(t3==0)
    {
      if(time_out>=1000)
      {
        time_out=0; 
        data_process(GET_GNSS);    
      }
      else
        time_out++;
      if(time_out%50==0&&time_out>0&&battery_status()>1)
        LED_Init(STATUS_LED,TOGGLE);    
    }
    
    else
      t3=0;
  }
  
  battery_quantity();
  gsm_module_pwr();
}

