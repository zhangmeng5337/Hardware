#include "sensor.h"
#include "cs1237.h"
//#include "cs1237_32.h"
#include <math.h>

#include "main.h"
#include "kalman.h"
#include "cw32l010_gtim.h"
#include "reg.h"
#include "key.h"
#include "display.h"
extern struct cs1237_device g_cs1237_device_st;
kalman g_kfp_st = {0};

adc_stru adc_usr;


adc_stru *getadc(void)
{
    return &adc_usr;
}
unsigned char *getAdcReconfig(void)
{
    return &g_cs1237_device_st.reconfig;
}
void adc_init(void)
{
    //cs1237_init(&g_cs1237_device_st, DEV_FREQUENCY_10, DEV_PGA_1, DEV_CH_A);
    cs1237_init(&g_cs1237_device_st, GetReg()->pb[eREG_ADC_RATE].val_u32ToFloat + 1,
                getPgaToADC(GetRegPrivate()->pga), DEV_CH_A);



    kalman_init(&g_kfp_st);


}

void adc_config()
{

    if (g_cs1237_device_st.reconfig)
    {
        if (getKey()->indat[0].spd == 0)
        {
            GetReg()->pb[eREG_ADC_RATE].val_u32ToFloat = getKey()->indat[0].spd;

        }
        else
            GetReg()->pb[eREG_ADC_RATE].val_u32ToFloat = 1;

    }

    cs1237_init(&g_cs1237_device_st, GetReg()->pb[eREG_ADC_RATE].val_u32ToFloat + 1,
                getPgaToADC(GetRegPrivate()->pga), DEV_CH_A);
    g_cs1237_device_st.reconfig = 0;
}
unsigned char unit_convert(float dat)
{
    unsigned char i = 0;
    for (i = 0; i < 13; i++)
    {
        if (GetReg()->pb[eREG_UNIT].val_u32ToFloat == i)
            break;
    }
    if (i == 13)
        return 0;



    switch (GetReg()->pb[eREG_UNIT].val_u32ToFloat)
    {
        case epa   :
            adc_usr.data_unit_app = dat;
            break;
        case eKpa  :
            adc_usr.data_unit_app = dat / 1000.0;
            break;
        case eMpa  :
            adc_usr.data_unit_app = dat / 1000000.0;
            break;
        case emmH2O:
            adc_usr.data_unit_app = dat * 0.101972;
            break;
        case emH2O :
            adc_usr.data_unit_app = dat * 0.000101972;
            break;
        case ebar  :
            adc_usr.data_unit_app = dat * 0.00001;
            break;
        case epsi  :
            adc_usr.data_unit_app = dat * 0.000145037744;
            break;
        case eatm  :
            adc_usr.data_unit_app = dat * 0.000009869233;
            break;
        case ekg   :
            adc_usr.data_unit_app = dat * 0.101971621298;
            break;
        case emm   :
            adc_usr.data_unit_app = dat * 0.007500616827;
            break;
        case em    :
            adc_usr.data_unit_app = dat * 0.000007500616827;
            break;
        case eT    :
            adc_usr.data_unit_app = dat;
            break;
        case eF    :
            adc_usr.data_unit_app = dat + 273.15;
            break;
        default    :
            return 0;
            break;

    }
    float tmp;

    return 1;
    //filter(5,sensor_pressure.buf);
}
unsigned char factory_unit_convert(float dat)
{

    unsigned char result;
    float tmp, tmp1, tmp2;
    result = 1;

    if (GetRegPrivate()->typ == 1)
    {
        tmp1 = *((float *)GetReg()->pb[eREG_RANGZ_HF16].val_u32ToFloat);
        tmp1 = GetRegPrivate()->maskzero * tmp1;
        if (dat <= tmp1)
            tmp2 = 0;
        else
            tmp2 = dat;

        switch (GetRegPrivate()->unit)
        {
            case epa:
                tmp = tmp2 * 1;
                result = 1;
                break;
            case eKpa  :
                tmp = tmp2 * 1000.0;
                break;
            case eMpa  :
                tmp = tmp2 * 1000000.0;
                break;
            case emmH2O:
                tmp = tmp2 * 9.806613580198485;
                break;
            case emH2O :
                tmp = tmp2 * 9806.613580198486;
                break;
            case ebar  :
                tmp = tmp2 * 10000;
                break;
            case epsi  :
                tmp = tmp2 * 6894.756995117078;
                break;
            case eatm  :
                tmp = tmp2 * 101324.99658281449;
                break;
            case ekg   :
                tmp = tmp2 * 9.806649999980076;
                break;
            case emm   :
                tmp = tmp2 * 133.3223684217938;
                break;
            case em    :
                tmp = tmp2 * 133322.3684217938;
                break;
            default    :
                result = 0;
                break;

        }
    }
    else if (GetRegPrivate()->typ == 0)
    {
        switch (GetRegPrivate()->unit)
        {
            case eT    :
                tmp = dat * 1;
                break;
            case eF    :
                tmp = dat - 273.15;
                break;
            default    :
                result = 0;
                break;
        }
    }
    else
        result = 0;
    if (unit_convert(tmp) == 1)//pa or T --->user unit
        result = 1;
    else
        result = 0;
    return result;
}

