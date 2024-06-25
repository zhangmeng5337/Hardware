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


    relay_ctrl(ROOT_UP_RELAY, OFF);
    relay_ctrl(HIGHT_FLOW_UP_RELAY, OFF);
    relay_ctrl(LOW_FLOW_UP_RELAY, OFF);
    relay_ctrl(DOWN_RELAY, ON);
    relay_ctrl(LOW_FLOW_UP_RELAY, ON);
    relay_ctrl(HIGHT_FLOW_UP_RELAY, ON);
    HAL_Delay(2000);
    verifer_dat.state = NORMAL_OPERATION;
    relay_ctrl(ROOT_UP_RELAY, OFF);
    relay_ctrl(HIGHT_FLOW_UP_RELAY, OFF);
    relay_ctrl(LOW_FLOW_UP_RELAY, OFF);
    relay_ctrl(DOWN_RELAY, OFF);


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
            relay_ctrl(ROOT_UP_RELAY, ON);
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
        case PURGE_START:
        {
            relay_ctrl(ROOT_UP_RELAY, ON);
            relay_ctrl(HIGHT_FLOW_UP_RELAY, ON);
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
unsigned char pressure_steday_proc(unsigned char state)
{
    unsigned char result;
    static uint32_t tick_start, tick_total;
    //  float error;

    result = 1;
    if (verifer_dat.state == NORMAL_OPERATION)
    {
        tick_start = HAL_GetTick();
    }
    if (verifer_dat.state == LINE_PURGED_START)
    {

        verifer_dat.pressure_vac_thValue = get_cdg_status()->pressure_filter_torr;
        verifer_dat.error = verifer_dat.pressure_vac_thValue * 2;
        // if (verifer_dat.error <= get_config()->pressure_error
        //        || get_config()->debug_cmd == DEBUG_STEADY)
        if (get_calib()->cail_nozzle_num == NOZZLE_H)
        {
            if (verifer_dat.error <= get_calib()->p20_pre_H
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
                    || get_config()->debug_cmd == DEBUG_STEADY)
            {
                result = 0;
                get_config()->debug_cmd = 0;
                tick_total = 0;
            }
        }


    }
//#if DEBUG_LOG1_ENABLE==0
    if (verifer_dat.state == FLOW_SET_POINT_SHOCK)
    {
        static float last_pressure;
        verifer_dat.error = get_cdg_status()->pressure_filter_torr - last_pressure;
        verifer_dat.error = fabs(verifer_dat.error);
        if (verifer_dat.error >= get_config()->p20_pressure_error
                && get_config()->debug_cmd != DEBUG_STEADY)
        {

            tick_start = HAL_GetTick();
            last_pressure = get_cdg_status()->pressure_filter_torr;
            if (last_pressure >= 0.85 * FULL_SCALE)
                result = 2;
        }
        else
        {
            if (tick_total >= STEADY_TIME)
            {
                result = 0;
                get_config()->debug_cmd = 0;
                //tick_total = 0;
            }
            else
                tick_total = HAL_GetTick() - tick_start;
        }

    }
//#endif

    if (verifer_dat.state == PRESSURE_RISE_START)
    {


        verifer_dat.error = get_cdg_status()->pressure_filter_torr -
                            get_calib()->targe_p;
        verifer_dat.pressure_steady_peak = get_cdg_status()->pressure_nofilter_torr;
        if (verifer_dat.error <= get_config()->pressure_error
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
        case FLOW_SET_POINT_START:
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
                get_calib()->state = 4;
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
                verifer_dat.state  = NORMAL_OPERATION;
                get_calib()->state = 4;
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
                        verifer_dat.state = PRESSURE_STEADY_END;
                    else
                        ;
                }
                else if (pressORTime_steady_proc() == 2) //time out
                {
                    verifer_dat.state = NORMAL_OPERATION;
                    get_calib()->state = 4;
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
            if (DEBUG_LOG1_ENABLE)
                printf("pressure steady----->\r\n");

            break;
        case PURGE_START:

            ;

            break;
        case PURGE_END:

            verifer_dat.state = NORMAL_OPERATION;
            if (DEBUG_LOG1_ENABLE)
                printf("pressure steady----->\r\n");

            break;



        case DRAIN_START:

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

void verifier_proc(void)
{
    adc_proc();
    sensor_cdg_proc();//cdf data analy
    if (get_flash_status() == SUCESS)
    {
        cali_proc();
        pressure_ctrl();
        pressure_time_proc();
        pressure_state_proc();//state machine

    }
}

