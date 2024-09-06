#include "motor.h"
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim6;

/*
PWMƵ�ʼ��㣺
Ƶ�� = ��ʱ��ʱ�� / ��Prescaler Ԥ��Ƶ + 1��/ ��Counter Period ����ֵ + 1��Hz
����Ŀ��12KHz
ռ�ձ� = Pulse ( �Ա�ֵ) / ��C ounter Period ����ֵ��%

*/
/*��ʼ��������*/

/*����һ������Ľṹ��*/
MOTOR my_motor = {1, 0, 20, 0, 0}; //����һ������ṹ��

void Motor_Init(void)
{
//    HAL_TIM_Encoder_Start(&ENCODER_TIM, TIM_CHANNEL_ALL);      //������������ʱ��
//    __HAL_TIM_ENABLE_IT(&ENCODER_TIM,TIM_IT_UPDATE);           //������������ʱ�������ж�,���������
//    HAL_TIM_Base_Start_IT(&GAP_TIM);                       //����100ms��ʱ���ж�
//    HAL_TIM_PWM_Start(&PWM_TIM, TIM_CHANNEL_2);            //����PWM
//    HAL_TIM_PWM_Start(&PWM_TIM, TIM_CHANNEL_1);            //����PWM
//    __HAL_TIM_SET_COUNTER(&htim2,
   // 10000);                //��������ʱ����ʼֵ�趨Ϊ10000
    my_motor.lastCount = 0;                                   //�ṹ�����ݳ�ʼ��
    my_motor.totalCount = 0;
    my_motor.overflowNum = 0;
    my_motor.Speed = 0;
    my_motor.Direction = 0;
}


/*�����������*/
void Six_Step_Phase(uint8_t hall_state, uint8_t direction)
{
    if (direction != 0) //�����ת
    {
        switch (hall_state)
        {
            case 1:
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); //U+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //W+
                UL_OFF;
                VL_OFF;
                WL_ON;//�¹�
                break;
            case 2:
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3); //U+
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //W+
                UL_ON;
                VL_OFF;
                WL_OFF;//�¹�
                break;
            case 3:
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3); //U+
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //W+
                UL_OFF;
                VL_OFF;
                WL_ON;//�¹�
                break;
            case 4:
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3); //U+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //W+
                UL_OFF;
                VL_ON;
                WL_OFF;//�¹�
                break;
            case 5:
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); //U+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //W+
                UL_OFF;
                VL_ON;
                WL_OFF;//�¹�
                break;
            case 6:
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3); //U+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //W+
                UL_ON;
                VL_OFF;
                WL_OFF;//�¹�
                break;

            default:
                Motor_Stop();//���ͣת
                break;
        }
    }
    else //�����ת
    {
        switch (hall_state)
        {
            case 1:
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //U+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); //W+
                UL_ON;
                VL_OFF;
                WL_OFF;//�¹�
                break;
            case 2:
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //U+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3); //W+
                UL_OFF;
                VL_ON;
                WL_OFF;//�¹�
                break;
            case 3:
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //U+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); //W+
                UL_OFF;
                VL_ON;
                WL_OFF;//�¹�
                break;
            case 4:
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //U+
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3); //W+
                UL_OFF;
                VL_OFF;
                WL_ON;//�¹�
                break;
            case 5:
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //U+
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3); //W+
                UL_ON;
                VL_OFF;
                WL_OFF;//�¹�
                break;
            case 6:
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); //U+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //W+
                UL_OFF;
                VL_OFF;
                WL_ON;//�¹�
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
    duty = (duty * PWM_COUNTER) / 100;
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
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //U+
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); //V+
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3); //W+
    //2.�ر������¹�
    UL_OFF;
    VL_OFF;
    WL_OFF;//�¹�
    my_motor.Status = HALT;
}

/*��������£��״λ�ȡת��λ�ã�Ȼ���򣬴����ж�*/
void Motor_Start()
{
    my_motor.Status = RUN;
    my_motor.Hall_status = get_hall_state();
    Six_Step_Phase(my_motor.Hall_status, my_motor.Direction);
}
MOTOR *getMotr(void)
{
    return &my_motor;
}

float speed_Record[SPEED_RECORD_NUM] = {0};

