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
#include "math.h"

//#define VersionNumber 1.1.1
unsigned char VersionNumber[5] = {'1', '.', '0', '.', '0'} ;

#define TX              1       // 发送模式
#define RX              0       // 接收模式

//参数配置
//Flash 存储
unsigned char Flash_Data[5] = {0};
unsigned char Flash_Write_Done = 1;
//stm8控制线以及工作状态线
unsigned char LoraM0Flag = 0;
unsigned char LoraM1Flag = 0;
unsigned char AUX = 0;
/*参数1*/
unsigned char ADDH = 0x00;
/*参数2*/
unsigned char ADDL = 0x00;
/*参数3*/
u32 SerialRate = 9600;
unsigned char AirRate = 2;   //空口速度 默认2.4k
/*参数4*/
unsigned char Channel = 0x17; //默认通道为23通道 433MHZ
/*参数5*/
unsigned char TranMode = 0x01;
unsigned int WakeupTime = 250;

unsigned int pream_long[ ] = {7, 30, 61, 244, 488, 976};
//工作模式
#define NormalMode      0x00
#define WakeMode        0x10
#define PowerSavingMode 0x01
#define SleepMode       0x11
u8 CheckBit = 0;
u8 CurrentMode = 0;
u8 LastMode = 0;
u8 ConfigureDone = 0;

#define ACK_LENGTH      10      // 应答信号长度
#define SEND_LENGTH     10      // 发送数据每包的长度
INT8U   SendBuffer[SEND_LENGTH] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
INT8U   SendFlag = 1;           // =1，发送无线数据，=0不处理
INT16U  SendCnt = 0;            // 计数发送的数据包数
INT16U  SendTime = 1;           // 计数数据发送间隔时间
INT16U  RecvWaitTime = 0;       // 接收等待时间
#define RECV_TIMEOUT    800     // 接收超时
//需要应答的数据
INT8U   AckBuffer[ACK_LENGTH] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
INT16U  RecvCnt = 0;            // 计数接收的数据包数

INT16U SingleWait = 0;
unsigned char InitFlag = 0;

#define AUX_CONFIGURING()       GPIO_ResetBits(PORT_SX127X_AUX, PIN_SX127X_AUX)
#define AUX_CONFIGURED()        GPIO_SetBits(PORT_SX127X_AUX, PIN_SX127X_AUX)

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
    u8 i = 0;
    while(num--)
    {
        for (i=0; i<100; i++);
    }
}

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

