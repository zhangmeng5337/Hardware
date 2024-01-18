#include "LteHardware.h"
#include "LteHal.h"
#include "gps.h"
#include "string.h"
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern DMA_HandleTypeDef hdma_usart1_rx;

extern DMA_HandleTypeDef hdma_usart3_rx;
 char  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
 uint16_t USART_RX_STA;         		//����״̬���
uint8_t gps_res;
 
void LtePowerManage(unsigned char moduleType,unsigned char powerCtrl)
{
    if(moduleType == LTE_4G)
    {
        if(powerCtrl == ON)
        {
            HAL_GPIO_WritePin(GPIOB,EN_3V8_Pin, GPIO_PIN_SET);//lte��Դʹ��

            HAL_GPIO_WritePin(GPIOA, SIM_PWR_Pin, GPIO_PIN_RESET);//4g����
            HAL_GPIO_WritePin(GPIOA, SIM_PWR_Pin, GPIO_PIN_SET);
            HAL_Delay(1000);
            HAL_GPIO_WritePin(GPIOA, SIM_PWR_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOC, POWER_CTRL_GPS_Pin, GPIO_PIN_SET);//��gps
        }
        else
        {
            HAL_GPIO_WritePin(GPIOB,EN_3V8_Pin, GPIO_PIN_RESET);//lte��Դ��ֹ
            HAL_GPIO_WritePin(GPIOC, POWER_CTRL_GPS_Pin, GPIO_PIN_RESET);//gps��ֹ
        }
    }
    else if((moduleType == LTE_NBIOT))
    {
        if(powerCtrl == ON)
        {
					 HAL_GPIO_WritePin(GPIOB,EN_3V8_Pin, GPIO_PIN_RESET);//lte��Դʹ��
            HAL_GPIO_WritePin(GPIOB,EN_3V8_Pin, GPIO_PIN_SET);//lte��Դʹ��
						HAL_GPIO_WritePin(GPIOC, POWER_CTRL_GPS_Pin, GPIO_PIN_SET);//
            HAL_GPIO_WritePin(GPIOA, SIM_PWR_Pin, GPIO_PIN_RESET);//4g����
            HAL_GPIO_WritePin(GPIOA, SIM_PWR_Pin, GPIO_PIN_SET);
            HAL_Delay(1000);
            HAL_GPIO_WritePin(GPIOA, SIM_PWR_Pin, GPIO_PIN_RESET);

            
        }
        else
        {
            HAL_GPIO_WritePin(GPIOB,EN_3V8_Pin, GPIO_PIN_RESET);//lte��Դ��ֹ
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

        HAL_UART_Transmit(&huart1, str, len, 1000);

    }
    else if((moduleType == LTE_NBIOT))
    {

        HAL_UART_Transmit(&huart1, str, len, 1000);
    }
    else
    {

    }

}
void LteUart_SendByte(unsigned char moduleType,uint8_t *str,uint32_t len)
{

    if(moduleType == LTE_4G)
    {

        HAL_UART_Transmit(&huart1, str, len, 1000);

    }
    else if((moduleType == LTE_NBIOT))
    {

        HAL_UART_Transmit(&huart1, str, len, 1000);
    }
    else
    {

    }

}

void LteUartConfig(void)
{
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    __HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);	//ʹ�ܿ����ж�
    HAL_UART_DMAStop(&huart1);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);	//ʹ�ܿ����ж�
    HAL_UART_Receive_DMA(&huart1,GetLteStru()->lterxbuffer,BUFFERSIZE);

    __HAL_UART_CLEAR_IDLEFLAG(&huart2);
    __HAL_UART_DISABLE_IT(&huart2, UART_IT_RXNE);	//ʹ�ܿ����ж�


    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);	//ʹ�ܿ����ж�
    HAL_UART_Receive_IT(&huart2,&gps_res,1);
	



	
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
//        __HAL_UART_CLEAR_IDLEFLAG(&huart2);
//        temp=huart2.Instance->ISR;
//        temp=huart2.Instance->RDR;
//        GetLteStru()->rxSize = BUFFERSIZE -hdma_usart2_rx.Instance->CNDTR;
//        GetLteStru()->GpsReceivedFlag = 1;
//        HAL_UART_DMAStop(&huart2);
//        HAL_UART_Receive_DMA(&huart2,GetLteStru()->gpsrxbuffer,BUFFERSIZE);
    }

}

