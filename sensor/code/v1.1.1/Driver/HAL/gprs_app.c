#include "stm8l15x.h"
#include <string.h>
#include "AT_CMD.h"
#include "bsp.h"
#include "string.h"
#include "gprs_app.h"
#include "stdlib.h"

#define Network_Thres    30
unsigned char one_net_key[]="*296832#571202701#json*";//296832：产品编号；571498701：鉴权码；json：脚本
unsigned char Establish_TCP_Connection[100]="AT+CIPSTART=\"TCP\",\"dtu.heclouds.com\",1811\r";
unsigned char	NET_STAUS=SIMCOM_NET_NOT;//网络链接状态信息标志
uint32_t  SIMCOM_TimeOut_Count;
unsigned char Get_Network_status(void)
{
  return NET_STAUS;
}
void SIMCOM_ReConnect()
{
  if(NET_STAUS!=SIMCOM_NET_NOT&&NET_STAUS!=SIMCOM_NET_OK) //控制系统超时机制，保证系统能够在超时后重新启动链接
    
    // if(NET_STAUS!=SIMCOM_NET_OK) //控制系统超时机制，保证系统能够在超时后重新启动链接
    SIMCOM_TimeOut_Count++;
  else if(NET_STAUS==SIMCOM_NET_OK)
    SIMCOM_TimeOut_Count=0;
  if(SIMCOM_TimeOut_Count>=SIMCOM_TIME_OUT)
  {
    SIMCOM_TimeOut_Count=0;
    NET_STAUS=SIMCOM_NET_NOT;
  }
  
}
#define debug  1
void set_NetStatus(unsigned char flag)
{
  NET_STAUS = flag;
  SIMCOM_TimeOut_Count = 0;
}

void SIMCOM_Register_Network()
{
  unsigned char *p;
  p=malloc(sizeof(unsigned char) * 64);
  
  switch(NET_STAUS)
  {
    
    
  case SIMCOM_NET_NOT:
    {     
    //SIMCOM_TimeOut_Count++;
     //if(SIMCOM_TimeOut_Count>=5000000)
      {
        SIMCOM_TimeOut_Count = 0;
        NET_STAUS = SIMCOM_NET_NOT;
        GSM_HardwareInit(ON);                   //复位重启
        // GSM_HardwareInit(ON);
        NET_STAUS=SIMCOM_POWER_ON;       //状态机复位
      }
    }
    break;
  case SIMCOM_POWER_ON://SIMCOM_READY_YES:
    {
      if (sendCommand("AT\r\n", "OK\r\n", 1500, 1) == Success)
        NET_STAUS = SIMCOM_READY_YES;
      else
      {
        NET_STAUS = SIMCOM_NET_NOT;
        SIMCOM_TimeOut_Count = 5000000;
      }
        
    }
    break;
  case SIMCOM_READY_YES:
    {
      if (sendCommand("ATE0\r\n", "OK\r\n", 500, 1) == Success)
        NET_STAUS = SIMCOM_CLOSE_ECHO;  
    }
    break;
    
  case SIMCOM_CLOSE_ECHO:
    {
      if (sendCommand("AT+CPIN?\r\n", "READY", 500, 1) == Success)
        NET_STAUS = SIMCOM_CARD_DET; 
    }
    break;
  case SIMCOM_CARD_DET:
    {
      if (sendCommand("AT+CGATT?\r\n", "+CGATT: 1", 1300, 1) == Success)
      {
        
        NET_STAUS=SIMCOM_GPRS_READY;
      }
    }
    break;
    
  case SIMCOM_GPRS_READY:
    {
      if (sendCommand("AT+CIPSHUT\r\n", "OK", 300, 1) == Success)
        NET_STAUS = SIMCOM_NET_CLOSE; 
    }
    break;
  case SIMCOM_NET_CLOSE:
    {
      if (sendCommand("AT+CIPMODE=1\r\n", "OK", 300, 1) == Success)
        NET_STAUS = SIMCOM_NET_PASS_THROUGH; 
      SIMCOM_TimeOut_Count = 0;
    }
    break;
  case SIMCOM_NET_PASS_THROUGH:
    {
      if (sendCommand((char*)Establish_TCP_Connection, "OK", 50000, 1) == Success)
      {
#if debug 
        NET_STAUS=SIMCOM_Connect_Platform;
#else
        NET_STAUS=SIMCOM_NET_OK;
#endif    
        
      } 
      
    }
    break;
  case SIMCOM_Connect_Platform:
    {
      static unsigned char tx_count;
      
      if (sendCommand((char*)one_net_key, "received", 140000, 1) == Success)
      {
        NET_STAUS=SIMCOM_NET_OK;          
      } 
      else
      {
        tx_count++;
        if(tx_count>=100)
        {
         // NET_STAUS=SIMCOM_NET_OK;
          tx_count = 0;        
        }
        
      }
    }
    break;
    
    
  case SIMCOM_NET_OK:
    {
      if (sendCommand(one_net_key, "CLOSED", 20, 0) == Success||
          sendCommand(one_net_key, "+CIPOPEN: 0,4", 20, 0) == Success||
            sendCommand(one_net_key, "NO CARRIER", 20, 0) == Success)
      {
        NET_STAUS=SIMCOM_NET_ERROR;
      }
    }
    break;
  case SIMCOM_NET_ERROR:
    {
      GSM_HardwareInit(ON);                   //复位重启
      // GSM_HardwareInit(ON);
      NET_STAUS=SIMCOM_POWER_ON;       //状态机复位
    }
    break;
  default : break;
  }
  SIMCOM_ReConnect();
  free(p);
}




