#include "stm8l15x.h"
#include <string.h>
#include "AT_CMD.h"
#include "GSM.h"
#include "bsp.h"
#include "string.h"
#include "GSM.h"
#include "stdlib.h"

#define Network_Thres    30




unsigned char Establish_TCP_Connection[100]="AT+CIPSTART=\"TCP\",\"15k801n729.iok.la\",36732\r";
//unsigned char Establish_TCP_Connection[100]="AT+CIPOPEN=0,\"TCP\",\"www. baidu.com.cn\",80\r";



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
    uart_str.receive_flag=0;

  }
    if(waittime==0){
      res=0;
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

//unsigned char  SIMCOM_Get_Echo_Staus(unsigned int waittime)
//{
//	unsigned char res=0;
//	Send_Comm((unsigned char*)Echo_Dis,strlen((const char*)Echo_Dis));
//
//	if(waittime)		//需要等待应答
//	{
//		while(--waittime)	//等待倒计时
//		{
//			delay_ms(10);
//			if(UsartType5.receive_flag==1)//接收到期待的应答结果
//			{
//				if(AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)Respond_OK))
//				{
//					uart_str.receive_flag=0;
//					res=1;
//					break;//得到有效数据
//				}
//
//			}
//		}
//		if(waittime==0)res=0;
//	}
//	return res;
//}
//signed char  SIMCOM_Get_NormalMode_Staus(unsigned int waittime)
//{
//	unsigned char res=0;
//	Send_Comm((unsigned char*)Normal_Mode,strlen((const char*)Normal_Mode));
//
//	if(waittime)		//需要等待应答
//	{
//		while(--waittime)	//等待倒计时
//		{
//			delay_ms(10);
//			if(UsartType5.receive_flag==1)//接收到期待的应答结果
//			{
//				if(AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)Respond_OK))
//				{
//					UsartType5.receive_flag=0;
//					res=1;
//					break;//得到有效数据
//				}
//
//			}
//		}
//		if(waittime==0)res=0;
//	}
//	return res;
//}
unsigned int tmpp;
//signed char  SIMCOM_Get_SimCard_Staus(unsigned int waittime)
//{
//	unsigned char res=0;
//	Send_Comm((unsigned char*)Check_SIM,strlen((const char*)Check_SIM));
//
//	if(waittime)		//需要等待应答
//	{
//		tmpp=waittime;
//		while(--waittime)	//等待倒计时
//		{
//			delay_ms(10);
//			if(UsartType5.receive_flag==1)//接收到期待的应答结果
//			{
//				if(AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)Respond_CPIN))
//				{
//					UsartType5.receive_flag=0;
//					res=1;
//					break;//得到有效数据
//				}
//
//			}
//		}
//		if(waittime==0)res=0;
//	}
//	return res;
//}
//signed char  SIMCOM_Get_Network_Intensity_Staus(unsigned int waittime)
//{
//	unsigned char res=0;
//	Send_Comm((unsigned char*)Network_Intensity,strlen((const char*)Network_Intensity));
//
//	if(waittime)		//需要等待应答
//	{
//		while(--waittime)	//等待倒计时
//		{
//			delay_ms(10);
//			if(UsartType5.receive_flag==1)//接收到期待的应答结果
//			{
//				if(AT_cmd_ack&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)Respond_Network_Intensity))
//				{
//					UsartType5.receive_flag=0;
//					res=0;
//					break;//得到有效数据
//				}
//				else if(AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)Respond_Network_Intensity_eff))
//				{
//					res=1;
//					UsartType5.receive_flag=0;
//
//				}
//				break;
//				//res=0;
//
//			}
//		}
//		if(waittime==0)res=0;
//	}
//	return res;
//}
//signed char  SIMCOM_Get_GPRS_Register_Staus(unsigned int waittime)
//{
//	unsigned char res=0;
//	Send_Comm((unsigned char*)GPRS_Register,strlen((const char*)GPRS_Register));
//
//	if(waittime)		//需要等待应答
//	{
//		while(--waittime)	//等待倒计时
//		{
//			delay_ms(10);
//			if(UsartType5.receive_flag==1)//接收到期待的应答结果
//			{
//				if(AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)Respond_OK))
//				{
//					res=1;
//					UsartType5.receive_flag=0;
//					break;//得到有效数据
//
//				}
//
//				//res=0;
//			}
//		}
//		if(waittime==0)res=0;
//	}
//	return res;
//}

//signed char  SIMCOM_Get_StartGPS_Register_Staus(unsigned int waittime)
//{
//	unsigned char res=0;
//	Send_Comm((unsigned char*)Start_GPS,strlen((const char*)Start_GPS));
//
//	if(waittime)		//需要等待应答
//	{
//		while(--waittime)	//等待倒计时
//		{
//			delay_ms(10);
//			if(UsartType5.receive_flag==1)//接收到期待的应答结果
//			{
//				if(AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)Respond_OK))
//				{
//					res=1;
//					UsartType5.receive_flag=0;
//					break;//得到有效数据
//
//				}
//
//				//res=0;
//			}
//		}
//		if(waittime==0)res=0;
//	}
//	return res;
//}
//unsigned char  SIMCOM_GetGPS_Info(unsigned char * buff,GPS_INFO *gps,unsigned char gps_format,unsigned int waittime)
//{
//	unsigned char res=0;
//	unsigned char i;
//	Send_Comm((unsigned char*)AT_GPS_info,strlen((const char*)AT_GPS_info));
//
//	if(waittime)		//需要等待应答
//	{
//		while(--waittime)	//等待倒计时
//		{
//			delay_ms(10);
//			if(UsartType5.receive_flag==1)//接收到期待的应答结果
//			{
//				for(i=0; i<UsartType5.rx_len_var; i++)
//				{
//					if(buff[i]=='+')
//						break;
//
//				}
//
//
//				if(GPS_RMC_Parse(&buff[i], gps,1))
//				{
//					res=1;
//					UsartType5.receive_flag=0;
//					if(SIMCOM_Get_TO_DataMode_Staus(150)==1)
//						return 1;
//					break;//得到有效数据
//				}
//			}
//		}
//		if(waittime==0)res=0;
//	}
//	return res;
//}

//signed char  SIMCOM_Get_Transparent_Staus(unsigned int waittime)
//{
//	unsigned char res=0;
//	Send_Comm((unsigned char*)Pass_Through,strlen((const char*)Pass_Through));
//
//	if(waittime)		//需要等待应答
//	{
//		while(--waittime)	//等待倒计时
//		{
//			delay_ms(10);
//			if(UsartType5.receive_flag==1)//接收到期待的应答结果
//			{
//				if(AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)Respond_OK))
//				{
//					res=1;
//					UsartType5.receive_flag=0;
//					break;//得到有效数据
//
//				}
//
//				//res=0;
//			}
//		}
//		if(waittime==0)res=0;
//	}
//	return res;
//}
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
//signed char  SIMCOM_Get_NetClose_Staus(unsigned int waittime)
//{
//	unsigned char res=0;
//	Send_Comm((unsigned char*)Net_Close,strlen((const char*)Net_Close));
//
//	if(waittime)		//需要等待应答
//	{
//		while(--waittime)	//等待倒计时
//		{
//			delay_ms(10);
//			if(UsartType5.receive_flag==1)//接收到期待的应答结果
//			{
//				if(AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)Respond_OK))
//				{
//					res=1;
//					UsartType5.receive_flag=0;
//					break;//得到有效数据
//
//				}
//
//			}
//			if(waittime==0)res=0;
//		}
//
//	}
//			return res;
//}
//signed char  SIMCOM_Get_CIPClose_Staus(unsigned int waittime)
//{
//	unsigned char res=0;
//	Send_Comm((unsigned char*)CIP_Close,strlen((const char*)CIP_Close));
//
//	if(waittime)		//需要等待应答
//	{
//		while(--waittime)	//等待倒计时
//		{
//			delay_ms(10);
//			if(UsartType5.receive_flag==1)//接收到期待的应答结果
//			{
//				if(AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)Respond_OK))
//				{
//					res=1;
//					UsartType5.receive_flag=0;
//					break;//得到有效数据
//
//				}
//
//			}
//			if(waittime==0)res=0;
//		}
//
//	}
//			return res;
//}
//
//signed char  SIMCOM_Get_EstablishTCP_Staus(unsigned int waittime,unsigned char *server_ip)
//{
//	unsigned char res=0;
//	Send_Comm((unsigned char*)server_ip,strlen((const char*)server_ip));
//
//	if(waittime)		//需要等待应答
//	{
//		while(--waittime)	//等待倒计时
//		{
//			delay_ms(10);
//			if(UsartType5.receive_flag==1)//接收到期待的应答结果
//			{
//				if(AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)Respond_TCP_Connect))
//				{
//					res=1;
//					UsartType5.receive_flag=0;
//					break;//得到有效数据
//				}
//
//
//				//res=0;
//			}
//		}
//		if(waittime==0)res=0;
//	}
//	return res;
//}
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
        if(AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)TCP_Closed)
           ||AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)Respond_No_Carrier)
             ||AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)TCP_ERROR))
        {
          res=1;
          uart_str.receive_flag=0;
          break;//得到有效数据
          
        }
        
        //res=0;TIMEOUT
      }
    }
    if(waittime==0)res=0;
  }
  return res;
}
//signed char  SIMCOM_Get_TIMEOUT_Staus(unsigned int waittime)
//{
//	unsigned char res=0;
//	Send_Comm((unsigned char*)TIMEOUT,strlen((const char*)TIMEOUT));
//
//	if(waittime)		//需要等待应答
//	{
//		while(--waittime)	//等待倒计时
//		{
//			delay_ms(5);
//			if(UsartType5.receive_flag==1)//接收到期待的应答结果
//			{
//				if(AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)TCP_Closed))
//					//||AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)TCP_ERROR))
//				{
//					res=1;
//					UsartType5.receive_flag=0;
//					break;//得到有效数据
//
//				}
//
//				//res=0;
//			}
//		}
//		if(waittime==0)res=0;
//	}
//	return res;
//}

