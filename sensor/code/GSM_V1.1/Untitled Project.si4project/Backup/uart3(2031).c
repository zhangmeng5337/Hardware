#include "uart3.h"
#include "stm8l15x.h"
#include "stm8l15x_usart.h"
#include "gps.h" 

 char  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
 u16 USART_RX_STA;         		//接收状态标记
void Uart3_Init(u32 boundrate)
{
 /** #if module == smartbox
   CLK_PeripheralClockConfig(CLK_Peripheral_USART2,ENABLE);
          GPIO_Init(GPIOE, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);//TXD
  	GPIO_Init(GPIOE, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT);//RXD
  	USART_DeInit(USART2);		//复位UART1 
        
        USART_Init(USART2,9600, USART_WordLength_8b, USART_StopBits_1, 
				USART_Parity_No, USART_Mode_Tx|USART_Mode_Rx);
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启接收中断
        USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//开启接收中断
	//USART_ITConfig(USART1, USART_IT_TC, ENABLE);  //开启发送中断
	USART_Cmd(USART2, ENABLE);	//使能UART2     
  
  
  
#else if module == sensor*/
        CLK_PeripheralClockConfig(CLK_Peripheral_USART3,ENABLE);
        
        SYSCFG_REMAPDeInit();
        //SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA,ENABLE);
        
	GPIO_Init(GPIOE, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast);//TXD
  	GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_In_PU_No_IT);//RXD
  	USART_DeInit(USART3);		//复位UART1 
 

	/*
	 * 将UART1配置为：
	 * 波特率 = 115200
	 * 数据位 = 8
	 * 1位停止位
	 * 无校验位
	 * 使能接收和发送
	 */
	//USART_Init(USART1,boundrate, USART_WordLength_8b, USART_StopBits_1, 
	//			USART_Parity_No, USART_Mode_Tx|USART_Mode_Rx);
        USART_Init(USART3,boundrate, USART_WordLength_8b, USART_StopBits_1, 
				USART_Parity_No, USART_Mode_Tx|USART_Mode_Rx);
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启接收中断
        USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//开启接收中断
	//USART_ITConfig(USART1, USART_IT_TC, ENABLE);  //开启发送中断
	USART_Cmd(USART3, ENABLE);	//使能UART2
//endif
}

/*******************************************************************************
 * 名称: UART1_SendByte
 * 功能: UART1发送一个字节
 * 形参: data -> 要发送的字节
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/
void UART3_SendByte(u8 data)
{
  	USART_SendData8(USART3, data);
	/* 等待传输结束 */
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}

/*******************************************************************************
 * 名称: UART1_SendStr
 * 功能: UART1发送len个字符
 * 形参: data -> 指向要发送的字符串
 *       len -> 要发送的字节数
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/
void UART3_SendStr(u8 *str)
{
	while(*str != '\0')
	{
		UART3_SendByte(*str++);	/* 循环调用发送一个字符函数 */
	}	
}

/*******************************************************************************
 * 名称: UART2_ReceiveByte
 * 功能: UART2接收一个字符
 * 形参: 无
 * 返回: 接收到的字符
 * 说明: 无 
 ******************************************************************************/
u8 UART3_ReceiveByte(void)
{
	u8 UART3_RX_BUF; 
#if module == smartbox
	/* 等待接收完成 */
	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
	
	UART3_RX_BUF = USART_ReceiveData8(USART2);
#else if module == sensor
	/* 等待接收完成 */
	while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);
	
	UART3_RX_BUF = USART_ReceiveData8(USART3);	
#endif
	return  UART3_RX_BUF;
}

//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
//u16 USART_RX_STA=0;       //接收状态标记	  
u16 point1 = 0;
_SaveData Save_Data;
void uart3_interrupt_handler()
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

