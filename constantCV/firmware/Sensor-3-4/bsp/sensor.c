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
kalman g_kfp_st2 = {0};
kalman g_kfp_st3 = {0};

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
    filter_level_sel(GetRegPrivate()->filter_level);
	
    g_kfp_st2.Last_P = 300;
    g_kfp_st2.Now_P = 0;
    g_kfp_st2.out = 0;
    g_kfp_st2.Kg = 0;
    g_kfp_st2.Q = 0.01;
    g_kfp_st2.R = 0.15;

    g_kfp_st3.Last_P = 300;
    g_kfp_st3.Now_P = 0;
    g_kfp_st3.out = 0;
    g_kfp_st3.Kg = 0;
    g_kfp_st3.Q = 0.001;
    g_kfp_st3.R = 0.1;

}

void adc_config()
{

    if (g_cs1237_device_st.reconfig == 1)
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


    return 1;
    //filter(5,sensor_pressure.buf);
}
unsigned char factory_unit_convert(float dat)
{

    unsigned char result;
    float tmp, tmp1, tmp2, tmp3;
    uint32_t dat_reg;
    result = 1;

    if (GetRegPrivate()->typ == 1)
    {
        dat_reg = GetReg()->pb[eREG_RANGZ_HF16].val_u32ToFloat;
        tmp3 = *(float *)(&dat_reg);
        tmp1 = GetRegPrivate()->maskzero * tmp1;
        tmp2 = dat - tmp3;
        tmp2 = fabs(tmp2);
        tmp1 = fabs(tmp1);
        if (tmp2 <= tmp1)
            tmp2 = tmp3;
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
    int32_t tmp_val;
    float tmp_f, tmp6;
    float cal_val1, cal_val2;
    if (factory_unit_convert(dat) == 1)//factory unit to pa or T
    {
        tmp6 = adc_usr.data_unit_app;
        tmp6 = kalman_filter(&g_kfp_st3, tmp6);
        adc_usr.data_unit_app = tmp6;

        cal_val1 = adc_usr.data_unit_app;
        cal_val2 = *((float *) &
                     (GetReg()->pb[eREG_RANGZ_HF16].val_u32ToFloat)); //-zero
        cal_val1 = cal_val1;// - cal_val2;
        cal_val1 = cal_val1 * GetRegPrivate()->coe;//*coe;


        //   tmp = *((uint32_t *)& cal_val1);

        tmp = *((uint32_t *)& cal_val1);

        GetReg()->pb[eREG_HF16_4].val_u32ToFloat = tmp;
        GetReg()->pb[eREG_LF16_4].val_u32ToFloat = tmp;


        unsigned int decm_bit;
        decm_bit = GetReg()->pb[eREG_DECM_BIT].val_u32ToFloat;
        switch (decm_bit)
        {
            case 0:
                tmp_f = cal_val1;
                break;
            case 1:
                cal_val1 = cal_val1 * 10;
                //tmp_val = round(cal_val1);
                tmp_f = cal_val1 / 10.0;
                break;

            case 2:
                cal_val1 = cal_val1 * 100;
                //  tmp_val = round(cal_val1);
                tmp_f = cal_val1 / 100.0;
                break;
            case 3:
                cal_val1 = cal_val1 * 1000;
                //  tmp_val = round(cal_val1);
                tmp_f = cal_val1 / 1000.0;
                break;
            default :
                tmp_f = cal_val1;
                break;
        }
//        if (dat < 0)
//        {
//            tmp_f = -tmp_f;

//        }


        //if (cal_val1 < 0)
        {
            //  tmp_f = -tmp_f;
            tmp = *((uint32_t *)& tmp_f);


        }
        adc_usr.data_unit_app = tmp_f;
        GetReg()->pb[eREG_HF16].val_u32ToFloat = tmp;
        GetReg()->pb[eREG_LF16].val_u32ToFloat = tmp;


        cal_val1 = adc_usr.data_unit_app * 10;
        if (cal_val1 < 0)
        {
            //cal_val1 = -cal_val1;
            tmp = (uint32_t) cal_val1;
            tmp = ~tmp + 1;
            GetReg()->pb[eREG_X10].val_u32ToFloat = tmp;

            cal_val1 = adc_usr.data_unit_app * 100;
            tmp = (uint32_t) cal_val1;
            tmp = ~tmp + 1;
            GetReg()->pb[eREG_X100].val_u32ToFloat = tmp;

            cal_val1 = adc_usr.data_unit_app * 1000;
            cal_val1 = -cal_val1;
            tmp = (uint32_t) cal_val1;
            tmp = ~tmp + 1;
            GetReg()->pb[eREG_X1000].val_u32ToFloat = tmp;
        }
        else
        {
            tmp = (uint32_t) cal_val1;
            GetReg()->pb[eREG_X10].val_u32ToFloat = tmp;

            cal_val1 = adc_usr.data_unit_app * 100;
            tmp = (uint32_t) cal_val1;
            GetReg()->pb[eREG_X100].val_u32ToFloat = tmp;

            cal_val1 = cal_val1 * 100.0;
            tmp = (uint32_t) cal_val1;;
            GetReg()->pb[eREG_X1000].val_u32ToFloat = tmp;
        }






//        if (tmp & 0x80000000)
//        {
//           // tmp = ~tmp + 1;
//            GetReg()->pb[eREG_X10].val_u32ToFloat = tmp;
//            GetReg()->pb[eREG_X100].val_u32ToFloat = tmp * 10;
//            GetReg()->pb[eREG_X100].val_u32ToFloat =
//                GetReg()->pb[eREG_X100].val_u32ToFloat;
//            GetReg()->pb[eREG_X10].val_u32ToFloat = GetReg()->pb[eREG_X10].val_u32ToFloat;
//            GetReg()->pb[eREG_X1000].val_u32ToFloat = tmp * 100;
//            GetReg()->pb[eREG_X1000].val_u32ToFloat =
//                -GetReg()->pb[eREG_X1000].val_u32ToFloat;
//
//        }
//        else
//        {
//            GetReg()->pb[eREG_X10].val_u32ToFloat = tmp;
//            GetReg()->pb[eREG_X100].val_u32ToFloat = tmp * 10;
//            GetReg()->pb[eREG_X1000].val_u32ToFloat = tmp * 100;
//
//        }

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


void adc_proc(void)
{


    static uint32_t tick_tmp;
    //if (getuart()->recv_update == 0)


    if (GetRegPrivate()->save & 0x80)
    {
        filter_level_sel(GetRegPrivate()->filter_level);
        GetRegPrivate()->save = GetRegPrivate()->save & 0x7f;
    }

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
                adc_usr.adc_ori = g_cs1237_device_st.adc_data;
                adc_usr.adc_dat_LF = g_cs1237_device_st.adc_calculate_deal_data;
            }
        }


        if (GetTick() - tick_tmp >= 30)
        {
            tick_tmp = GetTick();
            cal_press();
        }
    }

}