//signed char  SIMCOM_Get_QuitTransparent_Staus(unsigned int waittime)
//{
//	unsigned char res=0;
//	Send_Comm((unsigned char*)Quit_transparent,strlen((const char*)Quit_transparent));
//  
//	if(waittime)		//需要等待应答
//	{
//		while(--waittime)	//等待倒计时
//		{
//			delay_ms(1);
//			if(UsartType5.receive_flag==1)//接收到期待的应答结果
//			{
//				if(AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)Respond_OK))
//				{
//					res=1;
//					delay_ms(200);
//					UsartType5.receive_flag=0;
//					break;//得到有效数据
//				}
//
//
//				//res=0;
//			}
//		}
//		if(waittime==0)
//			res=0;
//	}
//	return res;
//}
//signed char  SIMCOM_Get_TO_DataMode_Staus(unsigned int waittime)
//{
//	unsigned char res=0;
//	Send_Comm((unsigned char*)TO_DATA_MODE,strlen((const char*)TO_DATA_MODE));
//
//	if(waittime)		//需要等待应答
//	{
//		while(--waittime)	//等待倒计时
//		{
//			delay_ms(10);
//			if(UsartType5.receive_flag==1)//接收到期待的应答结果
//			{
//				if(AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)Respond_TCP_Connect))
//				{
//					res=1;
//					UsartType5.receive_flag=0;
//					break;//得到有效数据
//
//				}
//
//
//				//res=0;
//			}
//		}
//		if(waittime==0)res=0;
//	}
//	return res;
//}
//unsigned char  SIMCOM_Get_Trans_Staus(unsigned int waittime)
//{
//	unsigned char res=0;
//	Send_Comm((unsigned char*)Pass_Through,strlen((const char*)Pass_Through));
//
//	if(waittime)		//需要等待应答
//	{
//		while(--waittime)	//等待倒计时
//		{
//			delay_ms(10);
//			if(UsartType5.receive_flag==1)//接收到期待的应答结果
//			{
//				if(AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],(unsigned char*)Respond_OK))
//				{
//					res=1;
//					UsartType5.receive_flag=0;
//					break;//得到有效数据
//
//				}
//
//
//				//res=0;
//			}
//		}
//		if(waittime==0)res=0;
//	}
//	return res;
//}
//
//
////向sim900a发送命令
////cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
////ack:期待的应答结果,如果为空,则表示不需要等待应答
////waittime:等待时间(单位:10ms)
////返回值:0,发送成功(得到了期待的应答结果)
////       1,发送失败
//unsigned char  SIMCOM_SendAT(unsigned char *cmd,unsigned char *ack,unsigned int waittime)
//{
//	unsigned char res=0;
//	Send_Comm((unsigned char*)cmd,strlen((const char*)cmd));
//
//	if(waittime)		//需要等待应答
//	{
//		while(--waittime)	//等待倒计时
//		{
//			delay_ms(10);
//			if(UsartType5.receive_flag==1)//接收到期待的应答结果
//			{
//				if(AT_cmd_ack(&uart_str.UsartReceiveData[uart_str.real_index],ack))
//				{
//					res=1;
//					UsartType5.receive_flag=0;
//					break;//得到有效数据
//
//				}
//
//
//			}
//		}
//		if(waittime==0)res=0;
//	}
//	return res;
//}


