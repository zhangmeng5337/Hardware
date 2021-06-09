#include "adc.h"
#include "app.h"
#include "filter.h"
#include "FdrAlgorithm.h"
#include "calman.h"
extern ADC_HandleTypeDef hadc1;


sensor_stru sensor_usr;
extern uint32_t	freq_count;
float adc_value;

float adcBuf_humid[N];	//湿度电压缓冲区
float adcBuf_ref[N];	//参考电压缓冲区
float adcBuf_humid[N];	//湿度电压缓冲区
float adcBuf_ta[N];//温度a电压缓冲区
float adcBuf_tb[N];//温度b电压缓冲区
float adcBuf_cali[N];//温度b电压缓冲区



sensor_stru *getSensor()
{
    return &sensor_usr;
}
    static unsigned int rhbuf[10],rh_index,tempbuf[10];
/*********************************************************
            adc采集探针，温度传感器电压
*********************************************************/
void Get_Adc_Average(unsigned char times)
{

    float temp_val=0;
    unsigned char i,t;


    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_Delay(20);
    for(i=0; i<N; i++)
    {
        //开启ADC

        for(t=1; t<5; t++)
        {

            HAL_ADC_Start(&hadc1);
            HAL_ADC_PollForConversion(&hadc1,0xffff);
            temp_val = HAL_ADC_GetValue(&hadc1);




            switch(t)
            {
            //case 0: adcBuf_ref[i]=temp_val;break;
            case 1:
                adcBuf_humid[i]=temp_val;
                break;
            case 2:
                adcBuf_ta[i]=temp_val;
                break;
            case 3:
                adcBuf_tb[i]=temp_val;
                break;
            case 4:
                adcBuf_cali[i]=temp_val;
                break;

            }
        }
    }

    HAL_ADC_Stop(&hadc1);

    /***************************filter adc value*************************************/

    sensor_usr.ADC_REF = 3;
    //Claman(adcBuf_ta,0.01,10);
    adc_value = filter(1);
    sensor_usr.sensor[0] = adc_value;
		
    adc_value = filter(2);
    sensor_usr.sensor[1] = adc_value;
		
    adc_value = filter(3);
    sensor_usr.sensor[2] = adc_value;
		
    adc_value = filter(4);
    sensor_usr.sensor[3] = adc_value;


    /****************************calculabute humid and temperature*********************/
    sensor_usr.sensor[0] = sensor_usr.sensor[0] ;
    sensor_usr.rh = SoilHumid(MEASURE,sensor_usr.sensor[0]);
    unsigned char tmp;
    if(sensor_usr.CalibrationT&0x80)
    {
        tmp = ~sensor_usr.CalibrationT+1;
        sensor_usr.temperature = SoilTemperature(MEASURE,sensor_usr.sensor[1],sensor_usr.sensor[2])-tmp;

    }
    else
    {
        tmp = sensor_usr.CalibrationT;
        sensor_usr.temperature = SoilTemperature(MEASURE,sensor_usr.sensor[1],sensor_usr.sensor[2])+tmp;
    }
    /**************************稳定数据，防止频繁跳动*******************/
    static uint32_t repeatTick;

    unsigned int tmp2,tmp3;

    unsigned char k,j;

    static unsigned char flag;   
	sensor_usr.temperatureOri = sensor_usr.temperature;

	rhbuf[rh_index] = sensor_usr.rh;
    tempbuf[rh_index] = sensor_usr.temperature; 
		sensor_usr.rh = 0;
    sensor_usr.temperature = 0;
    rh_index++;
		 if(flag == 1)
		 {
			 seq_fileter(rhbuf,10);
			 seq_fileter(tempbuf,10);		 
		 }


    if(rh_index>=3||flag == 1)
    {

        if(flag ==1)
        {
            for (k = 1; k<9; k++)
            {
                sensor_usr.rh = sensor_usr.rh + rhbuf[k];
                sensor_usr.temperature = sensor_usr.temperature + tempbuf[k];

            }
            sensor_usr.rh = sensor_usr.rh/(8);
            sensor_usr.temperature = sensor_usr.temperature/(8);
        }
        else
        {
            for (k = 1; k<rh_index-2; k++)
            {
                sensor_usr.rh = sensor_usr.rh + rhbuf[k];
                sensor_usr.temperature = sensor_usr.temperature + tempbuf[k];

            }

            sensor_usr.rh = sensor_usr.rh/(rh_index-2);
            sensor_usr.temperature = sensor_usr.temperature/(rh_index-2);
        }


    }
    else
    {
        for (k = 0; k<rh_index; k++)
        {
            sensor_usr.rh = sensor_usr.rh + rhbuf[k];
            sensor_usr.temperature = sensor_usr.temperature + tempbuf[k];

        }
        sensor_usr.rh = sensor_usr.rh/rh_index;
        sensor_usr.temperature = sensor_usr.temperature/rh_index;
    }


    if(rh_index>=10)
    {
        rh_index	= 0;
        flag = 1;
    }

    if(abs(sensor_usr.last_Temp-sensor_usr.temperature)>=TERROR)
    {

        if((HAL_GetTick()-repeatTick)>=FRESH)
        {
            repeatTick = HAL_GetTick();
            sensor_usr.last_Temp=sensor_usr.temperature;
        }
        else
        {
            sensor_usr.temperature=sensor_usr.last_Temp;

        }
    }
    else
    {
        repeatTick = HAL_GetTick();
        sensor_usr.temperature=sensor_usr.last_Temp;
    }
    static uint32_t repeatTick2;
    if(abs(sensor_usr.rh -sensor_usr.last_humid)>=RHERROR)
    {

        if((HAL_GetTick()-repeatTick2)>=FRESH)
        {
            repeatTick = HAL_GetTick();
            sensor_usr.last_humid=sensor_usr.rh;
        }
        else
        {
            sensor_usr.rh=sensor_usr.last_humid;

        }
    }
    else
    {
        repeatTick2 = HAL_GetTick();
        sensor_usr.rh=sensor_usr.last_humid;

    }
    /****************************稳定数据结束**********************************/
}
