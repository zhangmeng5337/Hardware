#include "global.h"
#include "adc.h"
#include "other.h"
#include "CC1110.h"
#include "spi.h"
#include "infrared.h"
#include "rtc.h"

RCC_ClocksTypeDef RCC_Clocks;
static uint8_t  fac_us=0;
static uint16_t fac_ms=0;
extern WPStatus PD_WAKEUP_Flag;
/**************************************************************
������void Bsp_Config (void)
���ܣ�Ӳ����ʼ��
**************************************************************/
void RCC_Config(void)
{
    RCC_HSICmd(ENABLE);//ʹ���ڲ����پ���
    while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
    {}
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
    RCC_MSIRangeConfig(RCC_MSIRange_6);
    RCC_HSEConfig(RCC_HSE_OFF);
    if(RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET )
    {
        while(1);
    }
}

void ControlGPIO_Initt(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
//	    EXTI_InitTypeDef EXTI_InitStructure;
		
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	
}

void ControlGPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
//	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
//	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
//	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
//	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
//	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
/**************************************************************
������void Wake_Config(void)
���ܣ�������������
**************************************************************/
void Wake_Config(void)
{
	
	  NVIC_InitTypeDef  NVIC_InitStructure;

    EXTI_InitTypeDef EXTI_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_DeInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
 //   
	
  EXTI_ClearITPendingBit(EXTI_Line20);
  EXTI_InitStructure.EXTI_Line = EXTI_Line20;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Wakeup Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


	

    /****************PB0��Ӧ��0��********************/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    /****************PB1��Ӧ��1��********************/
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_Init(&EXTI_InitStructure);	
		EXTI_ClearITPendingBit(EXTI_Line0);
		EXTI_ClearITPendingBit(EXTI_Line1);
		//ControlGPIO_Init();

}
/**************************************************************
������void EnterLowPower(void)
���ܣ�����͹���ģʽ
**************************************************************/
void EnterLowPower(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE , ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC , ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	    GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA| RCC_AHBPeriph_GPIOC |RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE , DISABLE);//Ϊ��M0M1��λ����C�ڹر�ȥ����
    TIM2->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));     //�رն�ʱ������
    TIM3->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));     //�رն�ʱ������
    SPI2->CR1 &= (uint16_t)~((uint16_t)SPI_CR1_SPE);       //�ر�SPI
    SPI1->CR1 &= (uint16_t)~((uint16_t)SPI_CR1_SPE);       //�ر�SPI	
    USART3->CR1 &= (uint16_t)~((uint16_t)USART_CR1_UE);    //�ر�USART
    ADC1->CR2 &= (uint32_t)(~ADC_CR2_ADON);                //�ر�ADC����
    RCC->APB2ENR &= ~ RCC_APB2Periph_ADC1;
    RCC->APB1ENR &= ~(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_SPI2|RCC_APB1Periph_USART3); //�ر�����ʱ��
    Wake_Config();	
//		
//    //RtcWakeUpConfig();

//		
    PWR_ClearFlag(PWR_FLAG_WU);
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

}
/**************************************************************
������WPStatus PD_WAKEUP_status (void)
���ܣ��������Ѽ�� ��PB1״̬ ����ߵ�ƽ��⵽����*
**************************************************************/
PDStatus PD_WAKEUP_status (void)
{
    PDStatus status = Low;
    if(GPIOB->IDR & GPIO_Pin_1)status=High;
    else status=Low;
    return status;
}
/**************************************************************
������void delay_init(void)
���ܣ���ʱ
**************************************************************/
void delay_init(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    fac_us=SystemCoreClock/16000000;
    fac_ms=(uint16_t)fac_us*1000;
}
/**************************************************************
������void delay_us(uint32_t nus)
���ܣ���ʱ΢��
**************************************************************/
void delay_us(uint32_t nus)
{
    uint32_t temp;
    SysTick->LOAD=nus*fac_us; //ʱ�����
    SysTick->VAL=0x00;        //��ռ�����
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; //��ʼ����
    do
    {
        temp=SysTick->CTRL;
    }
    while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //�رռ�����
    SysTick->VAL =0X00;       //��ռ�����
}
/**************************************************************
������void delay_ms(uint16_t nms)
���ܣ���ʱ���� nms<=0xffffff*8*1000/SYSCLK ��32M������,nms<=4148
**************************************************************/
void delay_ms(uint16_t nms)
{
    uint32_t temp;
    SysTick->LOAD=(uint32_t)nms*fac_ms;
    SysTick->VAL =0x00;
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
    do
    {
        temp=SysTick->CTRL;
    }
    while(temp&0x01&&!(temp&(1<<16)));
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL =0X00;
}

/**************************************************************
������void Bsp_Config (uint8_t (*Fuction_Cfg)())
���ܣ�Ӳ����ʼ��
**************************************************************/
void Bsp_Config (void)
{
    RCC_Config();
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency /100);
    Power_Control_GPIO();     //̽ͷ����
    ADC_Config();             //ADC��ʼ��
    Battery_Monitor_Config(); //��ؼ��
    delay_init();             //��ʱ

}