void RF_Initial( )
{
    SX1276_Init(MODE_LORA);         //SX127X 模块复位并初始化为LoRa模式
    SX1276_LoRa_SetDataRate(AirRate);
    SX1276_SetFreq(Channel);				//配置信道为23，即433Hz
    SX1276_SetPower(15, 1);         //配置RFO PIN输出，功率20dBm
    SX1276_SetRxMode();  
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

int error_temp = 0;
void main(void)
{
    INT8U length = 0, recv_buffer[128]={ 0 };  //接收数据的长度以及数据所需存储的数组
    INT8U tmp_76 = 0, tmp_543 = 0;             //分别代表串口校验位、波特率

    char i = 0;

    disableInterrupts();
    SystemClock_Init();     // 系统时钟初始化
    GPIO_Initial();         // 初始化GPIO
    SPI_Initial();          //SPI初始化
    enableInterrupts();
    LED_Init();             //调试LED初始化
    RF_Initial( );

    FLASH_SetProgrammingTime(FLASH_ProgramTime_TProg);  //Flash 初始化
    
    if(FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 5)) == 1)  //判断Flash中是否有数据
    {
        Flash_Data[0] = FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS);
        Flash_Data[1] = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 1));
        Flash_Data[2] = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 2));
        Flash_Data[3] = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 3));
        Flash_Data[4] = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 4));
        ADDH = Flash_Data[0];
                                    
        ADDL = Flash_Data[1];

        if (Flash_Data[2] >> 6 == 0x01)
            CheckBit = 0x06;
        else if (Flash_Data[2] >> 6 == 0x02)
            CheckBit = 0x04;
        else CheckBit = 0x00;
        if (((Flash_Data[2]>>3) & 0x7) < 6)
            SerialRate = 1200 * (unsigned int)(pow(2, ((Flash_Data[2]>>3) & 0x7)));
        else if (((Flash_Data[2]>>3) & 0x7) == 6)
            SerialRate = 57600;
        else SerialRate = 115200;
        
        AirRate = Flash_Data[2]& 0x7;
                                    
        Channel =  Flash_Data[3]&0x1F;

        TranMode = Flash_Data[4]>>7;
        WakeupTime = 250*(((Flash_Data[4]>>3) & 0x7) + 1);
    }

    while(1)
    {
        /*判断M0 M1 进入不同的工作模式
         *M0 = 0  M1 = 1 一般模式
         *M0 = 1  M1 = 0 唤醒模式
         *M0 = 0  M1 = 1 省电模式
         *M0 = 1  M1 = 1 休眠模式
         */
        LoraM1Flag = GPIO_ReadInputDataBit(PORT_SX127X_M1, PIN_SX127X_M1);
        LoraM0Flag = GPIO_ReadInputDataBit(PORT_SX127X_M0, PIN_SX127X_M0);

        if((LoraM0Flag == 0) && (LoraM1Flag == 0))
            CurrentMode = NormalMode;
        else if((LoraM0Flag != 0) && (LoraM1Flag == 0))
            CurrentMode = WakeMode;
        else if((LoraM0Flag == 0) && (LoraM1Flag != 0))             //非0 是0x02
            CurrentMode = PowerSavingMode;
        else CurrentMode = SleepMode;

        if(LastMode != CurrentMode) {                               //当模式发生变化时，
            AUX_CONFIGURING();
            ConfigureDone = 0;
            RtcWakeUp = 0;
            RTC_DeInit();                                           //初始化默认状态 
            CLK_PeripheralClockConfig(CLK_Peripheral_RTC, DISABLE); //禁止RTC时钟 
            RTC_WakeUpCmd(DISABLE);                                 //禁止RTC唤醒命令
            RTC_ITConfig(RTC_IT_WUT, DISABLE);                      //禁止RTC中断
            LastMode = CurrentMode;
        }

        switch (PowerSavingMode) {
            case NormalMode:
                if(ConfigureDone) {
                    SX1276_SetRxMode();
                    if( UsartReceiveFlag == 1 )
                    {
                        //前导码的设置  短前导码的模块发送数据到设置长前导码模块，可以接收；长前导码的模块发送数据到设置短前导码模块，不可以接收
                        SX1276_SetPreambleSize(8);                              //发送数据时候前导码设置短些
                        SX1276_SetRxMode();
                        AUX_CONFIGURING();
                        SX1276_SendPacket(UsartReceiveData, (usart_i-1));
                        //USART_SendData8(USART1, (usart_i-1));
                        //while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                        for(i = 0; i < (usart_i-1); i++) {
                            USART_SendData8(USART1, UsartReceiveData[i]);
                            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                        }
                        AUX_CONFIGURED();
                        SX1276_SetPreambleSize(65530);                          //数据发送完成后，前导码设置到最长，方便接收唤醒模式的数据
                        usart_i = 0;
                        LED_TOG();
                        UsartReceiveFlag = 0;
                    }
                    AUX_CONFIGURED();
                    if( ExitInterFlag) {
                        ExitInterFlag = 0;
                        length = SX1276_ReceivePacket(recv_buffer);             // 检测是否收到一个数据包
                        if( length )
                        {
                            AUX_CONFIGURING();
                            delay_ms(50);
                            for(i = 0; i < length; i++) {
                                USART_SendData8(USART1, recv_buffer[i]);
                                while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                            }
                            AUX_CONFIGURED();
                            length = 0;
                            LED_TOG();
                        }
                    }
                } else {
                    AUX_CONFIGURING();
                    Uart1_Init(SerialRate, CheckBit);                           //设置串口波特率
                    USART_SendStr("时钟+串口初始化完成\r\n");
                    SX1276_SetPreambleSize(65530);
                    ConfigureDone = 1;
                    AUX_CONFIGURED();
                }
                break;
            case WakeMode:
                if(ConfigureDone) {
                    SX1276_SetRxMode();
                    if( UsartReceiveFlag == 1 )
                    {
                        SX1276_SetPreambleSize((pream_long[AirRate]*(WakeupTime/250)));   //前导码为972 BW 500kHZ SF为7 则空中传输时间250ms
                        SX1276_SetRxMode();
                        SX1276_SendPacket(UsartReceiveData, (usart_i-1));
                        USART_SendData8(USART1, (usart_i-1));
                        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                        for(i = 0; i < (usart_i-1); i++) {
                            USART_SendData8(USART1, UsartReceiveData[i]);
                            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                        }
                        SX1276_SetPreambleSize(65530);                          //数据发送完成后，前导码设置到最长，方便接收唤醒模式的数据
                        usart_i = 0;
                        LED_TOG();
                        UsartReceiveFlag = 0;
                    }
                    if( ExitInterFlag) {
                        ExitInterFlag = 0;
                        length = SX1276_ReceivePacket(recv_buffer);             // 检测是否收到一个数据包
                        if( length )
                        {
                            AUX_CONFIGURING();
                            delay_ms(50);
                            //USART_SendData8(USART1, length);
                            //while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                            for(i = 0; i < length; i++) {
                                USART_SendData8(USART1, recv_buffer[i]);
                                while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                            }
                            AUX_CONFIGURED();
                            length = 0;
                            LED_TOG();
                        }
                    }
                } else {
                    AUX_CONFIGURING();
                    Uart1_Init(SerialRate, CheckBit);                           //设置串口波特率
                    USART_SendStr("时钟+串口初始化完成\r\n");
                    SX1276_SetPreambleSize((pream_long[AirRate]*(WakeupTime/250)));   //前导码为972 BW 500kHZ SF为7 则空中传输时间250ms
                    ConfigureDone = 1;
                    AUX_CONFIGURED();
                }
                break;
            case PowerSavingMode:
                if(ConfigureDone) {
                    if(RtcWakeUp == 1)
                    {
                        RtcWakeUp = 0;
                        SX1276_CADMode();
                        while (SingleWait <= 20000) {
                            SingleWait ++;
                            if(CadDoneFlag == 1) {
                                RTC_WakeUpCmd(DISABLE);
                                RTC_ITConfig(RTC_IT_WUT, DISABLE);
                                SX1276_SetRxMode();
                                if( ExitInterFlag) {
                                    ExitInterFlag = 0;
                                    length = SX1276_ReceivePacket(recv_buffer);           // 检测是否收到一个数据包
                                    if( length )
                                    {
                                        AUX_CONFIGURING();
                                        delay_ms(50);
                                        for(i = 0; i < length; i++) {
                                            USART_SendData8(USART1, recv_buffer[i]);
                                            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                                        }
                                        AUX_CONFIGURED();
                                        length = 0;
                                        SingleWait = 20001;
                                        CadDoneFlag = 0;
                                    }
                                }
                            }
                        }
                        SingleWait = 0;
                        RTC_ITConfig(RTC_IT_WUT, ENABLE);
                        RTC_WakeUpCmd(ENABLE);
                        while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==0);
                        EnterStopMode();

                        SystemClock_Init();     // 系统时钟初始化
                        GPIO_Initial();         // 初始化GPIO
                        Uart1_Init(SerialRate, CheckBit);                           //设置串口波特率
                        SPI_Initial();          //SPI初始化
                    }
                } else {
                    AUX_CONFIGURING();
                    Uart1_Init(SerialRate, CheckBit); //设置串口波特率
                    USART_SendStr("时钟+串口初始化完成\r\n");
                    /*SX1276数据接收外部中断
                      DIO3作为CADDone的出发中断
                      用RTC进行定时
                    */
                    SX1276_SetPreambleSize(65530);
                    RTC_Config(WakeupTime);
                    enableInterrupts();
                    ConfigureDone = 1;
                    AUX_CONFIGURED();
                }
                break;
            case SleepMode:
                if(ConfigureDone) {
                    if ((SleepModeFlag == 1) && (InitFlag == 0))
                    {
                        AUX_CONFIGURING();
                        SystemClock_Init();     // 系统时钟初始化
                        GPIO_Initial();         // 初始化GPIO
                        SPI_Initial();          //SPI初始化
                        LED_Init();             //调试LED初始化
                        RF_Initial( );
                        Uart1_Init(9600, 0);
                        InitFlag = 1;
                        AUX_CONFIGURED();
                    } else if(InitFlag == 1) {
                        if(UsartReceiveFlag == 1) 
                        {
                            switch (UsartReceiveData[0]) {
                                case 0xC0:
                                    FLASH_Unlock(FLASH_MemType_Data);

                                    FLASH_EraseByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS);
                                    FLASH_ProgramByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS, UsartReceiveData[1]);
                                    FLASH_WaitForLastOperation(FLASH_MemType_Data);
                                    ADDH = UsartReceiveData[1];

                                    FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 1));
                                    FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 1), UsartReceiveData[2]);
                                    FLASH_WaitForLastOperation(FLASH_MemType_Data);
                                    ADDL = UsartReceiveData[2];

                                    FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 2));
                                    FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 2), UsartReceiveData[3]);
                                    FLASH_WaitForLastOperation(FLASH_MemType_Data);
                                    if (UsartReceiveData[3] >> 6 == 0x01)
                                        CheckBit = 0x06;
                                    else if (UsartReceiveData[3] >> 6 == 0x02)
                                        CheckBit = 0x04;
                                    else CheckBit = 0x00;
                                    if (((UsartReceiveData[3]>>3) & 0x7) < 6)
                                        SerialRate = 1200 * (unsigned int)(pow(2, ((UsartReceiveData[3]>>3) & 0x7)));
                                    else if (((UsartReceiveData[3]>>3) & 0x7) == 6)
                                        SerialRate = 57600;
                                    else SerialRate = 115200;
                                    
                                    AirRate = UsartReceiveData[3]& 0x7;
                                    
                                    FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+3));
                                    FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 3), UsartReceiveData[4]);
                                    FLASH_WaitForLastOperation(FLASH_MemType_Data);
                                    Channel =  UsartReceiveData[4]&0x1F;

                                    FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+4));
                                    FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 4), UsartReceiveData[5]);
                                    FLASH_WaitForLastOperation(FLASH_MemType_Data);
                                    TranMode = UsartReceiveData[5]>>7;
                                    WakeupTime = 250*(((UsartReceiveData[5]>>3) & 0x7) + 1);

                                    Flash_Write_Done = 1;
                                    FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+5));
                                    FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 5), Flash_Write_Done);
                                    FLASH_WaitForLastOperation(FLASH_MemType_Data);
                                    FLASH_Lock(FLASH_MemType_Data);

                                    LED_TOG();
                                break;
                                case 0xC1: //
                                    USART_SendData8(USART1, ADDH);
                                    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                                    USART_SendData8(USART1, ADDL);
                                    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);

                                    if(CheckBit == 0x00)
                                        tmp_76 = 0x00;
                                    else if (CheckBit == 0x06)
                                        tmp_76 = 0x01;
                                    else tmp_76 = 0x02;
                                    tmp_543 = (unsigned char) (ceil(log2(SerialRate/1200)));
                                    USART_SendData8(USART1, (tmp_76 << 6 | tmp_543 << 3 | AirRate)); 
                                    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);

                                    USART_SendData8(USART1, Channel);
                                    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                                    
                                    USART_SendData8(USART1, ((TranMode<<7) | ((WakeupTime/250-1)<<3)));
                                    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                                    LED_TOG();
                                    break;
                                case 0xC2:
                                    ADDH = UsartReceiveData[1];
                                    
                                    ADDL = UsartReceiveData[2];

                                    if (UsartReceiveData[3] >> 6 == 0x01)
                                        CheckBit = 0x06;
                                    else if (UsartReceiveData[3] >> 6 == 0x02)
                                        CheckBit = 0x04;
                                    else CheckBit = 0x00;
                                    if (((UsartReceiveData[3]>>3) & 0x7) < 6)
                                        SerialRate = 1200 * (unsigned int) (pow(2, ((UsartReceiveData[3]>>3) & 0x7)));
                                    else if (((UsartReceiveData[3]>>3) & 0x7) == 6)
                                        SerialRate = 57600;
                                    else SerialRate = 115200;
                                    
                                    AirRate = UsartReceiveData[3]& 0x7;
                                    
                                    Channel =  UsartReceiveData[4]&0x1F;        

                                    TranMode = UsartReceiveData[5]>>7;
                                    WakeupTime = 250*(((UsartReceiveData[5]>>3) & 0x7) + 1);
                                    //传输模式没配 IO驱动方式没配 FEC开关没配 发射功率没配
                                    LED_TOG();
                                    break;
                                case 0xC3:
                                    if((UsartReceiveData[1] == 0xC3) && (UsartReceiveData[2] == 0xC3))
                                    {
                                        for(i = 0; i < 5; i++) {
                                            USART_SendData8(USART1, VersionNumber[i]);
                                            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                                        }
                                    }
                                    break; 
                                case 0xC4:
                                    if((UsartReceiveData[1] == 0xC4) && (UsartReceiveData[2] == 0xC4))
                                    {
                                        WWDG->CR = 0x80;
                                    }
                                    break;
                            }
                            UsartReceiveFlag = 0;
                        }
                    }
                } else {
                    ConfigureDone = 1;
                    SleepModeFlag = 0;  //串口接受数据管脚有数据时，产生外部中断，对休眠进行唤醒，将SleepModeFlag置1
                    InitFlag = 0;
                    EnterStopMode();
                }
                break;
            default:
                break;
        }
    }
}
