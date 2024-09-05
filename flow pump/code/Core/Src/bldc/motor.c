#include "motor.h"
extern TIM_HandleTypeDef htim1;
/*
PWM频率计算：
频率 = 定时器时钟 / （Prescaler 预分频 + 1）/ （Counter Period 计数值 + 1）Hz
本项目中12KHz
占空比 = Pulse ( 对比值) / （C ounter Period 计数值）%

*/
/*开始声明变量*/

/*定义一个电机的结构体*/
MOTOR my_motor={1,0,20,0,0};//定义一个电机结构体


/*六步换相程序*/
void Six_Step_Phase(uint8_t hall_state,uint8_t direction)
{
    if(direction!=0)//电机正转
    {
        switch (hall_state)
        {
            case 1:
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);//U+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//W+
                UL_OFF;VL_OFF;WL_ON;//下管
                break;
            case 2:
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);//U+
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//W+
                UL_ON;VL_OFF;WL_OFF;//下管
            break;
            case 3:
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);//U+
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//W+
                UL_OFF;VL_OFF;WL_ON;//下管
            break;
            case 4:
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);//U+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);//W+
                UL_OFF;VL_ON;WL_OFF;//下管
            break;
            case 5:
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);//U+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//W+
                UL_OFF;VL_ON;WL_OFF;//下管
            break;
            case 6:
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);//U+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);//W+
                UL_ON;VL_OFF;WL_OFF;//下管
            break;
            
            default:
                Motor_Stop();//电机停转
                break;
        }
    }else//电机反转
    {
        switch (hall_state)
        {
            case 1:
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//U+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);//W+
                UL_ON;VL_OFF;WL_OFF;//下管
                break;
            case 2:
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);//U+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);//W+
                UL_OFF;VL_ON;WL_OFF;//下管
            break;
            case 3:
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//U+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);//W+
                UL_OFF;VL_ON;WL_OFF;//下管
            break;
            case 4:
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//U+
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);//W+
                UL_OFF;VL_OFF;WL_ON;//下管
            break;
            case 5:
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//U+
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);//W+
                UL_ON;VL_OFF;WL_OFF;//下管
            break;
            case 6:
                HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);//U+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);//V+
                HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//W+
                UL_OFF;VL_OFF;WL_ON;//下管
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
    duty = (duty*PWM_COUNTER)/100;
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
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);//U+
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);//V+
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);//W+
    //2.关闭所有下管
    UL_OFF;VL_OFF;WL_OFF;//下管
    my_motor.Status = HALT;
}

/*电机启动下，首次获取转子位置，然后换向，触发中断*/
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


