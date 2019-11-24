#include "stm8l15x.h"
#include <string.h>
#include "AT_CMD.h"
#include "GSM.h"
#include "bsp.h"
#include "string.h"
#include "GSM.h"
#include "stdlib.h"

#define Network_Thres    30



//unsigned char Establish_TCP_Connection[100]="AT+CIPSTART=\"TCP\",\"15k801n729.iok.la\",36732\r";
//unsigned char one_net_key[]="*284261#abab#json* ";
unsigned char one_net_key[]="*296832#571498615#json*";//284261：产品编号；abab：鉴权码；json：脚本
unsigned char Establish_TCP_Connection[100]="AT+CIPSTART=\"TCP\",\"dtu.heclouds.com\",1811\r";





unsigned char	NET_STAUS=SIMCOM_NET_NOT;//网络链接状态信息标志
uint32_t  SIMCOM_TimeOut_Count;
unsigned char Network_Intens;
unsigned char* server_ip=Establish_TCP_Connection;
extern Uart_Types uart_str;

void reconfig_IP(unsigned char *pbuff,unsigned int len)
{
  unsigned int i;
  for(i=0;i<len;len++)
  {
    Establish_TCP_Connection[i]=pbuff[i];
  }
}

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
    return 0;
  }
  
  while(i<srclen)
  {
    //循环条件
    if(src[i] == ack[j])
    {
      if(j==tarlen-1)
      {
        // printf("匹配成功...");
        return 1;
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
  return 0;
}


unsigned char at_cmd_ok(unsigned char *src)
{
  if(AT_cmd_ack(src,(unsigned char *)Respond_OK)==1)//reset
  {
    return 1;
  }
  return 0;
  
}
unsigned char SIMCOM_GetStatus(unsigned char *p,unsigned char *ack,uint32_t waittime)
{
  unsigned char res=0;
  if(*p!=NULL)
    Send_Comm((unsigned char*)p,strlen((const char*)p));
  
  if(waittime)		//需要等待应答
  {
    while(--waittime)	//等待倒计时
    {
      //delay_ms(10);
      if(uart_str.receive_flag==1)//接收到期待的应答结果
      {
        if(AT_cmd_ack(uart_str.UsartReceiveData,(unsigned char*)ack))
        {
          
          uart_str.real_index = 0;
          res=1;
          uart_str.receive_flag = 0;
          break;//得到有效数据
        }
        
        //memset(uart_str.UsartReceiveData,0,256);
        
      }
    } 
    // uart_str.real_index = 0;
    
  }
  if(waittime==0){
    res=0;
    //uart_str.receive_flag=0;
    //memset(uart_str.UsartReceiveData,0,256);
  }  
  return res;
  
}
unsigned char  SIMCOM_Get_InitReady_Staus(unsigned int waittime)
{
  unsigned char res=0;
  //Send_Comm((const char*)Normal_Mode,strlen((const char*)Normal_Mode));
  if(waittime)		//需要等待应答
  {
    while(--waittime)	//等待倒计时
    {
      delay_ms(10);
      if(uart_str.receive_flag==1)//接收到期待的应答结果
      {
        if(AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)Respond_Start))
        {
          uart_str.receive_flag=0;
          res=1;
          break;//得到有效数据
        }
      }
    }
    if(waittime==0)
      res=0;
  }
  return res;
}


signed char  SIMCOM_Get_NetOpen_Staus(unsigned int waittime)
{
  unsigned char res=0;
  Send_Comm((unsigned char*)Net_Open,strlen((const char*)Net_Open));
  
  if(waittime)		//需要等待应答
  {
    while(--waittime)	//等待倒计时
    {
      delay_ms(10);
      if(uart_str.receive_flag==1)//接收到期待的应答结果
      {
        if(AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)Respond_Network_Open2)||
           AT_cmd_ack(&uart_str.UsartReceiveData [uart_str.real_index],(unsigned char*)Respond_Network_Open))
        {
          res=1;
          uart_str.receive_flag=0;
          break;//得到有效数据
          
        }
        
      }
      if(waittime==0)res=0;
    }
    
  }
  return res;
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
      if(uart_str.receive_flag==1)//接收到期待的应答结果
      {
        if(AT_cmd_ack(&uart_str.UsartReceiveData[0],(unsigned char*)TCP_Closed)
           ||AT_cmd_ack(&uart_str.UsartReceiveData[0],(unsigned char*)Respond_No_Carrier)
             ||AT_cmd_ack(&uart_str.UsartReceiveData[0],(unsigned char*)TCP_ERROR))
        {
          res=1;
          
          break;//得到有效数据
          
        }
        
        //res=0;TIMEOUT
      }
    }
    if(waittime==0){res=0;uart_str.receive_flag=0;}
  }
  return res;
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
    NET_STAUS=SIMCOM_NET_ERROR;
  }
  
}

#define debug  1
void test()
{
  Send_Comm((unsigned char*)Test,strlen((const char*)Test));
}

