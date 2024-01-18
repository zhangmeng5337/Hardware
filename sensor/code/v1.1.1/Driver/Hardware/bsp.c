/*===========================================================================
* 网址 ：http://www.cdebyte.com/   http://yhmcu.taobao.com/                 *
* 作者 ：李勇  原 亿和电子工作室  现 亿佰特电子科技有限公司                 *
* 邮件 ：yihe_liyong@126.com                                                *
* 电话 ：18615799380                                                        *
============================================================================*/
#include "bsp.h"
#include "uart1.h"
#include "uart3.h"

#include "stm8l15x_dma.h"
#include "stm8l15x_tim1.h"
#include "gprs_app.h"

#define USART_DMA_CHANNEL_RX   DMA1_Channel2
#define USART_DR_ADDRESS       (uint16_t)0x5231  /* USART1 Data register Address */

float ADC_RATIO= ((uint16_t) 733); /*ADC_RATIO = ( 3 * 1000 * 1000)/4095 */
extern unsigned int rtctime;//10:9s 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ADCdata = 0;
void delay_ms(uint32_t num)//不是很精确
{
  u16 i = 0;
  while(num--)
  {
    for (i=0; i<2654; i++);
  }
}

void GPIO_Initial(void)
{
  /*#if module == smartbox
  //GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOC, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOD, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  
  GPIO_Init( PORT_LED, PIN_LED, GPIO_Mode_Out_PP_High_Fast );
  GPIO_Init( PORT_POWER_ON, PIN_POWER_ON, GPIO_Mode_Out_PP_High_Fast );     
  GPIO_Init( PORT_PWRKEY_IN, PIN_PWRKEY_IN, GPIO_Mode_Out_PP_Low_Slow );   
  GPIO_Init( PORT_SENSOR_EN, PIN_SENSOR_EN, GPIO_Mode_Out_PP_Low_Fast ); 
  GPIO_Init( PORT_GNSS_PORT, PIN_GNSS, GPIO_Mode_Out_PP_Low_Fast ); 
#elif module == sensor||module == DEGUG_SENSOR*/
  //GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  // GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  //GPIO_Init( GPIOC, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  //GPIO_Init( GPIOD, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOC, GPIO_Pin_4, GPIO_Mode_In_PU_No_IT );
//GPIO_Init( GPIOC, GPIO_Pin_7, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( PORT_FLOW, PIN_FLOW, GPIO_Mode_Out_PP_Low_Fast );
  
  GPIO_Init( PORT_LED, PIN_LED, GPIO_Mode_Out_PP_High_Fast );
  
  GPIO_Init( PORT_SENSOR_EN, PIN_SENSOR_EN, GPIO_Mode_Out_PP_Low_Fast ); 
  GPIO_Init(PORT_KEY,PIN_KEY,GPIO_Mode_In_FL_IT);
  EXTI_SetPinSensitivity(EXTI_Pin_6, EXTI_Trigger_Rising_Falling);
  
  //#endif
  
  
  
}
void Init_Timer1(void)
{
  GPIO_Init(GPIOD, GPIO_Pin_2, GPIO_Mode_In_FL_No_IT);   //输入浮动，没有外部中断  
  //EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Falling_Low);    
  TIM1_DeInit();     
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
  //tim1 定时器寄存器全部复位  
  //	TIM1_TimeBaseInit(0x0f, TIM1_CounterMode_Up, 50000-1, 0);
  TIM1_TimeBaseInit(2560-1, TIM1_CounterMode_Up, 1562-1, 0);//  250ms  最小捕获50.2hz      设置时间分频  向上计数    捕捉50Hz及 以上的周期和占空比
  TIM1_PWMIConfig(TIM1_Channel_1, TIM1_ICPolarity_Rising, TIM1_ICSelection_DirectTI, TIM1_ICPSC_DIV1, 0x00); //选择TI1输入上升沿触发 选择TI2输入下降沿触发 输入捕捉预定标器  无滤波
  TIM1_SelectInputTrigger(TIM1_TRGSelection_TI1FP1);   //滤波后输入TI1
  TIM1_SelectSlaveMode(TIM1_SlaveMode_Reset);//复位输入
  TIM1_ClearFlag(TIM1_FLAG_CC1);                         //指定要清除的标志
  TIM1_ClearFlag(TIM1_FLAG_CC2);                         //指定要清除的标志
  TIM1_ITConfig( TIM1_IT_CC1, ENABLE);                  //去掉是查询方式，否则是中断方式
  TIM1_ITConfig( TIM1_IT_CC2, ENABLE);                  //去掉是查询方式，否则是中断方式
  TIM1_CCxCmd(TIM1_Channel_1, ENABLE);                   //使能输入捕获通道1
  TIM1_CCxCmd(TIM1_Channel_2, ENABLE);                   //使能输入捕获通道2
  TIM1_Cmd(ENABLE);                                     //使能定时器
  
}


