/**
  ******************************************************************************
  * @file    bsp_bldcm_control.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   无刷电机控制接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

#include "bldc_ctronl.h"
#include "pid.h"
#include "bsp_encoder.h"
extern TIM_HandleTypeDef htim1;
/* 私有变量 */
static bldcm_data_t bldcm_data;

/* 局部函数 */
static void sd_gpio_config(void);
void set_bldcm_enable(void)
{
    bldcm_data.is_enable = 1;
    //  hall_enable();
}
void stop_pwm_output(void)
{
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); //?? PWM ??
    BLDCM_DISABLE_SD();
    BLDCM_DISABLE_BREAK();
}
void set_pwm_pulse(uint16_t pulse)
{
    /* 设置定时器通道输出 PWM 的占空比 */
    bldcm_data.bldcm_pulse = pulse;
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //?? PWM ??
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, bldcm_data.bldcm_pulse);
    BLDCM_ENABLE_SD();
    BLDCM_DISABLE_BREAK();
}
/**
  * @brief  禁用电机
  * @param  无
  * @retval 无
  */
void set_bldcm_disable(void)
{
    /* 禁用霍尔传感器接口 */
    // hall_disable();

    /* 停止 PWM 输出 */
    stop_pwm_output();
    //__HAL_TIM_SET_COMPARE(&htimx_bldcm, TIM_CHANNEL_3, 0);
    bldcm_data.is_enable = 0;
}

/**
  * @brief  电机初始化
  * @param  无
  * @retval 无
  */
void bldcm_init(void)
{
    // PWM_TIMx_Configuration();    // 电机控制定时器，引脚初始化
    //hall_tim_config();           // 霍尔传感器初始化
    bldcm_data.arr =  SystemCoreClock / (TIM1PS + 1) / bldcm_data.freq;
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    __HAL_TIM_SET_AUTORELOAD(&htim1, bldcm_data.arr);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, bldcm_data.arr / 2);
    sd_gpio_config();        // sd 引脚初始化
}

/**
  * @brief  电机 SD 控制引脚初始化
  * @param  无
  * @retval 无
  */
static void sd_gpio_config(void)
{

    BLDCM_DISABLE_SD();
    BLDCM_DISABLE_BREAK();
    HAL_Delay(1);
}

/**
  * @brief  设置电机速度
  * @param  v: 速度（占空比）
  * @retval 无
  */
void set_bldcm_speed(uint16_t v)
{
    bldcm_data.dutyfactor = v;

    set_pwm_pulse(v);     // 设置速度
}
void setMotorSpeed(float speed)
{
	set_pid_target(speed);
}
/**
  * @brief  设置电机方向
  * @param  无
  * @retval 无
  */
void set_bldcm_direction(motor_dir_t dir)
{
    bldcm_data.direction = dir;
}

/**
  * @brief  获取电机当前方向
  * @param  无
  * @retval 无
  */
motor_dir_t get_bldcm_direction(void)
{
    return bldcm_data.direction;
}

/**
  * @brief  使能电机
  * @param  无
  * @retval 无
  */

/**
  * @brief  电机位置式 PID 控制实现(定时调用)
  * @param  无
  * @retval 无
  */
void bldcm_pid_control(void)
{
    float speed_actual = get_encoder()->speed;   // 电机旋转的当前速度

    if (bldcm_data.is_enable)
    {
        float cont_val = 0;    // 当前控制值

        cont_val = PID_realize(speed_actual);

        if (cont_val < 0)
        {
            cont_val = -cont_val;
            bldcm_data.direction = MOTOR_REV;
        }
        else
        {
            bldcm_data.direction = MOTOR_FWD;
        }

        cont_val = (cont_val > bldcm_data.arr) ? bldcm_data.arr : cont_val;  // 上限处理

        set_bldcm_speed(cont_val);

    }
	else
	{
		stop_pwm_output();
	}
}


///**
//  * @brief  定时器每100ms产生一次中断回调函数
//  * @param  htim：定时器句柄
//  * @retval 无
//  */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//    if(htim==(&TIM_TimeBaseStructure))
//    {
//        bldcm_pid_control();
//    }
//}

/*********************************************END OF FILE**********************/
