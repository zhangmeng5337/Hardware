#include "motor.h"
#include "key.h"
#include "filter.h"
#include "sys.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;

extern TIM_HandleTypeDef htim6;

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

/*
PWMƵ�ʼ��㣺
Ƶ�� = ��ʱ��ʱ�� / ��Prescaler Ԥ��Ƶ + 1��/ ��Counter Period ����ֵ + 1��Hz
����Ŀ��12KHz
ռ�ձ� = Pulse ( �Ա�ֵ) / ��C ounter Period ����ֵ��%

*/
/*��ʼ��������*/

/*����һ������Ľṹ��*/
MOTOR my_motor = {1, 0, 20, 0, 0}; //����һ������ṹ��
void Current_Sense_Init(void)
{
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)my_motor.current_adc, ADC_BUF_SIZE);
    HAL_ADC_Start(&hadc1);
    registerTick(ADC_CAL_TICK, 10);
}
void Current_Sense_Proc(void)
{
    float adc_vol;
    if (GetTickResult(ADC_CAL_TICK) == 1)
    {
        adc_vol = my_motor.current_adc * ADC_REF / 4096;
        adc_vol = adc_vol / 0.1;
        my_motor.current = average_filter(adc_vol);
        reset_registerTick(ADC_CAL_TICK);
        registerTick(ADC_CAL_TICK, ADC_POLL_TIME);

    }
}
void Motor_Init(void)
{
//    HAL_TIM_Encoder_Start(&ENCODER_TIM, TIM_CHANNEL_ALL);      //������������ʱ��
//    __HAL_TIM_ENABLE_IT(&ENCODER_TIM,TIM_IT_UPDATE);           //������������ʱ�������ж�,���������
//    HAL_TIM_Base_Start_IT(&GAP_TIM);                       //����100ms��ʱ���ж�
//    HAL_TIM_PWM_Start(&PWM_TIM, TIM_CHANNEL_2);            //����PWM
//    HAL_TIM_PWM_Start(&PWM_TIM, TIM_CHANNEL_1);            //����PWM
    HAL_TIM_Base_Start_IT(&htim6);                     //����100ms��ʱ���ж�
    __HAL_TIM_CLEAR_FLAG(&htim2, TIM_IT_UPDATE);

    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);    //������������ʱ��

    __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);
    __HAL_TIM_SET_COUNTER(&htim2,
                          20000);               //��������ʱ����ʼֵ�趨Ϊ10000

    __HAL_TIM_CLEAR_FLAG(&htim4, TIM_IT_UPDATE);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1 | TIM_CHANNEL_2); //������������ʱ��

    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);



    my_motor.lastCount = 0;                                   //�ṹ�����ݳ�ʼ��
    my_motor.totalCount = 0;
    my_motor.overflowNum = 0;
    my_motor.Speed = 0;
    my_motor.Direction = 0;
    Current_Sense_Init();
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
    if (htim->Instance == htim4.Instance) //���������붨ʱ������жϣ����ڷ����
    {
        my_motor.Direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(
                                 &htim2);//������ϼ�������ת
    }

    if (htim->Instance == htim6.Instance) //���������붨ʱ������жϣ����ڷ����
    {

        my_motor.Direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(
                                 &htim2);//������ϼ�������ת��������ֵΪ0�����򷵻�ֵΪ1
        my_motor.totalCount = COUNTERNUM + my_motor.overflowNum *
                              RELOADVALUE;//һ�������ڵ��ܼ���ֵ����Ŀǰ����ֵ��������ļ���ֵ

        if (my_motor.lastCount - my_motor.totalCount > 19000)
        {
            my_motor.overflowNum++;
            my_motor.totalCount = COUNTERNUM + my_motor.overflowNum *
                                  RELOADVALUE;//һ�������ڵ��ܼ���ֵ����Ŀǰ����ֵ��������ļ���ֵ

        }
        else if (my_motor.totalCount - my_motor.lastCount > 19000)
        {
            my_motor.overflowNum--;
            my_motor.totalCount = COUNTERNUM + my_motor.overflowNum *
                                  RELOADVALUE;//һ�������ڵ��ܼ���ֵ����Ŀǰ����ֵ��������ļ���ֵ

        }

        my_motor.Speed = (float)(my_motor.totalCount - my_motor.lastCount) /
                         (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) *
                         10;//���ÿ�����ת,����4����Ϊ4��Ƶ
        my_motor.lastCount = my_motor.totalCount; //��¼��һ�εļ���ֵ

///////////////////////////////////////////////button
        get_key_state()->encode_Direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4);
        get_key_state()->totalCount = __HAL_TIM_GetCounter(&htim4) +
                                      get_key_state()->overnumber *
                                      __HAL_TIM_GetAutoreload(&htim4);
        if (get_key_state()->lastCount - get_key_state()->totalCount > 9900)
        {
            get_key_state()->overnumber++;
            get_key_state()->totalCount = __HAL_TIM_GetCounter(&htim4) +
                                          get_key_state()->overnumber *
                                          __HAL_TIM_GetAutoreload(&htim4);
        }
        else if (get_key_state()->totalCount - get_key_state()->lastCount > 9900)
        {
            get_key_state()->overnumber--;
            get_key_state()->totalCount = __HAL_TIM_GetCounter(&htim4) +
                                          get_key_state()->overnumber *
                                          __HAL_TIM_GetAutoreload(&htim4);

        }
        if (get_key_state()->encode_Direction != get_key_state()->encode_last_Direction)
        {
            if (get_key_state()->encode_cnt == 0)
            {
                get_key_state()->encode_nodivcnt = 0;


            }
            get_key_state()->encode_Direction = get_key_state()->encode_last_Direction;

        }


        if (get_key_state()->lastCount != get_key_state()->totalCount)
        {

            get_key_state()->detal_cnt = (get_key_state()->totalCount -
                                          get_key_state()->lastCount) ;
            get_key_state()->encode_nodivcnt = get_key_state()->encode_nodivcnt +
                                               get_key_state()->detal_cnt;
            get_key_state()->encode_cnt =   get_key_state()->encode_nodivcnt / 4;

            if (get_key_state()->encode_cnt <= 0)
                get_key_state()->encode_cnt = 0;
            get_key_state()->lastCount = get_key_state()->totalCount;

        }




    }
}

void motor_ctrl(void)
{
    static uint32_t tick;
    if (getMotr()->current >= MAX_CURR)
    {

    }
    else
    {

        if (getMotr()->Status == RUN)
        {
            HAL_TIM_Base_Start(&htim6);
            HAL_TIM_Base_Start_IT(&htim6);
            if ((HAL_GetTick() - tick) >= PID_CAL_TIME)
            {

                getMotr()->Duty = PID_realize(getMotr()->Set_Speed);
                Change_PWM_Duty(getMotr()->Duty);//ctrl pulse width
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
            Motor_Stop();
        }
    }


}
void motor_ctrl_proc(void)
{
    Current_Sense_Proc();
    //motor_ctrl();
    Motor_Stop();

}

