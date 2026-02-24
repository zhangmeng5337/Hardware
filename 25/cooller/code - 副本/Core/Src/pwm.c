#include "pwm.h"
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
pwm_stru pwm_usr;

void pwm_init()
{
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);  /* COOLER_1 */
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);  /* COOLER_2 */
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);  /* COOLER_3 */
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);  /* COOLER_4 */

    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);  /* HEATER_1 */
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);  /* HEATER_2 */
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);  /* HEATER_3 */
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);  /* HEATER_4 */
//  HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);  /* HEATER_1 */
    pwm_usr.air_freq = 0;

}
void air_pwm_set(uint32_t freq)
{
 pwm_usr.air_freq = freq;
    if (pwm_usr.air_freq <= 10000)
    {
        if (pwm_usr.air_freq > 0)
        {
            HAL_TIM_Base_Start_IT(&htim4);
            HAL_TIM_Base_Start(&htim4);
            pwm_usr.air_counter = TIM_BASE/pwm_usr.air_freq/2 -1 ;
			__HAL_TIM_SET_AUTORELOAD(&htim4,pwm_usr.air_counter); 
		
        }
        else
        {
            pwm_usr.air_counter = 0;
            HAL_TIM_Base_Stop_IT(&htim4);
            HAL_TIM_Base_Stop(&htim4);
        }
    }
}
void air_pwm_out()
{

    static uint32_t start_tick = 0;
    if (pwm_usr.air_freq <= 10000)
    {

        if (pwm_usr.air_freq == 0)
        {
            HAL_GPIO_WritePin(air_FREQ_GPIO_Port, air_FREQ_Pin, GPIO_PIN_RESET);

        }
        else
        {
            start_tick = start_tick + 1;
            //if (start_tick >= pwm_usr.air_counter)
            {
                start_tick = 0;
                HAL_GPIO_TogglePin(air_FREQ_GPIO_Port, air_FREQ_Pin);
               // __HAL_TIM_SET_COUNTER(&htim4,2999);
               
            }

        }
    }

}

void pwm_set(unsigned char pwm_num, uint32_t counter)
{
    switch (pwm_num)
    {
        case COOLER_1:
            HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, counter);
            break;
        case COOLER_2:
            HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, counter);
            break;
        case COOLER_3:
            HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, counter);
            break;

        case COOLER_4:
            HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, counter);
            break;
        case HEATER_1:
            HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, counter);
            break;
        case HEATER_2:
            HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, counter);
            break;
        case HEATER_3:
            HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, counter);
            break;

        case HEATER_4:
            HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, counter);
            break;
        default :

            break;



    }
}
