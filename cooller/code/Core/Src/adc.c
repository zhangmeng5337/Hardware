#include "adc.h"
#include "adc.h"
#include "filter.h"
#include "config.h"
#include "stmflash.h"
#include "flash.h"
extern ADC_HandleTypeDef hadc1;
extern  DMA_HandleTypeDef hdma_adc1;
adc_stru sensor_adc_value;

void adc_init(void)
{

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)sensor_adc_value.adc_value,
                      4); //  ¿ªÆôDMA
    HAL_ADC_Start(&hadc1);//  Æô¶¯ADC×ª»»
    sensor_adc_value.valid = 0;

}


float adc_pt1_v, adc_pt2_v, pt_r;
void adc_proc()
{
    float sensor_temp;

    float x2, x1, xb;
    float tmp;
    unsigned char i;
    static int j = 0;
    static float buf[4][N];
    static uint32_t tick_start;
    static float time_cnt = 0;
    if ((HAL_GetTick() - tick_start) >= ADC_POLL_T)
    {
        tick_start = HAL_GetTick();
        time_cnt = time_cnt + ADC_POLL_T / 1000.0;
        if (time_cnt >= 60000)
        {
            getConfig()->record_time = getConfig()->record_time + time_cnt / 60;
            time_cnt = 0;

        }
        getConfig()->interval_tick = getConfig()->interval_tick + ADC_POLL_T / 1000.0;

        if (getConfig()->interval_tick >= getConfig()->record_interval)
        {
            // getConfig()->record_time = 0;
            getConfig()->update_T = 1;
	            getConfig()->update_fig = 1;		
            getConfig()->interval_tick = 0;
            //data_flash_proc();
        }
        if (sensor_adc_value.update == 1)
        {
            sensor_adc_value.update = 0;


            for (i = 0; i < 1; i++)
            {

                sensor_adc_value.ref = 4096 * 1.2 / sensor_adc_value.adc_value[3];
                //sensor_adc_value.ref = 3.295;
                adc_pt1_v = sensor_adc_value.adc_value[0] * sensor_adc_value.ref;
                adc_pt1_v = adc_pt1_v / 4095;
                adc_pt2_v = sensor_adc_value.adc_value[1] * sensor_adc_value.ref;
                adc_pt2_v = adc_pt2_v / 4095;
                sensor_adc_value.T_value[1] = sensor_adc_value.adc_value[2] *
                                              sensor_adc_value.ref / 4095;
                sensor_adc_value.T_value[1] = sensor_adc_value.T_value[1] * 11;
                sensor_adc_value.T_value[1] = sensor_adc_value.T_value[1] / BATTERY_V+1;
                x2 = 2 * adc_pt1_v;
                x1 = sensor_adc_value.ref - adc_pt1_v;
                //x1=x1-adc_pt1_v;
                x2 = x2 / x1; //v1  0.064 bridge voltage

                sensor_adc_value.v_value[i] = adc_pt1_v ;
                pt_r =  x2 * 1000;
                //sensor_adc_value.R_value[0] = average_filter(x2);
                static unsigned int index = 0;
                static unsigned char flag;
                sensor_adc_value.R_value[0] = pt_r;
                sensor_adc_value.M_value[index] = sensor_adc_value.R_value[0];
                index = index + 1;
                if (index >= 5)
                {
                   sensor_adc_value.valid = 1;
                    if (flag == 0)
                        sensor_adc_value.R_value[0] = GetMedianNum(sensor_adc_value.M_value, index);
                    else
                        sensor_adc_value.R_value[0] = GetMedianNum(sensor_adc_value.M_value, N);
                    if (index >= N)
                    {
                        flag = 1;
                        index = 0;
                    }
                    float result;

                    result = PT100_Temp(sensor_adc_value.R_value[0]) * 0.5 -
                             60;
                    sensor_adc_value.T_value[0] = average_filter(result)-2;
                }
				


            }
        }

    }
}
adc_stru *get_temperature(void)
{
    return &sensor_adc_value;
}

void set_adc_update()
{
    sensor_adc_value.update = 1;

}


