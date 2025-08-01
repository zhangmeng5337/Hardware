//
#include "verifier.h"
#include "relay_driver.h"
#include "config.h"
#include "cdg.h"
#include "adc.h"
#include "debug.h"
#include "math.h"
#include "calibration.h"
#include "flash.h"

//extern TIM_HandleTypeDef htim1;
VERIFIER_STRU verifer_dat;

void timer_start()
{

    // HAL_TIM_Base_Start_IT(&htim1);
    //HAL_TIM_Base_Start(&htim1);
    verifer_dat.timer_tick = 0;
    verifer_dat.update = 0;
}

void verifier_init(void)
{

//
//    relay_ctrl(ROOT_UP_RELAY, OFF);
//    relay_ctrl(HIGHT_FLOW_UP_RELAY, OFF);
//    relay_ctrl(LOW_FLOW_UP_RELAY, OFF);
//    relay_ctrl(DOWN_RELAY, ON);
//    relay_ctrl(LOW_FLOW_UP_RELAY, ON);
//    relay_ctrl(HIGHT_FLOW_UP_RELAY, ON);
//    HAL_Delay(2000);
    verifer_dat.state = NORMAL_OPERATION;
    relay_ctrl(ROOT_UP_RELAY, OFF);
    relay_ctrl(HIGHT_FLOW_UP_RELAY, OFF);
    relay_ctrl(LOW_FLOW_UP_RELAY, OFF);
    relay_ctrl(DOWN_RELAY, OFF);
    verifer_dat.sw_flag = 0;
    verifer_dat.mfc_sig = 0;
    verifer_dat.cali_count = 1;
    verifer_dat.timer_tick = 0;
    if (verifer_dat.purge_cycle >= 0xffffffff || verifer_dat.purge_cycle < 1)
        verifer_dat.purge_cycle = 1;
    verifer_dat.purge_status = 0;
    verifer_dat.nozzle_testT = 0;
    verifer_dat.nozzle_tick = 0;
    if (verifer_dat.purge_time >= 0xffffffff || verifer_dat.purge_time < 1)
        verifer_dat.purge_time = 120;//unit:s
    if (get_calib()->compen_time >= 0.05 || get_calib()->compen_time <= -0.05)
        get_calib()->compen_time = 0.010;//unit:sec
    else
        get_calib()->compen_time = 0;//unit:sec

    get_calib()->perror = PRESS_ERROR;//3 2
    //get_calib()->compen_time = 0.015;//unit:sec

    // timer_start();
}

VERIFIER_STRU *get_verifier()
{
    return &verifer_dat;
}



