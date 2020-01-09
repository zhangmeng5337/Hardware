/*===========================================================================
* 网址 ：http://www.cdebyte.com/   http://yhmcu.taobao.com/                 *
* 作者 ：李勇  原 亿和电子工作室  现 亿佰特电子科技有限公司                 *
* 邮件 ：yihe_liyong@126.com                                                *
* 电话 ：18615799380                                                        *
============================================================================*/
#include "bsp.h"
#include "uart1.h"
#include "stm8l15x_dma.h"
#include "stm8l15x_tim1.h"
#include "uart_hal.h"
#define USART2_DMA_CHANNEL_RX   DMA1_Channel3
#define USART_DMA_CHANNEL_RX   DMA1_Channel2
#define USART_DR_ADDRESS       (uint16_t)0x5231  /* USART1 Data register Address */

uart_stru uart;
float ADC_RATIO= ((uint16_t) 733); /*ADC_RATIO = ( 3 * 1000 * 1000)/4095 */
extern unsigned int rtctime;//10:9s 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ADCdata = 0;
unsigned char delay_ms(uint32_t num)//不是很精确
{
  u16 i = 0;
  while(num--)
  {
    for (i=0; i<2654; i++);
  }
  return 0;
}


void TIM3_Initial(void)

{
  
  TIM3_DeInit();
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
  
  // 配置Timer3相关参数，内部时钟为16MHZ，定时时间 1ms  需要计次125
  TIM3_TimeBaseInit(TIM3_Prescaler_128, TIM3_CounterMode_Up, 50000); //625定时5ms
  TIM3_SetCounter(0);
  //  TIM3->EGR = TIM3_PSCReloadMode_Immediate;
  TIM3_ITConfig(TIM3_IT_Update, ENABLE);
  TIM3_Cmd(DISABLE);
}

void GPIO_Initial(void)
{ 
  //GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOC, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOD, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT ); 
  
  // battery quantity ios
  GPIO_Init( BATEERY_QUANTITY_PORT, BATEERY_QUANTITY_LEVEL1_PIN|
            BATEERY_QUANTITY_LEVEL2_PIN|BATEERY_QUANTITY_LEVEL3_PIN,
            GPIO_Mode_Out_PP_High_Slow );  
  // module status io
  GPIO_Init( MODULE_STATUS_PORT, MODULE_STATUS_PIN, GPIO_Mode_Out_PP_High_Slow );
  //elec lock ios
  GPIO_Init( LOCK_CTRL_PORT, LOCK_CTRL_PIN, GPIO_Mode_Out_PP_High_Slow );  
  GPIO_Init( LOCK_FB_PORT, LOCK_FB_PIN, GPIO_Mode_In_PU_No_IT );   
  //gsm power and gps enbale ios
  GPIO_Init( GNSS_ENABLE_PORT, GNSS_ENABLE_PIN, GPIO_Mode_Out_PP_Low_Slow ); 
  GPIO_Init( GSM_PWR_PORT, GSM_PWR_PIN, GPIO_Mode_Out_PP_Low_Slow ); 
  
}
/*******************************************************************
function: ctronl  gnss enable or disable
@param    newstate:
this param can be one of the following values:
ON:           enable gnss
OFF:          disable gnss
********************************************************************/
void gnss_state(unsigned char newstate)
{
  if(newstate == ON)
  {
    //GPIO_WriteBit(GNSS_ENABLE_PORT, GNSS_ENABLE_PIN, RESET); //delay_ms(1000);//
    GPIO_WriteBit(GNSS_ENABLE_PORT, GNSS_ENABLE_PIN, SET);delay_ms(1000);//
    // GPIO_WriteBit(GNSS_ENABLE_PORT, GNSS_ENABLE_PIN, RESET);
  }
  else
  {
    GPIO_WriteBit(GNSS_ENABLE_PORT, GNSS_ENABLE_PIN, RESET);  
  }
}
/*******************************************************************
function: ctronl  gsm module power on or off
@param    newstate:
this param can be one of the following values:
ON:           module power on
OFF:          module power off
********************************************************************/
void gsm_power_state(unsigned char newstate)
{
  if(newstate == ON)
  {
    GPIO_WriteBit(GSM_PWR_PORT, GSM_PWR_PIN, RESET);
    delay_ms(10);//
    GPIO_WriteBit(GSM_PWR_PORT, GSM_PWR_PIN, SET);
    delay_ms(900);//
    GPIO_WriteBit(GSM_PWR_PORT, GSM_PWR_PIN, RESET);
    delay_ms(10);
    
    
    
  }
  else
  {
    GPIO_WriteBit(GSM_PWR_PORT, GSM_PWR_PIN, RESET);
    delay_ms(2000);
    GPIO_WriteBit(GSM_PWR_PORT, GSM_PWR_PIN, SET);
    delay_ms(1000);
    GPIO_WriteBit(GSM_PWR_PORT, GSM_PWR_PIN, RESET);
  }
}
void lock_state(unsigned char newstate)
{
  unsigned char i;
  for(i=0;i<3;i++)
  {
    if(newstate == ON)
    {
      GPIO_WriteBit(LOCK_CTRL_PORT, LOCK_CTRL_PIN, SET);
      
      while(GPIO_ReadInputDataBit(LOCK_FB_PORT, LOCK_FB_PIN)==ON)
      {
        
        if(delay_ms(700)==0)
        {
          GPIO_WriteBit(LOCK_CTRL_PORT, LOCK_CTRL_PIN, RESET);  
          break;
        }
      }
      GPIO_WriteBit(LOCK_CTRL_PORT, LOCK_CTRL_PIN, RESET);  
      break;
    }
    else
    {
      GPIO_WriteBit(LOCK_CTRL_PORT, LOCK_CTRL_PIN, RESET); 
      while(GPIO_ReadInputDataBit(LOCK_FB_PORT, LOCK_FB_PIN)==OFF)
      {
        
        if(delay_ms(500)==0)
        {
          GPIO_WriteBit(LOCK_CTRL_PORT, LOCK_CTRL_PIN, RESET);  
          break;
        }
      }
      break;
    }  
  }
  
}
unsigned char get_lock_status()
{
  return GPIO_ReadInputDataBit(LOCK_FB_PORT, LOCK_FB_PIN);
}