void SIMCOM_Register_Network()
{
  unsigned char *p;
  p=malloc(sizeof(unsigned char) * 64);
  
  switch(NET_STAUS)
  {
    
    
  case SIMCOM_NET_NOT:
    {       *p = 0;
    if(SIMCOM_GetStatus((unsigned char*)Test,(unsigned char*)Respond_OK,20000)==1)
    {  
      NET_STAUS=SIMCOM_READY_YES;
      //SIMCOM_ReConnect();
      memset(uart_str.UsartReceiveData,0,buffer_size);
    }
    
    }
    break;
  case SIMCOM_READY_YES://SIMCOM_READY_YES:
    {
      if( SIMCOM_GetStatus((unsigned char*)Echo_Dis,(unsigned char*)Respond_OK,5000)==1)
      {  
        NET_STAUS=SIMCOM_ATE0;
        
        memset(uart_str.UsartReceiveData,0,buffer_size);
      }
    }
    break;
  case SIMCOM_ATE0:
    {
      *p = 0;
      if(SIMCOM_GetStatus((unsigned char*)p,(unsigned char*)SMS_Ready,10000)==1)
      {  
      	NET_STAUS=SIMCOM_Network_Intensity_READY;
      	memset(uart_str.UsartReceiveData,0,buffer_size);
        delay_ms(3000);
      }
      //server_ip_tmp=Establish_TCP_Connection;
    }
    break;
    //  case SIMCOM_SIM_OK:
    //    {
    //      if(SIMCOM_GetStatus((unsigned char*)Normal_ModeT,(unsigned char*)Respond_Network_Normal_ModeT,5000)==1)
    //      { //NET_STAUS=SIMCOM_SIM_OK;
    //        
    //        NET_STAUS=SIMCOM_Network_Normal_ModeT;
    //        memset(uart_str.UsartReceiveData,0,buffer_size);
    //      }
    //    }
    //    break;
    //  case SIMCOM_Network_Normal_ModeT:
    //    {
    //      if(SIMCOM_GetStatus((unsigned char*)Network_Intensity,(unsigned char*)Respond_Network_Intensity,160000)==1)
    //      { 
    //        NET_STAUS=SIMCOM_Network_Intensity_READY;
    //        memset(uart_str.UsartReceiveData,0,buffer_size);
    //      }
    //       // NET_STAUS=SIMCOM_Network_Normal_ModeT;
    //    }
    //    break;
    
    
  case SIMCOM_Network_Intensity_READY:
    {
      
      // if(SIMCOM_GetStatus((unsigned char*)IPR_SET,(unsigned char*)Respond_OK,80000)==1)
      
      if(SIMCOM_GetStatus((unsigned char*)GPRS_Attached_State,(unsigned char*)Respond_Attached_Ok,50000)==1)
      {
        
        NET_STAUS=SIMCOM_GPRS_READY;
        memset(uart_str.UsartReceiveData,0,buffer_size);
      }
    }
    break;
    
    
    
    
  case SIMCOM_GPRS_READY:
    {
      //  if(SIMCOM_GetStatus((unsigned char*)IPR_GET,(unsigned char*)Respond_IPR,80000)==1)
      if(SIMCOM_GetStatus((unsigned char*)AT_SHUNT,(unsigned char*)Respond_OK,30000)==1)
      {
        NET_STAUS=SIMCOM_NET_PORT_CLOSE;
        //   NET_STAUS=SIMCOM_GPRS_READY;
        
        memset(uart_str.UsartReceiveData,0,buffer_size);
      }
    }
    break;
  case SIMCOM_NET_PORT_CLOSE:
    {
      if(SIMCOM_GetStatus((unsigned char*)Pass_Through,(unsigned char*)Respond_OK,5000)==1)
      {
        NET_STAUS=SIMCOM_NET_TRANSPARENT;
        memset(uart_str.UsartReceiveData,0,buffer_size);
      }
    }
    break;
  case SIMCOM_NET_TRANSPARENT:
    {
      //server_ip_tmp=Establish_TCP_Connection;
      if(SIMCOM_GetStatus((unsigned char*)server_ip,(unsigned char*)Respond_TCP_Connect,60000)==1)
      {
#if debug 
     	NET_STAUS=SIMCOM_Connect_Platform;
#else
        NET_STAUS=SIMCOM_NET_OK;
#endif
        memset(uart_str.UsartReceiveData,0,buffer_size);
      }
      //Establish_TCP_Connection=server_ip_tmp;
    }
    break;
  case SIMCOM_Connect_Platform:
    {
      //server_ip_tmp=Establish_TCP_Connection;
      if(SIMCOM_GetStatus((unsigned char*)one_net_key,(unsigned char*)platform_received,240000)==1)
      {
     	NET_STAUS=SIMCOM_NET_OK;
        
        memset(uart_str.UsartReceiveData,0,buffer_size);
      }
      //Establish_TCP_Connection=server_ip_tmp;
    }
    break;
    
    
  case SIMCOM_NET_OK:
    {
      if(SIMCOM_Get_TCP_Staus(40)==1)
      {   
        
        if(SIMCOM_GetStatus((unsigned char*)Quit_transparent,(unsigned char*)Respond_OK,5000)==1)
        {
          NET_STAUS=SIMCOM_NET_ERROR;
          memset(uart_str.UsartReceiveData,0,buffer_size);
        }
        
      }
    }
    break;
  case SIMCOM_NET_ERROR:
    {
      // GSM_HardwareInit(ON);                   //复位重启
      GSM_HardwareInit(ON);
      NET_STAUS=SIMCOM_NET_NOT;       //状态机复位
    }
    break;
  default : break;
  }
  /*if(NET_STAUS!=SIMCOM_NET_NOT&&NET_STAUS!=SIMCOM_NET_OK)
  {
  Network_Intens=Network_Intens++;
}
	else
  {
  Network_Intens=0;
  
}*/
  SIMCOM_ReConnect();
  free(p);
}
void SIMCOM_Reconnect_status()
{
  NET_STAUS=SIMCOM_NetOpen_READY;
}
void  Server_IP_Config(unsigned char * pbuff)
{
  //	Establish_TCP_Connection=pbuff;
}
unsigned char Get_Network_status()
{
  return NET_STAUS;
}
void Set_Network_status()
{
  SIMCOM_TimeOut_Count = 0;
  NET_STAUS = SIMCOM_NET_NOT;
}


unsigned char Get_Network_Thres()
{
  if(Network_Intens>=Network_Thres)
    return 1;
  else
    return 0;
}




