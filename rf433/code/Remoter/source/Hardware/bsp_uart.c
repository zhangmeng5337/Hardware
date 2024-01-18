#include "bsp_uart.h"
#include "stdio.h"
#include "ddl.h"
#include "uart.h"
#include "gpio.h"
#include "bt.h"
/**
* @brief  Configure USART peripheral  
* @param  None
* @retval None
*/
void RxIntCallback(void);
void ErrIntCallback(void);
unsigned char UsartReceiveData[BUFFERSIZE];
unsigned char UsartReceiveFlag,usart_i,uartReceivewriteIndex;
void UART_Config(void)
{
  uint16_t timer=0;
  uint32_t pclk=0;

  stc_uart_config_t  stcConfig;
  stc_uart_irq_cb_t stcUartIrqCb;
  stc_uart_multimode_t stcMulti;
  stc_uart_baud_config_t stcBaud;
  stc_bt_config_t stcBtConfig;
  
  
  DDL_ZERO_STRUCT(stcUartIrqCb);
  DDL_ZERO_STRUCT(stcMulti);
  DDL_ZERO_STRUCT(stcBaud);
  DDL_ZERO_STRUCT(stcBtConfig);
  
  
  //  Gpio_InitIO(T1_PORT,T1_PIN,GpioDirIn); 
  // Gpio_InitIO(0,3,GpioDirOut);
  // Gpio_SetIO(0,3,1);
  
  Gpio_InitIOExt(UART1_PORT,UART1_TXD,GpioDirOut,TRUE,FALSE,FALSE,FALSE);   
  Gpio_InitIOExt(UART1_PORT,UART1_RXD,GpioDirOut,TRUE,FALSE,FALSE,FALSE); 
  
  //ͨ���˿�����
  Gpio_SetFunc_UART1TX_P35();
  Gpio_SetFunc_UART1RX_P36();
  
  //����ʱ��ʹ��
  Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);//ģʽ0/2���Բ�ʹ��
  Clk_SetPeripheralGate(ClkPeripheralUart1,TRUE);
  
  
  
  stcUartIrqCb.pfnRxIrqCb = RxIntCallback;
  stcUartIrqCb.pfnTxIrqCb = NULL;
  stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback;
  stcConfig.pstcIrqCb = &stcUartIrqCb;
  stcConfig.bTouchNvic = TRUE;
  
  
  stcConfig.enRunMode = UartMode3;//��������Ĵ˴���ת��4��ģʽ����
  
  
  stcMulti.enMulti_mode = UartNormal;//��������Ĵ˴���ת��������ģʽ��mode2/3���ж�����ģʽ
  
  stcConfig.pstcMultiMode = &stcMulti;
  
  stcBaud.bDbaud = 0u;//˫�������ʹ���
  stcBaud.u32Baud =4800u;//���²�����λ��
  stcBaud.u8Mode = UartMode3; //���㲨������Ҫģʽ����
  pclk = Clk_GetPClkFreq();
  timer=Uart_SetBaudRate(UARTCH1,pclk,&stcBaud);
  
  stcBtConfig.enMD = BtMode2;
  stcBtConfig.enCT = BtTimer;
  Bt_Init(TIM1, &stcBtConfig);//����basetimer1���ú�������������
  Bt_ARRSet(TIM1,timer);
  Bt_Cnt16Set(TIM1,timer);
  Bt_Run(TIM1);
  
  Uart_Init(UARTCH1, &stcConfig);
  Uart_EnableIrq(UARTCH1,UartRxIrq);
  Uart_ClrStatus(UARTCH1,UartRxFull);
  Uart_EnableFunc(UARTCH1,UartRx);
  
}

void USART_SendStr(unsigned char *Str) 
{
  while(*Str!=0)
  {
    Uart_SendData(UARTCH1, *Str);
    Str++;
  }
}
void UART1_SendByte(uint8_t data)
{  	
  Uart_SendData(UARTCH1, data);
  ;
}
void UART1_SendBytes(uint8_t *data,uint8_t len)
{  	
  unsigned char i;
  i=0;
  while(len--)
  Uart_SendData(UARTCH1, data[i++]);
  ;
}
/*******************************************************************************
* ����: fputc
* ����: �ض���c�⺯��printf��UART1
* �β�: ��
* ����: Ҫ��ӡ���ַ�
* ˵��: ��printf���� 
******************************************************************************/
//#ifdef _IAR_
//typedef struct __FILE FILE;
//int fputc(int ch, FILE *f)
//{  
//  /* ��Printf���ݷ������� */
//  Uart_SendData(UARTCH1, ch);
//  
//  return (ch);
//}
//#else
//PUTCHAR_PROTOTYPE
//{
//  /* Write a character to the UART1 */
//  Uart_SendData(UARTCH1, c);
//  return (c);
//}
//#endif

//GETCHAR_PROTOTYPE
//{
//#ifdef _COSMIC_
//  char c = 0;
//#else
//  int c = 0;
//#endif
//  /* Loop until the Read data register flag is SET */
//  while (Uart_GetStatus(UARTCH1,UartRxFull) == FALSE);
//  c = Uart_ReceiveData(UARTCH1);
//  return (c);
//}
void RxIntCallback(void)
{
  
  if(Uart_GetStatus(UARTCH1,UartRxFull) == TRUE)
  {
    UsartReceiveFlag =1;
    Uart_ClrStatus(UARTCH1,UartRxFull);//����жϱ�־
    UsartReceiveData[uartReceivewriteIndex] = Uart_ReceiveData(UARTCH1);
    uartReceivewriteIndex++;
    if(uartReceivewriteIndex>=BUFFERSIZE)
      uartReceivewriteIndex=0;
  }
}
void ErrIntCallback(void)
{
  Uart_ClrStatus(UARTCH1,UartRFRAMEError);
}

