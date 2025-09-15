#include "motor_runtime_param.h"
#include "foc.h"
#include "kalman.h"
#include "global_def.h"
#include "arm_math.h"


extern ADC_HandleTypeDef hadc1;
motor_runtime_stru motor_runtime;


HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == IN1_Pin)  // 假设使用PC13引脚
	 {
	   /* 处理按键中断的逻辑 */

	   if(HAL_GPIO_ReadPin(IN1_GPIO_Port,IN1_Pin) == 0)
	   	{
	   		

	   }
	 }
	if (GPIO_Pin == IN2_Pin)  // 假设使用PC13引脚
	 {
	   /* 处理按键中断的逻辑 */

	   if(HAL_GPIO_ReadPin(IN2_GPIO_Port,IN2_Pin) == 0)
	   	{

	   }
	 }
	if (GPIO_Pin == IN3_Pin)  // 假设使用PC13引脚
	 {
	   /* 处理按键中断的逻辑 */

	   if(HAL_GPIO_ReadPin(IN3_GPIO_Port,IN3_Pin) == 0)
	   	{

	   }
	 }
	if (GPIO_Pin == IN4_Pin)  // 假设使用PC13引脚
	 {
	   /* 处理按键中断的逻辑 */

	   if(HAL_GPIO_ReadPin(IN4_GPIO_Port,IN4_Pin) == 0)
	   	{

	   }
	 }

}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        float u_1 = ADC_REFERENCE_VOLT * ((float)HAL_ADCEx_InjectedGetValue(&hadc1,
                                          ADC_INJECTED_RANK_1) / ((1 << ADC_BITS) - 1) - 0.5);
        float u_2 = ADC_REFERENCE_VOLT * ((float)HAL_ADCEx_InjectedGetValue(&hadc1,
                                          ADC_INJECTED_RANK_2) / ((1 << ADC_BITS) - 1) - 0.5);
        float u_3 = ADC_REFERENCE_VOLT * ((float)HAL_ADCEx_InjectedGetValue(&hadc1,
                                          ADC_INJECTED_RANK_3) / ((1 << ADC_BITS) - 1) - 0.5);

        u_1 = (u_1 - OP_GAIN * ADC_REFERENCE_VOLT) / OP_GAIN;
        u_2 = (u_2 - OP_GAIN * ADC_REFERENCE_VOLT) / OP_GAIN;
        u_3 = (u_3 - OP_GAIN * ADC_REFERENCE_VOLT) / OP_GAIN;

        float i_1 = u_1 / R_SHUNT  ;
        float i_2 = u_2 / R_SHUNT  ;
        float i_3 = u_3 / R_SHUNT  ;
        motor_runtime.motor_i_u = i_1;
        motor_runtime.motor_i_v = i_2;
        motor_runtime.motor_i_w = i_3;

    float i_alpha = 0;
    float i_beta = 0;
    arm_clarke_f32(motor_runtime.motor_i_u, motor_runtime.motor_i_v, &i_alpha, &i_beta);
    float sin_value = arm_sin_f32(motor_runtime.rotor_logic_angle);
    float cos_value = arm_cos_f32(motor_runtime.rotor_logic_angle);
    float _motor_i_d = 0;
    float _motor_i_q = 0;
    arm_park_f32(i_alpha, i_beta, &_motor_i_d, &_motor_i_q, sin_value, cos_value);
    float filter_alpha_i_d = 0.1;
    float filter_alpha_i_q = 0.1;
    motor_runtime.motor_i_d = kalman_filter (get_kalman_p(0), motor_runtime.motor_i_d);
    motor_runtime.motor_i_q =  kalman_filter (get_kalman_p(1), motor_runtime.motor_i_q);

	switch (get_motor_control_state()->type)
		{
		case control_type_position:
		  lib_position_control(get_motor_control_state()->position);
		  break;
		case control_type_speed:
		  lib_speed_control(get_motor_control_state()->speed);
		  break;
		case control_type_torque:
		  lib_torque_control(get_motor_control_state()->torque_norm_d, get_motor_control_state()->torque_norm_q);
		  break;
		case control_type_position_speed_torque:
		  lib_position_speed_torque_control(get_motor_control_state()->position, get_motor_control_state()->max_speed, get_motor_control_state()->max_torque_norm);
		  break;
		default:
		  break;
		}

	
    }
}



void adc_init()
{
    HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED);
    HAL_ADCEx_InjectedStart_IT(&hadc1);

}
void motor_init()
{
	//main.c
	//......
	  set_pwm_duty(0.5, 0, 0);				// 生成SVPWM模型中的基础矢量1，即对应转子零度位置
	  HAL_Delay(400);						// 保持一会，转子吸引过来需要时间
	  motor_runtime.rotor_zero_angle = motor_runtime.encoder_angle;
	  motor_runtime.status = STOP;
	  set_pwm_duty(0, 0, 0);				// 松开电机
	  HAL_Delay(10);
	  /* USER CODE END 2 */
	//......

}

motor_runtime_stru *get_motor_runtime()
{
    return &motor_runtime;
}
void get_motor_phy_angele()
{
    motor_runtime.rotor_phy_angle =
        motor_runtime.encoder_angle - motor_runtime.rotor_zero_angle;	   // 转子物理角度
    motor_runtime.rotor_logic_angle  = motor_runtime.rotor_phy_angle * POLE_PAIRS;		  // 转子多圈角度

}

void motor_test()
{
    //main.c
    //......
    while (1)
    {
        //......
        /* USER CODE BEGIN 3 */
        for (unsigned int i = 0; i < 360; i += 20)
        {
            HAL_Delay(2);
            foc_forward(0, 0.5, deg2rad(i));
        }
        //......
        //main.c
        //......
        /* USER CODE BEGIN WHILE */
        set_pwm_duty(0.5, 0, 0);
        HAL_Delay(300);
        //printf("%f,%f,%f\n", motor_i_u, motor_i_v, -(motor_i_u + motor_i_v));

        set_pwm_duty(0, 0.5, 0);
        HAL_Delay(300);
       // printf("%f,%f,%f\n", motor_i_u, motor_i_v, -(motor_i_u + motor_i_v));

        set_pwm_duty(0, 0, 0.5);
        HAL_Delay(300);
       // printf("%f,%f,%f\n", motor_i_u, motor_i_v, -(motor_i_u + motor_i_v));
        set_pwm_duty(0, 0, 0);

    }
}

