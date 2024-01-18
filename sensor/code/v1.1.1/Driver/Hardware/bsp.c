/*===========================================================================
* ��ַ ��http://www.cdebyte.com/   http://yhmcu.taobao.com/                 *
* ���� ������  ԭ �ں͵��ӹ�����  �� �ڰ��ص��ӿƼ����޹�˾                 *
* �ʼ� ��yihe_liyong@126.com                                                *
* �绰 ��18615799380                                                        *
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
void delay_ms(uint32_t num)//���Ǻܾ�ȷ
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
  GPIO_Init(GPIOD, GPIO_Pin_2, GPIO_Mode_In_FL_No_IT);   //���븡����û���ⲿ�ж�  
  //EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Falling_Low);    
  TIM1_DeInit();     
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
  //tim1 ��ʱ���Ĵ���ȫ����λ  
  //	TIM1_TimeBaseInit(0x0f, TIM1_CounterMode_Up, 50000-1, 0);
  TIM1_TimeBaseInit(2560-1, TIM1_CounterMode_Up, 1562-1, 0);//  250ms  ��С����50.2hz      ����ʱ���Ƶ  ���ϼ���    ��׽50Hz�� ���ϵ����ں�ռ�ձ�
  TIM1_PWMIConfig(TIM1_Channel_1, TIM1_ICPolarity_Rising, TIM1_ICSelection_DirectTI, TIM1_ICPSC_DIV1, 0x00); //ѡ��TI1���������ش��� ѡ��TI2�����½��ش��� ���벶׽Ԥ������  ���˲�
  TIM1_SelectInputTrigger(TIM1_TRGSelection_TI1FP1);   //�˲�������TI1
  TIM1_SelectSlaveMode(TIM1_SlaveMode_Reset);//��λ����
  TIM1_ClearFlag(TIM1_FLAG_CC1);                         //ָ��Ҫ����ı�־
  TIM1_ClearFlag(TIM1_FLAG_CC2);                         //ָ��Ҫ����ı�־
  TIM1_ITConfig( TIM1_IT_CC1, ENABLE);                  //ȥ���ǲ�ѯ��ʽ���������жϷ�ʽ
  TIM1_ITConfig( TIM1_IT_CC2, ENABLE);                  //ȥ���ǲ�ѯ��ʽ���������жϷ�ʽ
  TIM1_CCxCmd(TIM1_Channel_1, ENABLE);                   //ʹ�����벶��ͨ��1
  TIM1_CCxCmd(TIM1_Channel_2, ENABLE);                   //ʹ�����벶��ͨ��2
  TIM1_Cmd(ENABLE);                                     //ʹ�ܶ�ʱ��
  
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
    RTC_DeInit(); //��ʼ��Ĭ��״̬ 
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //����RTCʱ�� 
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1); //ѡ��RTCʱ��ԴLSI 38K��2=19K
    while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET);
    
    //RTC_WakeUpCmd(DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //����RTCʱ��
    RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
    RTC_ITConfig(RTC_IT_WUT, ENABLE); //�����ж�    
    RTC_SetWakeUpCounter(time); //����RTC Weakup��������ֵ	

   // RTC_ClearITPendingBit(RTC_IT_WUT);
    RTC_WakeUpCmd(ENABLE);
    
  }
  else
  {
    RTC_WakeUpCmd(DISABLE);
   // CLK_PeripheralClockConfig(CLK_Peripheral_RTC, DISABLE); //����RTCʱ��
    //RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
    // RTC_ITConfig(RTC_IT_WUT, DISABLE); //�����ж�
    //RTC_ClearITPendingBit(RTC_IT_WUT);
    // RTC_SetWakeUpCounter(time); //����RTC Weakup��������ֵ
    
    
  }
  
}



extern u8 CurrentMode ;
void EnterStopMode(void) 
{
  disableInterrupts(); 
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1,DISABLE);
  //  LCD_Cmd(DISABLE);                                 // ����LCD��������
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
  CLK_PeripheralClockConfig(CLK_Peripheral_LCD,DISABLE);   //���û����ָ������Χʱ��

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
  SystemClock_Init();     // ϵͳʱ�ӳ�ʼ��
  GPIO_Initial(); 
  Uart1_Init(115200);// ��ʼ��GPIO
  Uart3_Init(115200);
  DMA_Config();
  //LED_Init();
  //delay_ms(5000);
  //����LED��ʼ��
  

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
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);//����ADC1ʱ��
  
  ADC_VrefintCmd(ENABLE); //ʹ���ڲ��ο���ѹ
  ADC_SamplingTimeConfig(ADC1,ADC_Group_SlowChannels,ADC_SamplingTime_96Cycles);
  ADC_Init(ADC1,ADC_ConversionMode_Single,ADC_Resolution_12Bit,ADC_Prescaler_1);//����ת����12λ��ת��ʱ��1��Ƶ
  ADC_Cmd(ADC1,ENABLE);//ADCʹ��	
  ADC_ChannelCmd(ADC1,num,ENABLE);//ʹ���ڲ��ο���ѹͨ��
  
  
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
    ADC_SoftwareStartConv(ADC1); //�������ת��
    delay_ms(10);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
    {}
    ADC_ClearFlag(ADC1,ADC_FLAG_EOC);//�����Ӧ��־
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


