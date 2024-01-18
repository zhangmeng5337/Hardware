/*===========================================================================
* 网址 ：http://www.cdebyte.com/   http://yhmcu.taobao.com/                 *
* 作者 ：李勇  原 亿和电子工作室  现 亿佰特电子科技有限公司                 *
* 邮件 ：yihe_liyong@126.com                                                *
* 电话 ：18615799380                                                        *
============================================================================*/
#include "bsp.h"
#include "stm8l15x_iwdg.h"
#include "stm8l15x_iwdg.h"
#include "stm8l15x_rst.h"
#include "radio.h"



extern volatile Module_mode_stru Module_mode;
/**
  * @brief  Configures the IWDG to generate a Reset if it is not refreshed at the
  *         correct time. 
  * @param  None
  * @retval None
  */
 void IWDG_Config(void)
{

    if (RST_GetFlagStatus(RST_FLAG_IWDGF) != RESET)
  {
    /* IWDGF flag set */
    /* Turn on LED1 */

    /* Clear IWDGF Flag */
    RST_ClearFlag(RST_FLAG_IWDGF);
  }
  
  
  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable();
  
  /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
     dispersion) */
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  
  /* IWDG counter clock: LSI/128 */
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  
  /* Set counter reload value to obtain 250ms IWDG Timeout.
    Counter Reload Value = 250ms/IWDG counter clock period
                         = 250ms / (LSI/128)
                         = 0.25s / (LsiFreq/128)
                         = LsiFreq/(128 * 4)
                         = LsiFreq/512
   */
  IWDG_SetReload((uint8_t)(254));
  
  /* Reload IWDG counter */
  IWDG_ReloadCounter();
}




/*===========================================================================
* 函数 ：TIM3_Initial() => 初始化定时器3，定时时间为1ms                     *
============================================================================*/
void TIM3_Initial(void)
{
    TIM3_DeInit();

    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);

    // 配置Timer3相关参数，内部时钟为16MHZ，定时时间 1ms  需要计次125
    TIM3_TimeBaseInit(TIM3_Prescaler_128, TIM3_CounterMode_Up, 16250); //625定时5ms
    TIM3_ITConfig(TIM3_IT_Update, ENABLE);
    TIM3_Cmd(DISABLE);
}

void RTC_Config(uint16_t time) 
{
    RTC_DeInit(); //初始化默认状态 
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //允许RTC时钟 
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_2); //选择RTC时钟源LSI 38K、2=19K
    while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET);
    
    RTC_WakeUpCmd(DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //允许RTC时钟
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
    RTC_ITConfig(RTC_IT_WUT, ENABLE); //开启中断
    RTC_SetWakeUpCounter(time); //设置RTC Weakup计算器初值
    RTC_WakeUpCmd(ENABLE);
}
extern u8 CurrentMode ;
void EnterStopMode(void) 
{
    disableInterrupts(); 
    //SX1276_LoRa_SetMode( LORA_MODE_SLEEP );//lora enter sleep mode

    
    #if MODULE==0	 //GPIOA
    GPIO_Init( RADIO_TX_PORT, RADIO_TX_PIN|RADIO_RX_PIN, GPIO_Mode_Out_PP_Low_Slow ); //lora tx and rx enbale signals 
#endif
    GPIO_Init( GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Slow );
    GPIO_Init( GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Slow );

    //GPIOB
    GPIO_Init(GPIOB, PIN_SX127X_AUX, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOB, PIN_SX127X_M0, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOB, PIN_SX127X_M1, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_SetBits( RADIO_NSS_PORT, RADIO_NSS_PIN ); 

    //GPIOC
    GPIO_Init( GPIOC, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow );
    GPIO_Init( RADIO_DIO3_PORT, GPIO_Pin_0|RADIO_DIO3_PIN|PIN_SX127X_DIO4|PIN_SX127X_DIO5, GPIO_Mode_Out_PP_Low_Slow );  

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
    
    if(Module_mode.CurrentMode == SleepMode) 
    {
        SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA, DISABLE);
        GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_Mode_In_FL_IT);
        EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Rising);
    }
    enableInterrupts();
    halt();  //enter stop mode
}



