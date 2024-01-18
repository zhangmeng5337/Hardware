#include "uart3.h"
#include "stm8l15x.h"
#include "stm8l15x_usart.h"
#include "gps.h" 

 char  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
 u16 USART_RX_STA;         		//����״̬���
void Uart3_Init(u32 boundrate)
{
 /** #if module == smartbox
   CLK_PeripheralClockConfig(CLK_Peripheral_USART2,ENABLE);
          GPIO_Init(GPIOE, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);//TXD
  	GPIO_Init(GPIOE, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT);//RXD
  	USART_DeInit(USART2);		//��λUART1 
        
        USART_Init(USART2,9600, USART_WordLength_8b, USART_StopBits_1, 
				USART_Parity_No, USART_Mode_Tx|USART_Mode_Rx);
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//���������ж�
        USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//���������ж�
	//USART_ITConfig(USART1, USART_IT_TC, ENABLE);  //���������ж�
	USART_Cmd(USART2, ENABLE);	//ʹ��UART2     
  
  
  
#else if module == sensor*/
        CLK_PeripheralClockConfig(CLK_Peripheral_USART3,ENABLE);
        
    //    SYSCFG_REMAPDeInit();
        //SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA,ENABLE);
        
	GPIO_Init(GPIOE, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast);//TXD
  	GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_In_FL_No_IT);//RXD
  	USART_DeInit(USART3);		//��λUART1 
 

	/*
	 * ��UART1����Ϊ��
	 * ������ = 115200
	 * ����λ = 8
	 * 1λֹͣλ
	 * ��У��λ
	 * ʹ�ܽ��պͷ���
	 */
	//USART_Init(USART1,boundrate, USART_WordLength_8b, USART_StopBits_1, 
	//			USART_Parity_No, USART_Mode_Tx|USART_Mode_Rx);
        USART_Init(USART3,boundrate, USART_WordLength_8b, USART_StopBits_1, 
				USART_Parity_No, USART_Mode_Tx|USART_Mode_Rx);
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//���������ж�
        USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//���������ж�
	//USART_ITConfig(USART1, USART_IT_TC, ENABLE);  //���������ж�
	USART_Cmd(USART3, ENABLE);	//ʹ��UART2
//endif
}

/*******************************************************************************
 * ����: UART1_SendByte
 * ����: UART1����һ���ֽ�
 * �β�: data -> Ҫ���͵��ֽ�
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
void UART3_SendByte(u8 data)
{
  	USART_SendData8(USART3, data);
	/* �ȴ�������� */
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}

/*******************************************************************************
 * ����: UART1_SendStr
 * ����: UART1����len���ַ�
 * �β�: data -> ָ��Ҫ���͵��ַ���
 *       len -> Ҫ���͵��ֽ���
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
void UART3_SendStr(u8 *str)
{
	while(*str != '\0')
	{
		UART3_SendByte(*str++);	/* ѭ�����÷���һ���ַ����� */
	}	
}

/*******************************************************************************
 * ����: UART2_ReceiveByte
 * ����: UART2����һ���ַ�
 * �β�: ��
 * ����: ���յ����ַ�
 * ˵��: �� 
 ******************************************************************************/
u8 UART3_ReceiveByte(void)
{
	u8 UART3_RX_BUF; 
//#if module == smartbox
//	/* �ȴ�������� */
//	while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);
//	
//	UART3_RX_BUF = USART_ReceiveData8(USART3);
//#else if module == sensor
	/* �ȴ�������� */
	while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);
	
	UART3_RX_BUF = USART_ReceiveData8(USART3);	
//#endif
	return  UART3_RX_BUF;
}

//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
//u16 USART_RX_STA=0;       //����״̬���	  
u16 point1 = 0;
_SaveData Save_Data;
void uart3_interrupt_handler()
{
   u8 Res;
  Res =UART3_ReceiveByte();//(USART1->DR);	//��ȡ���յ�������
    
    if(Res == '$')
    {
      point1 = 0;	
    }
    
    
    USART_RX_BUF[point1++] = Res;
    
    if(USART_RX_BUF[0] == '$' && USART_RX_BUF[4] == 'M' && USART_RX_BUF[5] == 'C')			//ȷ���Ƿ��յ�"GPRMC/GNRMC"��һ֡����
    {
      if(Res == '\n')									   
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

