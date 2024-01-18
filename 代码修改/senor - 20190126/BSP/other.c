#include "global.h"
#include "other.h"
#include "spi.h"
unsigned char _menu_tab[]= {0x9e,0xa7,0xc6,0xbc,0xb3,0xf0};
unsigned char _idcl_Tab[]= {0xbf,0xff};    //ID��ʾ��־ ��������
#if (defined Liceng)||(defined Weiyan)||(defined Yali)
unsigned char _flag1_tab[]= {0xfe,0xf7};             //��㴫�����ı�ʾ
#elif (defined Maogan)||(defined Maosuo)
unsigned char _flag2_tab[]= {0xfe,0xbf,0xf7,0xbe};   //�����������ı�ʾ
#endif
unsigned char _data_tab[]= {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x7f}; //0123456789.
/**************************************************************
������void NVIC_Config(void)
���ܣ��жϷ������
**************************************************************/
void NVIC_Config(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    /**************TIM4 ���������ȼ�ʱ�ж�****************/
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /****************TIM2��ͨ��ʱ�жϷ���********************/
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x08;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =  0x04;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /****************PB0��Ӧ��0���жϷ���********************/
    NVIC_InitStructure.NVIC_IRQChannel =EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0e;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0e;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /****************PB0��Ӧ��1���жϷ���********************/
    NVIC_InitStructure.NVIC_IRQChannel =EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}