void GPIO_int()
{
    GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
    GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
    GPIO_Init( GPIOC, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
    GPIO_Init( GPIOD, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  
  /****************************************
   RF_NSS
  ****************************************/
    GPIO_Init(RADIO_NSS_PORT, RADIO_NSS_PIN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(RADIO_NSS_PORT, RADIO_NSS_PIN);
     GPIO_Init(PORT_LED, PIN_LED, GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(PORT_LED, PIN_LED);   

  /****************************************
   RF_RST
  ****************************************/
    GPIO_Init(RADIO_nRESET_PORT, RADIO_nRESET_PIN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(RADIO_nRESET_PORT, RADIO_nRESET_PIN);
 #if MODULE==1	
    /*SX1276数据接收外部中断*/
    GPIO_Init(RADIO_DIO1_PORT, RADIO_DIO1_PIN, GPIO_Mode_In_PU_IT);                //dio1
    EXTI_SetPinSensitivity(EXTI_Pin_2, EXTI_Trigger_Rising);  

    //EXTI_SetHalfPortSelection(EXTI_HalfPort_D_LSB, ENABLE);
    /*SX1276数据接收外部中断*/
    //GPIO_Init(RADIO_DIO3_PORT, RADIO_DIO3_PIN, GPIO_Mode_In_FL_IT);                //dio3
    //EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Rising); 	

	/****************************************
   Radio_BUSY
  ****************************************/
    GPIO_Init(RADIO_BUSY_PORT, RADIO_BUSY_PIN, GPIO_Mode_In_PU_No_IT);                //Radio_BUSY
    //EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Rising);
   
    
    /*PB2 PB1 分别代表M0 M1*/
    GPIO_Init(PORT_SX127X_M0, PIN_SX127X_M0, GPIO_Mode_In_PU_No_IT);//GPIO_Mode_In_PU_IT
    EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Rising_Falling);
    GPIO_Init(PORT_SX127X_M1, PIN_SX127X_M1, GPIO_Mode_In_PU_No_IT);
    EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Rising_Falling);


    /*PB0 AUX*/
    GPIO_Init(PORT_SX127X_AUX, PIN_SX127X_AUX, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_ResetBits(PORT_SX127X_AUX, PIN_SX127X_AUX);




  //  GPIO_Init(RADIO_DIO2_PORT, RADIO_DIO2_PIN, GPIO_Mode_In_PU_IT);                //dio3
   // EXTI_SetPinSensitivity(EXTI_Pin_4, EXTI_Trigger_Rising); 
    
#else
 /*SX1276数据接收外部中断*/
    GPIO_Init(RADIO_DIO1_PORT, RADIO_DIO1_PIN, GPIO_Mode_In_PU_IT);                //dio1
    EXTI_SetPinSensitivity(EXTI_Pin_5, EXTI_Trigger_Rising);  

    //EXTI_SetHalfPortSelection(EXTI_HalfPort_D_LSB, ENABLE);
    /*SX1276数据接收外部中断*/
    //GPIO_Init(RADIO_DIO3_PORT, RADIO_DIO3_PIN, GPIO_Mode_In_FL_IT);                //dio3
    //EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Rising); 	

	/****************************************
   Radio_BUSY
  ****************************************/
    GPIO_Init(RADIO_BUSY_PORT, RADIO_BUSY_PIN, GPIO_Mode_In_PU_No_IT);                //Radio_BUSY
    //EXTI_SetPinSensitivity(EXTI_Pin_6, EXTI_Trigger_Rising);
    
     GPIO_Init(RADIO_TX_PORT, RADIO_TX_PIN, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_ResetBits(RADIO_TX_PORT, RADIO_TX_PIN);

    GPIO_Init(RADIO_RX_PORT, RADIO_RX_PIN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(RADIO_RX_PORT, RADIO_RX_PIN); 
#endif
}
void LedToggle(void)
{
  GPIO_ToggleBits(PORT_LED, PIN_LED);   
}
void HW_int(void)
{
  disableInterrupts(); 
  
  SystemClock_Init();     // 系统时钟初始化
  CLK_LSEConfig(CLK_LSE_OFF);
  //SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortC, DISABLE);
  // SYSCFG_REMAPPinConfig(REMAP_Pin_SPI1Full, DISABLE);
  // SYSCFG_REMAPPinConfig(REMAP_Pin_TIM2Channel1, DISABLE);  
  // SYSCFG_REMAPPinConfig(REMAP_Pin_TIM2Channel2, DISABLE);   
  // Tick_Configration();
  //RCC_Configuration();
  GPIO_int();
  Uart1_Init(115200, 0);
  SPI_Int();
  enableInterrupts();
  //LED_Init();             //调试LED初始化 
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
/*===========================================================================
* 函数 ：SPI_ExchangeByte() => 通过SPI进行数据交换                          *
* 输入 ：需要写入SPI的值                                                    *
* 输出 ：通过SPI读出的值                                                    *
============================================================================*/
//INT8U SPI_ExchangeByte(INT8U input)
//{
//    SPI_SendData(input);
//	while (RESET == SPI_GetFlagStatus(SPI_FLAG_TXE));   // 等待数据传输完成
//	while (RESET == SPI_GetFlagStatus(SPI_FLAG_RXNE));  // 等待数据接收完成
//	return (SPI_ReceiveData());
//}

/*===========================================================================
-----------------------------------文件结束----------------------------------
===========================================================================*/
