/*===========================================================================
* 网址 ：http://www.cdebyte.com/   http://yhmcu.taobao.com/                 *
* 作者 ：李勇  原 亿和电子工作室  现 亿佰特电子科技有限公司                 *
* 邮件 ：yihe_liyong@126.com                                                *
* 电话 ：18615799380                                                        *
============================================================================*/

#include "bsp.h"
void GPIO_Initial(void)
{
  
    GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
    GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
    GPIO_Init( GPIOC, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
    GPIO_Init( GPIOD, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  
    /* 配置SX127X相关控制引脚NSS(PB4), RST(PB3), TX_CTRL(PA5), RX_CTRL(PA4)*/
    GPIO_Init(PORT_SX127X_CSN, PIN_SX127X_CSN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(PORT_SX127X_CSN, PIN_SX127X_CSN);

    GPIO_Init(PORT_SX127X_RST, PIN_SX127X_RST, GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(PORT_SX127X_RST, PIN_SX127X_RST);

    GPIO_Init(PORT_SX127X_TX_CTRL, PIN_SX127X_TX_CTRL, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_ResetBits(PORT_SX127X_TX_CTRL, PIN_SX127X_TX_CTRL);

    GPIO_Init(PORT_SX127X_RX_CTRL, PIN_SX127X_RX_CTRL, GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(PORT_SX127X_RX_CTRL, PIN_SX127X_RX_CTRL);
    
    /*PB2 PB1 分别代表M0 M1*/
    GPIO_Init(PORT_SX127X_M0, PIN_SX127X_M0, GPIO_Mode_In_PU_No_IT);//GPIO_Mode_In_PU_IT
    EXTI_SetPinSensitivity(EXTI_Pin_2, EXTI_Trigger_Rising_Falling);
    GPIO_Init(PORT_SX127X_M1, PIN_SX127X_M1, GPIO_Mode_In_PU_No_IT);
    EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Rising_Falling);

    /*PB0 AUX*/
    GPIO_Init(PORT_SX127X_AUX, PIN_SX127X_AUX, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_ResetBits(PORT_SX127X_AUX, PIN_SX127X_AUX);
    /*SX1276数据接收外部中断*/
    GPIO_Init(GPIOD, GPIO_Pin_1, GPIO_Mode_In_FL_IT);                //dio0
    EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Rising);

    /*SX1276数据接收外部中断*/
    GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_In_FL_IT);                //dio3
    EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Rising);
    //EXTI_SetHalfPortSelection(EXTI_HalfPort_D_LSB, ENABLE);
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
    
    if(CurrentMode == SleepMode) 
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
    SystemClock_Init();     // 系统时钟初始化
    GPIO_Initial();         // 初始化GPIO
    SPI_Initial();          //SPI初始化
    enableInterrupts();
    LED_Init();             //调试LED初始化
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
* 函数 ：SPI_ExchangeByte() => 通过SPI进行数据交换                          *
* 输入 ：需要写入SPI的值                                                    *
* 输出 ：通过SPI读出的值                                                    *
============================================================================*/
INT8U SPI_ExchangeByte(INT8U input)
{
    SPI_SendData(input);
	while (RESET == SPI_GetFlagStatus(SPI_FLAG_TXE));   // 等待数据传输完成
	while (RESET == SPI_GetFlagStatus(SPI_FLAG_RXNE));  // 等待数据接收完成
	return (SPI_ReceiveData());
}

/*===========================================================================
-----------------------------------文件结束----------------------------------
===========================================================================*/
