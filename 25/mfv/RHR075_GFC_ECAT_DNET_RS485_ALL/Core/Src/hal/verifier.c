#include "verifier.h"
#include "relay_driver.h"
#include "config.h"
#include "cdg.h"
#include "adc.h"
#include "debug.h"
#include "math.h"
#include "calibration.h"
#include "flash.h"


VERIFIER_STRU verifer_dat;



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

}

VERIFIER_STRU *get_verifier()
{
    return &verifer_dat;
}


/*******************************************************


********************************************************/
void pressure_time_proc(void)
{
    if (verifer_dat.state == FLOW_SET_POINT_STEADY)//close downstream valve
    {
        verifer_dat.pressure_rise_sTime  = HAL_GetTick();
        verifer_dat.pressure_flow_steady = get_cdg_status()->pressure_filter_torr;
        verifer_dat.flow_start_temp_averT = get_temperature()->average_T;

    }
    else if (verifer_dat.state == PRESSURE_RISE_START) //target time
    {
        verifer_dat.pressure_steady_eTime  = HAL_GetTick();
        verifer_dat.pressure_steady_tTime = verifer_dat.pressure_steady_eTime -
                                            verifer_dat.pressure_rise_sTime;
        verifer_dat.pressure_steady_averT = get_temperature()->average_T;
        verifer_dat.pressure_steady = get_cdg_status()->pressure_filter_torr;

        if (DEBUG_LOG1_ENABLE)
            printf("pressure_steady_tTime = %d\r\n", verifer_dat.pressure_steady_tTime);
    }

    else if (verifer_dat.state ==
             PRESSURE_STEADY_START) //pressure steady,close upstream valve
    {
        verifer_dat.pressure_steady_eTime  = HAL_GetTick();
        verifer_dat.pressure_steady_tTime = verifer_dat.pressure_steady_eTime -
                                            verifer_dat.pressure_rise_sTime;
        verifer_dat.pressure_steady_averT = get_temperature()->average_T;

        if (DEBUG_LOG1_ENABLE)
            printf("pressure_steady_tTime = %d\r\n", verifer_dat.pressure_steady_tTime);
    }
    else if (verifer_dat.state ==
             PRESSURE_STEADY_END)//pressure steady,close upstream valve
    {
        verifer_dat.pressure_steady = get_cdg_status()->pressure_filter_torr;
        if (DEBUG_LOG1_ENABLE)
            printf("pressure_steady_tTime = %d\r\n", verifer_dat.pressure_steady_tTime);
        flow_proc();
        verifer_dat.pressure_steady_peak = 0;
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

        case NORMAL_OPERATION:
        {
            relay_ctrl(ROOT_UP_RELAY, OFF);
            relay_ctrl(flow_mode_relay_sel(), OFF);
            relay_ctrl(DOWN_RELAY, OFF);
        }
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
            get_calib()->valve = 0;

        }
        break;
        case PURGE_START:
        {
//            relay_ctrl(ROOT_UP_RELAY, ON);
//            relay_ctrl(HIGHT_FLOW_UP_RELAY, ON);
//            relay_ctrl(DOWN_RELAY, ON);

            get_calib()->valve = get_calib()->valve | 0x0001;
            get_calib()->valve = get_calib()->valve & 0xfffd;
            get_calib()->valve = get_calib()->valve | 0x0004;

        }
        break;
        case DRAIN_START:
        {
            relay_ctrl(ROOT_UP_RELAY, ON);
            relay_ctrl(HIGHT_FLOW_UP_RELAY, ON);
            relay_ctrl(DOWN_RELAY, OFF);

        }
        break;

        case NORMAL_OPERATION:
        {
//            relay_ctrl(ROOT_UP_RELAY, OFF);
//            relay_ctrl(flow_mode_relay_sel(), OFF);
//            relay_ctrl(DOWN_RELAY, OFF);
            get_calib()->valve = 0;


        }
        case PRESSURE_STEADY_END:
        {
			get_calib()->valve = 0x00005;//0101

        }

        default:
        {
            ;//relay_ctrl(ROOT_UP_RELAY, OFF);
            //relay_ctrl(flow_mode_relay_sel(), ON);
            // relay_ctrl(DOWN_RELAY, ON);
        }

        break;

    }
}

