#include "hall.h"
#include "motor.h"

extern uint8_t Duty;

extern MOTOR my_motor;


/*HALL中断服务函数*/
/*在HALL值发生变化的时候会触发这个中断*/
/*上升沿和下降沿触发*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    //static uint8_t HALL_state; 
    LED0_ON;
    #if 1//条件编译，调试用
        if(my_motor.Status == RUN)
        {
            /*1.修改三个通道的占空比*/
            my_motor.Duty = Duty;//将菜单中占空比赋值过来，降低程序耦合性
            Change_PWM_Duty(my_motor.Duty);
            /*2.开始获取HALL值*/
            //user_main_info("HALL的值为：%d",get_hall_state());
            my_motor.Hall_status = get_hall_state();
            /*3.下面开启6步换相*///--->依据HALL值进行换向
            //上管调制，下管驱动
            Six_Step_Phase(my_motor.Hall_status,my_motor.Direction);
        }

    #endif
    LED0_OFF;

}


//获取霍尔传感器值
uint8_t get_hall_state(void)
{
    uint8_t state = 0; 
    uint8_t A,B,C;
    A = HAL_GPIO_ReadPin(HALLA_GPIO_Port, HALLA_Pin);
    B = HAL_GPIO_ReadPin(HALLB_GPIO_Port, HALLB_Pin);
    C = HAL_GPIO_ReadPin(HALLC_GPIO_Port, HALLC_Pin);
    state = (C << 2) + (B << 1) + A;
    return state;
}


