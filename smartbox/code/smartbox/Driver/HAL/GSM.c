#include "stm8l15x.h"
#include <string.h>
#include "AT_CMD.h"
#include "GSM.h"
#include "bsp.h"
#include "string.h"
#include "GSM.h"
#include "stdlib.h"
#include "uart_hal.h"
#include "uart1.h"
#define debug  1
extern uart_stru uart;
unsigned char one_net_key[]="*296832#571498702#json*";//284261：产品编号；abab：鉴权码；json：脚本
unsigned char Establish_TCP_Connection[100]="AT+CIPSTART=\"TCP\",\"dtu.heclouds.com\",1811\r";
unsigned char* server_ip=Establish_TCP_Connection;
unsigned char NET_STAUS = 0;
//找出temp在target的位置
int FindIndex(char *target,char temp)
{
  int i= 0;
  if(target ==NULL)
  {
    //  printf("搜索词为空...");
    return 0;
  }
  for(i = strlen(target) -1; i>=0; i--)
  {
    if(target[i] == temp)
    {
      return i;
    }
  }
  return -1;  //未找到字符匹配位置
  
}
/************************************************************
Function:  string match or not that return froom LTE module
Name:      AT_cmd_ack(unsigned char *src,unsigned char *dest)
Parameter: 1:string match, 0: string not match
************************************************************/
unsigned char AT_cmd_ack(unsigned char *src,unsigned char *ack)
{
  int i= 0,j = 0;
  int srclen = strlen((const char *)src);
  int tarlen=strlen((const char *)ack);
  int temp  =0;
  int index = -1;
  if(src ==NULL || ack ==NULL)
  {
    return 1;
  }
  
  while(i<srclen)
  {
    //循环条件
    if(src[i] == ack[j])
    {
      if(j==tarlen-1)
      {
        // printf("匹配成功...");
        return 0;
      }
      i++;
      j++;
    }
    else
    {
      //发现不相等位置
      temp = tarlen - j + i;  //字符串后面的第一个字符位置
      index = FindIndex((char *)ack,src[temp]);
      if(index==-1)
      {
        //未找到位置，后移
        i = temp+1;
        j = 0;
      }
      else
      {
        //找到位置
        i = temp-index;
        j = 0;
      }
    }
  }
  //  printf("匹配失败..");
  return 1;
}