void GSM_HardwareInit(unsigned char flag)
{
  GPIO_Init( PORT_GNSS_PORT, PIN_GNSS, GPIO_Mode_Out_PP_Low_Fast );
  GPIO_Init( PORT_POWER_ON, PIN_POWER_ON, GPIO_Mode_Out_PP_High_Fast ); 
  //GPIO_Init( GPIOC, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast ); 
  GPIO_Init( PORT_PWRKEY_IN, PIN_PWRKEY_IN, GPIO_Mode_Out_PP_Low_Slow );	
  
  
  if(flag == ON)
  {
    // GPIO_ResetBits( GPIOC, GPIO_Pin_2 );
    //GPIO_SetBits( PORT_POWER_ON, PIN_POWER_ON ); 
    //  GPIO_ResetBits( PORT_POWER_ON, PIN_POWER_ON ); 
    //GPIO_SetBits( PORT_PWRKEY_IN, PIN_PWRKEY_IN );
    //GPIO_ResetBits( PORT_PWRKEY_IN, PIN_PWRKEY_IN );
    //GPIO_SetBits( PORT_PWRKEY_IN, PIN_PWRKEY_IN );
    GPIO_ResetBits( PORT_PWRKEY_IN, PIN_PWRKEY_IN );
    
    GPIO_SetBits( PORT_PWRKEY_IN, PIN_PWRKEY_IN );
    delay_ms(1000);
    GPIO_ResetBits( PORT_PWRKEY_IN, PIN_PWRKEY_IN );		
    //GPIO_SetBits( PORT_PWRKEY_IN, PIN_PWRKEY_IN );                
    delay_ms(2000);
    GPIO_WriteBit(PORT_GNSS_PORT, PIN_GNSS, SET);
    // GPIO_SetBits(GPIOD, GPIO_Pin_2);
    
    //delay_ms(3000);
    //delay_ms(4000);
    //delay_ms(4000);
    //delay_ms(4000);
    //delay_ms(2000);
    //delay_ms(2000);
    //delay_ms(2000);
    //delay_ms(2000);
  }
  else
  {
    GPIO_ResetBits( PORT_PWRKEY_IN, PIN_PWRKEY_IN );
    
    GPIO_SetBits( PORT_PWRKEY_IN, PIN_PWRKEY_IN );
    delay_ms(1000);
    GPIO_ResetBits( PORT_PWRKEY_IN, PIN_PWRKEY_IN );
    GPIO_WriteBit(PORT_GNSS_PORT, PIN_GNSS, RESET); 
  }
  
}
void Sensor_HardwareInit(unsigned char flag)
{
  
  
  if(flag == ON)
  {
    GPIO_ResetBits( PORT_SENSOR_EN, PIN_SENSOR_EN );
    
    
  }
  else
  {
    
    GPIO_SetBits( PORT_SENSOR_EN, PIN_SENSOR_EN );
    
  }
  
}
void FLOW_Ctrl(unsigned char flag)
{
  
  
  if(flag == ON)
  {
    GPIO_ResetBits( PORT_FLOW, PIN_FLOW );
    delay_ms(50);
    GPIO_SetBits( PORT_FLOW, PIN_FLOW );  
    delay_ms(50);
    // GPIO_ResetBits( PORT_FLOW, PIN_FLOW );
    
    
    //  GPIO_ResetBits( PORT_FLOW, PIN_FLOW );    
  }
  else
  {
    
    GPIO_SetBits( PORT_FLOW, PIN_FLOW );
    delay_ms(50);
    GPIO_ResetBits( PORT_FLOW, PIN_FLOW );	
    delay_ms(50);
    //  GPIO_SetBits( PORT_FLOW, PIN_FLOW );
    
    
    
    
  }
  
}

