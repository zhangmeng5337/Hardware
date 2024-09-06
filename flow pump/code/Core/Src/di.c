#include "di.h"
#include "hall.h"
#include "motor.h"

di_stru di_usr;

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
//    if (GPIO_Pin == DI_IN0_Pin)
//    {
//        while(HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin)== 0)
//        {
//			HAL_Delay(20);
//			pulse_exti();//脉冲计数
//			break;
//
//		}
//         while(HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin)== 0)
//        {
//			HAL_Delay(20);
//			pulse_exti();//脉冲计数
//			break;
//
//		}       	
//    }

    if (GPIO_Pin == DI_IN0_Pin)
    {
        if(HAL_GPIO_ReadPin(DI_IN0_GPIO_Port, DI_IN0_Pin)== 0)
        	pulse_exti();//脉冲计数
    }
    if (GPIO_Pin == MHU_Pin || GPIO_Pin == MHV_Pin || GPIO_Pin == MHW_Pin)
    {
       if(HAL_GPIO_ReadPin(MHU_GPIO_Port, MHU_Pin)== 0||
	   	HAL_GPIO_ReadPin(MHV_GPIO_Port, MHV_Pin)== 0||
	   	HAL_GPIO_ReadPin(MHW_GPIO_Port, MHW_Pin)== 0)
        if (getMotr()->Status == RUN)
        {

            /*2.开始获取HALL值*/
            //user_main_info("HALL的值为：%d",get_hall_state());
            getMotr()->Hall_status = get_hall_state();
            /*3.下面开启6步换相*///--->依据HALL值进行换向
            //上管调制，下管驱动
            Six_Step_Phase(getMotr()->Hall_status, getMotr()->Direction);
        }
		else
		{
			Six_Step_Phase(7, getMotr()->Direction);//停机

		}

    }

}



