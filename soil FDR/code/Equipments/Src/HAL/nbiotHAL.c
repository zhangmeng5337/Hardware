#include <string.h>
#include "AT_CMD.h"
#include "nbiotHW.h"
#include "string.h"
#include "nbiotHAL.h"
#include "stdlib.h"



#define Network_Thres    30
unsigned char one_net_key[]="*296832#571202701#json*";//296832����Ʒ��ţ�571498701����Ȩ�룻json���ű�
unsigned char Establish_TCP_Connection[100]="AT+CIPSTART=\"TCP\",\"dtu.heclouds.com\",1811\r";
unsigned char	NET_STAUS=SIMCOM_NET_NOT;//��������״̬��Ϣ��־
uint32_t  SIMCOM_TimeOut_Count;
unsigned char Get_Network_status(void)
{
  return NET_STAUS;
}
void ServerIP_Pack(unsigned char *p)
{
    unsigned char i;
	i = 0;
	memcpy(Establish_TCP_Connection,TCP_ConnectCMD,sizeof(TCP_ConnectCMD)-1);
	i = sizeof(TCP_ConnectCMD)-1;

	Establish_TCP_Connection[i++]=p[0];
	Establish_TCP_Connection[i++] = '.';
	
	Establish_TCP_Connection[i++]=p[1];
	Establish_TCP_Connection[i++] = '.';
	
	Establish_TCP_Connection[i++]=p[2];
	Establish_TCP_Connection[i++] = '.';
	
	Establish_TCP_Connection[i++]=p[3];
	Establish_TCP_Connection[i++] = '.';
	
	Establish_TCP_Connection[i++] = '\"';
	Establish_TCP_Connection[i++] = ',';
	
	Establish_TCP_Connection[i++] = p[4];
	
	Establish_TCP_Connection[i++] = '\r';
	Establish_TCP_Connection[i++] = '"';
 
}
void SIMCOM_ReConnect()
{
  if(NET_STAUS!=SIMCOM_NET_NOT&&NET_STAUS!=SIMCOM_NET_OK) //����ϵͳ��ʱ���ƣ���֤ϵͳ�ܹ��ڳ�ʱ��������������
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
//static uint32_t count = 0;
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
        SIMCOM_TimeOut_Count = 0;
        NET_STAUS = SIMCOM_NET_NOT;
        nbiot_HardwareInit(ON);                   //��λ����
        NET_STAUS=SIMCOM_POWER_ON;       //״̬����λ
    }
    break;
  case SIMCOM_POWER_ON://SIMCOM_READY_YES:
    {
      if (sendCommand("AT\r\n", "OK\r\n", 5000, 1) == Success)
        NET_STAUS = SIMCOM_READY_YES;
      else
      {
        NET_STAUS = SIMCOM_NET_NOT;
        
      }
        
    }
    break;
  case SIMCOM_READY_YES:
    {
      if (sendCommand("ATE0\r\n", "OK\r\n", 5000, 1) == Success)
        NET_STAUS = SIMCOM_CLOSE_ECHO;  
    }
    break;
    
  case SIMCOM_CLOSE_ECHO:
    {
      if (sendCommand("AT+CPIN?\r\n", "READY", 5000, 1) == Success)
        NET_STAUS = SIMCOM_CARD_DET; 
    }
    break;
  case SIMCOM_CARD_DET:
    {
      if (sendCommand("AT+CGATT?\r\n", "+CGATT: 1", 1300000, 1) == Success)
      {
        
        NET_STAUS=SIMCOM_GPRS_READY;
      }
    }
    break;
    
  case SIMCOM_GPRS_READY:
    {
      if (sendCommand("AT+CIPSHUT\r\n", "OK", 3000, 1) == Success)
        NET_STAUS = SIMCOM_NET_CLOSE; 
    }
    break;
  case SIMCOM_NET_CLOSE:
    {
      if (sendCommand("AT+CIPMODE=1\r\n", "OK", 3000, 1) == Success)
        NET_STAUS = SIMCOM_NET_PASS_THROUGH; 
      SIMCOM_TimeOut_Count = 0;
    }
    break;
  case SIMCOM_NET_PASS_THROUGH:
    {
      if (sendCommand(Establish_TCP_Connection, "OK", 500000, 1) == Success)
      {

        NET_STAUS=SIMCOM_NET_OK;
     NET_STAUS=SIMCOM_Connect_Platform;
      } 
      
    }
    break;
  case SIMCOM_Connect_Platform:
    {
      static unsigned char tx_count;
      
      if (sendCommand(one_net_key, "received", 140000, 1) == Success)
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
      nbiot_HardwareInit(ON);                   //��λ����
      NET_STAUS=SIMCOM_POWER_ON;       //״̬����λ
    }
    break;
  default : break;
  }
  SIMCOM_ReConnect();
  free(p);
}