void RTC_Config(uint16_t time,unsigned char flag) 
{
  if(flag == ON)
  {
    RTC_DeInit(); //初始化默认状态 
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //允许RTC时钟 
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1); //选择RTC时钟源LSI 38K、2=19K
    while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET);
    
    //RTC_WakeUpCmd(DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //允许RTC时钟
    RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
    RTC_ITConfig(RTC_IT_WUT, ENABLE); //开启中断    
    RTC_SetWakeUpCounter(time); //设置RTC Weakup计算器初值	

   // RTC_ClearITPendingBit(RTC_IT_WUT);
    RTC_WakeUpCmd(ENABLE);
    
  }
  else
  {
    RTC_WakeUpCmd(DISABLE);
   // CLK_PeripheralClockConfig(CLK_Peripheral_RTC, DISABLE); //允许RTC时钟
    //RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
    // RTC_ITConfig(RTC_IT_WUT, DISABLE); //开启中断
    //RTC_ClearITPendingBit(RTC_IT_WUT);
    // RTC_SetWakeUpCounter(time); //设置RTC Weakup计算器初值
    
    
  }
  
}



extern u8 CurrentMode ;
void EnterStopMode(void) 
{
  disableInterrupts(); 
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1,DISABLE);
  //  LCD_Cmd(DISABLE);                                 // 启用LCD控制器。
  GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
 /********************pa******************************************/
      
  
  GPIO_Init( GPIOA,  GPIO_Pin_2, GPIO_Mode_In_FL_No_IT ); 
  GPIO_Init( GPIOA,  GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Fast );

  
  //GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOC, GPIO_Pin_5|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4, GPIO_Mode_In_PU_No_IT );
  
  GPIO_Init( PORT_LED, PIN_LED, GPIO_Mode_Out_PP_High_Fast );
  GPIO_Init( PORT_KEY, PIN_KEY, GPIO_Mode_In_FL_IT ); 
  EXTI_SetPinSensitivity(EXTI_Pin_6, EXTI_Trigger_Rising_Falling);
  //GPIO_Init( PORT_POWER_ON, PIN_POWER_ON, GPIO_Mode_Out_PP_High_Fast );     
 // GPIO_Init( PORT_PWRKEY_IN, PIN_PWRKEY_IN, GPIO_Mode_Out_PP_Low_Fast ); 
  //GPIO_Init( PORT_PWRKEY_IN, PIN_PWRKEY_IN, GPIO_Mode_In_FL_No_IT ); 
  GPIO_Init( PORT_PWRKEY_IN, PIN_PWRKEY_IN, GPIO_Mode_Out_OD_Low_Slow );   
  GPIO_Init( PORT_SENSOR_EN, PIN_SENSOR_EN, GPIO_Mode_Out_PP_High_Fast ); 
  
  GPIO_Init(PORT_KEY,PIN_KEY,GPIO_Mode_In_FL_IT);
  GPIO_ResetBits( PORT_PWRKEY_IN, PIN_PWRKEY_IN );
  
  GPIO_SetBits( PORT_SENSOR_EN, PIN_SENSOR_EN );
  GPIO_WriteBit(PORT_GNSS_PORT, PIN_GNSS, RESET); 

  
  /* Deinitialize DMA channels */
  DMA_GlobalDeInit();
  
  DMA_DeInit(DMA1_Channel1);
  DMA_DeInit(DMA1_Channel2);
  /* Enable the USART Tx/Rx DMA requests */
  USART_DMACmd(USART1, USART_DMAReq_RX, DISABLE);
  /* Global DMA Enable */
  DMA_GlobalCmd(DISABLE);
  DMA_Cmd(USART_DMA_CHANNEL_RX, DISABLE);  
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, DISABLE);
  TIM1_DeInit();  
  TIM2_DeInit();
  TIM3_DeInit();
  TIM4_DeInit();
  ADC_DeInit(ADC1);
    while((TIM1->CR1 & TIM_CR1_CEN) != 0x00); 
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1,DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,DISABLE);
  while((TIM2->CR1 & TIM_CR1_CEN) != 0x00); 
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,DISABLE);
  
  TIM3_Cmd(DISABLE); 
  while((TIM3->CR1 & TIM_CR1_CEN) != 0x00); 
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
  
  TIM4_Cmd(DISABLE); 
  while((TIM4->CR1 & TIM4_CR1_CEN) != 0x00);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4,DISABLE);
 // LCD_DeInit();

LCD_Cmd(DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_LCD,DISABLE);   //启用或禁用指定的外围时钟

  CLK_PeripheralClockConfig(CLK_Peripheral_I2C1,DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI1,DISABLE); 
  PWR_UltraLowPowerCmd(ENABLE); //low power enable
  PWR_FastWakeUpCmd(ENABLE);  //wake up enable
  
  
}

