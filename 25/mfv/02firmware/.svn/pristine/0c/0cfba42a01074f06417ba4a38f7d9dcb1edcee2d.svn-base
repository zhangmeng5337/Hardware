#include <stdint.h>
#include <stdbool.h>
#include "math.h"
#include "adc.h"
#include "table.h"
#include "filter.h"
#include "pt100.h"
#include "config.h"

adc_struct adcG4 = {0};

/**
  @brief SDADC initialization setsup configuration for ADC1
*/
void init_SDADC(void) {
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
}

/**
  @brief Starts conversion of SDADC. 
*/
void start_SDADC(void) {
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcG4.adc_value, 5);  // start conversin for 5 channels
}

/**
  @brief When conversion is completed, this callaback is called by HAL drivers. Need to buffer data and start the next 
         channel conversion.
*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {  
  if (hadc->Instance == ADC1) {
    adcG4.update = true;
  }
}

/**
  @brief temperature processing function
*/
void temperature_proc()
{
  float error1, error2, tmp;
  unsigned char i, j;
  static float buf[N];
  static uint32_t k;
  error1 = adcG4.T_value[0] - adcG4.T_value[1];
  error1 = fabsf(error1);
  error2 = adcG4.T_value[1] - adcG4.T_value[2];
  error2 = fabs(error2);

    //unsigned char i,j;
    j = 0;
    for (i = 0; i < 3; i++)
    {
        if (adcG4.T_value[i] <= 100 && adcG4.T_value[i] >= -40)
            buf[j++] = adcG4.T_value[i];
    }
    tmp = 0;
    for (i = 0; i < j; i++)
    {
        tmp = tmp + buf[i];
    }
    tmp = tmp / i;
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

    tmp = average_filter(tmp, 0);
    adcG4.average_T = tmp - get_config()->tC;
    tmp = average_filter(adcG4.T_value[2], 1);
    adcG4.average_P1_T = tmp - get_config()->tC;

}

/***************************************************************************//**
 * @brief   根据ADC的采样值计算温度.
 * @param   vadc, ADC采样后换算的电压值, 单位: mv
 * @return  温度的100倍，温度超过瓶颈则为0xffff.
 *******************************************************************************/
float temp_convert(float vadc)
{
  //二分法查找温度值
  unsigned int start = 0, mid = 0, endd = ROM_NUM - 1;
  float dat = 0;
  float sensor_temp = vadc;
  
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
    } else if (sensor_temp < dat) {
      if (mid)
      {
        endd = mid - 1;
      } else {  //mid = 0的特殊情况
        break;
      }
    } else {
      break;  //找到为中间值mid了
    }
  }//运行完后得到值mid。

  //根据下标计算温度值
  if (dat == sensor_temp) // 正好相等
  {
    return mid - 59 ;
  } else {
    if (dat > sensor_temp)
    {
      return ((mid - 1 - 59)  + (sensor_temp - table[mid - 1]) / (table[mid] - table[mid - 1]));
    } else {
      return (mid - 59 + (sensor_temp - table[mid]) / (table[mid + 1] - table[mid]));
    }
  }
}

/**
  @brief This is the main ADC process function
*/
void adc_proc(void)
{
  float adc_conv_tmp;
  unsigned char i;
  static int j = 0;

  if (adcG4.update == 1)
  {
    adcG4.update = 0;
    for (i = 0; i < 4; i++)
    {
      float x2, x1;
      adcG4.ref = 3.0;
      adc_conv_tmp = adcG4.adc_value[i] * adcG4.ref;
      adc_conv_tmp = adc_conv_tmp / 4096;

#ifdef PT_SENSOR_TYPE
      x2 = adc_conv_tmp; // R1_V ;//10---factor
      x2 = x2 + 10 * V2_V; //v1  0.064 bridge voltage
      x2 = x2 / 10;
      adcG4.v_value[i] = x2;
      x2 = (PT_VREF - x2);
      x2 = x2 / R2_V;
      x1 = adcG4.v_value[i] / x2;
#else
      adc_conv_tmp = adc_conv_tmp / VOL_FACTOR ;
#endif
          
      uint32_t tmp_r;
      tmp_r = x1 * 1000000;
      adcG4.R_value[i] = tmp_r/1000.0;

#ifdef PT_SENSOR_TYPE
              adcG4.T_value[0] = PT100_Temp(adcG4.R_value[0]) * 0.2 - 60;
              adcG4.T_value[1] = PT100_Temp(adcG4.R_value[1]) * 0.2 - 60;
              adcG4.T_value[2] = PT100_Temp(adcG4.R_value[2]) * 0.2 - 60;
              adcG4.T_value[3] = PT100_Temp(adcG4.R_value[3]) * 0.2 - 60;
#else
              adcG4.T_value[0] = temp_convert(adcG4.R_value[0]);
              adcG4.T_value[1] = temp_convert(adcG4.R_value[1]);
              adcG4.T_value[2] = temp_convert(adcG4.R_value[2]);
              adcG4.T_value[3] = temp_convert(adcG4.R_value[3]);
#endif
      }
      temperature_proc();
      j++;
      start_SDADC();
  }
        

}
