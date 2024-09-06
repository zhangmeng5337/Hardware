#include "motor.h"
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim6;

/*
PWM频率计算：
频率 = 定时器时钟 / （Prescaler 预分频 + 1）/ （Counter Period 计数值 + 1）Hz
本项目中12KHz
占空比 = Pulse ( 对比值) / （C ounter Period 计数值）%

*/
/*开始声明变量*/

/*定义一个电机的结构体*/
MOTOR my_motor = {1, 0, 20, 0, 0}; //定义一个电机结构体

void Motor_Init(void)
{
//    HAL_TIM_Encoder_Start(&ENCODER_TIM, TIM_CHANNEL_ALL);      //开启编码器定时器
//    __HAL_TIM_ENABLE_IT(&ENCODER_TIM,TIM_IT_UPDATE);           //开启编码器定时器更新中断,防溢出处理
//    HAL_TIM_Base_Start_IT(&GAP_TIM);                       //开启100ms定时器中断
//    HAL_TIM_PWM_Start(&PWM_TIM, TIM_CHANNEL_2);            //开启PWM
//    HAL_TIM_PWM_Start(&PWM_TIM, TIM_CHANNEL_1);            //开启PWM
//    __HAL_TIM_SET_COUNTER(&htim2,
   // 10000);                //编码器定时器初始值设定为10000
    my_motor.lastCount = 0;                                   //结构体内容初始化
    my_motor.totalCount = 0;
    my_motor.overflowNum = 0;
    my_motor.Speed = 0;
    my_motor.Direction = 0;
}


/*六步换相程序*/
void Six_Step_Phase(uint8_t hall_state, uint8_t direction)
{
    if (direction != 0) //电机正转
    {
        switch (hall_state)
        {
            case 1:
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); //U+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //W+
                UL_OFF;
                VL_OFF;
                WL_ON;//下管
                break;
            case 2:
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3); //U+
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //W+
                UL_ON;
                VL_OFF;
                WL_OFF;//下管
                break;
            case 3:
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3); //U+
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //W+
                UL_OFF;
                VL_OFF;
                WL_ON;//下管
                break;
            case 4:
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3); //U+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //W+
                UL_OFF;
                VL_ON;
                WL_OFF;//下管
                break;
            case 5:
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); //U+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //W+
                UL_OFF;
                VL_ON;
                WL_OFF;//下管
                break;
            case 6:
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3); //U+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //W+
                UL_ON;
                VL_OFF;
                WL_OFF;//下管
                break;

            default:
                Motor_Stop();//电机停转
                break;
        }
    }
    else //电机反转
    {
        switch (hall_state)
        {
            case 1:
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //U+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); //W+
                UL_ON;
                VL_OFF;
                WL_OFF;//下管
                break;
            case 2:
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //U+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3); //W+
                UL_OFF;
                VL_ON;
                WL_OFF;//下管
                break;
            case 3:
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //U+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); //W+
                UL_OFF;
                VL_ON;
                WL_OFF;//下管
                break;
            case 4:
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //U+
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3); //W+
                UL_OFF;
                VL_OFF;
                WL_ON;//下管
                break;
            case 5:
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //U+
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3); //W+
                UL_ON;
                VL_OFF;
                WL_OFF;//下管
                break;
            case 6:
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); //U+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); //V+
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //W+
                UL_OFF;
                VL_OFF;
                WL_ON;//下管
                break;

            default:
                Motor_Stop();//电机停转
                break;
        }
    }


}

/*
更改三个通道的PWM占空比
输入:uint8_t duty:占空比数值0-100;
*/
void Change_PWM_Duty(uint16_t duty)
{
    //1.开始占空比的映射;
    duty = (duty * PWM_COUNTER) / 100;
    //2.开始修改占空比
//    TIM1->CCR1 = duty;
//    TIM1->CCR2 = duty;
//    TIM1->CCR3 = duty;

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, duty);  // U+
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, duty);  // V+
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, duty);  // W+

}

/*电机停转*/
void Motor_Stop()
{

    //1.关闭PWM输出
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //U+
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); //V+
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3); //W+
    //2.关闭所有下管
    UL_OFF;
    VL_OFF;
    WL_OFF;//下管
    my_motor.Status = HALT;
}

/*电机启动下，首次获取转子位置，然后换向，触发中断*/
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
 * 进行速度的平均滤波
 * 输入新采样到的速度，存放速度的数组，
 * 返回滤波后的速度
 */
float Speed_Low_Filter(float new_Spe, float *speed_Record)
{
    float sum = 0.0f;
    for (uint8_t i = SPEED_RECORD_NUM - 1; i > 0; i--) //将现有数据后移一位
    {
        speed_Record[i] = speed_Record[i - 1];
        sum += speed_Record[i - 1];
    }
    speed_Record[0] = new_Spe;//第一位是新的数据
    sum += new_Spe;
    my_motor.Speed_Filter = sum / SPEED_RECORD_NUM;
    return sum / SPEED_RECORD_NUM; //返回均值
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef
                                   *htim)//定时器回调函数，用于计算速度
{
//    if(htim->Instance==htim2.Instance)//编码器输入定时器溢出中断，用于防溢出
//    {
//        if(COUNTERNUM < 10000) my_motor.overflowNum++;       //如果是向上溢出
//        else if(COUNTERNUM >= 10000) my_motor.overflowNum--; //如果是向下溢出
//        __HAL_TIM_SetCounter(&htim2, 10000);             //重新设定初始值
//    }
//    else if(htim->Instance==htim6)//间隔定时器中断，是时候计算速度了
//    {
//        my_motor.Direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2);//如果向上计数（正转），返回值为0，否则返回值为1
//        my_motor.totalCount = COUNTERNUM + my_motor.overflowNum * RELOADVALUE;//一个周期内的总计数值等于目前计数值加上溢出的计数值
//        my_motor.Speed = (float)(my_motor.totalCount - my_motor.lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10;//算得每秒多少转
//        //motor1.speed = (float)(motor1.totalCount - motor1.lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10 * LINE_SPEED_C//算得车轮线速度每秒多少毫米
//        my_motor.lastCount = my_motor.totalCount; //记录这一次的计数值
//    }
    if (htim->Instance == htim6.Instance) //编码器输入定时器溢出中断，用于防溢出
    {
        my_motor.Direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(
                                 &htim2);//如果向上计数（正转），返回值为0，否则返回值为1
        my_motor.totalCount = COUNTERNUM + my_motor.overflowNum *
                              RELOADVALUE;//一个周期内的总计数值等于目前计数值加上溢出的计数值

        if (my_motor.lastCount - my_motor.totalCount >
                19000) // 在计数值溢出时进行防溢出处理
        {
            my_motor.overflowNum++;
            my_motor.totalCount = COUNTERNUM + my_motor.overflowNum *
                                  RELOADVALUE;//一个周期内的总计数值等于目前计数值加上溢出的计数值
        }
        else if (my_motor.totalCount - my_motor.lastCount >
                 19000) // 在计数值溢出时进行防溢出处理
        {
            my_motor.overflowNum--;
            my_motor.totalCount = COUNTERNUM + my_motor.overflowNum *
                                  RELOADVALUE;//一个周期内的总计数值等于目前计数值加上溢出的计数值
        }

        my_motor.Speed = (float)(my_motor.totalCount - my_motor.lastCount) /
                         (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) *
                         10;//算得每秒多少转,除以4是因为4倍频
        my_motor.lastCount = my_motor.totalCount; //记录这一次的计数值

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

