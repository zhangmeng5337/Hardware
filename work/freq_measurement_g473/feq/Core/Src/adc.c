#include "adc.h"

adc_stru adc_usr;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
/********************************************************
功能：控制激光发射二极管的开关，1--->开，0-->关
*********************************************************/
void laser_ctrl(unsigned char ctrl)
{
    if (ctrl == 1)
        HAL_GPIO_WritePin(laser_en_GPIO_Port, laser_en_Pin, GPIO_PIN_SET);
    else if (ctrl == 0)
        HAL_GPIO_WritePin(laser_en_GPIO_Port, laser_en_Pin, GPIO_PIN_RESET);

}
void adc_ctrl(unsigned char ctrl)
{
    adc_usr.adc_smple_freq = 4000;//4K
    //adc_usr.index = 0;
    adc_usr.index_tx = 0;
    adc_usr.real_index = 0;
    if (ctrl == 1)
    {
        laser_ctrl(1);

        HAL_Delay(2000);
        HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
        adc_usr.count = 0;
        // adc_usr.tick_start = 1;
        get_adc_data()->sum_count = 0;
        get_adc_data()->tx_state = 1;
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_usr.adc_voltage, ADC_BUF_SIZE);
        HAL_ADC_Start(&hadc1);
       // HAL_TIM_Base_Start_IT(&htim2);
       // HAL_TIM_Base_Start(&htim2);
    }
    else
    {

        laser_ctrl(0);
        HAL_TIM_Base_Stop_IT(&htim2);
        HAL_TIM_Base_Stop(&htim2);
        HAL_ADC_Stop(&hadc1);
        //HAL_ADCEx_Calibration_Start(&hadc1);
        HAL_ADC_Stop_DMA(&hadc1);
    }

}

void adc_copy()
{
    //  if (adc_usr.adc_sample[1] <= 1835 && adc_usr.adc_sample[1] >= 1238
    //    && adc_usr.conv_flag == 1)

    if (adc_usr.conv_flag == 1)
    {
        adc_usr.conv_flag = 0;
        if (adc_usr.update == 0)
        {

            adc_usr.real_index = adc_usr.real_index + ADC_BUF_SIZE;
			adc_usr.tx_flag = 1;
            if (adc_usr.index_tx >= (ADC_BUF_SIZE))
            {
                {
                    adc_usr.index_tx = 0;
					adc_usr.tx_flag = 0;
                }
            }
            if (adc_usr.real_index >= (ADC_BUF_SIZE))
            {
                {
                    adc_usr.real_index = 0;
                }
            }
        }
    }
}
adc_stru *get_adc_data()
{
    return &adc_usr;
}

