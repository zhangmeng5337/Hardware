/*===========================================================================
* ��ַ ��http://www.cdebyte.com/   http://yhmcu.taobao.com/                 *
* ���� ������  ԭ �ں͵��ӹ�����  �� �ڰ��ص��ӿƼ����޹�˾                 *
* �ʼ� ��yihe_liyong@126.com                                                *
* �绰 ��18615799380                                                        *
============================================================================*/
#include "RF.h"
#include "bsp.h"
#include "stm8l15x_iwdg.h"
#include "stm8l15x_iwdg.h"
#include "stm8l15x_rst.h"
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

void GPIO_Initial(void)
{
  
    GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
    GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
    GPIO_Init( GPIOC, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
    GPIO_Init( GPIOD, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  
    /* ����SX127X��ؿ�������NSS(PB4), RST(PB3), TX_CTRL(PA5), RX_CTRL(PA4)*/
    GPIO_Init(PORT_SX127X_CSN, PIN_SX127X_CSN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(PORT_SX127X_CSN, PIN_SX127X_CSN);

    GPIO_Init(PORT_SX127X_RST, PIN_SX127X_RST, GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(PORT_SX127X_RST, PIN_SX127X_RST);

    GPIO_Init(PORT_SX127X_TX_CTRL, PIN_SX127X_TX_CTRL, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_ResetBits(PORT_SX127X_TX_CTRL, PIN_SX127X_TX_CTRL);

    GPIO_Init(PORT_SX127X_RX_CTRL, PIN_SX127X_RX_CTRL, GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(PORT_SX127X_RX_CTRL, PIN_SX127X_RX_CTRL);
    
    /*PB2 PB1 �ֱ����M0 M1*/
    GPIO_Init(PORT_SX127X_M0, PIN_SX127X_M0, GPIO_Mode_In_PU_No_IT);//GPIO_Mode_In_PU_IT
    EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Rising_Falling);
    GPIO_Init(PORT_SX127X_M1, PIN_SX127X_M1, GPIO_Mode_In_PU_No_IT);
    EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Rising_Falling);


    /*PB0 AUX*/
    GPIO_Init(PORT_SX127X_AUX, PIN_SX127X_AUX, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_ResetBits(PORT_SX127X_AUX, PIN_SX127X_AUX);
    /*SX1276���ݽ����ⲿ�ж�*/
    GPIO_Init(PORT_SX127X_BUSY, PIN_SX127X_BUSY, GPIO_Mode_In_FL_IT);                //dio0
    EXTI_SetPinSensitivity(EXTI_Pin_6, EXTI_Trigger_Rising);

    /*SX1276���ݽ����ⲿ�ж�*/
    GPIO_Init(PORT_SX127X_DIO1, PIN_SX127X_DIO1, GPIO_Mode_In_FL_IT);                //dio3
    EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Rising);
    //EXTI_SetHalfPortSelection(EXTI_HalfPort_D_LSB, ENABLE);
}

void SPI_Initial(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);
    SPI_DeInit(SPI1);

    // ����SPI��ز���,2��Ƶ��8MHZ��
    SPI_Init(SPI1, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_2,
                SPI_Mode_Master, SPI_CPOL_Low, SPI_CPHA_1Edge,
                SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft, 7);
    SPI_Cmd(SPI1, ENABLE);

    // SPI���IO������
    GPIO_Init(PORT_SPI, PIN_MISO, GPIO_Mode_In_PU_No_IT);       // MISO (PB7)
    GPIO_Init(PORT_SPI, PIN_SCLK, GPIO_Mode_Out_PP_High_Slow);  // SCLK (PB5)
    GPIO_Init(PORT_SPI, PIN_MOSI, GPIO_Mode_Out_PP_High_Slow);  // MOSI (PB6)
}
/*===========================================================================
* ���� ��TIM3_Initial() => ��ʼ����ʱ��3����ʱʱ��Ϊ1ms                     *
============================================================================*/
void TIM3_Initial(void)
{
    TIM3_DeInit();

    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);

    // ����Timer3��ز������ڲ�ʱ��Ϊ16MHZ����ʱʱ�� 1ms  ��Ҫ�ƴ�125
    TIM3_TimeBaseInit(TIM3_Prescaler_128, TIM3_CounterMode_Up, 16250); //625��ʱ5ms
    TIM3_ITConfig(TIM3_IT_Update, ENABLE);
    TIM3_Cmd(DISABLE);
}

void RTC_Config(uint16_t time) 
{
    RTC_DeInit(); //��ʼ��Ĭ��״̬ 
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //����RTCʱ�� 
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_2); //ѡ��RTCʱ��ԴLSI 38K��2=19K
    while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET);
    
    RTC_WakeUpCmd(DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //����RTCʱ��
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
    RTC_ITConfig(RTC_IT_WUT, ENABLE); //�����ж�
    RTC_SetWakeUpCounter(time); //����RTC Weakup��������ֵ
    RTC_WakeUpCmd(ENABLE);
}
extern u8 CurrentMode ;
void EnterStopMode(void) 
{
    disableInterrupts(); 
    SX1276_LoRa_SetMode( LORA_MODE_SLEEP );//lora enter sleep mode

    //GPIOA
    GPIO_Init( PORT_SX127X_TX_CTRL, PIN_SX127X_RX_CTRL|PIN_SX127X_TX_CTRL, GPIO_Mode_Out_PP_Low_Slow ); //lora tx and rx enbale signals 
    GPIO_Init( GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Slow );
    GPIO_Init( GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Slow );

    //GPIOB
    GPIO_Init(GPIOB, PIN_SX127X_AUX, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOB, PIN_SX127X_M0, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOB, PIN_SX127X_M1, GPIO_Mode_Out_PP_Low_Slow);
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
    
    if(Module_mode.CurrentMode == SleepMode) 
    {
        SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA, DISABLE);
        GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_Mode_In_FL_IT);
        EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Rising);
    }
    enableInterrupts();
    halt();  //enter stop mode
}
void HardwareInit()
{
    disableInterrupts();
    SystemClock_Init();     // ϵͳʱ�ӳ�ʼ��
    GPIO_Initial();         // ��ʼ��GPIO
    SPI_Initial();          //SPI��ʼ��
    enableInterrupts();
    //LED_Init();             //����LED��ʼ��
}
void LED_Init(void)
{
    GPIO_Init(GPIOD, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Fast);
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
* ���� ��SPI_ExchangeByte() => ͨ��SPI�������ݽ���                          *
* ���� ����Ҫд��SPI��ֵ                                                    *
* ��� ��ͨ��SPI������ֵ                                                    *
============================================================================*/
//INT8U SPI_ExchangeByte(INT8U input)
//{
//    SPI_SendData(input);
//	while (RESET == SPI_GetFlagStatus(SPI_FLAG_TXE));   // �ȴ����ݴ������
//	while (RESET == SPI_GetFlagStatus(SPI_FLAG_RXNE));  // �ȴ����ݽ������
//	return (SPI_ReceiveData());
//}

/*===========================================================================
-----------------------------------�ļ�����----------------------------------
===========================================================================*/
