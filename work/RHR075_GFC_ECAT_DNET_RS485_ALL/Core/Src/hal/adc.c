#include "adc.h"
#include "table.h"
#include "filter.h"
#include "config.h"
#include "math.h"

extern ADC_HandleTypeDef hadc1;
extern  DMA_HandleTypeDef hdma_adc1;
adc_stru sensor_adc_value;

void adc_init(void)
{
    HAL_Delay(1000);
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)sensor_adc_value.adc_value,
                      5); //  开启DMA
    HAL_ADC_Start(&hadc1);//  启动ADC转换


}
float sensor_temp;
void temperature_proc()
{
    float error1, error2, tmp;
    unsigned char i, j;
    static float buf[N];
    static uint32_t k;
    error1 = sensor_adc_value.T_value[0] - sensor_adc_value.T_value[1];
	error1 = fabs(error1);
    error2 = sensor_adc_value.T_value[1] - sensor_adc_value.T_value[2];
	error2 = fabs(error2);

	//unsigned char i,j;
	j = 0;
	for(i=0;i<3;i++)
	{
		if(sensor_adc_value.T_value[i]<=100&&sensor_adc_value.T_value[i]>=-40)
			buf[j++] = sensor_adc_value.T_value[i];
	}
	tmp = 0;
	for(i=0;i<j;i++)
	{
		tmp = tmp + buf[i];
	}   
	tmp =tmp /i;
	buf[k] = tmp;
	//tmp2 = tmp;

    if (k >= N)
    {
        k = 0;
        //tmp = filter(0, &buf[0]); //filter
    }
    else
    {
        
        k++;

    }

		tmp = average_filter(tmp);
	
	sensor_adc_value.average_T = tmp-0.8;


    //sensor_adc_value.average_T = sliding_average_filter(4,tmp);//
    //sensor_adc_value.average_T = tmp;
}

/***************************************************************************//**
 * @brief   根据ADC的采样值计算温度.
 * @param   vadc, ADC采样后换算的电压值, 单位: mv
 * @return  温度的100倍，温度超过瓶颈则为0xffff.
 *******************************************************************************/
float temp_convert(float vadc)
{

    //= (((unsigned long)((PT100_U3 + ((vadc) / (PT100_VADC_TIME)))) * PT100_R2 * 100) / (PT100_VDD - (PT100_U3 + ((vadc) / (PT100_VADC_TIME)))));

    //二分法查找温度值
    unsigned int start = 0, mid = 0, endd = ROM_NUM - 1;
    float dat = 0;
    sensor_temp = vadc;
    // 1. 检查数据合法性
    if ((sensor_temp > MAX_TEMP_VOL) || (sensor_temp < MIN_TEMP_VOL))
    {
        return 0xffff;  //错误标志
    }

    // 2. 开始二分法查找
    while (start <= endd)
    {
        mid = (endd + start) >> 1;
        dat = table[mid];
        if (sensor_temp > dat)
        {
            start = mid + 1;
        }
        else if (sensor_temp < dat)
        {
            if (mid)
            {
                endd = mid - 1;
            }
            else    //mid = 0的特殊情况
            {
                break;
            }
        }
        else
        {
            break;  //找到为中间值mid了
        }
    }//运行完后得到值mid。


    //根据下标计算温度值

    if (dat == sensor_temp) // 正好相等
    {

        return mid - 59 ;
    }
    else
    {
        if (dat > sensor_temp)
        {
            return ((mid - 1 - 59)  + (sensor_temp - table[mid - 1]) /
                    (table[mid] - table[mid - 1]));
        }
        else
        {
            return (mid - 59 + (sensor_temp - table[mid]) / (table[mid + 1] - table[mid]));
        }
    }

}