void RTC_Config(uint16_t time,unsigned char flag) 
{
  if(flag == ON)
  {
    RTC_DeInit(); //初始化默认状态 
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //允许RTC时钟 
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1); //选择RTC时钟源LSI 38K、2=19K
    while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET);
    
    RTC_WakeUpCmd(DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //允许RTC时钟
    RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
    
    RTC_SetWakeUpCounter(time); //设置RTC Weakup计算器初值	
    RTC_ITConfig(RTC_IT_WUT, ENABLE); //开启中断
    RTC_ClearITPendingBit(RTC_IT_WUT);
    RTC_WakeUpCmd(ENABLE);
    
  }
  else
  {
    RTC_WakeUpCmd(DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, DISABLE); //允许RTC时钟
    //RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
    RTC_ITConfig(RTC_IT_WUT, DISABLE); //开启中断
    RTC_ClearITPendingBit(RTC_IT_WUT);
    // RTC_SetWakeUpCounter(time); //设置RTC Weakup计算器初值
    
    
  }
  
}



extern u8 CurrentMode ;
void EnterStopMode(void) 
{
  disableInterrupts(); 
  GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOA,  GPIO_Pin_2, GPIO_Mode_In_FL_No_IT ); 
  GPIO_Init( GPIOA,  GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Fast );
  GPIO_Init( GPIOA,  GPIO_Pin_4, GPIO_Mode_Out_OD_Low_Fast );   
  GPIO_Init( GPIOA,  GPIO_Pin_5, GPIO_Mode_In_PU_No_IT ); 
  GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOC, GPIO_Pin_5|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4, GPIO_Mode_In_PU_No_IT );
  
  
}