void dat_cal_proc(float dat)
{
    uint32_t tmp;
    float tmp_f;
    float cal_val1, cal_val2;
    if (factory_unit_convert(dat) == 1)//factory unit to pa or T
    {

        cal_val1 = adc_usr.data_unit_app;
        cal_val2 = *((float *) &
                     (GetReg()->pb[eREG_RANGZ_HF16].val_u32ToFloat)); //-zero
        cal_val1 = cal_val1 - cal_val2;
        cal_val1 = cal_val1 * GetRegPrivate()->coe;//*coe;


        tmp = *((uint32_t *)& cal_val1);
		tmp_f = cal_val1 * 10000.0;
		tmp = round(tmp_f);
		tmp_f = tmp / 10000.0;
        tmp = *((uint32_t *)& tmp_f);

        GetReg()->pb[eREG_HF16_4].val_u32ToFloat = tmp;
        GetReg()->pb[eREG_LF16_4].val_u32ToFloat = tmp;


        unsigned int decm_bit;
        decm_bit = GetReg()->pb[eREG_DECM_BIT].val_u32ToFloat;
        switch (decm_bit)
        {
            case 0:
                tmp_f = round(cal_val1);
                break;
            case 1:
                cal_val1 = cal_val1 * 10;
                tmp = round(cal_val1);
                tmp_f = tmp / 10.0;
                break;

            case 2:
                cal_val1 = cal_val1 * 100;
                tmp_f = round(cal_val1);
                tmp = tmp / 100.0;
                break;
            case 3:
                cal_val1 = cal_val1 * 1000;
                tmp = round(cal_val1);
                tmp_f = tmp / 1000.0;
                break;
            default :
                tmp_f = round(cal_val1);
                break;
        }
				tmp = *((uint32_t *)& tmp_f);
        GetReg()->pb[eREG_HF16].val_u32ToFloat = tmp;
        GetReg()->pb[eREG_LF16].val_u32ToFloat = tmp;
		adc_usr.data_unit_app = tmp_f; 
        cal_val1 = adc_usr.data_unit_app;
        cal_val1 = cal_val1 * 10;
        tmp = cal_val1;
        if (tmp & 0x8000)
        {
            tmp = ~tmp + 1;
            GetReg()->pb[eREG_X10].val_u32ToFloat = tmp;
            GetReg()->pb[eREG_X100].val_u32ToFloat = tmp * 10;
            GetReg()->pb[eREG_X100].val_u32ToFloat =
                -GetReg()->pb[eREG_X100].val_u32ToFloat;
            GetReg()->pb[eREG_X10].val_u32ToFloat = -GetReg()->pb[eREG_X10].val_u32ToFloat;

        }
        else
        {
            GetReg()->pb[eREG_X10].val_u32ToFloat = tmp;
            GetReg()->pb[eREG_X100].val_u32ToFloat = tmp * 10;

        }

//        cal_val1 = cal_val1 * 10;
//        tmp = cal_val1;
//
//        if (tmp & 0x8000)
//        {
//            tmp = ~tmp + 1;
//            GetReg()->pb[eREG_X100].val_u32ToFloat = tmp;
//            GetReg()->pb[eREG_X100].val_u32ToFloat = -GetReg()->pb[eREG_X100].val_u32ToFloat;
//
//        }
//        else
//            GetReg()->pb[eREG_X100].val_u32ToFloat = tmp;


    }
}

extern uint32_t time_cal;