void adc_proc()
{
    float adc_conv_tmp;
    unsigned char i;
    static int j = 0;
    //static float buf[4][N];
    static uint32_t tick_start;
    if ((HAL_GetTick() - tick_start) >= 10)
    {
        tick_start = HAL_GetTick();
        if (sensor_adc_value.update == 1)
        {
            sensor_adc_value.update = 0;


            for (i = 0; i < 4; i++)
            {
                float x2, x1;
							  //sensor_adc_value.ref = 4095 * 1.2 / sensor_adc_value.adc_value[4];
                sensor_adc_value.ref = 3.0;
                adc_conv_tmp = sensor_adc_value.adc_value[i] * sensor_adc_value.ref;
                adc_conv_tmp = adc_conv_tmp / 4096;

                if (SENSOR_TYPE == PT)   //v1r1/(v2(1+r1/r2)-v1)
                {
                    x2 = adc_conv_tmp / R1_V ;//10---factor
                    x2 = x2 + V2_V; //v1  0.064 bridge voltage
                    x1 = R2_V * x2;
                    x2 = round(x2 * 1000) / 1000;
                    x1 = round(x1 * 1000) / 1000;
                    sensor_adc_value.v_value[i] = x2;
                    //sensor_adc_value.v_value[i] = x2;
                    x2 = (PT_VREF - x2);
                    x1 = x1 / x2;
									 x1 = round(x1 * 1000000) / 1000000;
                    adc_conv_tmp =  x1 * 1000;
                }



                else
                    adc_conv_tmp = adc_conv_tmp / VOL_FACTOR ;
								uint32_t tmp_r;
								 tmp_r = adc_conv_tmp * 1000;
                sensor_adc_value.R_value[i] = tmp_r/1000.0-0.103;
//                buf[i][j] = adc_conv_tmp;
//                if (j >= N)
//                {
//                    j = 0;
//                    tmp = filter(0, &buf[0][0]); //filter
//                    if (tmp != -1)
//                        sensor_adc_value.R_value[0] = round(tmp*1000)/1000;
//                    tmp = filter(1, &buf[1][0]);
//                    if (tmp != -1)
//                        sensor_adc_value.R_value[1] = round(tmp*1000)/1000;;
//                    tmp = filter(2, &buf[2][0]);
//                    if (tmp != -1)
//                        sensor_adc_value.R_value[2] = round(tmp*1000)/1000;;
//                    tmp = filter(3, &buf[3][0]);
//                    if (tmp != -1)
//                        sensor_adc_value.R_value[3] = round(tmp*1000)/1000;;
//                }
//                else
//                {
//                    sensor_adc_value.R_value[i] = adc_conv_tmp;
//
//                }
                if (SENSOR_TYPE == PT)
                {
                   
                    sensor_adc_value.T_value[0] = PT100_Temp(sensor_adc_value.R_value[0]) * 0.2 -
                                                  60;
                    sensor_adc_value.T_value[1] = PT100_Temp(sensor_adc_value.R_value[1]) * 0.2 -
                                                  60;
                    sensor_adc_value.T_value[2] = PT100_Temp(sensor_adc_value.R_value[2]) * 0.2 -
                                                  60;
                    sensor_adc_value.T_value[3] = PT100_Temp(sensor_adc_value.R_value[3]) * 0.2 -
                                                  60;


                }


                else
                {
                    sensor_adc_value.T_value[0] = temp_convert(sensor_adc_value.R_value[0]);
                    sensor_adc_value.T_value[1] = temp_convert(sensor_adc_value.R_value[1]);
                    sensor_adc_value.T_value[2] = temp_convert(sensor_adc_value.R_value[2]);
                    sensor_adc_value.T_value[3] = temp_convert(sensor_adc_value.R_value[3]);
                }

            }
            temperature_proc();
            j++;
            HAL_ADC_Start_DMA(&hadc1, (uint32_t *)sensor_adc_value.adc_value,
                              5); //  开启DMA
        }

        if (ADC_LOG_ENABLE)
            printf("%0.2f\r\n", sensor_adc_value.T_value[0]);
        //printf("%0.2f\r\n",sensor_adc_value.T_value[3]);
    }




}
adc_stru *get_temperature(void)
{
    return &sensor_adc_value;
}


