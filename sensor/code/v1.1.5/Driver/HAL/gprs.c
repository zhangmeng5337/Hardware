#include "gprs_app.h"
#include "string.h"

extern _uart uart1;
unsigned long  Time_Cont = 0;       //定时器计数器
unsigned int count = 0;
void errorLog(int num)
{
}
void phone(char *number)
{
  char send_buf[20] = {0};
  memset(send_buf, 0, 20);    //清空
  strcpy(send_buf, "ATD");
  strcat(send_buf, number);
  strcat(send_buf, ";\r\n");
  
  if (sendCommand(send_buf, "OK\r\n", 10000, 10) == Success);
  else errorLog(4);
}
unsigned char TimeOutFlag=0;
unsigned char RetryCount;
unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
  unsigned char *USARTX_RX_BUF;
  USARTX_RX_BUF =uart1.Uart_Buffer;
  unsigned char n;
  //USART1_CLR_Buf();
  if(TimeOutFlag == 0)
  {
    if(RetryCount<=Retry)
    {
      if(Retry>0)
        UART1_SendStr(Command); 		//发送GPRS指令  
      RetryCount = RetryCount +1;
      Time_Cont = 0;
    }
    else
    {
        RetryCount = 0;
        
    }
    TimeOutFlag = 1;
  }
  
  if(TimeOutFlag==1)
  {
    Time_Cont++;
  } 
  if (Time_Cont >= Timeout)
  {
    Time_Cont = 0;
    TimeOutFlag = 0;
    if (strstr(USARTX_RX_BUF, Response) != NULL)
    {				
      USART1_CLR_Buf();
      return Success;
    }
    else
    {
      USART1_CLR_Buf();
      return Failure;
    } 
  }
  else
    return Failure;
  
  
  
//  for (n = 0; n < Retry; n++)
//  {
//    if(Retry>0)
//      UART1_SendStr(Command); 		//发送GPRS指令
//    
//    // printf("\r\n***************send****************\r\n");
//    //printf(Command);
//    
//    Time_Cont = 0;
//    while (Time_Cont < Timeout)
//    {
//      //  delay_ms(100);
//      Time_Cont += 1;
//      if (strstr(USARTX_RX_BUF, Response) != NULL)
//      {				
//        // printf("\r\n***************receive****************\r\n");
//        // printf(USARTX_RX_BUF);
//        USART1_CLR_Buf();
//        return Success;
//      }
//      
//    }
//    Time_Cont = 0;
//  }
//  if(Retry==0)
//  {
//    Time_Cont = 0;
//    while (Time_Cont < Timeout)
//    {
//      Time_Cont += 1;
//      if (strstr(USARTX_RX_BUF, Response) != NULL)
//      {				
//        USART1_CLR_Buf();
//        return Success;
//      }
//    }    
//  }
//  
//  Time_Cont = 0;
//  // printf("\r\n***************receive****************\r\n");
//  // printf(USARTX_RX_BUF);
//  USART1_CLR_Buf();
//  return Failure;
}