void timer_stop()
{

//    HAL_TIM_Base_Stop_IT(&htim1);
    // HAL_TIM_Base_Stop(&htim1);
    verifer_dat.timer_tick = 0;
    verifer_dat.update = 0;
}
//void timer_callback()
//{
//    verifer_dat.timer_tick = verifer_dat.timer_tick + 0.1;
//
//}
void flow_cal_proc(void)
{
    if (verifer_dat.update == 2) //pressure steady,close upstream valve
    {
        if (DEBUG_LOG1_ENABLE)
            printf("pressure_steady_tTime = %d\r\n", verifer_dat.pressure_steady_tTime);
        timer_stop();

        flow_proc();
        timer_start();
        verifer_dat.update = 0;
    }

}
//void timer_state()
//{
//    if (verifer_dat.update == 0)
//        verifer_dat.update = 1;
//    verifer_dat.timer_tick = verifer_dat.timer_tick + 1;
//
//}
/*******************************************************

laser_ctrl(0);
HAL_TIM_Base_Stop_IT(&htim2);
HAL_TIM_Base_Stop(&htim2);
HAL_ADC_Stop(&hadc1);

********************************************************/
void pressure_time_proc(void)
{
    //timer_state();

    //verifer_dat.update = 1;
    {
        if (verifer_dat.state == FLOW_SET_POINT_STEADY)//close downstream valve
        {
            verifer_dat.pressure_rise_sTime  = verifer_dat.timer_tick;
            verifer_dat.flow_start_temp_averT = get_temperature()->average_T;
            //verifer_dat.update = 0;

        }
        else if (verifer_dat.state == PRESSURE_RISE_START) //target time
        {
            verifer_dat.pressure_steady_eTime  = verifer_dat.timer_tick;
            verifer_dat.pressure_steady_averT = get_temperature()->average_T;
            verifer_dat.pressure_steady_tTime = verifer_dat.pressure_steady_eTime -
                                                verifer_dat.pressure_rise_sTime;
            //verifer_dat.update = 0;

            if (DEBUG_LOG1_ENABLE)
                printf("pressure_steady_tTime = %d\r\n", verifer_dat.pressure_steady_tTime);
        }

        else if (verifer_dat.state ==
                 PRESSURE_STEADY_START) //pressure steady,close upstream valve
        {
            //verifer_dat.pressure_steady_eTime  = verifer_dat.timer_tick;
            // verifer_dat.pressure_steady_tTime = verifer_dat.pressure_steady_eTime -
            verifer_dat.pressure_rise_sTime;
            //  verifer_dat.pressure_steady_averT = get_temperature()->average_T;
            //verifer_dat.update = 0;

            if (DEBUG_LOG1_ENABLE)
                printf("pressure_steady_tTime = %d\r\n", verifer_dat.pressure_steady_tTime);
        }
        else if (verifer_dat.state ==
                 PRESSURE_STEADY_END)//pressure steady,close upstream valve
        {
            if (DEBUG_LOG1_ENABLE)
                printf("pressure_steady_tTime = %d\r\n", verifer_dat.pressure_steady_tTime);
            /// flow_proc();
            //timer_stop();
            //verifer_dat.update = 0;
        }

    }



}
/*
NORMAL_OPERATION--->LINE_PURGED_START-->FLOW_SET_POINT_START--->
FLOW_SET_POINT_SHOCK--->FLOW_SET_POINT_STEADY--->PRESSURE_RISE_START
--->PRESSURE_STEADY_START--->NORMAL_OPERATION

*/
void pressure_ctrl()
{
    switch (verifer_dat.state)
    {
        case LINE_PURGED_START:
        {
            relay_ctrl(ROOT_UP_RELAY, OFF);
            relay_ctrl(flow_mode_relay_sel(), OFF);
            relay_ctrl(DOWN_RELAY, ON);
        }
        break;
		case FLOW_WAIT:
        case FLOW_SET_POINT_START:
        {
            relay_ctrl(ROOT_UP_RELAY, ON);
            relay_ctrl(flow_mode_relay_sel(), ON);
            relay_ctrl(DOWN_RELAY, ON);
        }
        break;
        case FLOW_SET_POINT_SHOCK:
        {
            relay_ctrl(ROOT_UP_RELAY, ON);
            relay_ctrl(flow_mode_relay_sel(), ON);
            relay_ctrl(DOWN_RELAY, ON);
        }
        break;
        case FLOW_SET_POINT_SHOCK2:
        {
            relay_ctrl(ROOT_UP_RELAY, OFF);
            relay_ctrl(flow_mode_relay_sel(), OFF);
            relay_ctrl(DOWN_RELAY, OFF);
        }
        break;
        case FLOW_SET_POINT_STEADY2:
        {
            relay_ctrl(ROOT_UP_RELAY, OFF);
            relay_ctrl(flow_mode_relay_sel(), OFF);
            relay_ctrl(DOWN_RELAY, ON);
        }
        break;

        case FLOW_SET_POINT_STEADY:
        {
            relay_ctrl(ROOT_UP_RELAY, ON);
            relay_ctrl(flow_mode_relay_sel(), ON);
            relay_ctrl(DOWN_RELAY, OFF);
        }
        break;
        case PRESSURE_RISE_START:
        {
            relay_ctrl(ROOT_UP_RELAY, ON);
            relay_ctrl(flow_mode_relay_sel(), ON);
            relay_ctrl(DOWN_RELAY, OFF);
        }
        break;
        case PRESSURE_STEADY_START:
        {
            relay_ctrl(ROOT_UP_RELAY, OFF);
            relay_ctrl(flow_mode_relay_sel(), OFF);
            relay_ctrl(DOWN_RELAY, OFF);
        }
        break;
        case PRESSURE_STEADY_SHOCK:
        {
            relay_ctrl(ROOT_UP_RELAY, OFF);
            relay_ctrl(flow_mode_relay_sel(), OFF);
            relay_ctrl(DOWN_RELAY, OFF);
        }
        break;
        case PRESSURE_STEADY_END:
        {
            relay_ctrl(ROOT_UP_RELAY, ON);
            relay_ctrl(flow_mode_relay_sel(), ON);
            relay_ctrl(DOWN_RELAY, ON);
        }
        break;

        case PURGE_START:
        {
            relay_ctrl(ROOT_UP_RELAY, ON);
            relay_ctrl(HIGHT_FLOW_UP_RELAY, ON);
            relay_ctrl(DOWN_RELAY, ON);
        }
        break;
        case PH_SCTRL:
        {
            relay_ctrl(ROOT_UP_RELAY, OFF);
            relay_ctrl(HIGHT_FLOW_UP_RELAY, OFF);
            relay_ctrl(DOWN_RELAY, ON);
        }
        break;

        case DRAIN_START:
        {
            relay_ctrl(ROOT_UP_RELAY, ON);
            relay_ctrl(HIGHT_FLOW_UP_RELAY, ON);
            relay_ctrl(DOWN_RELAY, OFF);
        }
        break;
        /**************************leak ctrl***********************************/
        case DEV_LEAK_START:
        case DEV_LEAK_IDL:
        {
            relay_ctrl(ROOT_UP_RELAY, OFF);
            relay_ctrl(HIGHT_FLOW_UP_SRELAY, OFF);
            relay_ctrl(LOW_FLOW_UP_SRELAY, OFF);
            relay_ctrl(DOWN_RELAY, ON);
        }
        break;
        case DEV_LEAK_MID:
        {
            relay_ctrl(ROOT_UP_RELAY, OFF);
            relay_ctrl(HIGHT_FLOW_UP_SRELAY, OFF);
            relay_ctrl(LOW_FLOW_UP_SRELAY, OFF);
            relay_ctrl(DOWN_RELAY, OFF);
        }
        break;
        case SYS_LEAK_START:
        case SYS_LEAK_IDL:
        {
            relay_ctrl(ROOT_UP_RELAY, ON);
            relay_ctrl(HIGHT_FLOW_UP_SRELAY, ON);
            relay_ctrl(LOW_FLOW_UP_SRELAY, ON);
            relay_ctrl(DOWN_RELAY, ON);
        }
        break;
        case SYS_LEAK_MID:
        {
            relay_ctrl(ROOT_UP_RELAY, ON);
            relay_ctrl(HIGHT_FLOW_UP_SRELAY, ON);
            relay_ctrl(LOW_FLOW_UP_SRELAY, ON);
            relay_ctrl(DOWN_RELAY, OFF);
        }
        break;

        /**********************   end leak ctrl*************************/
        case NORMAL_OPERATION2:
        {
            relay_ctrl(ROOT_UP_RELAY, ON);
            relay_ctrl(flow_mode_relay_sel(), ON);
            relay_ctrl(DOWN_RELAY, ON);
        }
        break;
        case NORMAL_OPERATION:
        {
            relay_ctrl(ROOT_UP_RELAY, OFF);
            relay_ctrl(flow_mode_relay_sel(), OFF);
            relay_ctrl(DOWN_RELAY, OFF);
        }
        break;
        default:
        {
            ;//relay_ctrl(ROOT_UP_RELAY, OFF);
            //relay_ctrl(flow_mode_relay_sel(), ON);
            // relay_ctrl(DOWN_RELAY, ON);
        }

        break;

    }
}
void vavle_state()
{
    switch (verifer_dat.state)
    {
        case LINE_PURGED_START:
        {
//            relay_ctrl(ROOT_UP_RELAY, ON);
//            relay_ctrl(flow_mode_relay_sel(), OFF);
//            relay_ctrl(DOWN_RELAY, ON);
            get_calib()->valve = 0x0004;
        }
        break;
        case FLOW_SET_POINT_START:
        {
//            relay_ctrl(ROOT_UP_RELAY, ON);
//            relay_ctrl(flow_mode_relay_sel(), ON);
//            relay_ctrl(DOWN_RELAY, ON);
            if (get_config()->flow_mode == HIGH_FLOW)
            {
                get_calib()->valve = get_calib()->valve | 0x0001;
                get_calib()->valve = get_calib()->valve & 0xfffd;

            }

            else
            {
                get_calib()->valve = get_calib()->valve | 0x0002;
                get_calib()->valve = get_calib()->valve & 0xfffe;

            }
            get_calib()->valve = get_calib()->valve | 0x0004;

        }
        break;
        case FLOW_SET_POINT_SHOCK:
//        {
//            relay_ctrl(ROOT_UP_RELAY, ON);
//            relay_ctrl(flow_mode_relay_sel(), ON);
//            relay_ctrl(DOWN_RELAY, ON);
//        }
            if (get_config()->flow_mode == HIGH_FLOW)
            {
                get_calib()->valve = get_calib()->valve | 0x0001;
                get_calib()->valve = get_calib()->valve & 0xfffd;

            }

            else
            {
                get_calib()->valve = get_calib()->valve | 0x0002;
                get_calib()->valve = get_calib()->valve & 0xfffe;

            }
            get_calib()->valve = get_calib()->valve | 0x0004;

            break;

        case FLOW_SET_POINT_STEADY:
        {
//            relay_ctrl(ROOT_UP_RELAY, ON);
//            relay_ctrl(flow_mode_relay_sel(), ON);
//            relay_ctrl(DOWN_RELAY, OFF);
            if (get_config()->flow_mode == HIGH_FLOW)
            {
                get_calib()->valve = get_calib()->valve | 0x0001;
                get_calib()->valve = get_calib()->valve & 0xfffd;

            }

            else
            {
                get_calib()->valve = get_calib()->valve | 0x0002;
                get_calib()->valve = get_calib()->valve & 0xfffe;

            }
            get_calib()->valve = get_calib()->valve & 0xfffb;

        }
        break;
        case FLOW_SET_POINT_SHOCK2:
        {
//            relay_ctrl(ROOT_UP_RELAY, OFF);
//            relay_ctrl(flow_mode_relay_sel(), OFF);
//            relay_ctrl(DOWN_RELAY, OFF);
            get_calib()->valve = 0;

        }
        break;
        case FLOW_SET_POINT_STEADY2:
        {
//            relay_ctrl(ROOT_UP_RELAY, OFF);
//            relay_ctrl(flow_mode_relay_sel(), OFF);
//            relay_ctrl(DOWN_RELAY, ON);
            get_calib()->valve = 0x0004;

        }
        break;


        case PRESSURE_RISE_START:
        {
//            relay_ctrl(ROOT_UP_RELAY, ON);
//            relay_ctrl(flow_mode_relay_sel(), ON);
//            relay_ctrl(DOWN_RELAY, OFF);
            if (get_config()->flow_mode == HIGH_FLOW)
            {
                get_calib()->valve = get_calib()->valve | 0x0001;
                get_calib()->valve = get_calib()->valve & 0xfffd;

            }

            else
            {
                get_calib()->valve = get_calib()->valve | 0x0002;
                get_calib()->valve = get_calib()->valve & 0xfffe;

            }
            get_calib()->valve = get_calib()->valve & 0xfffb;


        }
        break;
        case PRESSURE_STEADY_START:
        {
//            relay_ctrl(ROOT_UP_RELAY, OFF);
//            relay_ctrl(flow_mode_relay_sel(), OFF);
//            relay_ctrl(DOWN_RELAY, OFF);
            // get_calib()->valve = 0;

        }
        case PRESSURE_STEADY_SHOCK:
        {
            // if (verifer_dat.valve_status == 1)
            {
                verifer_dat.valve_status = 0;
                get_calib()->valve = 0;

            }

        }

        break;
        case PURGE_START:
        {
//            relay_ctrl(ROOT_UP_RELAY, ON);
//            relay_ctrl(HIGHT_FLOW_UP_RELAY, ON);
//            relay_ctrl(DOWN_RELAY, ON);

            get_calib()->valve = get_calib()->valve | 0x0001;
            get_calib()->valve = get_calib()->valve | 0x0002;
            get_calib()->valve = get_calib()->valve | 0x0004;

        }
        break;
        /******************************leak check***********************/

        case DEV_LEAK_START:
        {
            get_calib()->valve = get_calib()->valve & 0xfffe;
            get_calib()->valve = get_calib()->valve & 0xfffd;
            get_calib()->valve = get_calib()->valve | 0x0004;
//            relay_ctrl(ROOT_UP_RELAY, OFF);
//            relay_ctrl(HIGHT_FLOW_UP_RELAY, OFF);
//          relay_ctrl(LOW_FLOW_UP_RELAY, OFF);
//            relay_ctrl(DOWN_RELAY, ON);
        }
        break;
        case DEV_LEAK_MID:
        {
            get_calib()->valve = 0;


//            relay_ctrl(ROOT_UP_RELAY, OFF);
//            relay_ctrl(HIGHT_FLOW_UP_RELAY, OFF);
//            relay_ctrl(LOW_FLOW_UP_RELAY, OFF);
//            relay_ctrl(DOWN_RELAY, OFF);
        }
        break;
        case SYS_LEAK_START:
        {
            get_calib()->valve = get_calib()->valve | 0x0001;
            get_calib()->valve = get_calib()->valve | 0x0002;
            get_calib()->valve = get_calib()->valve | 0x0004;

//            relay_ctrl(ROOT_UP_RELAY, ON);
//            relay_ctrl(HIGHT_FLOW_UP_RELAY, ON);
//          relay_ctrl(LOW_FLOW_UP_RELAY, OFF);
//            relay_ctrl(DOWN_RELAY, ON);
        }
        break;
        case SYS_LEAK_MID:
        {
            get_calib()->valve = get_calib()->valve | 0x0001;
            get_calib()->valve = get_calib()->valve | 0x0002;
            get_calib()->valve = get_calib()->valve & 0xfffb;
//            relay_ctrl(ROOT_UP_RELAY, ON);
//            relay_ctrl(HIGHT_FLOW_UP_RELAY, ON);
//            relay_ctrl(LOW_FLOW_UP_RELAY, OFF);
//            relay_ctrl(DOWN_RELAY, OFF);
        }
        break;

        case NORMAL_OPERATION:
        {
//            relay_ctrl(ROOT_UP_RELAY, OFF);
//            relay_ctrl(flow_mode_relay_sel(), OFF);
//            relay_ctrl(DOWN_RELAY, OFF);
            get_calib()->valve = 0;


        }
        break;
        case NORMAL_OPERATION2:
        case PRESSURE_STEADY_END:
        {
            get_calib()->valve = 0x00005;//0101

        }
        break;

        default:
        {
            ;//relay_ctrl(ROOT_UP_RELAY, OFF);
            //relay_ctrl(flow_mode_relay_sel(), ON);
            // relay_ctrl(DOWN_RELAY, ON);
        }

        break;

    }
}

