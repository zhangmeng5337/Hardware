#include "stm32l1xx_it.h"
#include "main.h"
#include "global.h"
#include "other.h"
#include "infrared.h"
#include "adc.h"

WPStatus CC1100_WAKEUP_Flag;
WPStatus PD_WAKEUP_Flag;
/************************��ʱ�������*************************/
uint8_t Timer=0;
uint8_t g_IR_State = 0;
uint32_t g_IR_Code[IR_CODE_LEN];
extern uint32_t CV_Value[4];
/**************************************************************
������void NMI_Handler(void)
���ܣ���������
**************************************************************/
void NMI_Handler(void)
{

}
/**************************************************************
������void HardFault_Handler(void)
���ܣ�Ӳ������
**************************************************************/
void HardFault_Handler(void)
{
    while (1)
    {
    }
}
/**************************************************************
������void MemManage_Handler(void)
���ܣ��ڴ�������
**************************************************************/
void MemManage_Handler(void)
{
    while (1)
    {
    }
}
/**************************************************************
������void BusFault_Handler(void)
���ܣ����ߴ����жϣ�һ�㷢�������ݷ����쳣������FSMC���ʲ���
**************************************************************/
void BusFault_Handler(void)
{
    while (1)
    {
    }
}
/**************************************************************
������void UsageFault_Handler(void)
���ܣ��÷������жϣ�һ����Ԥȡֵ������λ��ָ����ݴ���ȴ���
**************************************************************/
void UsageFault_Handler(void)
{
    while (1)
    {
    }
}
/**************************************************************
������void SVC_Handler(void)
���ܣ�ϵͳ�����쳣����Ҫ��Ϊ�˵��ò���ϵͳ�ں˷���
**************************************************************/
void SVC_Handler(void)
{
}
/**************************************************************
������void DebugMon_Handler(void)
���ܣ����Լ����쳣
**************************************************************/
void DebugMon_Handler(void)
{
}
/**************************************************************
������void PendSV_Handler(void)
���ܣ������쳣
**************************************************************/
void PendSV_Handler(void)
{
}
/**************************************************************
������void SysTick_Handler(void)
���ܣ�
**************************************************************/
void SysTick_Handler(void)
{


}
/**************************************************************
������void EXTI0_IRQHandler(void)
���ܣ�PB0 WIREWAKEUP
**************************************************************/
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        CC1100_WAKEUP_Flag=High;
        PD_WAKEUP_Flag=Low;
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
/**************************************************************
������void EXTI1_IRQHandler(void)
���ܣ�PB1 PD_WAKEUP WIREWAKEUP
**************************************************************/
void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        CC1100_WAKEUP_Flag=Low;
        PD_WAKEUP_Flag=High;
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}
/**************************************************************
������void TIM2_IRQHandler(void)
���ܣ���ʱ����ʱ1������ж�
**************************************************************/
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
        //Get_ADC(Timer);
			      Get_ADC(0);
			 Get_ADC(1);
			 Get_ADC(2);
        Timer++;
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
  FLASH_Write(((uint32_t)0x08080020),((uint32_t)0x08080030),CV_Value);
//			          CC1110_Config((uint32_t)9600);	

//          CC1110_Send_Flag=1;		
//		ControlGPIO_Initt();
//		GPIO_ResetBits(GPIOC,GPIO_Pin_8);//���� 

//		USART_ReceiveData(USART3);
//		USART_ClearFlag(USART3,USART_FLAG_RXNE);
//		CC1110_Sendd();
    }
}

/**************************************************************
������void TIM3_IRQHandler(void)
���ܣ������жϽ���
**************************************************************/
void TIM4_IRQHandler(void)
{
    static uint32_t s_i = 0;   // ��̬�ֲ�����
    if(TIM_GetITStatus(TIM4, TIM_IT_CC1) == SET)
    {
        TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE); //�ر�����ж�
        switch (g_IR_State)
        {
        case IR_STATE_NO: // ��һ���½��أ���ʼ�����ݣ�׼����������
            TIM_SetCounter(TIM4,0);
            s_i = 0;
            g_IR_State = IR_STATE_READ;
            break;
        case IR_STATE_READ:// ���沶����ֵ
            if (s_i<IR_CODE_LEN)
            {
                g_IR_Code[s_i++]=TIM_GetCounter(TIM4);
                TIM_SetCounter(TIM4,0);

            } else {
                g_IR_State = IR_STATE_END;
                s_i = 0;
            }
            break;
        case IR_STATE_END:// �������
            break;
        default:
            break;
        }
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);   //����жϱ�־
        TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
        TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);   //������ж�

    }
    else if(TIM_GetITStatus(TIM4,TIM_IT_Update)== SET)// TIM�����˵���������趨����ֵ��Ч
    {
        if (g_IR_State == IR_STATE_READ)      // ��������ʱ���˵����������Ч,�������,����������״̬
        {
            g_IR_State = IR_STATE_NO;
            s_i = 0;
        } else if (g_IR_State == IR_STATE_END) // ��������ȡ����,��˵��������Ч
        {
            g_IR_State = IR_STATE_OK;
        }
        TIM_ClearITPendingBit(TIM4,TIM_IT_Update); //��������־
        TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);  //�ر�����ж�
    }
}