/**************************************************************
������void Battery_Monitor_Config (void)
���ܣ���ص�ѹ�������PC1
**************************************************************/
void Battery_Monitor_Config (void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/**************************************************************
������uint8_t Battery_Monitor(void)
���ܣ���ص�ѹ��� PC3
**************************************************************/
uint8_t Battery_Monitor(void)
{
    if(GPIOC->IDR & GPIO_Pin_1)return 0xCB;   //��ص�ѹ��
    else return 0xCA; //��ص�ѹ����
}
/**************************************************************
������void Power_Control_GPIO(void)
���ܣ� ��������Դ����PC5->3.3V PC2->5V
**************************************************************/
void Power_Control_GPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
#ifdef Liceng //������������3.3V����
    RCC->AHBENR |= RCC_AHBPeriph_GPIOC;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIOC->BSRRH = GPIO_Pin_5;
#else
    RCC->AHBENR |= RCC_AHBPeriph_GPIOC;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd =GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIOC->BSRRH = GPIO_Pin_2;
#endif
}
/**************************************************************
������void Power_Control(char state)
���ܣ��������������
**************************************************************/
void Power_Control(char state)
{
#ifdef Liceng //������������3.3V����
    if(state)GPIOC->BSRRL= GPIO_Pin_5;   //�߿�
    else GPIOC->BSRRH = GPIO_Pin_5;      //�͹�
#else  //�������������������5V����
    if(state)GPIOC->BSRRL = GPIO_Pin_2;  //�߿�
    else GPIOC->BSRRH = GPIO_Pin_2;      //�͹�
#endif
}
/**************************************************************
������void Led_Control_GPIO(void)
���ܣ�����ܿ���PC0
**************************************************************/
void Led_Control_GPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOC;
    /****************����ܿ���******************/
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIOC->BSRRH = GPIO_Pin_0;
}
/**************************************************************
������void LED_Control(char state)
���ܣ�����ܵ�Դ����
**************************************************************/
void LED_Control(char state)
{
    if(state)GPIOC->BSRRL = GPIO_Pin_0; //�߿�
    else GPIOC->BSRRH = GPIO_Pin_0;     //�͹�
}
/**************************************************************
������void Timer_Config(uint8_t (*Fuction_Cfg)())
���ܣ���ʱ����ʱ1��
**************************************************************/
void Timer_Config(uint16_t per)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC->APB1ENR |= RCC_APB1Periph_TIM2;

    TIM_TimeBaseStructure.TIM_Period = per;            //(1+per)/16MHz*(1+9999)=x
    TIM_TimeBaseStructure.TIM_Prescaler = 9999;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM2->SR = (uint16_t)~TIM_FLAG_Update;
    TIM2->CR1 |= TIM_CR1_ARPE;
    TIM2->CR1 |= TIM_CR1_CEN;
    TIM2->DIER |= TIM_IT_Update;  //ʹ�ܶ�ʱ��TIM2
}
/**************************************************************
������void Storage_Data(void)
���ܣ�PB12����595����
**************************************************************/
void Storage_Data(void)
{
    GPIOB->BSRRL = GPIO_Pin_12;
    GPIOB->BSRRH = GPIO_Pin_12;
    GPIOB->BSRRL = GPIO_Pin_12;
}
/**************************************************************
������void Led_Display(uint16_t(*fun)(uint16_t),uint8_t *table,uint8_t i,uint8_t dis_bit)
���ܣ��������ʾ����λ
**************************************************************/
void Led_Display(uint16_t(*fun)(uint16_t),uint8_t *table,uint8_t i,uint8_t dis_bit)
{
    fun(*(table+i));
    fun(dis_bit);
    Storage_Data();
    delay_ms(2);
}
/**************************************************************
������void Led_Display_Data(int channel,int data)
���ܣ��������ʾ����
**************************************************************/
void Led_Display_Data(uint8_t channel,uint16_t data)
{
#if (defined Liceng)||(defined Weiyan)||(defined Yali)
    Led_Display(&SPI2_ReadSend_byte,_flag1_tab,channel,0x08);//��ʾ
#elif (defined Maogan)||(defined Maosuo)
    Led_Display(&SPI2_ReadSend_byte,_flag2_tab,channel,0x08);//��ʾ
#endif
    Led_Display(&SPI2_ReadSend_byte,_data_tab,data/100%10,0x04);//��λ
    Led_Display(&SPI2_ReadSend_byte,_data_tab,data/10%10,0x02); //ʮλ
    Led_Display(&SPI2_ReadSend_byte,_data_tab,data%10,0x01); //��λ
#if defined Yali
    Led_Display(&SPI2_ReadSend_byte,_data_tab,10,2);    //С����
#endif
}
/**************************************************************
������void Led_Display_XX(uint8_t sensor_id)
���ܣ��������ʾ������ID/NUM
**************************************************************/
void Led_Display_XX(uint8_t sensor_id)
{
    Led_Display(&SPI2_ReadSend_byte,_idcl_Tab,0,0x08);
    Led_Display(&SPI2_ReadSend_byte,_data_tab,sensor_id/10%10,0x04);//ʮλ
    Led_Display(&SPI2_ReadSend_byte,_data_tab,sensor_id%10,0x02);//��λ
    Led_Display(&SPI2_ReadSend_byte,_idcl_Tab,0,0x01);
}
/**************************************************************
������void Led_Display_Set(char type,uint8_t sensor_id)
���ܣ��������ʾ���������
**************************************************************/
void Led_Display_Set(uint8_t i,uint8_t data)
{
    Led_Display(&SPI2_ReadSend_byte,_menu_tab,i,0x08);
    Led_Display(&SPI2_ReadSend_byte,_data_tab,data/10%10,0x04);//ʮλ
    Led_Display(&SPI2_ReadSend_byte,_data_tab,data%10,0x02);//��λ
    Led_Display(&SPI2_ReadSend_byte,_menu_tab,i+3,0x01);
}
/**************************************************************
������void Led_Display_Set(uint8_t sensor_id)
���ܣ��������ʾ���������
**************************************************************/
void LED_Display_Clear(void)
{
    uint8_t i;
    for(i=0x08; i>=0x01; i=i>>1) //�������λ
    {
        Led_Display(&SPI2_ReadSend_byte,_idcl_Tab,1,i);
    }
    for(i=4; i>=1; i--)       //���С����
    {
        Led_Display(&SPI2_ReadSend_byte,_idcl_Tab,1,i);
    }
}