float cali_pressure_slope()
{
    static uint32_t tick_start, tick_total;
    static unsigned int i;
    float slop;


    if (get_calib_slop()->tick_total >= SLOPE_PERIOD)
    {

        get_calib_slop()->slope = get_cdg_status()->pressure_filter_torr -
                                  get_calib_slop()->pressure_last;

        get_calib_slop()->slope = get_calib_slop()->slope /
                                  get_calib_slop()->tick_total;
        get_calib_slop()->pressure_last = get_cdg_status()->pressure_filter_torr;
        get_calib_slop()->tick_total = 0;
        get_calib_slop()->tick_start = get_verifier()->timer_tick;
    }
    else
    {
        get_calib_slop()->tick_total = get_verifier()->timer_tick -
                                       get_calib_slop()->tick_start;
        get_calib_slop()->tick_total = get_calib_slop()->tick_total / 1000.0;

    }

    return get_calib_slop()->slope;
}
float get_slope(void)
{
    return get_calib_slop()->slope;

}
void nozzle_delay_test()
{

    if (verifer_dat.pressure_steady_tTime >= verifer_dat.nozzle_testT &&
            verifer_dat.nozzle_testT > 0)
        verifer_dat.nozzle_tick = 1;
    else
    {
        verifer_dat.nozzle_tick = 0;

    }

}
unsigned char pressure_steday_proc(unsigned char state)
{
    unsigned char result;
    static float last_pressure;
//    static uint32_t tick_start, tick_total;
    float tmp, slope_thre;

    //  float error;

    get_verifier()->pre_steady_result = 1;

    if (verifer_dat.state == NORMAL_OPERATION)
    {
        get_verifier()->tick_start = get_verifier()->timer_tick;
    }
    if (verifer_dat.state == LINE_PURGED_START)
    {
        get_verifier()->tick_start = get_verifier()->timer_tick;
        verifer_dat.pressure_vac_thValue = get_cdg_status()->pressure_filter_torr;
        verifer_dat.error = verifer_dat.pressure_vac_thValue * 2;
        if (get_calib()->cail_nozzle_num == NOZZLE_H)
        {
            if (verifer_dat.error <= get_calib()->p20_pre_H
                    || verifer_dat.error <=  1.2
                    || get_config()->debug_cmd == DEBUG_STEADY)
            {
                get_verifier()->pre_steady_result = 0;
                get_config()->debug_cmd = 0;
                get_verifier()->tick_total = 0;
            }
        }


        else
        {
            if (verifer_dat.error <= get_calib()->p20_pre_L
                    || verifer_dat.error <=  1.2
                    || get_config()->debug_cmd == DEBUG_STEADY)
            {
                get_verifier()->pre_steady_result = 0;
                get_config()->debug_cmd = 0;
                get_verifier()->tick_total = 0;
            }
        }


    }



    if (verifer_dat.state == FLOW_SET_POINT_START)
    {
//        get_verifier()->tick_total = 0;
        last_pressure = get_cdg_status()->pressure_filter_torr;
//        get_verifier()->tick_start = get_verifier()->timer_tick;
//        get_verifier()->pre_steady_result = 1;
    }

//#if DEBUG_LOG1_ENABLE==0
    if (verifer_dat.state == FLOW_SET_POINT_SHOCK)
    {

        tmp = cali_pressure_slope();
        tmp = fabs(tmp);
         get_verifier()->slope = tmp;
        if (get_calib()->cali_flow <= FLOW_THRE)
            slope_thre = SLOPE_THRE3;
        else
            slope_thre = SLOPE_THRE;
        if (tmp >= slope_thre)
        {

            get_verifier()->tick_start = get_verifier()->timer_tick;
            get_verifier()->tick_total = 0;
            get_config()->debug_cmd = 0;

            if (last_pressure >= 0.85 * FULL_SCALE
                    && (get_cdg_status()->pressure_filter_torr - last_pressure) >= 1)
                get_verifier()->pre_steady_result = 2;//over range
            last_pressure = get_cdg_status()->pressure_filter_torr;
        }
        else
        {
            if (get_calib()->cali_flow <= FLOW_SREADY_THRE)
            {
                if (get_verifier()->tick_total >= FLOW_STEADY_TIME2)
                {
                    get_verifier()->pre_steady_result = 0;
                    get_config()->debug_cmd = 0;
                    verifer_dat.pressure_flow_steady = get_cdg_status()->pressure_filter_torr;
                }
                else
                    get_verifier()->tick_total = get_verifier()->timer_tick -
                                                 get_verifier()->tick_start;

            }
            else
            {
                if (get_verifier()->tick_total >= FLOW_STEADY_TIME)
                {
                    get_verifier()->pre_steady_result = 0;
                    get_config()->debug_cmd = 0;
                    verifer_dat.pressure_flow_steady = get_cdg_status()->pressure_filter_torr;
                }
                else
                    get_verifier()->tick_total = get_verifier()->timer_tick -
                                                 get_verifier()->tick_start;

            }

        }

    }
//#endif

    if (verifer_dat.state == PRESSURE_RISE_START)
    {

        get_verifier()->tick_total = 0;
        verifer_dat.error = get_cdg_status()->pressure_nofilter_torr -
                            get_calib()->targe_p;
//        if (verifer_dat.error < 0 && (verifer_dat.error < 0
//                                      && get_config()->debug_cmd != DEBUG_STEADY) &&
//                verifer_dat.nozzle_tick == 0)
        if (verifer_dat.error < 0 && (verifer_dat.error < 0
                                      && get_config()->debug_cmd != DEBUG_STEADY))

            //if ((verifer_dat.error <= get_config()->pressure_error || verifer_dat.error < 0)
            //  && get_config()->debug_cmd != DEBUG_STEADY)
        {
            get_verifier()->tick_start = get_verifier()->timer_tick;

        }


        else
        {
            get_verifier()->pre_steady_result = 0;
            get_config()->debug_cmd = 0;
        }

    }
//    if (verifer_dat.state == PRESSURE_STEADY_START)
//    {
//        get_verifier()->tick_total = 0;
//        get_verifier()->pre_steady_result = 1;
//        get_verifier()->tick_start = get_verifier()->timer_tick;
//
//    }
    if (verifer_dat.state == PRESSURE_STEADY_SHOCK)
    {

        float tmp;
        tmp = cali_pressure_slope();
        tmp = fabs(tmp);

        if (tmp >= SLOPE_THRE2)
        {
            get_verifier()->tick_start = get_verifier()->timer_tick;
        }
        else
        {
            if (get_verifier()->tick_total >= STEADY_TIME2)
            {
                get_verifier()->pre_steady_result = 0;
                get_config()->debug_cmd = 0;
                get_verifier()->tick_total = 0;
                verifer_dat.pressure_steady = get_cdg_status()->pressure_filter_torr;

            }
            else
            {
                get_verifier()->tick_total = get_verifier()->timer_tick -
                                             get_verifier()->tick_start;
                //if (get_verifier()->tick_total >= 100)
                //  verifer_dat.valve_status = 1;

            }


        }

    }

    if (verifer_dat.state == DEV_LEAK_IDL ||
            verifer_dat.state == SYS_LEAK_IDL)
    {

        tmp = cali_pressure_slope();
        tmp = fabs(tmp);
        slope_thre = SLOPE_THRE;
        if (tmp >= slope_thre)
        {

            get_verifier()->tick_start = get_verifier()->timer_tick;
            get_verifier()->tick_total = 0;
            get_config()->debug_cmd = 0;

            if (last_pressure >= 0.85 * FULL_SCALE
                    && (get_cdg_status()->pressure_filter_torr - last_pressure) >= 1)
                get_verifier()->pre_steady_result = 2;//over range
            last_pressure = get_cdg_status()->pressure_filter_torr;
        }
        else
        {
            if (get_verifier()->tick_total >= FLOW_STEADY_TIME)
            {
                get_verifier()->pre_steady_result = 0;
                get_config()->debug_cmd = 0;
                //verifer_dat.pressure_flow_steady = get_cdg_status()->pressure_filter_torr;
            }
            else
                get_verifier()->tick_total = get_verifier()->timer_tick -
                                             get_verifier()->tick_start;
        }

    }
    return get_verifier()->pre_steady_result;
}

