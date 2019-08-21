#include "stm8l15x.h"
#include "sysclock.h"
#include "uart1.h"
#include "stm8l15x_clk.h"
#include "sx1276.h"
#include "stm8l15x_spi.h"
#include "bsp.h"
#include "stm8l15x_it.h"
#include "stm8l15x_tim3.h"
#include "stm8l15x_rtc.h"
#include "stm8l15x_flash.h"
#include "stm8l15x_pwr.h"
#include "bsp.h"
#include "RF.h"
void GPIO_Initial(void)
{
  
  
  
  /* 配置SX127X相关控制引脚NSS(PB4), RST(PA3), TX_CTRL(PB3), RX_CTRL(PB2), OSEN(PA2)*/
  
  GPIO_Init(PORT_SX127X_CSN, PIN_SX127X_CSN, GPIO_Mode_Out_PP_High_Slow);
  GPIO_SetBits(PORT_SX127X_CSN, PIN_SX127X_CSN);
  
  GPIO_Init(PORT_SX127X_RST, PIN_SX127X_RST, GPIO_Mode_Out_PP_High_Slow);
  GPIO_SetBits(PORT_SX127X_RST, PIN_SX127X_RST);
  
  GPIO_Init(PORT_SX127X_TX_CTRL, PIN_SX127X_TX_CTRL, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_ResetBits(PORT_SX127X_TX_CTRL, PIN_SX127X_TX_CTRL);
  
  GPIO_Init(PORT_SX127X_RX_CTRL, PIN_SX127X_RX_CTRL, GPIO_Mode_Out_PP_High_Slow);
  GPIO_SetBits(PORT_SX127X_RX_CTRL, PIN_SX127X_RX_CTRL);
  
 
  
  /*SX1276数据接收外部中断*/
   GPIO_Init( PORT_SX127X_DIO3, PIN_SX127X_DIO4|PIN_SX127X_DIO5, GPIO_Mode_In_FL_No_IT );  
   GPIO_Init(PORT_SX127X_DIO0,  PIN_SX127X_DIO2, GPIO_Mode_In_FL_No_IT);
  // dio0
  GPIO_Init(PORT_SX127X_DIO0, PIN_SX127X_DIO0, GPIO_Mode_In_PU_IT);
  EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Rising);
  //dio3
  GPIO_Init(PORT_SX127X_DIO3, PIN_SX127X_DIO3, GPIO_Mode_In_PU_IT);   
  EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Rising);
  
  
}

void SPI_Initial(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);
  SPI_DeInit(SPI1);
  
  // 配置SPI相关参数,2分频（8MHZ）
  SPI_Init(SPI1, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_2,
           SPI_Mode_Master, SPI_CPOL_Low, SPI_CPHA_1Edge,
           SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft, 7);
  SPI_Cmd(SPI1, ENABLE);
  
  // SPI相关IO口配置
  GPIO_Init(PORT_SPI, PIN_MISO, GPIO_Mode_In_PU_No_IT);       // MISO (PB7)
  GPIO_Init(PORT_SPI, PIN_SCLK, GPIO_Mode_Out_PP_High_Slow);  // SCLK (PB5)
  GPIO_Init(PORT_SPI, PIN_MOSI, GPIO_Mode_Out_PP_High_Slow);  // MOSI (PB6)
}




void RTC_Config(uint16_t time) 
{
  RTC_DeInit(); //初始化默认状态 
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //允许RTC时钟 
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_64); //选择RTC时钟源LSI、38K/64 
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16); //38k/64/16=37Hz t=26.9473ms 
  if (GetPowerMode().CurrentMode  == PowerSavingMode){
    
    RTC_ITConfig(RTC_IT_WUT, ENABLE); //开启中断 
    RTC_SetWakeUpCounter(time); //设置RTC Weakup计算器初值 
    RTC_WakeUpCmd(ENABLE); //使能自动唤醒 
    
  }
  else if(GetPowerMode().CurrentMode == NormalMode){
    RTC_ITConfig(RTC_IT_WUT, DISABLE); //开启中断 
    RTC_SetWakeUpCounter(time); //设置RTC Weakup计算器初值 
    RTC_WakeUpCmd(DISABLE); //使能自动唤醒 		
  }
  
}
void USART_SendStr(unsigned char *Str) 
{
  while(*Str!=0)
  {
    USART_SendData8(USART1, *Str);
    while(!USART_GetFlagStatus (USART1, USART_FLAG_TXE));
    Str++;
  }
}
void HardwareInit()
{
  disableInterrupts();
  SystemClock_Init();     // system clk init
      
  GPIO_Initial();         // gpio init
  SPI_Initial();
  Uart1_Init();
  enableInterrupts();
  RF_Initial();         // lora init
  
}

/*===========================================================================
* 函数 ：SPI_ExchangeByte() => 通过SPI进行数据交换                          *
* 输入 ：需要写入SPI的值                                                    *
* 输出 ：通过SPI读出的值                                                    *
============================================================================*/
INT8U SPI_ExchangeByte(INT8U input)
{
    SPI_SendData(SPI1, input);
    while (RESET == SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE));   // 等待数据传输完成
    while( SPI_GetFlagStatus(SPI1, SPI_FLAG_BSY) == SET );
    //while (RESET == SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE));  // 等待数据接收完成
    return (SPI_ReceiveData(SPI1));
}
//low power mode init
void EnterStopMode(void) 
{
  
  
  disableInterrupts(); 
  SX1276_LoRa_SetMode( LORA_MODE_SLEEP );//lora enter sleep mode
 
  //GPIOA
  GPIO_Init( PORT_SX127X_TX_CTRL, PIN_SX127X_RX_CTRL|PIN_SX127X_TX_CTRL, GPIO_Mode_Out_PP_Low_Slow ); //lora tx and rx enbale signals 
  GPIO_Init( GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Slow );
  GPIO_Init( GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Slow );
  
  //GPIOB
  GPIO_Init(LORA_AUX_PORT, LORA_AUX, GPIO_Mode_Out_PP_Low_Slow);       // 
  GPIO_Init(LORA_M0_PORT, LORA_M0, GPIO_Mode_Out_PP_Low_Slow);       // 
  GPIO_Init(LORA_M1_PORT, LORA_M1, GPIO_Mode_Out_PP_Low_Slow);       //    
  GPIO_SetBits( PORT_SX127X_CSN, PIN_SX127X_CSN ); 
  
  //GPIOC
  GPIO_Init( GPIOC, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow );
  GPIO_Init( PORT_SX127X_DIO3, GPIO_Pin_0|PIN_SX127X_DIO3|PIN_SX127X_DIO4|PIN_SX127X_DIO5, GPIO_Mode_Out_PP_Low_Slow );  
  
  //GPIOD
  GPIO_Init( GPIOD, GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Slow );   
  
  //close clk
  TIM2_DeInit();
  TIM3_DeInit();
  TIM4_DeInit();
  ADC_DeInit(ADC1);
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,DISABLE);
  while((TIM2->CR1 & TIM_CR1_CEN) != 0x00); 
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,DISABLE);
  
  TIM3_Cmd(DISABLE); 
  while((TIM3->CR1 & TIM_CR1_CEN) != 0x00); 
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
  
  TIM4_Cmd(DISABLE); 
  while((TIM4->CR1 & TIM4_CR1_CEN) != 0x00);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4,DISABLE);
  
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1,DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_I2C1,DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI1,DISABLE);  
 
  PWR_UltraLowPowerCmd(ENABLE); //low power enable
  PWR_FastWakeUpCmd(ENABLE);  //wake up enable
  enableInterrupts();
  halt();  //enter stop mode
}