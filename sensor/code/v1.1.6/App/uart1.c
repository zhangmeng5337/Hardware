#include "uart1.h"
#include "stm8l15x.h"
#include "stm8l15x_usart.h"

_uart uart1;
void Uart1_Init(u32 boundrate)
{

  CLK_PeripheralClockConfig(CLK_Peripheral_USART1,ENABLE);
   USART_DeInit(USART1);		//��λUART1 
  SYSCFG_REMAPDeInit();
//#if module == smartbox
//  //SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortC,ENABLE);
//  GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast);//TXD
//  GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT);//RXD
//#else if module == sensor
  SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA,ENABLE);
 GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast);//TXD
  GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_Mode_In_FL_No_IT);//RXD
//#endif
  
   
  

  
  
  USART_Init(USART1,boundrate, USART_WordLength_8b, USART_StopBits_1, 
             USART_Parity_No, USART_Mode_Tx|USART_Mode_Rx);
  USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//���������ж�
  USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//���������ж�
  //USART_ITConfig(USART1, USART_IT_TC, ENABLE);  //���������ж�
  USART_ClearITPendingBit(USART1,  USART_IT_IDLE);
  USART_Cmd(USART1, ENABLE);	//ʹ��UART2
}

/*******************************************************************************
* ����: UART1_SendByte
* ����: UART1����һ���ֽ�
* �β�: data -> Ҫ���͵��ֽ�
* ����: ��
* ˵��: �� 
******************************************************************************/
void UART1_SendByte(u8 data)
{
  USART_SendData8(USART1, data);
  /* �ȴ�������� */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/*******************************************************************************
* ����: UART1_SendStr
* ����: UART1����len���ַ�
* �β�: data -> ָ��Ҫ���͵��ַ���
*       len -> Ҫ���͵��ֽ���
* ����: ��
* ˵��: �� 
******************************************************************************/
void UART1_SendStr(u8 *str)
{
  unsigned char len;
  len = strlen((const char*)str);
	
  while(len>0)
  {
    UART1_SendByte(*str);
   // while(!USART_GetFlagStatus (USART1, USART_FLAG_TXE));
    str++;
    len--;
  }
}


/*******************************************************************************
* ����: UART2_ReceiveByte
* ����: UART2����һ���ַ�
* �β�: ��
* ����: ���յ����ַ�
* ˵��: �� 
******************************************************************************/
u8 UART1_ReceiveByte(void)
{
  u8 UART1_RX_BUF; 
  
  /* �ȴ�������� */
  while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
  
  UART1_RX_BUF = USART_ReceiveData8(USART1);
  
  return  UART1_RX_BUF;
}

/*******************************************************************************
* ����: fputc
* ����: �ض���c�⺯��printf��UART1
* �β�: ��
* ����: Ҫ��ӡ���ַ�
* ˵��: ��printf���� 
******************************************************************************/
#ifdef _IAR_
int fputc(int ch, FILE *f)
{  
  /* ��Printf���ݷ������� */
  UART1_SendByte(ch);
  
  return (ch);
}
#else
PUTCHAR_PROTOTYPE
{
  /* Write a character to the UART1 */
  UART1_SendByte(c);
  return (c);
}
#endif

GETCHAR_PROTOTYPE
{
#ifdef _COSMIC_
  char c = 0;
#else
  int c = 0;
#endif
  /* Loop until the Read data register flag is SET */
  while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
  c = USART_ReceiveData8(USART1);
  return (c);
}

void uart1_interrupt_handler()
{
  unsigned char t;
  uart1.isGetData =1;
  //uart_str.receive_flag ++;
  t = USART1->SR;
  t = USART1->DR;//IDLE������Ҫ���ζ�SR ��DR �Ĵ���
  USART_ClearITPendingBit(USART1,  USART_IT_IDLE);
  uart1.isLen =  DMA_GetCurrDataCounter(DMA1_Channel2);
  uart1.index = uart1.index + uart1.isLen;
  if(uart1.index >= BUFFERSIZE)
    uart1.index = uart1.index - BUFFERSIZE;
  DMA_START_RX();//���¿�������DMA����ģʽ
}
void USART1_CLR_Buf(void)                           // ���ڻ�������
{
  memset(uart1.Uart_Buffer , 0, BUFFERSIZE);      //���                  
}