void cali_pressure_slope()
{
    static uint32_t tick_start, tick_total;
    static unsigned int i;
    float slop[10];
    if (verifer_dat.state == PRESSURE_RISE_START|| 
		verifer_dat.state == PRESSURE_STEADY_SHOCK||
		verifer_dat.state == PRESSURE_STEADY_END
           )
    {
        if (get_calib_slop()->tick_total >= 0.2)
        {

            slop[i] = get_cdg_status()->pressure_filter_torr -
                      get_calib_slop()->pressure_last[i];

            slop[i] = slop[i] / get_calib_slop()->tick_total;
            printf("%.3f  %.3f  %.3f\r\n", get_cdg_status()->pressure_filter_torr,
                   get_calib_slop()->pressure_last[0], slop[i]);
            get_calib_slop()->pressure_last[0] = get_cdg_status()->pressure_filter_torr;
            //i++;
            get_calib_slop()->tick_total = 0;
            get_calib_slop()->tick_start = HAL_GetTick();
            if (i >= 10)
                i = 0;
        }
        else
        {
            get_calib_slop()->tick_total = HAL_GetTick() - get_calib_slop()->tick_start;
            get_calib_slop()->tick_total = get_calib_slop()->tick_total / 1000.0;

        }

    }
    else
    {
        i = 0;
        get_calib_slop()->pressure_last[0] = get_cdg_status()->pressure_filter_torr;
        get_calib_slop()->tick_start = HAL_GetTick();

    }
}
unsigned char pressure_steday_proc(unsigned char state)
{
    unsigned char result;
    static float last_pressure;
    static uint32_t tick_start, tick_total;

    //  float error;

    result = 1;
    
    if (verifer_dat.state == NORMAL_OPERATION)
    {
        tick_start = HAL_GetTick();
    }
    if (verifer_dat.state == LINE_PURGED_START)
    {
        tick_start = HAL_GetTick();
        verifer_dat.pressure_vac_thValue = get_cdg_status()->pressure_filter_torr;
        verifer_dat.error = verifer_dat.pressure_vac_thValue * 2;
        // if (verifer_dat.error <= get_config()->pressure_error
        //        || get_config()->debug_cmd == DEBUG_STEADY)
        if (get_calib()->cail_nozzle_num == NOZZLE_H)
        {
            if (verifer_dat.error <= get_calib()->p20_pre_H
                    || verifer_dat.error <=  1.2
                    || get_config()->debug_cmd == DEBUG_STEADY)
            {
                result = 0;
                get_config()->debug_cmd = 0;
                tick_total = 0;
            }
        }


        else
        {
            if (verifer_dat.error <= get_calib()->p20_pre_L
                    || verifer_dat.error <=  1.2
                    || get_config()->debug_cmd == DEBUG_STEADY)
            {
                result = 0;
                get_config()->debug_cmd = 0;
                tick_total = 0;
            }
        }


    }
    if (verifer_dat.state == FLOW_SET_POINT_START)
    {
        tick_total = 0;
        last_pressure = get_cdg_status()->pressure_filter_torr;
        tick_start = HAL_GetTick();
        result = 1;
    }
//#if DEBUG_LOG1_ENABLE==0
    if (verifer_dat.state == FLOW_SET_POINT_SHOCK)
    {

        verifer_dat.error = get_cdg_status()->pressure_filter_torr - last_pressure;
        verifer_dat.error = fabs(verifer_dat.error);
        if (verifer_dat.error >= get_config()->flow_steady_perror
                && get_config()->debug_cmd != DEBUG_STEADY)
        {

            tick_start = HAL_GetTick();
            get_config()->debug_cmd = 0;
            last_pressure = get_cdg_status()->pressure_filter_torr;
            if (last_pressure >= 0.85 * FULL_SCALE)
                result = 2;//over range
        }
        else
        {
            if (tick_total >= STEADY_TIME)
            {
                result = 0;
                get_config()->debug_cmd = 0;
                // tick_total = 0;
            }
            else
                tick_total = HAL_GetTick() - tick_start;
        }

    }
//#endif

    if (verifer_dat.state == PRESSURE_RISE_START)
    {

        tick_total = 0;

        verifer_dat.error = get_cdg_status()->pressure_filter_torr -
                            get_calib()->targe_p;
        verifer_dat.pressure_steady_peak = get_cdg_status()->pressure_nofilter_torr;
        if ((verifer_dat.error <= get_config()->pressure_error || verifer_dat.error < 0)
                && get_config()->debug_cmd != DEBUG_STEADY)
        {
            tick_start = HAL_GetTick();

        }

        else
        {
            result = 0;
            get_config()->debug_cmd = 0;
        }

    }
    if (verifer_dat.state == PRESSURE_STEADY_SHOCK)
    {

        static float last_pressure;
        verifer_dat.error = get_cdg_status()->pressure_filter_torr - last_pressure;
        verifer_dat.error = fabs(verifer_dat.error);
        if (verifer_dat.pressure_steady_peak < get_cdg_status()->pressure_filter_torr)
            verifer_dat.pressure_steady_peak = get_cdg_status()->pressure_filter_torr;
        if (verifer_dat.error >= get_config()->pressure_error
                && get_config()->debug_cmd != DEBUG_STEADY)
        {
            tick_start = HAL_GetTick();
            last_pressure = get_cdg_status()->pressure_filter_torr;
        }
        else
        {
            if (tick_total >= STEADY_TIME2)
            {
                result = 0;
                get_config()->debug_cmd = 0;
                tick_total = 0;

            }
            else
                tick_total = HAL_GetTick() - tick_start;

        }
    }


    return result;
}