/*
NORMAL_OPERATION--->LINE_PURGED_START-->FLOW_SET_POINT_START--->
FLOW_SET_POINT_SHOCK--->FLOW_SET_POINT_STEADY--->PRESSURE_RISE_START
--->PRESSURE_STEADY_START--->NORMAL_OPERATION

*/

void pressure_state_proc(void)
{
    cali_pressure_slope();

    switch (verifer_dat.state)
    {
        case NORMAL_OPERATION:
            pressure_steday_proc(NORMAL_OPERATION);
            verifer_dat.state = NORMAL_OPERATION;
            //timer_stop();
            set_led(2000);
            break;
        case LINE_PURGED_START:
            set_led(1000);
            verifer_dat.state = FLOW_SET_POINT_START;
            if (pressure_steday_proc(VACUUM_STEADY) == 0)//vacuum steady
            {
                verifer_dat.state = FLOW_SET_POINT_START;


                if (DEBUG_LOG1_ENABLE)
                    printf("FLOW_SET_POINT_START------>\r\n");
            }

            break;

        case FLOW_SET_POINT_START:
            pressure_steday_proc(FLOW_STEADY);
            {
                verifer_dat.state = FLOW_SET_POINT_SHOCK;
                verifer_dat.update = 0;
                get_verifier()->tick_total = 0;
                get_verifier()->tick_start = get_verifier()->timer_tick;
                get_verifier()->pre_steady_result = 1;

            }
            set_led(500);
            break;
        case FLOW_SET_POINT_STEADY2:
            if (pressure_steday_proc(FLOW_SET_POINT_STEADY2) == 0)//
            {

                {
                    verifer_dat.update = 0;

                    verifer_dat.state = FLOW_SET_POINT_START;
                }
            }

            break;


        case FLOW_SET_POINT_SHOCK:
            if (get_calib()->progress <= 1)
                get_calib()->progress = 1;
            verifer_dat.pressure_rise_sTime  = verifer_dat.timer_tick;
            if (pressure_steday_proc(FLOW_STEADY) == 2)//over range
            {
                verifer_dat.state = NORMAL_OPERATION;
                get_calib()->state = 9;
            }
            else
            {
                if (pressure_steday_proc(FLOW_STEADY) == 0)//flow steady
                {

                    {
                        verifer_dat.update = 0;
                        verifer_dat.state = FLOW_SET_POINT_STEADY;

                        if (DEBUG_LOG1_ENABLE)
                            printf("FLOW_SET_POINT_STEADY----->\r\n");
                    }
                }

            }

            break;


        case FLOW_SET_POINT_STEADY:
            if (perror_proc() == 1)//error below
            {
                if (verifer_dat.sw_flag == 0)
                {
                    if (get_calib()->cail_nozzle_num == NOZZLE_H)
                    {
                        verifer_dat.sw_flag = 1;
                        verifer_dat.update = 0;
                        
                        get_calib()->cail_nozzle_num = NOZZLE_L;
                        press_cal_proc(get_calib()->cail_nozzle_num);
                        nozzle_sel();
						#if (VAVLE_DELAY_ENABLE == 1)
					    verifer_dat.state = FLOW_WAIT;
						#else 
						 verifer_dat.state = FLOW_SET_POINT_START;
						#endif
                        get_verifier()->tick_total = 0;
					    get_verifier()->tick_start = get_verifier()->timer_tick;
                    }
                    else
                    {
                        verifer_dat.sw_flag = 0;
                        verifer_dat.pressure_rise_sTime  = verifer_dat.timer_tick;
                        verifer_dat.flow_start_temp_averT = get_temperature()->average_T;
                        //verifer_dat.update = 0;
                        get_verifier()->timeout_tick_start = get_verifier()->timer_tick;
                        get_verifier()->timeout_tick_stop = get_verifier()->timer_tick;
                        verifer_dat.state = PRESSURE_RISE_START;
                        //get_calib()->cail_nozzle_num = NOZZLE_H;

                    }
                }
                else
                {
                    verifer_dat.state  = NORMAL_OPERATION2;
                    get_calib()->state = 7;
                    verifer_dat.sw_flag = 0;
                }

            }
            else
            {
                verifer_dat.pressure_rise_sTime  = verifer_dat.timer_tick;
                verifer_dat.flow_start_temp_averT = get_temperature()->average_T;
                //verifer_dat.update = 0;
                get_verifier()->timeout_tick_start = get_verifier()->timer_tick;
                get_verifier()->timeout_tick_stop = get_verifier()->timer_tick;
                verifer_dat.state = PRESSURE_RISE_START;
                //get_calib()->progress = 0.05;
            }
            if (DEBUG_LOG1_ENABLE)
                printf("PRESSURE_RISE_START----->\r\n");
            set_led(300);
            break;
		#if (VAVLE_DELAY_ENABLE == 1)
		case FLOW_WAIT:
			if (get_verifier()->tick_total >= FLOW_WAIT_TIME)//over range
            {
				verifer_dat.state = FLOW_SET_POINT_START;
            }
			else
				get_verifier()->tick_total = get_verifier()->timer_tick-
					get_verifier()->tick_start;				

		break;
		#endif
        case PRESSURE_RISE_START:
            set_led(100);
            // verifer_dat.pressure_steady_eTime  = verifer_dat.timer_tick;
            // verifer_dat.pressure_steady_averT = get_temperature()->average_T;
            // verifer_dat.pressure_steady_tTime = verifer_dat.pressure_steady_eTime -
            //verifer_dat.pressure_rise_sTime;
            verifer_dat.pressure_steady_eTime  = verifer_dat.timer_tick;
            verifer_dat.pressure_steady_averT = get_temperature()->average_T;
            verifer_dat.pressure_steady_tTime = verifer_dat.pressure_steady_eTime -
                                                verifer_dat.pressure_rise_sTime;
            nozzle_delay_test();
            if (pressure_steday_proc(PRESSURE_STEADY) == 0)//pressure steady
            {

                {
                    verifer_dat.update = 0;
                    verifer_dat.state = PRESSURE_STEADY_START;
                    //get_calib()->progress = 0.08;
//                    if (verifer_dat.pressure_steady_tTime <= NOZZLE_SW_TIME &&
//                            verifer_dat.nozzle_tick == 0)//nozzle_tick = 1 test mode

                    if (verifer_dat.pressure_steady_tTime <=
                            NOZZLE_SW_TIME) //nozzle_tick = 1 test mode
                    {

                        if (get_calib()->cail_nozzle_num == NOZZLE_H)
                        {

                            get_calib()->cail_nozzle_num = NOZZLE_L;
                            press_cal_proc(get_calib()->cail_nozzle_num);
                            nozzle_sel();
                            if (get_calib()->p1 <= MAX_PRESSURE_WARN)
                            {
                                verifer_dat.update = 0;
                                verifer_dat.sw_flag = 1;
                                verifer_dat.state = FLOW_SET_POINT_START;

                            }
                            else
                            {
                                get_calib()->cail_nozzle_num = NOZZLE_H;
                                press_cal_proc(get_calib()->cail_nozzle_num);
                                nozzle_sel();

                            }

                        }

                    }

                    if (DEBUG_LOG1_ENABLE)
                        printf("pressure steady----->\r\n");
                }
            }
            else
            {
                if (pressORTime_steady_proc() == 0)//time out
                {
                    if (p20error_proc() == 0)

                    {
                        verifer_dat.update = 0;
                        verifer_dat.state = PRESSURE_STEADY_START;
//                      verifer_dat.pressure_steady_eTime  = verifer_dat.timer_tick;
//                      verifer_dat.pressure_steady_averT = get_temperature()->average_T;
//                      verifer_dat.pressure_steady_tTime = verifer_dat.pressure_steady_eTime -
//                                                verifer_dat.pressure_rise_sTime;
                    }

                }
                else if (pressORTime_steady_proc() == 2) //time out
                {
                    verifer_dat.state = NORMAL_OPERATION;
                    get_calib()->state = 8;

                }
                else
                {;}

            }


            break;
        case PRESSURE_STEADY_START:
            pressure_steday_proc(PRESSURE_STEADY);

            get_verifier()->tick_total = 0;
            get_verifier()->pre_steady_result = 1;
            get_verifier()->tick_start = get_verifier()->timer_tick;

            verifer_dat.pressure_steady_averT = get_temperature()->average_T;
            //verifer_dat.update = 0;

            {
                verifer_dat.update = 0;
                verifer_dat.state = PRESSURE_STEADY_SHOCK;
            }
            if (DEBUG_LOG1_ENABLE)
                printf("pressure steady start----->\r\n");
            break;

        case PRESSURE_STEADY_SHOCK:
            if (pressure_steday_proc(PRESSURE_STEADY_SHOCK) == 0)//pressure steady
            {

                {
                    verifer_dat.state = PRESSURE_STEADY_END;
                }
                if (DEBUG_LOG1_ENABLE)
                    printf("pressure steady shock----->\r\n");
            }

            break;
        case PRESSURE_STEADY_END:
            set_led(1000);

            // timer_stop();
            if (get_verifier()->pressure_steady_tTime <= CALI_TIME_THRE)
            {
                if (verifer_dat.cali_count == 0)
                {
                    verifer_dat.cali_count = 4;//cali count set when time is below thres
                    get_calib()->index = 0;
                }
            }
            else
            {
                if (get_calib()->cali_flow <= CALI_FLOW_THRE)
                {
                    if (verifer_dat.cali_count == 0)
                    {
                        verifer_dat.cali_count = 2;//cali count set when time is below thres
                        get_calib()->index = 0;
                    }

                }


            }
            flow_proc();

            //  timer_start();

            if (verifer_dat.cali_count == 0)
            {
                {
                    verifer_dat.update = 0;
                    verifer_dat.state = NORMAL_OPERATION;
                    verifer_dat.sw_flag = 0;
                }

            }
            else
            {
                if (get_calib()->index < (verifer_dat.cali_count))
                {
                    {
                        verifer_dat.update = 0;
                        verifer_dat.state = LINE_PURGED_START;
                    }
                    verifer_dat.sw_flag = 0;

                }
                else
                {
                    {
                        verifer_dat.update = 0;

                        get_calib()->index = 0;
                        verifer_dat.state = NORMAL_OPERATION;
                        verifer_dat.sw_flag = 0;
                    }

                }
                get_calib_slop()->tick_start = get_verifier()->timer_tick;
                get_calib_slop()->tick_total = get_verifier()->timer_tick;


            }


            if (DEBUG_LOG1_ENABLE)
                printf("pressure steady----->\r\n");

            break;
        case PURGE_START:

            get_calib()->pro_now_time = verifer_dat.timer_tick -
                                        verifer_dat.purge_tick;
            get_calib()->progress =  get_calib()->pro_now_time /
                                     get_calib()->pro_total_time * 100;
            if (get_calib()->progress >= 100)
                get_calib()->progress = 100;
            else if (get_calib()->progress  <= 0)
                get_calib()->progress = 0;

            if ((get_calib()->pro_now_time) >=
                    verifer_dat.purge_time * 1000)
            {

                if (verifer_dat.purge_cycle_count > 0)
                    verifer_dat.purge_cycle_count--;
                if (verifer_dat.purge_cycle_count <= 0)
                {
                    verifer_dat.state = PURGE_STOP;
                    verifer_dat.purge_status = 0;
                }
                verifer_dat.purge_tick = verifer_dat.timer_tick;

            }

            break;
        case PURGE_STOP:

            verifer_dat.purge_status = 0;
            verifer_dat.state = NORMAL_OPERATION;
            if (DEBUG_LOG1_ENABLE)
                printf("pressure steady----->\r\n");

            break;
        /***************************leak state**************************************/
        case DEV_LEAK_START:
            verifer_dat.purge_tick = verifer_dat.timer_tick;
            get_verifier()->tick_total = 0;
            get_verifier()->pre_steady_result = 1;
            get_verifier()->tick_start = get_verifier()->timer_tick;

            verifer_dat.state = DEV_LEAK_IDL;
            get_calib()->leak_target_p = LEAK_MAX_PRE;
            break;
        case DEV_LEAK_IDL:

            get_calib()->leak_target_p = LEAK_MAX_PRE;

            //verifer_dat.purge_tick = verifer_dat.timer_tick;
            if ((verifer_dat.timer_tick - verifer_dat.purge_tick) >= LEAK_VAVLE_OPEN_TIME)
            {
                if (get_cdg_status()->pressure_filter_torr <=
                        get_calib()->leak_target_p)//flow steady
                {
                    verifer_dat.state = DEV_LEAK_MID;
                    verifer_dat.purge_tick = verifer_dat.timer_tick;

                }

            }

            get_calib()->device_leak_rate = get_slope();

            break;

        case DEV_LEAK_MID:
            get_calib()->pro_now_time = verifer_dat.timer_tick -
                                        verifer_dat.purge_tick;
            get_calib()->progress =  get_calib()->pro_now_time /
                                     get_calib()->pro_total_time * 100;
            if (get_calib()->progress >= 100)
                get_calib()->progress = 100;
            else if (get_calib()->progress  <= 0)
                get_calib()->progress = 0;


            if ((get_calib()->pro_now_time) >=
                    verifer_dat.purge_time * 1000)
            {
                verifer_dat.state = DEV_LEAK_END;
            }
            else
            {
                get_calib()->device_leak_rate = get_slope();
            }
            break;
        case DEV_LEAK_END:

            verifer_dat.update = 0;
            verifer_dat.state = NORMAL_OPERATION;
            break;
        case SYS_LEAK_START:
            verifer_dat.purge_tick = verifer_dat.timer_tick;
            get_verifier()->tick_total = 0;
            get_verifier()->pre_steady_result = 1;
            get_verifier()->tick_start = get_verifier()->timer_tick;
            verifer_dat.state = SYS_LEAK_IDL;
            get_calib()->leak_target_p = LEAK_MAX_PRE;


            break;
        case SYS_LEAK_IDL:
            get_calib()->leak_target_p = LEAK_MAX_PRE;

            //verifer_dat.purge_tick = verifer_dat.timer_tick;
            if ((verifer_dat.timer_tick - verifer_dat.purge_tick) >= LEAK_VAVLE_OPEN_TIME)
            {
                if (get_cdg_status()->pressure_filter_torr <=
                        get_calib()->leak_target_p)//flow steady
                {
                    verifer_dat.state = SYS_LEAK_MID;
                    verifer_dat.purge_tick = verifer_dat.timer_tick;


                }

            }

            get_calib()->sys_leak_rate = get_slope();

            break;

        case SYS_LEAK_MID:
            get_calib()->pro_now_time = verifer_dat.timer_tick -
                                        verifer_dat.purge_tick;
            get_calib()->progress =  get_calib()->pro_now_time /
                                     get_calib()->pro_total_time * 100;
            if (get_calib()->progress >= 100)
                get_calib()->progress = 100;
            else if (get_calib()->progress  <= 0)
                get_calib()->progress = 0;

            if ((get_calib()->pro_now_time) >=
                    verifer_dat.purge_time * 1000)
            {
                verifer_dat.state = SYS_LEAK_END;
            }
            else
            {
                get_calib()->sys_leak_rate = get_slope();
            }
            break;
        case SYS_LEAK_END:

            verifer_dat.update = 0;
            verifer_dat.state = NORMAL_OPERATION;
            break;

        /*******************************end leak check*******************************/
        case DRAIN_START:

            ;

            break;
        case VAVLE_SCTRL:

            ;

            break;

        case DRAIN_END:

            verifer_dat.state = NORMAL_OPERATION;
            if (DEBUG_LOG1_ENABLE)
                printf("pressure steady----->\r\n");

            break;

    }



}

