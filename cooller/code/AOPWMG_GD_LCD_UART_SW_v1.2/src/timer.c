#include "timer.h"
#include "filter.h"
#if CPU == ST
TIM_HandleTypeDef TIM3_Handler;         //��ʱ��5���
#endif
//��ʱ��5ͨ��1���벶������
//arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
//psc��ʱ��Ԥ��Ƶ��
void TIM3_CH3_Cap_Init(uint32_t arr,unsigned int psc)
{
#if CPU == ST
    TIM_IC_InitTypeDef TIM3_CH33Config;

    TIM3_Handler.Instance=TIM3;                          //ͨ�ö�ʱ��5
    TIM3_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM3_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_IC_Init(&TIM3_Handler);//��ʼ�����벶��ʱ������

    TIM3_CH33Config.ICPolarity=TIM_ICPOLARITY_RISING;    //�����ز���
    TIM3_CH33Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//ӳ�䵽TI1��
    TIM3_CH33Config.ICPrescaler=TIM_ICPSC_DIV1;          //���������Ƶ������Ƶ
    TIM3_CH33Config.ICFilter=0;                          //���������˲��������˲�
    HAL_TIM_IC_ConfigChannel(&TIM3_Handler,&TIM3_CH33Config,TIM_CHANNEL_3);//����TIM5ͨ��1

    HAL_TIM_IC_Start_IT(&TIM3_Handler,TIM_CHANNEL_3);   //����TIM3�Ĳ���ͨ��3�����ҿ��������ж�
    __HAL_TIM_ENABLE_IT(&TIM3_Handler,TIM_IT_UPDATE);   //ʹ�ܸ����ж�
#else

#endif
}



//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
unsigned char  TIM3CH3_CAPTURE_STA=0;	//���벶��״̬
uint32_t	TIM3CH3_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)


//��ʱ��5�жϷ�����
/*void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);//��ʱ�����ô�����
}*/


#if CPU == ST
//��ʱ�������жϣ�����������жϴ���ص������� �ú�����HAL_TIM_IRQHandler�лᱻ����
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//�����жϣ����������ʱִ��
{

    if((TIM3CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
    {
        if(TIM3CH3_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
        {
            if((TIM3CH3_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
            {
                TIM3CH3_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
                TIM3CH3_CAPTURE_VAL=0XFFFFFFFF;
            }
            else TIM3CH3_CAPTURE_STA++;
        }
    }
}
#else


#endif

uint32_t timecount,test,timecount1,edge_cnt;
float fq=-1,fq_t;
unsigned char state,fq_flag;
uint32_t result;
//��ʱ�����벶���жϴ���ص��������ú�����HAL_TIM_IRQHandler�лᱻ����
void TIM3CaptureChannel3Callback()//�����жϷ���ʱִ��
{
    
    /*	if((TIM3CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
    	{
    		if(TIM3CH3_CAPTURE_STA&0X40)		//����һ���½���
    			{
    				TIM3CH3_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
                    TIM3CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM3_Handler,TIM_CHANNEL_3);//��ȡ��ǰ�Ĳ���ֵ.
                    TIM_RESET_CAPTUREPOLARITY(&TIM3_Handler,TIM_CHANNEL_1);   //һ��Ҫ�����ԭ�������ã���
                    TIM_SET_CAPTUREPOLARITY(&TIM3_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//����TIM5ͨ��1�����ز���
    			}else  								//��δ��ʼ,��һ�β���������
    			{
    				TIM3CH3_CAPTURE_STA=0;			//���
    				TIM3CH3_CAPTURE_VAL=0;
    				TIM3CH3_CAPTURE_STA|=0X40;		//��ǲ�����������
    				__HAL_TIM_DISABLE(&TIM3_Handler);        //�رն�ʱ��5
    				__HAL_TIM_SET_COUNTER(&TIM3_Handler,0);
    				TIM_RESET_CAPTUREPOLARITY(&TIM3_Handler,TIM_CHANNEL_3);   //һ��Ҫ�����ԭ�������ã���
    				TIM_SET_CAPTUREPOLARITY(&TIM3_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);//��ʱ��5ͨ��1����Ϊ�½��ز���
    				__HAL_TIM_ENABLE(&TIM3_Handler);//ʹ�ܶ�ʱ��5
    			}
    	}*/

#if CPU == ST

    {
        TIM_RESET_CAPTUREPOLARITY(&TIM3_Handler,TIM_CHANNEL_3);   //һ��Ҫ�����ԭ�������ã���
        //����жϱ�־λ
        if(state==0)    //�����һ��������
        {
            state=1;
            timecount=HAL_TIM_ReadCapturedValue(&TIM3_Handler,TIM_CHANNEL_3); 	   //��¼��һ�������ص�CNTֵ
        }
        else if(state==1)//����ڶ���������
        {
            fq_flag = 1;
            state=0;
            timecount1=HAL_TIM_ReadCapturedValue(&TIM3_Handler,TIM_CHANNEL_3);   //��¼�ڶ��������ص�CNTֵ
            if(timecount<timecount1)
            {
                test=timecount1-timecount; 		  //���������صĲ�ֵ
            }
            else if(timecount>timecount1)
            {
                test=(0xffff-timecount)+timecount1;  //���������صĲ�ֵ
            }
            else
                test=0;
            uint32_t result;
            result = filter2(test);
            if(result!=NULL)
                fq=1000000/result;					  //�����Ƶ��
            else
                fq = -1;
        }
    }



#else
    if(SET == timer_interrupt_flag_get(TIMER2,TIMER_INT_CH2))
    {
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(TIMER2,TIMER_INT_CH2);

        if(0 == state)
        {
            /* read channel 0 capture value */
            timecount = timer_channel_capture_value_register_read(TIMER2,TIMER_CH_2)+1;
			edge_cnt++;
            state = 1;
        }
        else if(1 == state)
        {
            fq_flag = 1;
            state=0;
            /* read channel 0 capture value */
            timecount1 = timer_channel_capture_value_register_read(TIMER2,TIMER_CH_2)+1;

            if(timecount1 > timecount)
            {
                test = (timecount1 - timecount);
            }
            else
            {
                test = ((0xFFFF - timecount) + timecount1);
            }

           // result = filter2(test);
            if(test!=NULL)
						{
						  fq=1000000/test;					 //�����Ƶ��
							fq_t = fq;
						}
              
            else
                fq = -1;


            state = 0;
        }
    }

#endif


}
float *GetCapture()
{

    // if(fq_flag==0)
    // return -1;
    return &fq;

}
uint32_t *GetCapture_cnt()
{

    // if(fq_flag==0)
    // return -1;
    return &edge_cnt;

}
#if CPU == ST
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
#else
void HAL_TIM_IC_CaptureCallback()
#endif
{
#if CPU == ST
    if(htim->Instance == TIM3)
    {
        if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_3)
        {
            TIM3CaptureChannel3Callback();
        }
    }
#else
    //if(SET == timer_interrupt_flag_get(TIMER2,TIMER_INT_CH2)){
    TIM3CaptureChannel3Callback();
    //}

#endif
}