extern _uart uart1;
static void DMA_Config(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
  
  /* Deinitialize DMA channels */
  DMA_GlobalDeInit();
  
  DMA_DeInit(DMA1_Channel1);
  DMA_DeInit(DMA1_Channel2);
  
  /* DMA channel Rx of USART Configuration */
  DMA_Init(USART_DMA_CHANNEL_RX, (uint16_t)uart1.Uart_Buffer , (uint16_t)USART_DR_ADDRESS,
           BUFFERSIZE, DMA_DIR_PeripheralToMemory, DMA_Mode_Normal,
           DMA_MemoryIncMode_Inc, DMA_Priority_Low, DMA_MemoryDataSize_Byte);
  
  
  /* Enable the USART Tx/Rx DMA requests */
  USART_DMACmd(USART1, USART_DMAReq_RX, ENABLE);
  /* Global DMA Enable */
  DMA_GlobalCmd(ENABLE);
  
  /* Enable the USART Tx DMA channel */
  // DMA_Cmd(USART_DMA_CHANNEL_TX, ENABLE);
  /* Enable the USART Rx DMA channel */
  //DMA_Cmd(USART_DMA_CHANNEL_RX, ENABLE);         
}
void DMA_START_RX(void)
{   DMA_ClearFlag(DMA1_FLAG_TC2);
DMA_ClearFlag(DMA1_FLAG_HT2);
DMA_Cmd(USART_DMA_CHANNEL_RX, DISABLE);
DMA_SetCurrDataCounter(USART_DMA_CHANNEL_RX, BUFFERSIZE); 
DMA_Cmd(USART_DMA_CHANNEL_RX, ENABLE);
}


void HardwareInit()
{
  disableInterrupts();
  SystemClock_Init();     // 系统时钟初始化
  GPIO_Initial(); 
  Uart1_Init(115200);// 初始化GPIO
  Uart3_Init(115200);
  DMA_Config();
  //LED_Init();
  //delay_ms(5000);
  //调试LED初始化
  

  Sensor_HardwareInit(ON);

  enableInterrupts();
}
void GSMInit()
{
  
  GSM_HardwareInit(ON);
  set_NetStatus(SIMCOM_POWER_ON); 
  
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
void USART_SenByte(unsigned char *Str,unsigned char len) 
{
  while(len>0)
  {
    USART_SendData8(USART1, *Str);
    while(!USART_GetFlagStatus (USART1, USART_FLAG_TXE));
    Str++;
    len--;
  }
}

void adcInit(ADC_Channel_TypeDef num)
{
  /* Enable ADC1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);//开启ADC1时钟
  
  ADC_VrefintCmd(ENABLE); //使能内部参考电压
  ADC_SamplingTimeConfig(ADC1,ADC_Group_SlowChannels,ADC_SamplingTime_96Cycles);
  ADC_Init(ADC1,ADC_ConversionMode_Single,ADC_Resolution_12Bit,ADC_Prescaler_1);//连续转换，12位，转换时钟1分频
  ADC_Cmd(ADC1,ENABLE);//ADC使能	
  ADC_ChannelCmd(ADC1,num,ENABLE);//使能内部参考电压通道
  
  
}
uint32_t tmp;
float tmp2,tmp3;
uint32_t adcGet(ADC_Channel_TypeDef num,unsigned int samplecount)
{
  unsigned int i;
  /* Waiting until press Joystick Up */
  /* Wait until End-Of-Convertion */
  tmp = 0;
  tmp3 = 0;

  ADC_RATIO= 3;//(1.225 * 4096)*samplecount/tmp3;
  
  
  ADC_DeInit(ADC1);
  tmp = 0;
  tmp2 =0;
  adcInit(num);  
  
  for(i=0;i<samplecount;i++)
  { 
    ADC_SoftwareStartConv(ADC1); //开启软件转换
    delay_ms(10);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
    {}
    ADC_ClearFlag(ADC1,ADC_FLAG_EOC);//清除对应标志
    /* Get conversion value */
    tmp = ADC_GetConversionValue(ADC1);
    
    /* Calculate voltage value in uV over capacitor  C67 for IDD measurement*/
    tmp2 = tmp2 + tmp;
    
  }
   tmp2 = tmp2 *ADC_RATIO*244.14;
  ADCdata = tmp2*1.0 /samplecount;
  ADC_DeInit(ADC1);
  
  /* Disable ADC1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
  return ADCdata;
  
  
}


