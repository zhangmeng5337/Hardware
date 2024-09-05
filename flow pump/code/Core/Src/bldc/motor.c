#include "motor.h"
extern TIM_HandleTypeDef htim1;
/*
PWMƵ�ʼ��㣺
Ƶ�� = ��ʱ��ʱ�� / ��Prescaler Ԥ��Ƶ + 1��/ ��Counter Period ����ֵ + 1��Hz
����Ŀ��12KHz
ռ�ձ� = Pulse ( �Ա�ֵ) / ��C ounter Period ����ֵ��%

*/
/*��ʼ��������*/

/*����һ������Ľṹ��*/
MOTOR my_motor={1,0,20,0,0};//����һ������ṹ��


/*�����������*/
void Six_Step_Phase(uint8_t hall_state,uint8_t direction)
{
    if(direction!=0)//�����ת
    {
        switch (hall_state)
        {
            case 1:
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);//U+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//W+
                UL_OFF;VL_OFF;WL_ON;//�¹�
                break;
            case 2:
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);//U+
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//W+
                UL_ON;VL_OFF;WL_OFF;//�¹�
            break;
            case 3:
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);//U+
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//W+
                UL_OFF;VL_OFF;WL_ON;//�¹�
            break;
            case 4:
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);//U+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);//W+
                UL_OFF;VL_ON;WL_OFF;//�¹�
            break;
            case 5:
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);//U+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//W+
                UL_OFF;VL_ON;WL_OFF;//�¹�
            break;
            case 6:
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);//U+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);//W+
                UL_ON;VL_OFF;WL_OFF;//�¹�
            break;
            
            default:
                Motor_Stop();//���ͣת
                break;
        }
    }else//�����ת
    {
        switch (hall_state)
        {
            case 1:
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//U+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);//W+
                UL_ON;VL_OFF;WL_OFF;//�¹�
                break;
            case 2:
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);//U+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);//W+
                UL_OFF;VL_ON;WL_OFF;//�¹�
            break;
            case 3:
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//U+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);//W+
                UL_OFF;VL_ON;WL_OFF;//�¹�
            break;
            case 4:
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//U+
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);//W+
                UL_OFF;VL_OFF;WL_ON;//�¹�
            break;
            case 5:
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//U+
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);//W+
                UL_ON;VL_OFF;WL_OFF;//�¹�
            break;
            case 6:
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);//U+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//W+
                UL_OFF;VL_OFF;WL_ON;//�¹�
            break;
            
            default:
                Motor_Stop();//���ͣת
                break;
        }
    }
    

}

/*
��������ͨ����PWMռ�ձ�
����:uint8_t duty:ռ�ձ���ֵ0-100;
*/
void Change_PWM_Duty(uint16_t duty)
{   
    //1.��ʼռ�ձȵ�ӳ��;
    duty = (duty*PWM_COUNTER)/100;
    //2.��ʼ�޸�ռ�ձ�
//    TIM1->CCR1 = duty;
//    TIM1->CCR2 = duty;
//    TIM1->CCR3 = duty;  

	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, duty);  // U+
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, duty);  // V+
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, duty);  // W+
	
}

/*���ͣת*/
void Motor_Stop()
{
    
    //1.�ر�PWM���
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//U+
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);//V+
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);//W+
    //2.�ر������¹�
    UL_OFF;VL_OFF;WL_OFF;//�¹�
    my_motor.Status = HALT;
}

/*��������£��״λ�ȡת��λ�ã�Ȼ���򣬴����ж�*/
void Motor_Start()
{
    my_motor.Status = RUN;
    my_motor.Hall_status = get_hall_state();
    Six_Step_Phase(my_motor.Hall_status,my_motor.Direction);
}
MOTOR *getMotr(void)
{
	return &my_motor;
}


