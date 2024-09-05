#include "di.h"
#include "hall.h"
#include "motor.h"

di_stru di_usr;
uint32_t Duty;
void di_init(void)
{
    di_usr.update = 0;
    di_usr.status = 0;

}
void do_init(void)
{
    HAL_GPIO_WritePin(DO_ctrl_0_GPIO_Port, DO_ctrl_0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DO_ctrl_1_GPIO_Port, DO_ctrl_1_Pin, GPIO_PIN_SET);
}
void do_ctrl(unsigned char relay_num)
{
    if (relay_num == 1)
        HAL_GPIO_WritePin(DO_ctrl_0_GPIO_Port, DO_ctrl_0_Pin, GPIO_PIN_RESET);
    if (relay_num == 2)
        HAL_GPIO_WritePin(DO_ctrl_1_GPIO_Port, DO_ctrl_1_Pin, GPIO_PIN_RESET);
}
di_stru di_input_read()
{
    GPIO_PinState status;
    status =  HAL_GPIO_ReadPin(DI_IN1_GPIO_Port, DI_IN1_Pin)&
              HAL_GPIO_ReadPin(DI_IN2_GPIO_Port, DI_IN2_Pin)&
              HAL_GPIO_ReadPin(DI_IN3_GPIO_Port, DI_IN3_Pin)&
              HAL_GPIO_ReadPin(DI_IN4_GPIO_Port, DI_IN4_Pin)&
              HAL_GPIO_ReadPin(DI_IN5_GPIO_Port, DI_IN5_Pin)&
              HAL_GPIO_ReadPin(DI_IN6_GPIO_Port, DI_IN6_Pin);

    if (status == 0)
    {
        if (HAL_GPIO_ReadPin(DI_IN1_GPIO_Port, DI_IN1_Pin) == 0)
        {
            di_usr.update = di_usr.update | 0x01;
            di_usr.status  = di_usr.status | 0x01;

        }
        else
        {
            di_usr.update = di_usr.update & (~0x01);
            di_usr.status  = di_usr.status & (~0x01);

        }

        if (HAL_GPIO_ReadPin(DI_IN2_GPIO_Port, DI_IN2_Pin) == 0)
        {
            di_usr.update = di_usr.update | 0x02;
            di_usr.status  = di_usr.status | 0x02;

        }
        else
        {
            di_usr.update = di_usr.update & (~0x02);
            di_usr.status  = di_usr.status & (~0x02);

        }

        if (HAL_GPIO_ReadPin(DI_IN3_GPIO_Port, DI_IN3_Pin) == 0)
        {
            di_usr.update = di_usr.update | 0x04;
            di_usr.status  = di_usr.status | 0x04;

        }
        else
        {
            di_usr.update = di_usr.update & (~0x04);
            di_usr.status  = di_usr.status & (~0x04);

        }

        if (HAL_GPIO_ReadPin(DI_IN4_GPIO_Port, DI_IN4_Pin) == 0)
        {
            di_usr.update = di_usr.update | 0x08;
            di_usr.status  = di_usr.status | 0x08;

        }
        else
        {
            di_usr.update = di_usr.update & (~0x08);
            di_usr.status  = di_usr.status & (~0x08);

        }

        if (HAL_GPIO_ReadPin(DI_IN5_GPIO_Port, DI_IN5_Pin) == 0)
        {
            di_usr.update = di_usr.update | 0x10;
            di_usr.status  = di_usr.status | 0x10;

        }
        else
        {
            di_usr.update = di_usr.update & (~0x10);
            di_usr.status  = di_usr.status & (~0x10);

        }

        if (HAL_GPIO_ReadPin(DI_IN6_GPIO_Port, DI_IN6_Pin) == 0)
        {
            di_usr.update = di_usr.update | 0x20;
            di_usr.status  = di_usr.status | 0x20;

        }
        else
        {
            di_usr.update = di_usr.update & (~0x20);
            di_usr.status  = di_usr.status & (~0x20);

        }

        if (HAL_GPIO_ReadPin(DI_IN1_GPIO_Port, DI_IN1_Pin) == 0)
        {
            di_usr.update = di_usr.update | 0x40;
            di_usr.status  = di_usr.status | 0x40;

        }
        else
        {
            di_usr.update = di_usr.update & (~0x40);
            di_usr.status  = di_usr.status & (~0x40);

        }

    }

}

di_stru *get_di_status(void)
{
    return &di_usr;
}
void pulse_exti(void)
{
    di_usr.pulse_cnt++;
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_9)
    {
        pulse_exti();
    }
	if(getMotr()->Status == RUN)
	{
		/*1.修改三个通道的占空比*/
		getMotr()->Duty = Duty;//将菜单中占空比赋值过来，降低程序耦合性
		Change_PWM_Duty(getMotr()->Duty);
		/*2.开始获取HALL值*/
		//user_main_info("HALL的值为：%d",get_hall_state());
		getMotr()->Hall_status = get_hall_state();
		/*3.下面开启6步换相*///--->依据HALL值进行换向
		//上管调制，下管驱动
		Six_Step_Phase(getMotr()->Hall_status,getMotr()->Direction);
	}


}



