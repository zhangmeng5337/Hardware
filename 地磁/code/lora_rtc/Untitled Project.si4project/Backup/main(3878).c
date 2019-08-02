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


#define debug   1
#define NormalMode      0x00
#define WakeMode        0x10
#define PowerSavingMode 0x01

u8 CurrentMode = 0;
u8 ConfigureDone = 0;
u8 lora_mode_satus;//dio0 ira flag 
u16 wake_up_period=384;//19------>500ms

uint32_t rx_length;  //payload count in debug mode
INT8U recv_buffer[128]={ 0 }; //receive data buffer
u8 length_tmp;
INT8U length = 0;  //every payload length 

unsigned char RtcWakeUp_tmp;
uint32_t receive_timeout; //
unsigned int scan_timeout;


void EnterStopMode(void) ;
void HardwareInit(void);

void DelayMs(INT8U x)
{
  unsigned int i;
  i=1000;
  for(;x>0;x--)  
    for(i=1000;i>0;i--)
      ;
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

void LED_Init(void)
{
  GPIO_Init(GPIOD, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Fast);
}

void delay_ms(u16 num)//不是很精确
{
  u16 i = 0;
  while(num--)
  {
    for (i=0; i<1266; i++);
  }
}

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

void RF_Initial( )
{
  SX1276_Init(MODE_LORA);         //SX127X 模块复位并初始化为LoRa模式
  SX1276_LoRa_SetDataRate(2);
  SX1276_SetFreq(0);				//配置信道为23，即433Hz
  SX1276_SetPower(15, 1);         //配置RFO PIN输出，功率20dBm
  SX1276_SetPreambleSize(65530);
  SX1276_SetRxMode(); 
}


void RTC_Config(uint16_t time) 
{
  RTC_DeInit(); //初始化默认状态 
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //允许RTC时钟 
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_64); //选择RTC时钟源LSI、38K/64 
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16); //38k/64/16=37Hz t=26.9473ms 
  if (CurrentMode == PowerSavingMode){
    
    RTC_ITConfig(RTC_IT_WUT, ENABLE); //开启中断 
    RTC_SetWakeUpCounter(time); //设置RTC Weakup计算器初值 
    RTC_WakeUpCmd(ENABLE); //使能自动唤醒 
    
  }
  else if(CurrentMode == NormalMode){
    RTC_ITConfig(RTC_IT_WUT, DISABLE); //开启中断 
    RTC_SetWakeUpCounter(time); //设置RTC Weakup计算器初值 
    RTC_WakeUpCmd(DISABLE); //使能自动唤醒 		
  }
  
}


 

void lora_rx_process()

{
  if(lora_mode_satus == 1)
  {
    length = SX1276_ReceivePacket(recv_buffer);			 // 检测是否收到一个数据包   
    if( length )
    {
      #if debug == 1
        rx_length = rx_length + length +4;
      #endif
      length_tmp = length+length_tmp;
      length = 0; 
    }  
    
  }
  
}

void PowerSavingMode_setting()
{   
   char i = 0; 
  if(ConfigureDone == 1)
  {
    if(RtcWakeUp !=0)
    {
      
      if(RtcWakeUp == 1)
      {   
        RtcWakeUp = 2;
        RtcWakeUp_tmp = 2;
        lora_mode_satus = 0;
        receive_timeout=0;             
        SX1276_SetPreambleSize(65530);//preamble 
        SX1276_CADMode();
        receive_timeout=0;     
      }
      
      while(CadDoneFlag==1)//preamble detect
      {      
        if(CadDoneFlag == 1)
        {
          RtcWakeUp_tmp = 0;
          receive_timeout=0;
          RTC_WakeUpCmd(DISABLE);
          SX1276_SetRxMode();
          CadDoneFlag = 0;         
        }
        while(receive_timeout <= 200000)//effect data timeout calculate
        {
          receive_timeout = receive_timeout + 1;
          if(lora_mode_satus==1)//effect data irq
          { 
            if(receive_timeout>=200000)
              receive_timeout = 500000;
            LED_TOG();
            lora_rx_process();    //data receive  
            for(i=0;i<length_tmp;i++)
            {
              USART_SendData8(USART1, recv_buffer[i]);
              while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
            }
            length_tmp = 0;   
            receive_timeout=500000;
            disableInterrupts();
            RTC_Config(wake_up_period);
            enableInterrupts();          
            SX1276_LoRa_SetMode( LORA_MODE_SLEEP ); 
            lora_mode_satus = 2;
            //RTC_WakeUpCmd(ENABLE);
          }                   
        }       
      }
        scan_timeout=scan_timeout+1;  //no cad timeout calculate    
        if(lora_mode_satus!=2&&scan_timeout>=10000)
        {
          scan_timeout=0;
          lora_mode_satus = 0;
          receive_timeout = 0; 
          SX1276_LoRa_SetMode( LORA_MODE_SLEEP );
          disableInterrupts();
          RTC_WakeUpCmd(ENABLE); 
          enableInterrupts();          
         EnterStopMode();//enter stop mode 
          HardwareInit(); // hardare init after wake up       
        }
      
    }
    
  } 
  else
  {
    disableInterrupts();
    GPIO_Init(PORT_SX127X_DIO3, PIN_SX127X_DIO3, GPIO_Mode_In_FL_IT);
    GPIO_Init(PORT_SX127X_DIO1, PIN_SX127X_DIO1, GPIO_Mode_In_FL_IT);  
    EXTI_SetPinSensitivity(EXTI_Pin_2, EXTI_Trigger_Rising);    
    EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Rising);
    RTC_Config(wake_up_period);
    enableInterrupts();
    ConfigureDone = 1; 
  }
  
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

void HardwareInit()
{
  disableInterrupts();
  SystemClock_Init();     // system clk init
      
  GPIO_Initial();         // gpio init
  SPI_Initial();
  CurrentMode =PowerSavingMode ;//NormalMode  PowerSavingMode 
  Uart1_Init();
  lora_mode_satus = 0;
  enableInterrupts();
  RF_Initial();         // lora init
  
}
void main(void)
{	
  
  
  HardwareInit();
  LED_Init();  //led init
  GPIO_SetBits(GPIOD, GPIO_Pin_0);//led off
  while(1)
  {
    PowerSavingMode_setting();
  }
}