float cal_buf[2];
void sort()
{
    unsigned char i, j, lt;
    lt = GetRegPrivate()->select_value;
	if(GetRegPrivate()->cal1val>=GetRegPrivate()->usr_cal1val&&GetRegPrivate()->usr_cal1ADC!=0)
    cal_buf[0] = GetRegPrivate()->usr_cal1ADC;
	else
	    cal_buf[0] = GetRegPrivate()->cal1ADC;	
	if(GetRegPrivate()->usr_cal2ADC!=0)
	{
	switch(lt)
		{
			case 5:
				if(GetRegPrivate()->cal5val<=GetRegPrivate()->usr_cal2val)
				cal_buf[1] = GetRegPrivate()->usr_cal2ADC;
				else
				cal_buf[1] = GetRegPrivate()->cal5val;				
				break;
		case 4:
			if(GetRegPrivate()->cal4val<=GetRegPrivate()->usr_cal2val)
			cal_buf[1] = GetRegPrivate()->usr_cal2ADC;
			else
			cal_buf[1] = GetRegPrivate()->cal4val;				
			break;
		case 3:
			if(GetRegPrivate()->cal3val<=GetRegPrivate()->usr_cal2val)
			cal_buf[1] = GetRegPrivate()->usr_cal2ADC;
			else
			cal_buf[1] = GetRegPrivate()->cal3val;				
			break;
	
		case 2:
			if(GetRegPrivate()->cal2val<=GetRegPrivate()->usr_cal2val)
			cal_buf[1] = GetRegPrivate()->usr_cal2ADC;
			else
			cal_buf[1] = GetRegPrivate()->cal2val;				
			break;
	   default:
			cal_buf[1] = GetRegPrivate()->cal5ADC;			
		}

	}
	






}
void cal_press(void)
{
    float tmp4, tmp5;
    float tmp6;

    float tmp1_conv1, tmp2_conv2;
    float tmp1, tmp2, tmp3;

    tmp2 = adc_usr.adc_dat_LF;

    if (GetRegPrivate()->pga > 6) // 128
    {
        tmp2 = tmp2 / 128.0 ; //90 100
    }
    else if (GetRegPrivate()->pga > 1) //64
    {
        tmp2 = tmp2 / 64.0 ; //300 200 50 25
        adc_usr.adc_ori = tmp2;
    }
    else if (GetRegPrivate()->pga >= 1) //1
    {
        tmp2 = tmp2 / 20.0 ; //200 100 20
        adc_usr.adc_ori = tmp2;
    }
    else//1
    {
        tmp2 = tmp2 / 20.0 ; //150 100 20
        adc_usr.adc_ori = tmp2;
    }


    // tmp2 = medium_aver(tmp2);
    adc_usr.adc_data_TF = tmp2;
    adc_usr.adc_data_KF = kalman_filter(&g_kfp_st, tmp2);
    tmp2 = adc_usr.adc_data_KF;
    tmp2 = SilderFilter(tmp2);
    // tmp2 = 13699;

    if (tmp2 != 0)
    {
        adc_usr.adc_data_SF = tmp2 ;
        adc_usr.adc_ori_filter = tmp2;
        tmp3 = g_cs1237_device_st.adc_data;
        tmp4 = adc_usr.adc_ori;

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

        float cal_val;
        if (GetRegPrivate()->cal5ADC == 0)
        {
            cal_val = GetRegPrivate()->cal4ADC;
            if (cal_val == 0)
                cal_val = GetRegPrivate()->cal3ADC;
            if (cal_val == 0)
                cal_val = GetRegPrivate()->cal2ADC;

        }
        else
            cal_val = GetRegPrivate()->cal5ADC;
		sort();

        tmp1_conv1 = cal_buf[1] - cal_buf[0];
        //tmp1_conv1 = tmp1_conv1 / getPga(GetRegPrivate()->pga);


        tmp2_conv2 = adc_usr.adc_vol - cal_buf[0];
        // tmp2_conv2 = tmp2_conv2 / getPga(GetRegPrivate()->pga);

        tmp2_conv2 = tmp2_conv2 / tmp1_conv1;
        adc_usr.dat_cal = tmp2_conv2;

        tmp1 = adc_usr.dat_cal;//1
        tmp2 = adc_usr.dat_cal * adc_usr.dat_cal;//2
        tmp3 = tmp2 * adc_usr.dat_cal;//3
        tmp4 = tmp3 * adc_usr.dat_cal;//4

        tmp5  = GetRegPrivate()->coe1 * tmp4;
        tmp6 = tmp3 * GetRegPrivate()->coe2;
        tmp6 = tmp6 + tmp5;//X^4+X^3

        tmp5 = tmp2 * GetRegPrivate()->coe3;
        tmp6 = tmp6 + tmp5 ;//X^4+X^3+X^2

        tmp5 = tmp1 * GetRegPrivate()->coe4;
        tmp6 = tmp6 + tmp5 ;    //X^4+X^3+X^2+X
        tmp6 = tmp6 + GetRegPrivate()->coe5 ;

        tmp6 =    tmp6 - GetRegPrivate()->offset;

        //tmp6 = kalman_filter(&g_kfp_st2, tmp6);
        adc_usr.dat_unit_factory = tmp6;
        dat_cal_proc(adc_usr.dat_unit_factory);
        g_cs1237_device_st.update = 0;
//                    printf("            %.5f            %.4f\r\n",adc_usr.adc_ori,adc_usr.adc_data_KF);

    }

}
uint32_t pulse;
void pwm_ctrl(float ratio)
{

    float tmp;
    tmp = GetRegPrivate()->cur_set / 5;

    pulse = tmp * PWM_COUNTER * 4;
    if (pulse <= PWM_COUNTER && pulse > 1)
        GTIM_SetCompare3(CW_GTIM1, pulse - 1); // 1*4   1khz 1000  250-1
}

