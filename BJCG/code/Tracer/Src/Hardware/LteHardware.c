#include "LteHardware.h"
#include "LteHal.h"
#include "gps.h"
#include "string.h"
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;
 char  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
 uint16_t USART_RX_STA;         		//接收状态标记
 unsigned char gps_res;
 
void LtePowerManage(unsigned char moduleType,unsigned char powerCtrl)
{
    if(moduleType == LTE_4G)
    {
        if(powerCtrl == ON)
        {
            HAL_GPIO_WritePin(GPIOB,EN_3V8_Pin, GPIO_PIN_SET);//lte电源使能

            HAL_GPIO_WritePin(GPIOA, SIM_PWR_Pin, GPIO_PIN_RESET);//4g开机
            HAL_GPIO_WritePin(GPIOA, SIM_PWR_Pin, GPIO_PIN_SET);
            HAL_Delay(1000);
            HAL_GPIO_WritePin(GPIOC, SIM_PWR_Pin, GPIO_PIN_RESET);

            HAL_GPIO_WritePin(GPIOC, POWER_CTRL_GPS_Pin, GPIO_PIN_SET);//开gps
        }
        else
        {
            HAL_GPIO_WritePin(GPIOB,EN_3V8_Pin, GPIO_PIN_RESET);//lte电源禁止
            HAL_GPIO_WritePin(GPIOC, POWER_CTRL_GPS_Pin, GPIO_PIN_RESET);//gps禁止
        }
    }
    else if((moduleType == LTE_NBIOT))
    {
        if(powerCtrl == ON)
        {
            HAL_GPIO_WritePin(GPIOB,EN_3V8_Pin, GPIO_PIN_SET);//lte电源使能

            HAL_GPIO_WritePin(GPIOA, SIM_PWR_Pin, GPIO_PIN_RESET);//4g开机
            HAL_GPIO_WritePin(GPIOA, SIM_PWR_Pin, GPIO_PIN_SET);
            HAL_Delay(1000);
            HAL_GPIO_WritePin(GPIOC, SIM_PWR_Pin, GPIO_PIN_RESET);

            HAL_GPIO_WritePin(GPIOC, POWER_CTRL_GPS_Pin, GPIO_PIN_RESET);//
        }
        else
        {
            HAL_GPIO_WritePin(GPIOB,EN_3V8_Pin, GPIO_PIN_RESET);//lte电源禁止
            HAL_GPIO_WritePin(GPIOC, POWER_CTRL_GPS_Pin, GPIO_PIN_RESET);//
        }

    }
    else
    {

    }
}

void LteUart_SendStr(unsigned char moduleType,uint8_t *str)
{
    unsigned char len;
    len = strlen((const char*)str);


    if(moduleType == LTE_4G)
    {

        HAL_UART_Transmit(&huart2, str, len, 1000);

    }
    else if((moduleType == LTE_NBIOT))
    {

        HAL_UART_Transmit(&huart2, str, len, 1000);
    }
    else
    {

    }

}
void LteUart_SendByte(unsigned char moduleType,uint8_t *str,uint32_t len)
{

    if(moduleType == LTE_4G)
    {

        HAL_UART_Transmit(&huart2, str, len, 1000);

    }
    else if((moduleType == LTE_NBIOT))
    {

        HAL_UART_Transmit(&huart2, str, len, 1000);
    }
    else
    {

    }

}

void LteUartConfig(void)
{
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    __HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);	//使能空闲中断
    HAL_UART_DMAStop(&huart1);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);	//使能空闲中断
    HAL_UART_Receive_DMA(&huart1,GetLteStru()->lterxbuffer,BUFFERSIZE);

    __HAL_UART_CLEAR_IDLEFLAG(&huart2);
    __HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断

    HAL_UART_DMAStop(&huart2);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
    HAL_UART_Receive_DMA(&huart2,GetLteStru()->gpsrxbuffer,BUFFERSIZE);
}
void USART1_CLR_Buf(void)
{
   memset(GetLteStru()->lterxbuffer,0,BUFFERSIZE);
}
void Lte_RxCpltCallback(unsigned char flag)
{
    unsigned char temp;

    temp=__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE);
    if(flag == 1)
    {
        if((temp!=RESET))
        {
            __HAL_UART_CLEAR_IDLEFLAG(&huart1);
            temp=huart1.Instance->ISR;
            temp=huart1.Instance->RDR;

            GetLteStru()->LteReceivedFlag = 1;
            HAL_UART_DMAStop(&huart1);
            GetLteStru()->rxSize = BUFFERSIZE -hdma_usart1_rx.Instance->CNDTR;
            HAL_UART_Receive_DMA(&huart1,GetLteStru()->lterxbuffer,BUFFERSIZE);
        }
    }
    else
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart2);
        temp=huart2.Instance->ISR;
        temp=huart2.Instance->RDR;
        GetLteStru()->rxSize = BUFFERSIZE -hdma_usart2_rx.Instance->CNDTR;
        GetLteStru()->GpsReceivedFlag = 1;
        HAL_UART_DMAStop(&huart2);
        HAL_UART_Receive_DMA(&huart2,GetLteStru()->gpsrxbuffer,BUFFERSIZE);
    }

}

unsigned char UART3_ReceiveByte()
{
	uint8_t err,result;
	if(HAL_UART_GetError(&huart2)) 
		err=1;//检测到噪音、帧错误或校验错误
	else 
		err=0;
	if(err==0)
	{
		result =gps_res;

	}  

	HAL_UART_Receive_IT(&huart2, &gps_res, 1); 	  // 重新注册一次，要不然下次收不到了
	return result;
}
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
//uint16_t USART_RX_STA=0;       //接收状态标记	  
uint16_t point1 = 0;
_SaveData Save_Data;
void Gps_RxCpltCallback()
{
   uint8_t Res;
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
unsigned char  parseGpsBuffer(unsigned char *p)
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
    unsigned char len;
    memcpy(p+len,Save_Data.E_W ,2);
    len = len +2;
    memcpy(p+len,Save_Data.longitude ,12);
    len = len +12;
	  memcpy(p+len,Save_Data.N_S ,2);
    len = len +2;
    memcpy(p+len,Save_Data.latitude ,11);
    len = len +11;
	return 0;
    
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