/*
NORMAL_OPERATION--->LINE_PURGED_START-->FLOW_SET_POINT_START--->
FLOW_SET_POINT_SHOCK--->FLOW_SET_POINT_STEADY--->PRESSURE_RISE_START
--->PRESSURE_STEADY_START--->NORMAL_OPERATION

*/

void pressure_state_proc(void)
{

    switch (verifer_dat.state)
    {
        case NORMAL_OPERATION:
            verifer_dat.state = NORMAL_OPERATION;
            set_led(2000);
            break;
        case LINE_PURGED_START:
            set_led(1000);
            if (pressure_steday_proc(VACUUM_STEADY) == 0)//vacuum steady
            {
                verifer_dat.state = FLOW_SET_POINT_START;


                if (DEBUG_LOG1_ENABLE)
                    printf("FLOW_SET_POINT_START------>\r\n");
            }
            else
            {
                static uint32_t tick = 0;
                if ((HAL_GetTick() - tick) >= 300)
                {
                    tick = HAL_GetTick();
                    if (DEBUG_LOG1_ENABLE)
                        printf("VACUUM pressure=%f,T=%f,elaspse time=%d\r\n",
                               get_cdg_status()->pressure_filter_torr, get_temperature()->average_T,
                               (HAL_GetTick() - verifer_dat.vac_sTime));

                }

            }

            break;
//        case IDLE_STATE_2:
//            verifer_dat.state = FLOW_SET_POINT_START;
//            if (DEBUG_LOG1_ENABLE)
//                ;// printf("FLOW_SET_POINT_SHOCK\r\n");
//            set_led(500);
//            break;
        case FLOW_SET_POINT_START:
            pressure_steday_proc(FLOW_STEADY);
            verifer_dat.state = FLOW_SET_POINT_SHOCK;
            if (DEBUG_LOG1_ENABLE)
                ;// printf("FLOW_SET_POINT_SHOCK\r\n");
            set_led(500);
            break;

        case FLOW_SET_POINT_SHOCK:
            //set_led(1000);


            if (pressure_steday_proc(FLOW_STEADY) == 2)//over range
            {
                verifer_dat.state = NORMAL_OPERATION;
                get_calib()->state = 9;
            }
            else
            {
                if (pressure_steday_proc(FLOW_STEADY) == 0)//flow steady
                {
                    verifer_dat.state = FLOW_SET_POINT_STEADY;
                    if (DEBUG_LOG1_ENABLE)
                        printf("FLOW_SET_POINT_STEADY----->\r\n");

                }

            }

            break;
        case FLOW_SET_POINT_STEADY:
            if (perror_proc() == 1)//error below
            {
                if (verifer_dat.sw_flag == 0)
                {

                    verifer_dat.sw_flag = 1;
                    verifer_dat.state = FLOW_SET_POINT_START;
                    if (get_calib()->cail_nozzle_num == NOZZLE_H)
                        get_calib()->cail_nozzle_num = NOZZLE_L;
                    else
                        get_calib()->cail_nozzle_num = NOZZLE_H;
                    press_cal_proc(get_calib()->cail_nozzle_num);
                    nozzle_sel();
                }
                else
                {
                    verifer_dat.state  = NORMAL_OPERATION;
                    get_calib()->state = 7;
                    verifer_dat.sw_flag = 0;

                }

            }
            else
                verifer_dat.state = PRESSURE_RISE_START;
            if (DEBUG_LOG1_ENABLE)
                printf("PRESSURE_RISE_START----->\r\n");
            set_led(300);
            break;
        case PRESSURE_RISE_START:
            set_led(100);
            if (pressure_steday_proc(PRESSURE_STEADY) == 0)//pressure steady
            {
                verifer_dat.state = PRESSURE_STEADY_START;
                if (DEBUG_LOG1_ENABLE)
                    printf("pressure steady----->\r\n");
            }
            else
            {
                if (pressORTime_steady_proc() == 0)//time out
                {
                    if (p20error_proc() == 0)
                        verifer_dat.state = PRESSURE_STEADY_START;

                }
                else if (pressORTime_steady_proc() == 2) //time out
                {
                    verifer_dat.state = NORMAL_OPERATION;
                    get_calib()->state = 8;
//              else
//              {
//                  verifer_dat.state = NORMAL_OPERATION;
//                  get_calib()->state = 7;

                }
                else
                {;}

            }


            break;
        case PRESSURE_STEADY_START:
            verifer_dat.state = PRESSURE_STEADY_SHOCK;
            if (DEBUG_LOG1_ENABLE)
                printf("pressure steady start----->\r\n");
            break;

        case PRESSURE_STEADY_SHOCK:
            if (pressure_steday_proc(PRESSURE_STEADY_SHOCK) == 0)//pressure steady
            {
                verifer_dat.state = PRESSURE_STEADY_END;
                if (DEBUG_LOG1_ENABLE)
                    printf("pressure steady shock----->\r\n");
            }

            break;
        case PRESSURE_STEADY_END:

            verifer_dat.state = NORMAL_OPERATION;
            verifer_dat.sw_flag = 0;
            if (DEBUG_LOG1_ENABLE)
                printf("pressure steady----->\r\n");

            break;
        case PURGE_START:

            ;

            break;
        case PURGE_STOP:

            verifer_dat.state = NORMAL_OPERATION;
            if (DEBUG_LOG1_ENABLE)
                printf("pressure steady----->\r\n");

            break;



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
//calibration_stru *get_calib(void)
//{
//    return &calibration;
//}
//void calibration_init()
//{
//    calibration.p1 = 1;
//    calibration.v1 = 0.00094;
//    calibration.v2 = 2.8105;
//    calibration.vm = 22.4;
//    calibration.R = 8.3138462;
//}
//void calibration_proc()
//{
//    float tmp;
//    calibration.p20 = verifer_dat.pressure_flow_steady;
//
//    calibration.p21 = verifer_dat.pressure_steady;
//    //n0 cal
//    calibration.T = verifer_dat.flow_start_temp_averT + KT;
//    calibration.n0 = calibration.p1 * calibration.v1;
//    calibration.n0 = calibration.n0 + calibration.p20 * calibration.v2;
//    calibration.n0 = calibration.n0 / (calibration.R * calibration.T);
//
//    //n1 cal
//    calibration.T = verifer_dat.pressure_steady_averT  + KT;
//    calibration.n1 = calibration.p21 * (calibration.v1 + calibration.v2);
//    calibration.n1 = calibration.n1 / (calibration.R * calibration.T);
//
//    tmp = (calibration.p21 - calibration.p1) * calibration.v1;
//    calibration.detalN = tmp;
//    tmp = (calibration.p21 - calibration.p20) * calibration.v2;
//    calibration.detalN = calibration.detalN + tmp;
//    calibration.detalN = calibration.detalN / (calibration.R * calibration.T);
//
//    tmp = calibration.detalN * calibration.vm;
//    tmp = tmp * 60000;
//    calibration.F = tmp / (verifer_dat.pressure_steady_tTime);
//
//
//    if (DEBUG_LOG2_ENABLE)
//    {
//        printf("%.3f      %.3f        %.3f        %.3f        %.1f        %.3f        %.3f    %.1f    %.3f\r\n",
//               get_config()->flow_setpoint,
//               verifer_dat.pressure_steady_tTime / 1000.0, calibration.p20, calibration.p21,
//               verifer_dat.pressure_steady_averT, calibration.v1, calibration.v2,
//               verifer_dat.flow_start_temp_averT, get_cdg_status()->pressure_nofilter_torr);
//    }
//    verifer_dat.pressure_steady_tTime = 0;
//
//
//
//}
void pressure_ov_proc(void)
{
    if (get_cdg_status()->pressure_filter_torr >= FULL_SCALE)
    {
        if (get_verifier()->state > LINE_PURGED_START
                && get_verifier()->state < PH_SCTRL)
            get_calib()->state = 5;
    }
}
void verifier_proc(void)
{
    static uint32_t ttick = 0;
    adc_proc();
    sensor_cdg_proc();//cdf data analy
    if (get_flash_status() == SUCESS)
    {
        cali_proc();
		pressure_time_proc();
        pressure_ctrl();
        vavle_state();        
        pressure_state_proc();//state machine
        pressure_ov_proc();
		#if SLOPE_DEBUG == 1
        cali_pressure_slope();
		#endif
    }
    else
    {

        // if ((HAL_GetTick() - ttick) >= 1000)
        {
            // ttick = HAL_GetTick();
            get_calib()->fault = 3;
            // printf("gas database lost\r\n");


        }
        set_led(0xffffffff);
        //printf("gas database lost\r\n");
    }
    // if ((ttick - HAL_GetTick()) >= 1000)
    {
        // ttick = HAL_GetTick();
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
            get_verifier()->state = NORMAL_OPERATION;
            get_calib()->state = 0;
            //get_calib()->fault = 3;
        }
        if (get_calib()->state == 7)
        {
            //get_verifier()->state = NORMAL_OPERATION;
            get_calib()->state = 0;
            get_calib()->fault = 4;//ÇÐ»»·§ºóÑ¹Á¦²»Âú×ãÒªÇó
            //printf("pressure high\r\n");
        }
        if (get_calib()->state == 8)
        {
            get_calib()->state = 0;
            get_calib()->fault = 5;//ÉýÑ¹³¬Ê±
        }
        if (get_calib()->state == 9)
        {
            get_calib()->state = 0;
            get_calib()->fault = 6;//µ×Ñ¹³¬·¶Î§
        }

#if SLOPE_DEBUG == 0
        switch (get_verifier()->state)
        {
            case NORMAL_OPERATION:
            case LINE_PURGED_START:
            case PRESSURE_STEADY_SHOCK:
                if (get_verifier()->mfc_sig == 0)
                {
                    printf("close mfc\r\n");
                    get_verifier()->mfc_sig = 1;
                }
                break;
            case FLOW_SET_POINT_START:
                if (get_verifier()->mfc_sig == 1)
                {
                    printf("open mfc\r\n");
                    get_verifier()->mfc_sig = 0;
                }
                break;
        }
#endif
    }

}