/*
 * �����ٶȵ�ƽ���˲�
 * �����²��������ٶȣ�����ٶȵ����飬
 * �����˲�����ٶ�
 */
float Speed_Low_Filter(float new_Spe, float *speed_Record)
{
    float sum = 0.0f;
    for (uint8_t i = SPEED_RECORD_NUM - 1; i > 0; i--) //���������ݺ���һλ
    {
        speed_Record[i] = speed_Record[i - 1];
        sum += speed_Record[i - 1];
    }
    speed_Record[0] = new_Spe;//��һλ���µ�����
    sum += new_Spe;
    my_motor.Speed_Filter = sum / SPEED_RECORD_NUM;
    return sum / SPEED_RECORD_NUM; //���ؾ�ֵ
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef
                                   *htim)//��ʱ���ص����������ڼ����ٶ�
{
//    if(htim->Instance==htim2.Instance)//���������붨ʱ������жϣ����ڷ����
//    {
//        if(COUNTERNUM < 10000) my_motor.overflowNum++;       //������������
//        else if(COUNTERNUM >= 10000) my_motor.overflowNum--; //������������
//        __HAL_TIM_SetCounter(&htim2, 10000);             //�����趨��ʼֵ
//    }
//    else if(htim->Instance==htim6)//�����ʱ���жϣ���ʱ������ٶ���
//    {
//        my_motor.Direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2);//������ϼ�������ת��������ֵΪ0�����򷵻�ֵΪ1
//        my_motor.totalCount = COUNTERNUM + my_motor.overflowNum * RELOADVALUE;//һ�������ڵ��ܼ���ֵ����Ŀǰ����ֵ��������ļ���ֵ
//        my_motor.Speed = (float)(my_motor.totalCount - my_motor.lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10;//���ÿ�����ת
//        //motor1.speed = (float)(motor1.totalCount - motor1.lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10 * LINE_SPEED_C//��ó������ٶ�ÿ����ٺ���
//        my_motor.lastCount = my_motor.totalCount; //��¼��һ�εļ���ֵ
//    }
    if (htim->Instance == htim6.Instance) //���������붨ʱ������жϣ����ڷ����
    {
        my_motor.Direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(
                                 &htim2);//������ϼ�������ת��������ֵΪ0�����򷵻�ֵΪ1
        my_motor.totalCount = COUNTERNUM + my_motor.overflowNum *
                              RELOADVALUE;//һ�������ڵ��ܼ���ֵ����Ŀǰ����ֵ��������ļ���ֵ

        if (my_motor.lastCount - my_motor.totalCount >
                19000) // �ڼ���ֵ���ʱ���з��������
        {
            my_motor.overflowNum++;
            my_motor.totalCount = COUNTERNUM + my_motor.overflowNum *
                                  RELOADVALUE;//һ�������ڵ��ܼ���ֵ����Ŀǰ����ֵ��������ļ���ֵ
        }
        else if (my_motor.totalCount - my_motor.lastCount >
                 19000) // �ڼ���ֵ���ʱ���з��������
        {
            my_motor.overflowNum--;
            my_motor.totalCount = COUNTERNUM + my_motor.overflowNum *
                                  RELOADVALUE;//һ�������ڵ��ܼ���ֵ����Ŀǰ����ֵ��������ļ���ֵ
        }

        my_motor.Speed = (float)(my_motor.totalCount - my_motor.lastCount) /
                         (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) *
                         10;//���ÿ�����ת,����4����Ϊ4��Ƶ
        my_motor.lastCount = my_motor.totalCount; //��¼��һ�εļ���ֵ

    }

}

void motor_ctrl(void)
{
    static uint32_t tick;
    if (getMotr()->Status == RUN)
    {
        HAL_TIM_Base_Start(&htim6); 
        HAL_TIM_Base_Start_IT(&htim6);
        if ((HAL_GetTick() - tick) >= PID_CAL_TIME)
        {

            getMotr()->Duty = PID_realize(getMotr()->Set_Speed);
            Change_PWM_Duty(getMotr()->Duty);
        }
        else
        {
            PID_init();
            Motor_Stop();
        }
    }
	else
	{
	    HAL_TIM_Base_Stop(&htim6); 
		HAL_TIM_Base_Stop_IT(&htim6);
	}

}

