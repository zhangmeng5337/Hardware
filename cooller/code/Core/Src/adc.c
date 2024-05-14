#include "adc.h"

#include "filter.h"
#include "config.h"
#include "stmflash.h"

extern ADC_HandleTypeDef hadc1;
extern  DMA_HandleTypeDef hdma_adc1;
adc_stru sensor_adc_value;

void adc_init(void)
{

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)sensor_adc_value.adc_value,
                      4); //  ¿ªÆôDMA
    HAL_ADC_Start(&hadc1);//  Æô¶¯ADC×ª»»


}
float sensor_temp;

float adc_pt1_v, adc_pt2_v, pt_r;
float x2, x1, xb;
void adc_proc()
{
    float tmp;
    unsigned char i;
    static int j = 0;
    static float buf[4][N];
    static uint32_t tick_start;
    if ((HAL_GetTick() - tick_start) >= ADC_POLL_T)
    {
        tick_start = HAL_GetTick();
        getConfig()->record_time = getConfig()->record_time + 1;
        if (getConfig()->record_time == getConfig()->record_interval * 10)
        {
            getConfig()->record_time = 0;
            getConfig()->update_T = 1;
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
                                              sensor_adc_value.ref/4095;
                sensor_adc_value.T_value[1] = sensor_adc_value.T_value[1] *11;
                sensor_adc_value.T_value[1] = sensor_adc_value.T_value[1] / BATTERY_V;
                x2 = 2 * adc_pt1_v;
                x1 = sensor_adc_value.ref - adc_pt1_v;
                //x1=x1-adc_pt1_v;
                x2 = x2 / x1; //v1  0.064 bridge voltage

                sensor_adc_value.v_value[i] = adc_pt1_v ;
                pt_r =  x2 * 1000;
                sensor_adc_value.R_value[0] = average_filter(x2);
                sensor_adc_value.T_value[0] = PT100_Temp(sensor_adc_value.R_value[0]) * 0.5 -
					                          60;

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