unsigned char at_cmd_ok(unsigned char *src)
{
  if(AT_cmd_ack(src,(unsigned char *)Respond_OK)==1)//reset
  {
    return 0;
  }
  return 1;
  
}
void Send_Comm(unsigned char* comm,unsigned short len)
{

	USART_SenByte(comm,len);

}
unsigned char SIMCOM_GetStatus(unsigned char *p,unsigned char *ack,uint32_t waittime)
{
  unsigned char res;
  res=1;
  if(*p!=NULL)
    Send_Comm((unsigned char*)p,strlen((const char*)p));
  
  if(waittime)		//需要等待应答
  {
    while(--waittime)	//等待倒计时
    {
      if(uart.received_flag ==1)//接收到期待的应答结果
      {
        if(AT_cmd_ack(uart.rxbuffer,(unsigned char*)ack)==0)
        {
          
          memset(uart.rxbuffer,0,BUFFERSIZE); 
          res=0;
          break;//得到有效数据
        }    
      }
    }
  }
  if(waittime==0){
    res=1;
  } 
  
  uart.received_flag = 0;
  
  return res;
  
}
uint32_t  SIMCOM_TimeOut_Count;
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
    NET_STAUS=SIMCOM_NET_ERROR;
  }
  
}
signed char  SIMCOM_Get_TCP_Staus(unsigned int waittime)
{
  unsigned char res=0;
  //Send_Comm((const char*)Establish_TCP_Connection,strlen((const char*)Establish_TCP_Connection));
  
  if(waittime)		//需要等待应答
  {
    while(--waittime)	//等待倒计时
    {
      //delay_ms(5);
      if(uart.received_flag==1)//接收到期待的应答结果
      {
        if(AT_cmd_ack(&uart.rxbuffer[0],(unsigned char*)TCP_Closed)
           ||AT_cmd_ack(&uart.rxbuffer[0],(unsigned char*)Respond_No_Carrier)
             ||AT_cmd_ack(&uart.rxbuffer[0],(unsigned char*)TCP_ERROR))
        {
          res=0;
          
          break;//得到有效数据
          
        }
        
        //res=0;TIMEOUT
      }
    }
    if(waittime==0){res=1;uart.received_flag=0;}
  }
  return res;
}
void SIMCOM_Register_Network()
{
  unsigned char *p;
  p=malloc(sizeof(unsigned char) * 64);
  
  switch(NET_STAUS)
  {                                                                                                                                                                                         
  case SIMCOM_NET_NOT:
    {      
      *p = 0;
      if(SIMCOM_GetStatus((unsigned char*)Test,(unsigned char*)Respond_OK,50)==0)//at 
      {  
        NET_STAUS=SIMCOM_READY_YES;
        memset(uart.rxbuffer,0,BUFFERSIZE);
      }
    }break;
  case SIMCOM_READY_YES://SIMCOM_READY_YES:
    {
      if( SIMCOM_GetStatus((unsigned char*)Echo_OFF,(unsigned char*)Respond_OK,50)==0)//close echo
      {  
        NET_STAUS=SIMCOM_ATE0;
        
        memset(uart.rxbuffer,0,BUFFERSIZE);
      }
    }
    break;
  case SIMCOM_ATE0:
    {
      *p = 0;
      if(SIMCOM_GetStatus((unsigned char*)p,(unsigned char*)SMS_Ready,1000)==0)//wait module init complete
      {  
        NET_STAUS=SIMCOM_Network_Intensity_READY;
        memset(uart.rxbuffer,0,BUFFERSIZE);
        //delay_ms(3000);
      }
    }
    break;
    
    
  case SIMCOM_Network_Intensity_READY:
    {        
      if(SIMCOM_GetStatus((unsigned char*)GPRS_Attached_State,(unsigned char*)Respond_Attached_Ok,50000)==0)
      {
        
        NET_STAUS=SIMCOM_GPRS_READY;
        memset(uart.rxbuffer,0,BUFFERSIZE);
      }
    }
    break;
    
  case SIMCOM_GPRS_READY:
    {
      if(SIMCOM_GetStatus((unsigned char*)AT_SHUNT,(unsigned char*)Respond_OK,3000)==0)
      {
        NET_STAUS=SIMCOM_NET_PORT_CLOSE;   
        memset(uart.rxbuffer,0,BUFFERSIZE);
      }
    }
    break;
  case SIMCOM_NET_PORT_CLOSE:
    {
      if(SIMCOM_GetStatus((unsigned char*)Pass_Through,(unsigned char*)Respond_OK,1000)==0)
      {
        NET_STAUS=SIMCOM_NET_TRANSPARENT;
        memset(uart.rxbuffer,0,BUFFERSIZE);
      }
    }
    break;
  case SIMCOM_NET_TRANSPARENT:
    {
      if(SIMCOM_GetStatus((unsigned char*)server_ip,(unsigned char*)Respond_TCP_Connect,60000)==0)
      {
#if debug
        
        NET_STAUS=SIMCOM_NET_TRANSPARENT; NET_STAUS=SIMCOM_Connect_Platform;
#else
        NET_STAUS=SIMCOM_NET_OK;
#endif
        memset(uart.rxbuffer,0,BUFFERSIZE);
      }
    }
    break;
  case SIMCOM_Connect_Platform:
    {
      if(SIMCOM_GetStatus((unsigned char*)one_net_key,(unsigned char*)platform_received,220000)==0)
      {
        NET_STAUS=SIMCOM_NET_OK;
        
        memset(uart.rxbuffer,0,BUFFERSIZE);
      }
    }
    break;
    
    
  case SIMCOM_NET_OK:
    {
      if(SIMCOM_Get_TCP_Staus(40)==1)
      {      
        if(SIMCOM_GetStatus((unsigned char*)Quit_transparent,(unsigned char*)Respond_OK,5000)==0)
        {
          NET_STAUS=SIMCOM_NET_ERROR;
          memset(uart.rxbuffer,0,BUFFERSIZE);
        }
      }
    }
    break;
  case SIMCOM_NET_ERROR:
    {
      // GSM_HardwareInit(ON);                   //复位重启
      //gsm_power_state(OFF);
      //GSM_HardwareInit(ON);
      NET_STAUS=SIMCOM_NET_NOT;       //状态机复位
    }
    break;
  default : break;
  }
  //SIMCOM_ReConnect();
  free(p);
}
unsigned char Get_Network_status()
{
  return NET_STAUS;
}
unsigned char gps_flag;
void gps_test()
{
  if(gps_flag==0)
  {
    gps_flag = 1;
    gnss_state(ON);
    
  }
  delay_ms(2000);
  Send_Comm2((unsigned char*)GNSS_PWR,strlen((const char*)GNSS_PWR));
}

