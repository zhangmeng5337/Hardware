/**--------------File Info------------------------------------------------------------------------------
** File name:			usart.c
** Last modified Date:  2017-08-8
** Last Version:		V1.02
** Descriptions:		usart1 Config
**------------------------------------------------------------------------------------------------------
** Created date:		2017-08-8
** Version:				V1.02
** Descriptions:		usart1 Config
**
********************************************************************************************************/
#define __USART_MANL
#include "stm32f0xx.h"
#include "usart.h"
#include "stdio.h"
#include "RF200_DRV.h"

unsigned char    USART_RXD_Buf[USART_REC_LEN];

unsigned int     USART_RXD_Cnt = 0;  

/**********************************************************************************************************
USART1_INIT������1��ʼ������
���ܣ����ò�ʹ�ܴ���1
������baud:������ֵ	
**********************************************************************************************************/
void USART1_INIT(unsigned long baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;  
	  NVIC_InitTypeDef NVIC_InitStruct;
	  USART_InitTypeDef USART_InitStructure;
	
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);       //  ʹ��GPIOAʱ��
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);   //  ʹ�ܴ���1ʱ��
	
	  //����1��Ӧ���Ÿ���
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);     
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);	
	
	  //GPIO�˿ڳ�ʼ��Ϊ����
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;              
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	  //USART1�˿ڳ�ʼ��
	  USART_InitStructure.USART_BaudRate            = baud;
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	  USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
		USART_InitStructure.USART_Parity              = USART_Parity_No;
		USART_InitStructure.USART_StopBits            = USART_StopBits_1;
		USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	  USART_Init(USART1, &USART_InitStructure);
		
		
//#ifdef EN_USART1_RX		

		//  ����NVIC����
	  NVIC_InitStruct.NVIC_IRQChannel         = USART1_IRQn;
	  NVIC_InitStruct.NVIC_IRQChannelCmd      = ENABLE;
	  NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
	  NVIC_Init(&NVIC_InitStruct);			
		/* NVIC configuration */
  /* Enable the USARTx Interrupt */


	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
//#endif	
	USART_Cmd(USART1, ENABLE);//ʹ�ܴ���1
		
}

/**********************************************************************************************************
Putchar�����ڷ��ͺ���
���ܣ�����һ���ַ�
������ch���ַ�				
**********************************************************************************************************/
unsigned char Putchar(unsigned char ch)
{
		USART_SendData(USART1, (unsigned char) ch);	
		while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
		USART_ClearFlag(USART1, USART_FLAG_TXE);
		return ch;
}

/**********************************************************************************************************
PutString�����ڷ��ͺ���
���ܣ�����һ֡�ַ�������
������*dat���ַ�������
       len������
**********************************************************************************************************/
void PutString(unsigned char *dat, unsigned char len)
{
	unsigned int    i;
	for(i = 0; i < len; i++) {
		  Putchar(*dat++);
	}
}

/**********************************************************************************************************
USART_RXD_Action�����ڽ��պ���
���ܣ�����һ֡����
����: time_sta����ʱ1msʱ���־����
**********************************************************************************************************/
void USART_RXD_Data_Process(void)
{
	unsigned int    length  ;    //  data length                     
	if (USART_RXD_Cnt>5) 
	{
		length = USART_RXD_Buf[4];
		if(USART_RXD_Cnt>=(length+7)&&USART_RXD_Buf[length+6] == 0x7e)//revice frame end
		{
			Usart_buff_copy(USART_RXD_Buf,length+7);
			USART_RXD_Cnt = 0;
		}
	} 
}

/**********************************************************************************************************
BoundRemanCode����ȡ������ֵ����
���ܣ������ʱ����Ӧ������ֵӳ�亯��
������BaudCode�������ʶ�Ӧ����ֵ
**********************************************************************************************************/
unsigned long BoundRemanCode(unsigned char BaudCode)
{
	  unsigned long bound[10]={1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 128000, 256000};
    return bound[BaudCode];
}

/**********************************************************************************************************
USART1_IRQHandler�����ڽ����ж�
���ܣ�����һ֡����
������
**********************************************************************************************************/

void USART1_IRQHandler(void)
{
    unsigned  char    res;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
			  res = USART_ReceiveData(USART1);
				if((USART_RXD_Cnt == 0)&&(res == 0xBB))// frame start must 0xbb
				{
					USART_RXD_Buf[0] = res;
					USART_RXD_Cnt++; 
				}
        else if((USART_RXD_Cnt>0)&&(USART_RXD_Cnt < USART_REC_LEN))
        {
            USART_RXD_Buf[USART_RXD_Cnt] = res; 
            USART_RXD_Cnt++; 
        }
    }
}