float tmp6;
void adc_proc(void)
{
    float tmp1, tmp2, tmp3;
    int32_t tmp_adc;
    //int32_t tmp4;

    if (g_cs1237_device_st.reconfig)
        adc_config();
    else
    {
        if (get_adc_flag() == 1)
        {
            g_cs1237_device_st.adc_calculate_deal_data = calculate_adc_num(
                        &g_cs1237_device_st);
            if (g_cs1237_device_st.adc_calculate_deal_data != 0)
            {
                adc_usr.adc_ori = g_cs1237_device_st.adc_calculate_deal_data;
                
                cal_press();
                 g_cs1237_device_st.update = 0;

            }

        }

    }

}

void cal_press(void)
{
    float tmp1_conv1, tmp2_conv2;
    float tmp1, tmp2, tmp3;
    tmp2 = adc_usr.adc_ori;

    if (GetRegPrivate()->pga > 6) // 128
    {
        tmp2 = tmp2 / 150.0;//400 300 50
        adc_usr.adc_ori = tmp2;

    }
    else if (GetRegPrivate()->pga > 1) //64
    {
        tmp2 = tmp2 / 150.0;//300 200 50
        adc_usr.adc_ori = tmp2;

    }
    else if (GetRegPrivate()->pga >= 1) //1
    {
        tmp2 = tmp2 / 100.0;//200 100
        adc_usr.adc_ori = tmp2;

    }
    else//1
    {
        tmp2 = tmp2 / 100.0;//150 100
        adc_usr.adc_ori = tmp2;

    }

    //tmp2 = tmp2 / 1.0;
    // adc_usr.adc_ori = tmp2;


    // tmp2 = kalman_filter(&g_kfp_st, tmp2);


    tmp2 = medium_aver(tmp2);
    adc_usr.adc_ori_filter = tmp2;
    tmp3 = adc_usr.adc_ori;
     // printf("    %.6f   %.6f\r\n",tmp2,tmp3);

    if (adc_usr.adc_ori_filter != 0)
    {
        if (GetRegPrivate()->zero_cmd == 1)
        {
            adc_usr.adc_ori_filter = adc_usr.adc_ori_filter -
                                     GetRegPrivate()->zero_value;
        }

        else
            adc_usr.adc_ori_filter = adc_usr.adc_ori_filter;

        adc_usr.adc_vol =  adc_usr.adc_ori_filter;

    }

    tmp1_conv1 = GetRegPrivate()->cal5ADC - GetRegPrivate()->cal1ADC;
    tmp1_conv1 = tmp1_conv1 / getPga(GetRegPrivate()->pga);

    tmp2_conv2 = adc_usr.adc_vol - GetRegPrivate()->cal1ADC;
    tmp2_conv2 = tmp2_conv2 / getPga(GetRegPrivate()->pga);
    float tmp4, tmp5;
    tmp2_conv2 = tmp2_conv2 / tmp1_conv1;
    adc_usr.dat_cal = tmp2_conv2;
    tmp4 = adc_usr.dat_cal;//1
    tmp1 = adc_usr.dat_cal * adc_usr.dat_cal;
    tmp2 = tmp1;//2
    tmp1 = tmp1 * adc_usr.dat_cal;//3
    tmp3 = adc_usr.dat_cal;//1
    tmp3  = GetRegPrivate()->coe1 * tmp1;
    tmp5 = tmp2 * GetRegPrivate()->coe2;
    tmp3 = tmp3 + tmp5;
    tmp5 = tmp4 * GetRegPrivate()->coe3;
    tmp3 = tmp3 + tmp5 ;
    tmp4 = tmp3 + GetRegPrivate()->coe4;
    tmp4 =    tmp4 - GetRegPrivate()->offset;
    adc_usr.dat_unit_factory = tmp4;
    dat_cal_proc(adc_usr.dat_unit_factory);

}
void pwm_ctrl(float ratio)
{
    unsigned int pulse;
    float tmp;
    if (GetRegPrivate()->cur_set >= 0.39 &&
            GetRegPrivate()->cur_set <= 0.5)
    {
        tmp = 0.2 / 5;  //pcb resistor changed 1K

    }
    else
        tmp = GetRegPrivate()->cur_set / 5;

    pulse = tmp * PWM_COUNTER;
    if (pulse <= PWM_COUNTER)
        GTIM_SetCompare3(CW_GTIM1, pulse - 1); // 1*4   1khz 1000  250-1
}

