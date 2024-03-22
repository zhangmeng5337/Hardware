#include "gps.h"
#include "string.h"
#include "nbiotHAL.h"

 char  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
 u16 USART_RX_STA;         		//接收状态标记
 unsigned char gps_res;
extern UART_HandleTypeDef huart3;

/*************************************************************
				485串口接收中断回调函数
*************************************************************/
void gps_uart_Init()
{
	unsigned char p;
	  	 	p = 53;
    RS485_SendData(&p,1,0);
	
    __HAL_UART_CLEAR_IDLEFLAG(&huart3);
    __HAL_UART_DISABLE_IT(&huart3, UART_IT_RXNE);	//使能空闲中断

	__HAL_UART_CLEAR_IDLEFLAG(&huart3);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);	//使能空闲中断
	  	 	p = 53;
    RS485_SendData(&p,1,0);
	HAL_UART_Receive_IT(&huart3, &gps_res, 1); 	  // 重新注册一次，要不然下次收不到了
	  	 	p = 53;
    RS485_SendData(&p,1,0);
}
 unsigned char gps_status;
void gps_powerON()
{
	 

	switch(gps_status)
	{


//		case 1:
//		{     
//			if(sendCommand("AT+CGNSPWR=1\r\n", "OK\r\n", 5000, 1) == Success)		
//			{
//			gps_status = 2;
//			}
//		}
//		break;
		case 0:
		{	
		//if(sendCommand("AT+CGNSINF\r\n", "OK\r\n", 5000, 3) == Success) 
			if(sendCommand("AT+SGNSCFG=\"NMEAOUTPORT\",2，115200\r\n", "OK\r\n", 100000, 1) == Success) 	 
			{
				;
			}gps_status = 1;
		}
		break;
		case 1:
		{	   
			if(sendCommand("AT+SGNSCMD=2,1000,0,1\r\n", "OK\r\n", 100000, 1) == Success) 	 
			{
				gps_status = 2;
			}
		}
		break;
	}

}
unsigned char UART3_ReceiveByte()
{
	u8 err,result;
	if(HAL_UART_GetError(&huart3)) 
		err=1;//检测到噪音、帧错误或校验错误
	else 
		err=0;
	if(err==0)
	{
		result =gps_res;

	}  

	HAL_UART_Receive_IT(&huart3, &gps_res, 1); 	  // 重新注册一次，要不然下次收不到了
	return result;
}
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
//u16 USART_RX_STA=0;       //接收状态标记	  
u16 point1 = 0;
_SaveData Save_Data;
void Gps_RxCpltCallback()
{
   u8 Res;
    Res =UART3_ReceiveByte();//(USART1->DR);	//读取接收到的数据
    
    if(Res == '$')
    {
      point1 = 0;	
    }
    
    
    USART_RX_BUF[point1++] = Res;
    
    if(USART_RX_BUF[0] == '$' && USART_RX_BUF[4] == 'M' && USART_RX_BUF[5] == 'C')			//确定是否收到"GPRMC/GNRMC"这一帧数据
    {
      if(Res == '\n')									   
      {
        memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
        memcpy(Save_Data.GPS_Buffer, USART_RX_BUF, point1); 	//保存数据
        Save_Data.isGetData = true;
        point1 = 0;
        memset(USART_RX_BUF, 0, USART_REC_LEN);      //清空				
      }	
      
    }
    
    if(point1 >= USART_REC_LEN)
    {
      point1 = USART_REC_LEN;
    }	
}
void errorLog(int num)
{
}
unsigned char * parseGpsBuffer()
{
  char *subString;
  char *subStringNext;
  char i = 0;
  if (Save_Data.isGetData)
  {
    Save_Data.isGetData = false;
    //printf("**************\r\n");
   // printf(Save_Data.GPS_Buffer);
    
    
    for (i = 0 ; i <= 6 ; i++)
    {
      if (i == 0)
      {
        if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
          errorLog(1);	//解析错误
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
          Save_Data.isParseData = true;
          if(usefullBuffer[0] == 'A')
            Save_Data.isUsefull = true;
          else if(usefullBuffer[0] == 'V')
            Save_Data.isUsefull = false;
          
        }
        else
        {
          errorLog(2);	//解析错误
        }
      }
      
      
    }
  }
    if(Save_Data.isUsefull == true)
  {
    unsigned char p[60];    
    unsigned char len;
    memcpy(p+len,Save_Data.E_W ,2);
    len = len +2;
    memcpy(p+len,Save_Data.longitude ,12);
    len = len +12;
	  memcpy(p+len,Save_Data.N_S ,2);
    len = len +2;
    memcpy(p+len,Save_Data.latitude ,11);
    len = len +11;
	return p;
    
  }
   else
   	return NULL;
}

void printGpsBuffer()
{
  if (Save_Data.isParseData)
  {
    Save_Data.isParseData = false;
    
    printf("Save_Data.UTCTime = ");
    printf(Save_Data.UTCTime);
    printf("\r\n");
    
    if(Save_Data.isUsefull)
    {
      Save_Data.isUsefull = false;
      printf("Save_Data.latitude = ");
      printf(Save_Data.latitude);
      printf("\r\n");
      
      
      printf("Save_Data.N_S = ");
      printf(Save_Data.N_S);
      printf("\r\n");
      
      printf("Save_Data.longitude = ");
      printf(Save_Data.longitude);
      printf("\r\n");
      
      printf("Save_Data.E_W = ");
      printf(Save_Data.E_W);
      printf("\r\n");
    }
    else
    {
      printf("GPS DATA is not usefull!\r\n");
    }
    
  }
}
