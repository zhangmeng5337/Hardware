#include "adc.h"

#include "filter.h"
//#include "config.h"

extern ADC_HandleTypeDef hadc1;
extern  DMA_HandleTypeDef hdma_adc1;
adc_stru sensor_adc_value;

void adc_init(void)
{
    //HAL_Delay(3000);
    //HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)sensor_adc_value.adc_value,
                      4); //  开启DMA
    HAL_ADC_Start(&hadc1);//  启动ADC转换


}
float sensor_temp;


void adc_proc()
{
    float adc_conv_tmp, tmp;
    unsigned char i;
    static int j = 0;
    static float buf[4][N];
    static uint32_t tick_start;
    if ((HAL_GetTick() - tick_start) >= ADC_POLL_T)
    {
        tick_start = HAL_GetTick();
        if (sensor_adc_value.update == 1)
        {
            sensor_adc_value.update = 0;


            for (i = 0; i < 4; i++)
            {
                float x2, x1, xb;
                sensor_adc_value.ref = 4095 * 1.2 / sensor_adc_value.adc_value[4];
                adc_conv_tmp = sensor_adc_value.adc_value[i] * 3.0;
                adc_conv_tmp = adc_conv_tmp / 4095;

                    x2 = adc_conv_tmp / R1_V ;//10---factor
                    x2 = x2 + V2_V; //v1  0.064 bridge voltage
                    x1 = R2_V * x2;
					sensor_adc_value.v_value[i] = x2;
                    x2 = (PT_VREF - x2);
                    x1 = x1 / x2;
                    adc_conv_tmp =  x1 * 1000;

                buf[i][j] = adc_conv_tmp;
                if (j >= N)
                {
                    j = 0;
                    tmp = filter(0, &buf[0][0]); //filter
                    if (tmp != -1)
                        sensor_adc_value.R_value[0] = tmp;
                    tmp = filter(1, &buf[1][0]);
                    if (tmp != -1)
                        sensor_adc_value.R_value[1] = tmp;
                    tmp = filter(2, &buf[2][0]);
                    if (tmp != -1)
                        sensor_adc_value.R_value[2] = tmp;
                    tmp = filter(3, &buf[3][0]);
                    if (tmp != -1)
                        sensor_adc_value.R_value[3] = tmp;
                }
                else
                {
                    sensor_adc_value.R_value[i] = adc_conv_tmp;

                }
                    sensor_adc_value.T_value[0] = PT100_Temp(sensor_adc_value.R_value[0]) * 0.5 -
                                                  60;
                    sensor_adc_value.T_value[1] = PT100_Temp(sensor_adc_value.R_value[1]) * 0.5 -
                                                  60;
                    sensor_adc_value.T_value[2] = PT100_Temp(sensor_adc_value.R_value[2]) * 0.5 -
                                                  60;
                    sensor_adc_value.T_value[3] = PT100_Temp(sensor_adc_value.R_value[3]) * 0.5 -
                                                  60;

            }
            j++;
            HAL_ADC_Start_DMA(&hadc1, (uint32_t *)sensor_adc_value.adc_value,
                              5); //  开启DMA
        }
        //printf("%0.2f\r\n",sensor_adc_value.T_value[3]);
    }




}
adc_stru *get_temperature(void)
{
    return &sensor_adc_value;
}



