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
USART1_INIT：串口1初始化函数
功能：配置并使能串口1
参数：baud:波特率值	
**********************************************************************************************************/
void USART1_INIT(unsigned long baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;  
	  NVIC_InitTypeDef NVIC_InitStruct;
	  USART_InitTypeDef USART_InitStructure;
	
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);       //  使能GPIOA时钟
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);   //  使能串口1时钟
	
	  //串口1对应引脚复用
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);     
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);	
	
	  //GPIO端口初始化为复用
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;              
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	  //USART1端口初始化
	  USART_InitStructure.USART_BaudRate            = baud;
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	  USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
		USART_InitStructure.USART_Parity              = USART_Parity_No;
		USART_InitStructure.USART_StopBits            = USART_StopBits_1;
		USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	  USART_Init(USART1, &USART_InitStructure);
		
		
//#ifdef EN_USART1_RX		

		//  串口NVIC配置
	  NVIC_InitStruct.NVIC_IRQChannel         = USART1_IRQn;
	  NVIC_InitStruct.NVIC_IRQChannelCmd      = ENABLE;
	  NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
	  NVIC_Init(&NVIC_InitStruct);			
		/* NVIC configuration */
  /* Enable the USARTx Interrupt */


	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
//#endif	
	USART_Cmd(USART1, ENABLE);//使能串口1
		
}

/**********************************************************************************************************
Putchar：串口发送函数
功能：发送一个字符
参数：ch：字符				
**********************************************************************************************************/
unsigned char Putchar(unsigned char ch)
{
		USART_SendData(USART1, (unsigned char) ch);	
		while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
		USART_ClearFlag(USART1, USART_FLAG_TXE);
		return ch;
}

/**********************************************************************************************************
PutString：串口发送函数
功能：发送一帧字符串数据
参数：*dat：字符串数组
       len：长度
**********************************************************************************************************/
void PutString(unsigned char *dat, unsigned char len)
{
	unsigned int    i;
	for(i = 0; i < len; i++) {
		  Putchar(*dat++);
	}
}

/**********************************************************************************************************
USART_RXD_Action：串口接收函数
功能：接收一帧数据
参数: time_sta：定时1ms时间标志输入
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
BoundRemanCode：获取波特率值函数
功能：波特率编码对应波特率值映射函数
参数：BaudCode：波特率对应编码值
**********************************************************************************************************/
unsigned long BoundRemanCode(unsigned char BaudCode)
{
	  unsigned long bound[10]={1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 128000, 256000};
    return bound[BaudCode];
}

/**********************************************************************************************************
USART1_IRQHandler：串口接收中断
功能：接收一帧数据
参数：
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