void pressure_ov_proc(void)
{
    static float last_filter_press;
    if ((get_cdg_status()->pressure_filter_torr - last_filter_press) >= 1 &&
            get_cdg_status()->pressure_nofilter_torr >= get_calib()->high_press)
    {
        if (get_verifier()->state > LINE_PURGED_START
                && get_verifier()->state < PH_SCTRL)
            get_calib()->state = 5;
    }
    last_filter_press = get_cdg_status()->pressure_filter_torr;
}
void verifier_fault_proc(void)
{
    if (get_flash_status() == SUCESS)
    {
        ;
    }
    else
    {

        get_calib()->fault = 3;
        set_led(0xffffffff);
    }


    if (get_calib()->state == 4)
    {
        get_verifier()->state = NORMAL_OPERATION;
        get_calib()->state = 0;
        get_calib()->fault = 1;
        // printf("over range\r\n");
    }

    if (get_calib()->state == 5)
    {
        get_verifier()->state = PH_SCTRL;
        get_calib()->state = 0;
        get_calib()->fault = 2;
        //printf("pressure high\r\n");
    }
    if (get_calib()->state == 6)
    {
        get_verifier()->state = NORMAL_OPERATION2;
        get_calib()->state = 0;
        //get_calib()->fault = 3;
    }
    if (get_calib()->state == 7)
    {
        //get_verifier()->state = NORMAL_OPERATION;
        get_calib()->state = 0;
        get_calib()->fault = 4;//�л�����ѹ��������Ҫ��
        //printf("pressure high\r\n");
    }
    if (get_calib()->state == 8)
    {
        get_calib()->state = 0;
        get_calib()->fault = 5;//��ѹ��ʱ
    }
    if (get_calib()->state == 9)
    {
        get_calib()->state = 0;
        get_calib()->fault = 6;//��ѹ����Χ
    }
    if (get_calib()->state == 10)
    {
        get_calib()->state = 0;
        get_calib()->fault = 7;//��������Һ��ѹ��
    }
    if (get_calib()->state == 11)
    {
        get_calib()->state = 0;
        get_calib()->fault = 8;//С�����ѹ��
    }
    if (get_calib()->state == 12)
    {
        get_calib()->state = 0;
        get_calib()->fault = 9;//
    }
    if (get_calib()->state == 13)
    {
        get_calib()->state = 0;
        get_calib()->fault = 10;//
    }
    if (get_calib()->state == 14)
    {
        get_calib()->state = 0;
        get_calib()->fault = 11;//
    }
    if (get_calib()->state == 15)
    {
        get_calib()->state = 0;
        get_calib()->fault = 12;//
    }

#if SLOPE_DEBUG == 0
    switch (get_verifier()->state)
    {
        case NORMAL_OPERATION:
        case LINE_PURGED_START:
        case PRESSURE_STEADY_SHOCK:
            // if (get_verifier()->mfc_sig == 0)
        {
            // printf("close mfc\r\n");
            get_verifier()->mfc_sig = 0;
        }
        break;
        case FLOW_SET_POINT_START:
            // if (get_verifier()->mfc_sig == 1)
        {
            //printf("open mfc\r\n");
            get_verifier()->mfc_sig = 1;
        }
        break;
    }
#endif
}
void debug_log(void)
{
    static uint32_t tick = 0;
    if ((verifer_dat.timer_tick - tick) >= 1000 && get_config()->debug_log == 1)
    {
        tick = verifer_dat.timer_tick;
        printf("%.3f  %u  %u  %.3f  %.3f	%.3f  %.3f	%.3f %u  %u  %u   %.5f    %.5f   %.5f   %u\r\n",
               get_calib()->cali_flow,
               get_calib()->gas_type,
               get_verifier()->state,
               get_cdg_status()->pressure_filter_torr,
               get_verifier()->pressure_steady_tTime / 1000.0,
               get_calib()->p20,
               get_calib()->p21,
               get_calib()->p1,
               get_calib()->cail_nozzle_num,
               get_calib()->valve,
               get_calib()->fault,
               get_calib()->F,
               get_verifier()->slope,
               get_calib()->targe_p,
               get_calib()->update);
    }


}
void verifier_proc(void)
{
    //static uint32_t ttick = 0;
    adc_proc();
    sensor_cdg_proc();//cdf data analy
    led_ctrl_irq();

    if (get_flash_status() == SUCESS)
    {
        cali_proc();
        vavle_state();
        pressure_ov_proc();
        verfier_state_proc();
    }
    debug_log();
    verifier_fault_proc();

}

void verfier_state_proc()
{
    if (get_flash_status() == SUCESS)
    {

        pressure_ctrl();
        pressure_state_proc();//state machine
#if SLOPE_DEBUG == 1
        cali_pressure_slope();
#endif
    }

}
void valve_test(void)
{
    relay_ctrl(ROOT_UP_RELAY, ON);
    relay_ctrl(HIGHT_FLOW_UP_RELAY, ON);
    relay_ctrl(DOWN_RELAY, ON);

}

