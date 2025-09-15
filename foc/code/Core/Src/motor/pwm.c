#include "pwm.h"
#include "global_def.h"
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim1;
void timer_pwm_init()
{
      HAL_TIM_Base_Start_IT(&htim3);
	 HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	 HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	 HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);

}
void pwm_ctrl(unsigned char mode)
{
    if(mode == START)
    {
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	 HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	 HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

	}
	else
	{
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	 HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	 HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);

	}


}
void set_pwm_duty(float d_u, float d_v, float d_w)
{
  d_u = min(d_u, 0.9);
  d_v = min(d_v, 0.9);
  d_w = min(d_w, 0.9);
  __disable_irq();
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, d_u * htim1.Instance->ARR);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, d_v * htim1.Instance->ARR);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, d_w * htim1.Instance->ARR);
  __enable_irq();
}
/* USER CODE END 0 */