unsigned char UART3_ReceiveByte()
{
	uint8_t err,result;
	if(HAL_UART_GetError(&huart2)) 
		err=1;//��⵽������֡�����У�����
	else 
		err=0;
	if(err==0)
	{
		result =gps_res;

	}  

	HAL_UART_Receive_IT(&huart2, &gps_res, 1); 	  // ����ע��һ�Σ�Ҫ��Ȼ�´��ղ�����
	return result;
}
 static unsigned char gps_status;
unsigned char gps_powerON()
{
unsigned char moduleType;
	 #if ROLE == 0
    moduleType = LTE_4G;
#else
    moduleType = LTE_NBIOT;
#endif
  
	switch(gps_status)
	{

	case 0:
		{	 if(sendCommand(moduleType,"AT+SGNSCFG=\"NMEAOUTPORT\",2,9600\r\n", "OK\r\n", 3000, 1,0) == Success) 		//if(sendCommand("AT+CGNSINF\r\n", "OK\r\n", 5000, 3) == Success)
     	
			//if(sendCommand("AT+SGNSCFG=\"NMEAOUTPORT\",2��115200\r\n", "OK\r\n", 100000, 1) == Success) 	 
			{
				gps_status = 1;
			}
		}
		break;
		case 1:
		{	   
      if(sendCommand(moduleType,"AT+SGNSCMD=2,1000,0,1\r\n","OK\r\n", 3000, 1,0) == Success)
   // if(sendCommand(moduleType,"AT+CGNSPWR=1\r\n", "OK\r\n", 1000, 1,0) == Success)
			//if(sendCommand("AT+CGNSPWR=1\r\n", "OK\r\n", 100000, 1) == Success) 	 
			{
				gps_status = 3;
				return 0;
			}
		}
		break;
		
	}
    return 1;
}

//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
//uint16_t USART_RX_STA=0;       //����״̬���	  
uint16_t point1 = 0;
_SaveData Save_Data;
void Gps_RxCpltCallback()
{
   uint8_t Res;
    gps_res =UART3_ReceiveByte();//(USART1->DR);	//��ȡ���յ�������
    
    if(gps_res == '$')
    {
      point1 = 0;	
    }
    
    
    USART_RX_BUF[point1++] = gps_res;
    if(USART_RX_BUF[0] == '$' && USART_RX_BUF[2] == 'N'&& USART_RX_BUF[3] == 'G'&& USART_RX_BUF[4] == 'G' && USART_RX_BUF[5] == 'A')			//ȷ���Ƿ��յ�"GPRMC/GNRMC"��һ֡����
    {
      if(gps_res == '\n')									   
      {
        memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
        memcpy(Save_Data.GPS_Buffer, USART_RX_BUF, point1); 	//��������
        Save_Data.isGetData = true;
        point1 = 0;
        memset(USART_RX_BUF, 0, USART_REC_LEN);      //���				
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
	unsigned char t;
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
    
    
    for (i = 0 ; i <= 9 ; i++)
    {
      if (i == 0)
      {
        if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
          errorLog(1);	//��������
      }
      else
      {
        subString++;
        if ((subStringNext = strstr(subString, ",")) != NULL)
        {
          char usefullBuffer[2]; 
		  usefullBuffer[0]='0'; 
          switch(i)
          {
          case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//��ȡUTCʱ��
          case 2:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//��ȡγ����Ϣ
          case 3:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//��ȡN/S
          case 4:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;//��
		  case 5: memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//��ȡE/Wbreak;	//��ȡ������Ϣ
          case 6:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//��ȡUTCʱ��
          case 9:memcpy(Save_Data.altitude, subString, subStringNext - subString);break;	//��ȡUTCʱ��
                    
          default:break;
          }
          
          subString = subStringNext;
          Save_Data.isParseData = true;
          if(usefullBuffer[0] != '0')
            Save_Data.isUsefull = true;
          else// if(usefullBuffer[0] == 'V')
            Save_Data.isUsefull = false;
          
        }
        else
        {
          errorLog(2);	//��������
        }
      }
      
      
    }
  }

	t=strlen(Save_Data.UTCTime);
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
  // else
   	return 0;
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
