//tim.c
#include "global_def.h"
#include "motor_runtime_param.h"
#include "foc.h"
#include "kalman.h"
#include "arm_math.h"
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3)
  {
    static float encoder_angle_last = 0;
    
    /******encoder_angle_last默认值是0，不能用于计算，需要赋初值*********/
    static int once = 1;
    if (once)
    {
      once = !once;
      encoder_angle_last = get_motor_runtime()->encoder_angle;
    }
    /***************/
    
    float diff_angle = cycle_diff(get_motor_runtime()->encoder_angle - encoder_angle_last, 2 * PI);
    encoder_angle_last = get_motor_runtime()->encoder_angle;
    get_motor_runtime()->motor_Ospeed  = diff_angle * motor_speed_calc_freq;
    get_motor_runtime()->motor_speed = kalman_filter(get_motor_runtime()->motor_Ospeed);
  }
}