extern uart_stru uart2;
static void DMA_Config(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
  
  /* Deinitialize DMA channels */
  DMA_GlobalDeInit();
  
  DMA_DeInit(DMA1_Channel1);
  DMA_DeInit(DMA1_Channel2);
  
  /* DMA channel Rx of USART Configuration */
  DMA_Init(USART_DMA_CHANNEL_RX, (uint16_t)uart.rxbuffer , (uint16_t)USART_DR_ADDRESS,
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
{   
  DMA_ClearFlag(DMA1_FLAG_TC2);
  DMA_ClearFlag(DMA1_FLAG_HT2);
  DMA_Cmd(USART_DMA_CHANNEL_RX, DISABLE);
  DMA_SetCurrDataCounter(USART_DMA_CHANNEL_RX, BUFFERSIZE); 
  DMA_Cmd(USART_DMA_CHANNEL_RX, ENABLE);
  
  
  
}
void DMA_START_RX2(void)
{   
  DMA_ClearFlag(DMA1_FLAG_TC3);
  DMA_ClearFlag(DMA1_FLAG_HT3);
  DMA_Cmd(USART2_DMA_CHANNEL_RX, DISABLE);
  DMA_SetCurrDataCounter(USART2_DMA_CHANNEL_RX, BUFFERSIZE); 
  DMA_Cmd(USART2_DMA_CHANNEL_RX, ENABLE);
  
  
  
}




/*******************************************************************
function: ctronl leds
@param    num:specify the led num
@param    newstate:specify the led on or off           
********************************************************************/
void LED_Init(unsigned char num,unsigned char newstate)
{
  
  switch(num)
  {
  case LEVEL1_LED:
    {
      if(newstate == ON)
      {
        GPIO_WriteBit(BATEERY_QUANTITY_PORT, BATEERY_QUANTITY_LEVEL1_PIN, RESET);     
        GPIO_WriteBit(BATEERY_QUANTITY_PORT, BATEERY_QUANTITY_LEVEL2_PIN, SET);
        GPIO_WriteBit(BATEERY_QUANTITY_PORT, BATEERY_QUANTITY_LEVEL3_PIN, SET);
      }
    }break;
  case LEVEL2_LED:
    {
      if(newstate == ON)
      {
        GPIO_WriteBit(BATEERY_QUANTITY_PORT, BATEERY_QUANTITY_LEVEL1_PIN, RESET);         
        GPIO_WriteBit(BATEERY_QUANTITY_PORT, BATEERY_QUANTITY_LEVEL2_PIN, RESET);     
        GPIO_WriteBit(BATEERY_QUANTITY_PORT, BATEERY_QUANTITY_LEVEL3_PIN, SET);
      }
    }break;
    
  case LEVEL3_LED:
    {
      if(newstate == ON)
      {
        GPIO_WriteBit(BATEERY_QUANTITY_PORT, BATEERY_QUANTITY_LEVEL1_PIN, RESET);         
        GPIO_WriteBit(BATEERY_QUANTITY_PORT, BATEERY_QUANTITY_LEVEL2_PIN, RESET);
        GPIO_WriteBit(BATEERY_QUANTITY_PORT, BATEERY_QUANTITY_LEVEL3_PIN, RESET);     
      }
      
    }break;    
  case STATUS_LED:
    {
      if(newstate == ON)
      {
        GPIO_WriteBit(MODULE_STATUS_PORT, MODULE_STATUS_PIN, RESET);     
      }
      else if(newstate == OFF)
      {
        GPIO_WriteBit(MODULE_STATUS_PORT, MODULE_STATUS_PIN, SET);
      }
      else
        GPIO_ToggleBits(MODULE_STATUS_PORT, MODULE_STATUS_PIN);     
    }break; 
    
  case LEVEL_ALL_LED:
    {
      if(newstate == ON)
      {
        GPIO_WriteBit(BATEERY_QUANTITY_PORT,BATEERY_QUANTITY_LEVEL1_PIN, RESET);
        GPIO_WriteBit(BATEERY_QUANTITY_PORT,BATEERY_QUANTITY_LEVEL2_PIN, RESET);        
        GPIO_WriteBit(BATEERY_QUANTITY_PORT,BATEERY_QUANTITY_LEVEL3_PIN, RESET);       
        GPIO_WriteBit(MODULE_STATUS_PORT, MODULE_STATUS_PIN, RESET);
      }
      else
      {
        GPIO_WriteBit(BATEERY_QUANTITY_PORT,BATEERY_QUANTITY_LEVEL1_PIN, SET);
        GPIO_WriteBit(BATEERY_QUANTITY_PORT,BATEERY_QUANTITY_LEVEL2_PIN, SET);        
        GPIO_WriteBit(BATEERY_QUANTITY_PORT,BATEERY_QUANTITY_LEVEL3_PIN, SET);
        GPIO_WriteBit(MODULE_STATUS_PORT, MODULE_STATUS_PIN, SET);
      }
    }break;     
  }
  
}

void adcInit(ADC_Channel_TypeDef num)
{
  /* Enable ADC1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);//开启ADC1时钟
  
  ADC_VrefintCmd(ENABLE); //使能内部参考电压
  ADC_Init(ADC1,ADC_ConversionMode_Continuous,ADC_Resolution_12Bit,ADC_Prescaler_1);//连续转换，12位，转换时钟1分频
  
  ADC_ChannelCmd(ADC1,num,ENABLE);//使能内部参考电压通道
  ADC_Cmd(ADC1,ENABLE);//ADC使能
  
}

uint32_t adcGet(ADC_Channel_TypeDef num)
{
  uint32_t tmp;
  unsigned char i;
  float tmp2;
 /* Waiting until press Joystick Up */
  /* Wait until End-Of-Convertion */
   adcInit(ADC_Channel_Vrefint);
  ADC_SoftwareStartConv(ADC1); //开启软件转换
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
  {}
  ADC_ClearFlag(ADC1,ADC_FLAG_EOC);//清除对应标志
  /* Get conversion value */
  tmp = ADC_GetConversionValue(ADC1);
   ADC_RATIO= (1.225 * 4096)/tmp;


  ADC_DeInit(ADC1);
  tmp = 0;
  tmp2 =0;
  adcInit(num);  ADC_SoftwareStartConv(ADC1); //开启软件转换
  for(i=0;i<100;i++)
  { 

	  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
	  {}
	  ADC_ClearFlag(ADC1,ADC_FLAG_EOC);//清除对应标志
	  /* Get conversion value */
	  tmp = ADC_GetConversionValue(ADC1);
	  
	  /* Calculate voltage value in uV over capacitor  C67 for IDD measurement*/
	  tmp2 = tmp2 + ((uint32_t)tmp*ADC_RATIO*244.14);
	  /* Waiting Delay 200ms */
	 // delay_ms(2);
	  
	  /* DeInitialize ADC1 */


  }
  ADCdata = tmp2 /100;
   ADC_DeInit(ADC1);
  
  /* Disable ADC1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
  
  return ADCdata;
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
void USART2_SenByte(unsigned char *Str,unsigned char len) 
{
  while(len>0)
  {
    USART_SendData8(USART2, *Str);
    while(!USART_GetFlagStatus (USART2, USART_FLAG_TXE));
    Str++;
    len--;
  }
}
void Send_Comm(unsigned char* comm,unsigned short len)
{
  
  //HAL_UART_DMAStop(&huart5);
  //HAL_UART_DMAResume(&huart5);
  //UsartType5.real_index=0;
  //UsartType5.rx_len=0;
  //UsartType5.rx_len_var=0;
  //UsartType5.loop_index=0;
  //HAL_UART_Receive_DMA(&huart5,UsartType5.usartDMA_rxBuf,buffer_size);
  //delay_ms(100);
  //uart5_dma_state=0;
  USART_SenByte(comm,len);
  
}
void Send_Comm2(unsigned char* comm,unsigned short len)
{
  
  //HAL_UART_DMAStop(&huart5);
  //HAL_UART_DMAResume(&huart5);
  //UsartType5.real_index=0;
  //UsartType5.rx_len=0;
  //UsartType5.rx_len_var=0;
  //UsartType5.loop_index=0;
  //HAL_UART_Receive_DMA(&huart5,UsartType5.usartDMA_rxBuf,buffer_size);
  //delay_ms(100);
  //uart5_dma_state=0;
  USART2_SenByte(comm,len);
  
}
void HardwareInit()
{
  disableInterrupts();
  SystemClock_Init();     // 系统时钟初始化
  GPIO_Initial(); 
  lock_state(OFF);
  Uart1_Init(115200);// 初始化GPIO
  DMA_Config();
  LED_Init(LEVEL_ALL_LED,ON);
  RTC_Config(20,ON);
  enableInterrupts();  
  gnss_state(ON);
  //gsm_power_state(ON);
  //adcGet(VBAT_SENSE_CHANNEL);
  
}