void SIMCOM_ReConnect()
{
  if(NET_STAUS!=SIMCOM_NET_NOT&&NET_STAUS!=SIMCOM_NET_OK) //控制系统超时机制，保证系统能够在超时后重新启动链接
    SIMCOM_TimeOut_Count++;
  else if(NET_STAUS==SIMCOM_NET_OK)
    SIMCOM_TimeOut_Count=0;
  if(SIMCOM_TimeOut_Count>=SIMCOM_TIME_OUT)
  {
    SIMCOM_TimeOut_Count=0;
    NET_STAUS=SIMCOM_NET_ERROR;
  }
  
}
//void Net_Status_Change(unsigned status_tmp,unsigned char update_ip_flag,unsigned char *server_ip)
//{
//	switch(status_tmp)
//		{
//		case SIMCOM_NET_NOT:
//		{
//			//if(SIMCOM_Get_InitReady_Staus(1000)==1)
//				NET_STAUS=SIMCOM_NET_NOT;
//		}
//		break;
//		case SIMCOM_READY_YES:
//		{
//			//if( SIMCOM_Get_Echo_Staus(500)==1)
//				NET_STAUS=SIMCOM_READY_YES;
//		}
//		break;
//		case SIMCOM_Echo_OFF:
//		{
//			//if(SIMCOM_Get_NormalMode_Staus(500)==1)
//				NET_STAUS=SIMCOM_Echo_OFF;
//		}
//		break;
//		case SIMCOM_NORMAL_MODE:
//		{
//			//if(SIMCOM_Get_SimCard_Staus(500)==1)
//				NET_STAUS=SIMCOM_NORMAL_MODE;
//		}
//		break;
//				
//		case SIMCOM_CIPClose_MODE:
//		{
//			//if(SIMCOM_Get_Network_Intensity_Staus(500)==1)
//				NET_STAUS=SIMCOM_CIPClose_MODE;
//			if(update_ip_flag==1)
//			server_ip_tmp=server_ip;
//		}
//		break;	
//		case SIMCOM_SIMCARD_READY:
//		{
//			//if(SIMCOM_Get_Network_Intensity_Staus(500)==1)
//				NET_STAUS=SIMCOM_SIMCARD_READY;
//
//
//		}
//		break;
//		case SIMCOM_Network_Intensity_READY:
//		{
//			//if(SIMCOM_Get_GPRS_Register_Staus(200)==1)
//				NET_STAUS=SIMCOM_Network_Intensity_READY;
//		}
//		break;
//		case SIMCOM_GPRS_READY:
//		{
//			//if(SIMCOM_Get_StartGPS_Register_Staus(200)==1)
//				NET_STAUS=SIMCOM_GPRS_READY;
//		}
//		break;
//		case SIMCOM_START_GPS:
//		{
//			//if(SIMCOM_Get_Transparent_Staus(200)==1)
//				NET_STAUS=SIMCOM_START_GPS;
//		}
//		break;
//		case SIMCOM_TRANSPERENT_MODE:
//		{
//			//if(SIMCOM_Get_NetOpen_Staus(200)==1)
//				NET_STAUS=SIMCOM_TRANSPERENT_MODE;
//		}
//		break;
//		case SIMCOM_NetOpen_READY:
//		{
//			//if(SIMCOM_Get_EstablishTCP_Staus(500)==1)
//				NET_STAUS=SIMCOM_NetOpen_READY;
//			
//		}
//		break;
//		case SIMCOM_NET_OK:
//		{
//			//if(SIMCOM_Get_TCP_Staus(40)==1)
//				NET_STAUS=SIMCOM_NET_OK;
//		}
//		break;
//		case SIMCOM_NET_ERROR:
//		{
//			LET_reset();					//复位重?
//			NET_STAUS=SIMCOM_NET_NOT;
//			//NET_STAUS=SIMCOM_NET_ERROR;		//状态机复位
//		}
//		break;
//		}
//
//
//
//	
//
//}
void test()
{
Send_Comm((unsigned char*)Test,strlen((const char*)Test));
}
void SIMCOM_Register_Network()
{
  unsigned char *p;
  p=malloc(sizeof(unsigned char) * 64);
 // SIMCOM_ReConnect();
  switch(NET_STAUS)
  {
    
    
  case SIMCOM_NET_NOT:
    {       *p = 0;
    if(SIMCOM_GetStatus((unsigned char*)Test,(unsigned char*)Respond_OK,20000)==1)
    {  NET_STAUS=SIMCOM_READY_YES;
    memset(uart_str.UsartReceiveData,0,256);}
    
    }
    break;
  case SIMCOM_READY_YES://SIMCOM_READY_YES:
    {
      if( SIMCOM_GetStatus((unsigned char*)Echo_Dis,(unsigned char*)Respond_OK,100000)==1)
      {  NET_STAUS=SIMCOM_ATE0;
      memset(uart_str.UsartReceiveData,0,256);}
    }
    break;
 
  case SIMCOM_ATE0:
    {
      if(SIMCOM_GetStatus((unsigned char*)Check_SIM,(unsigned char*)Respond_CPIN,100000)==1)
      {  NET_STAUS=SIMCOM_SIM_OK;
      memset(uart_str.UsartReceiveData,0,256);}
      //server_ip_tmp=Establish_TCP_Connection;
    }
    break;
  case SIMCOM_SIM_OK:
  {
	if(SIMCOM_GetStatus((unsigned char*)Network_Intensity,(unsigned char*)Respond_Network_Intensity,100000)==1)
	{ NET_STAUS=SIMCOM_SIM_OK;
        memset(uart_str.UsartReceiveData,0,256);}
       //  NET_STAUS=SIMCOM_Network_Intensity_READY;
  }
  break;

    	
    case SIMCOM_Network_Intensity_READY:
    {
      if(SIMCOM_GetStatus((unsigned char*)GPRS_Attached_State,(unsigned char*)Respond_Attached_Ok,100000)==1)
        NET_STAUS=SIMCOM_GPRS_READY;
    }
    break;




  case SIMCOM_GPRS_READY:
    {
      if(SIMCOM_GetStatus((unsigned char*)AT_SHUNT,(unsigned char*)Respond_OK,30000)==1)
        NET_STAUS=SIMCOM_NET_PORT_CLOSE;
    }
    break;
    case SIMCOM_NET_PORT_CLOSE:
    {
      if(SIMCOM_GetStatus((unsigned char*)Pass_Through,(unsigned char*)Respond_OK,30000)==1)
        NET_STAUS=SIMCOM_NET_TRANSPARENT;
    }
    break;
  case SIMCOM_NET_TRANSPARENT:
    {
      //server_ip_tmp=Establish_TCP_Connection;
      if(SIMCOM_GetStatus((unsigned char*)server_ip,(unsigned char*)Respond_TCP_Connect,80000)==1)
        NET_STAUS=SIMCOM_NET_OK;
      //Establish_TCP_Connection=server_ip_tmp;
    }
    break;
  case SIMCOM_NET_OK:
    {
      if(SIMCOM_Get_TCP_Staus(40)==1)
        NET_STAUS=SIMCOM_NET_ERROR;
    }
    break;
  case SIMCOM_NET_ERROR:
    {
      GSM_HardwareInit(OFF);                   //复位重启
      GSM_HardwareInit(ON);
      NET_STAUS=SIMCOM_NET_NOT;       //状态机复位
    }
    break;
  }
  /*if(NET_STAUS!=SIMCOM_NET_NOT&&NET_STAUS!=SIMCOM_NET_OK)
  {
  Network_Intens=Network_Intens++;
}
	else
  {
  Network_Intens=0;
  
}*/
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
  NET_STAUS=SIMCOM_NetOpen_READY;
}

unsigned char Get_Network_Thres()
{
  if(Network_Intens>=Network_Thres)
    return 1;
  else
    return 0;
}




