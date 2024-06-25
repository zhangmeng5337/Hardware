#include "config.h"
#include "uart.h"
#include "cdg.h"
#include "verifier.h"
#include "stdlib.h"
#include "mystring.h"
#include "calibration.h"
#include "gas.h"
#include "flash.h"
#include "stdlib.h"
#include "relay_driver.h"
#include "adc.h"
CONFIG_STRU config_dat;
void config_init()
{
    config_dat.cmd = IDLE_STATE;
    config_dat.flow_mode = HIGH_FLOW;
    config_dat.pressure_error = 0.0094;  //set steady pressure error
    config_dat.vac_val = 2.04; //vacuum value
    config_dat.pressure_val = 100;//target pressure
    config_dat.flow_setpoint = 100;//target flow
    config_dat.p20_pressure_error = 0.001;
    config_dat.p20_pressure_error = 0.015;//0.0093

}
CONFIG_STRU *get_config()
{
    return &config_dat;
}
unsigned char string_analy(char *left_string, char *right_string,
                           unsigned char result_type)
{
    uart_recv_stru   *p;
    static unsigned char pb[64];
    unsigned char res;
    unsigned char buf[32];
    res = 1;
    memset(pb, 0, 64);
    if (COM_TYPE == RS_232)
        p = get_uart_recv_stru(UART_DEBUG);
    else
        p = get_uart_recv_stru(UART_485);
    if (result_type == RE_CMD)
    {
        if (uart_Check_Cmd(p->uartRecBuff, left_string))//start verify
        {
            config_dat.cmd = 1;
            res = 0;
        }

    }
    else if (result_type == RE_FLOAT)
    {
        if (uart_Check_Cmd(p->uartRecBuff, left_string))//steady error set;range:0-1
        {

            if (Find_string((char *)p->uartRecBuff, left_string, right_string, pb) == 1)
            {

                memcpy(buf, pb, 16);
                config_dat.res_float =  atof(&pb[0]);
                res = 0;
            }

        }
    }

    else if (result_type == RE_CHAR)
    {
        if (uart_Check_Cmd(p->uartRecBuff, left_string))//steady error set;range:0-1
        {
            if (Find_string((char *)p->uartRecBuff, left_string, right_string, pb) == 1)
            {
                //Find_string((char *)p->uartRecBuff, left_string, right_string, (char *)pb);
                config_dat.res_char =  atoi(&pb[0]);
                res = 0;

            }

        }
    }
    else if (result_type == RE_STRING)
    {
        if (uart_Check_Cmd(p->uartRecBuff, left_string))//steady error set;range:0-1
        {
            if (Find_string((char *)p->uartRecBuff, left_string, right_string, pb) == 1)
            {
                config_dat.string = pb;
                res = 0;
            }

        }

    }
    else
        res = 1;
    return res;
}
char recv_buf[128];
char *analy_string(char *pb_left, char *pb_right)
{

    uart_recv_stru *p;

    if (COM_TYPE == RS_232)
        p = get_uart_recv_stru(UART_DEBUG);
    else
        p = get_uart_recv_stru(UART_485);

    memset(recv_buf, 0, 128);
    if (Find_string((char *)p->uartRecBuff, pb_left, pb_right,
                    recv_buf) == 1)
        return recv_buf;
    else
        return NULL;
}
void config_proc()
{
    uart_recv_stru *p;
    char *pb;
    float tmp_f;
    unsigned int tmp_i;
    if (COM_TYPE == RS_232)
        p = get_uart_recv_stru(UART_DEBUG);
    else
        p = get_uart_recv_stru(UART_485);
    if (p->uartRecFlag == 1)
    {
        pb = analy_string("cmd=gh2022", "\r\n");
        if (pb != NULL) //start operation
        {
            config_dat.debug = 1;
        }

        if (config_dat.debug == 1)//****************start debug
        {
            pb = analy_string("cmd=start", "\r\n");
            if (pb != NULL) //start operation
            {
                config_dat.update = 1;
                config_dat.cmd = LINE_PURGED_START;

            }
            pb = analy_string("cmd=reboot", "\r\n");
            if (pb != NULL) //start operation
            {
                HAL_NVIC_SystemReset();
            }

            pb = analy_string("cmd=stop", "\r\n");
            if (pb != NULL) //start operation
            {
                config_dat.update = 1;
                config_dat.cmd = NORMAL_OPERATION;

            }
            pb = analy_string("cmd=debug", "\r\n");
            if (pb != NULL) //start operation
            {
                //config_dat.update = 1;
                config_dat.debug_cmd = DEBUG_STEADY;

            }
            pb = analy_string("hnozzle=", "\r\n");
            if (pb != NULL) //start operation
            {
                tmp_i = atoi(pb);
                relay_ctrl(HIGHT_FLOW_UP_RELAY, tmp_i);

            }
            pb = analy_string("lnozzle=", "\r\n");
            if (pb != NULL) //start operation
            {
                tmp_i = atoi(pb);
                relay_ctrl(LOW_FLOW_UP_RELAY, tmp_i);

            }
            pb = analy_string("dnozzle=", "\r\n");
            if (pb != NULL) //start operation
            {
                tmp_i = atoi(pb);
                relay_ctrl(DOWN_RELAY, tmp_i);

            }

        }//***********************end debug
//***********************************query**********************************************
        pb = analy_string("gas_count?", "\r\n");
        if (pb != NULL) //start operation
        {
            printf("gas_count = %u\r\n", get_flash_stru()->total_gas_count);
        }

        pb = analy_string("bp?", "\r\n");
        if (pb != NULL) //start operation
        {
            printf("bp = %u\r\n", get_flash_stru()->bp_status);
        }
        pb = analy_string("pe?", "\r\n");
        if (pb != NULL) //start operation
        {
            printf("pe = %u\r\n", get_flash_stru()->perror_status);
        }
        pb = analy_string("v?", "\r\n");
        if (pb != NULL) //start operation
        {
            printf("v = %u\r\n", get_flash_stru()->v_status);
        }
        pb = analy_string("pre?", "\r\n");//pressure
        if (pb != NULL) //start operation
        {
            printf("pre = %.3f\r\n", get_cdg_status()->pressure_filter_torr);
        }
        pb = analy_string("temp?", "\r\n");//T
        if (pb != NULL) //start operation
        {
            printf("temp = %.1f\r\n", get_temperature()->average_T);
        }
        pb = analy_string("state?", "\r\n"); //state
        if (pb != NULL) //start operation
        {
            printf("state = %u\r\n", get_verifier()->state);
        }
        pb = analy_string("cflow?", "\r\n");
        if (pb != NULL) //start operation
        {
            printf("cali_flow = %.3f\r\n", get_calib()->cali_flow);


        }
        pb = analy_string("rflow?", "\r\n");
        if (pb != NULL) //start operation
        {
            printf("real_flow = %.3f\r\n", get_calib()->F);
        }

        pb = analy_string("cali_gas?", "\r\n");
        if (pb != NULL) //start operation
        {
            printf("cali_gas = %u\r\n", get_calib()->gas_type);

        }
        pb = analy_string("version?", "\r\n");
        if (pb != NULL) //start operation
        {
            printf("Version 1.1\r\n");

        }

		//***********************************end query**********************************************
		//***********************************ctrl**********************************************


        pb = analy_string("purge_start", "\r\n");
        if (pb != NULL) //start operation
        {
            get_verifier()->state = PURGE_START;
            get_calib()->state = 0;
        }
        pb = analy_string("purge_stop", "\r\n");
        if (pb != NULL) //start operation
        {
            get_verifier()->state = PURGE_END;
            get_calib()->state = 0;

        }
        pb = analy_string("drain_start", "\r\n");
        if (pb != NULL) //start operation
        {
            get_verifier()->state = DRAIN_START;
            get_calib()->state = 0;

        }
        pb = analy_string("drain_end", "\r\n");
        if (pb != NULL) //start operation
        {
            get_verifier()->state = DRAIN_END;
            get_calib()->state = 0;

        }
//***********************************enf ctrl**********************************************

//        pb = analy_string("cali_flow=", "\r\n");
//        if (pb != NULL) //start operation
//        {
//            tmp_f = atof(&pb[0]);
//            get_calib()->cali_flow = tmp_f;
//            if (get_calib()->state == 2)
//                get_calib()->state = 3;
//            else
//                get_calib()->state = 1;
//            config_dat.update = 1;
//
//        }
        pb = analy_string("cmd=debug_log", "\r\n");
        if (pb != NULL) //start operation
        {
            config_dat.debug_log = 1;

        }

        pb = analy_string("cali_flow=", "\r\n");
        if (pb != NULL) //start operation
        {
            tmp_f = atof(&pb[0]);
            get_calib()->cali_flow = tmp_f;
            if (get_calib()->state == 2)
                get_calib()->state = 3;
            else
                get_calib()->state = 1;
            config_dat.update = 1;

        }
        pb = analy_string("cali_gas=", "\r\n");
        if (pb != NULL) //start operation
        {
            tmp_i = atoi(pb);
            get_calib()->gas_type = tmp_i;
            get_gas()->id = tmp_i;
            if (get_calib()->state == 1)
                get_calib()->state = 3;//
            else
                get_calib()->state = 2;
            config_dat.update = 1;
        }
        if (get_calib()->state == 3)
        {
            flash_proc(READ, GAS_SET);
            flash_proc(WRITE, PERROR_SET);
        }

//******************************************** set gas******************
        if (config_dat.debug == 1)
        {
            pb = analy_string("set_gas=", "\r\n");
            if (pb != NULL) //start operation
            {
                get_gas()->update_state = 0;
                tmp_i = atoi(pb);
                get_gas()->id = tmp_i;
                get_gas()->update_state = get_gas()->update_state | 0x01;
            }

            pb = analy_string("set_shr=", "\r\n");
            if (pb != NULL) //start operation
            {
                // get_nozzle()->state = 0;
                tmp_f = atof(pb);
                get_gas()->shr = tmp_f;

                get_gas()->update_state = get_gas()->update_state | 0x02;
            }

            pb = analy_string("set_mw=", "\r\n");
            if (pb != NULL) //start operation
            {
                // get_nozzle()->state = 0;
                tmp_f = atof(pb);
                get_gas()->mw = tmp_f;
                get_gas()->update_state = get_gas()->update_state | 0x04 ;

            }

            pb = analy_string("set_c1=", "\r\n");
            if (pb != NULL) //start operation
            {
                // get_nozzle()->state = 0;
                tmp_f = atof(pb);
                get_gas()->c1 = tmp_f;
                get_gas()->update_state = get_gas()->update_state | 0x08;
            }

            pb = analy_string("set_c2=", "\r\n");
            if (pb != NULL) //start operation
            {
                // get_nozzle()->state = 0;
                tmp_f = atof(pb);
                get_gas()->c2 = tmp_f;
                get_gas()->update_state = get_gas()->update_state | 0x10;
            }

            pb = analy_string("set_c3=", "\r\n");
            if (pb != NULL) //start operation
            {
                // get_nozzle()->state = 0;
                tmp_f = atof(pb);
                get_gas()->c3 = tmp_f;
                get_gas()->update_state = get_gas()->update_state | 0x20;
            }
            pb = analy_string("set_a=", "\r\n");
            if (pb != NULL) //start operation
            {
                // get_nozzle()->state = 0;
                tmp_f = atof(pb);
                get_gas()->a = tmp_f;
                get_gas()->update_state = get_gas()->update_state | 0x40;
            }
            pb = analy_string("set_b=", "\r\n");
            if (pb != NULL) //start operation
            {
                // get_nozzle()->state = 0;
                tmp_f = atof(pb);
                get_gas()->b = tmp_f;
                get_gas()->update_state = get_gas()->update_state | 0x80;
            }
            pb = analy_string("set_c=", "\r\n");
            if (pb != NULL) //start operation
            {
                // get_nozzle()->state = 0;
                tmp_f = atof(pb);
                get_gas()->c = tmp_f;
                get_gas()->update_state = get_gas()->update_state | 0x100;
            }
            pb = analy_string("set_d=", "\r\n");
            if (pb != NULL) //start operation
            {
                // get_nozzle()->state = 0;
                tmp_f = atof(pb);
                get_gas()->d = tmp_f;
                get_gas()->update_state = get_gas()->update_state | 0x200;
            }




            //******************************************** end set gas******************

            //******************************************** set nozzle******************
            pb = analy_string("set_nozzle=", "\r\n");
            if (pb != NULL) //start operation
            {
                get_nozzle()->state = 0;
                tmp_i = atof(pb);
                get_nozzle()->nozzle_num = tmp_i;
                get_nozzle()->state = get_nozzle()->state | 0x01;
            }
            if (get_nozzle()->state & 0x01)
            {
                pb = analy_string("set_gerr0=", "\r\n");
                if (pb != NULL) //start operation
                {
                    //get_nozzle()->state = 0;
                    tmp_f = atof(pb);
                    if (get_nozzle()->nozzle_num == NOZZLE_H)
                        get_nozzle()->Gerr0_H = tmp_f;
                    else
                        get_nozzle()->Gerr0_L = tmp_f;
                    get_nozzle()->state = get_nozzle()->state | 0x02;
                }


                pb = analy_string("set_gerr1=", "\r\n");
                if (pb != NULL) //start operation
                {

                    tmp_f = atof(pb);
                    if (get_nozzle()->nozzle_num == NOZZLE_H)
                        get_nozzle()->Gerr1_H = tmp_f;
                    else
                        get_nozzle()->Gerr1_L = tmp_f;
                    get_nozzle()->state = get_nozzle()->state | 0x04;
                }

                pb = analy_string("set_gerr2=", "\r\n");
                if (pb != NULL) //start operation
                {

                    tmp_f = atof(pb);
                    if (get_nozzle()->nozzle_num == NOZZLE_H)
                        get_nozzle()->Gerr2_H = tmp_f;
                    else
                        get_nozzle()->Gerr2_L = tmp_f;
                    get_nozzle()->state = get_nozzle()->state | 0x08;
                }

                pb = analy_string("set_s1=", "\r\n");
                if (pb != NULL) //start operation
                {

                    tmp_f = atof(pb);
                    if (get_nozzle()->nozzle_num == NOZZLE_H)
                        get_nozzle()->S1_H = tmp_f;
                    else
                        get_nozzle()->S1_L = tmp_f;
                    get_nozzle()->state = get_nozzle()->state | 0x10;

                }

                pb = analy_string("set_s2=", "\r\n");
                if (pb != NULL) //start operation
                {

                    tmp_f = atof(pb);
                    if (get_nozzle()->nozzle_num == NOZZLE_H)
                        get_nozzle()->S2_H = tmp_f;
                    else
                        get_nozzle()->S2_L = tmp_f;
                    get_nozzle()->state = get_nozzle()->state | 0x20;
                }

                pb = analy_string("set_tr=", "\r\n");
                if (pb != NULL) //start operation
                {


                    tmp_f = atof(&pb[0]);
                    if (get_nozzle()->nozzle_num == NOZZLE_H)
                    {
                        get_flash_stru()->nozzle_status = 0;
                        get_flash_stru()->nozzle_status = get_flash_stru()->nozzle_status | 0x01;
                        get_nozzle()->Tr_H = tmp_f;

                    }

                    else
                    {
                        get_nozzle()->Tr_L = tmp_f;
                        get_flash_stru()->nozzle_status = get_flash_stru()->nozzle_status | 0x02;

                    }

                    get_nozzle()->state = get_nozzle()->state | 0x40;

                }


            }
            pb = analy_string("set_bp0=", "\r\n");
            if (pb != NULL) //start operation
            {
                get_flash_stru()->bp_status = 0;
                tmp_f = atof(&pb[0]);
                get_calib()->bp0 = tmp_f;
                get_flash_stru()->bp_status = get_flash_stru()->bp_status | 0x01;
                get_nozzle()->state = get_nozzle()->state | 0x80;

            }
            pb = analy_string("set_bp1=", "\r\n");
            if (pb != NULL) //start operation
            {

                tmp_f = atof(&pb[0]);
                get_calib()->bp1 = tmp_f;
                get_flash_stru()->bp_status = get_flash_stru()->bp_status | 0x02;
                get_nozzle()->state = get_nozzle()->state | 0x100;
            }
            pb = analy_string("set_bp2=", "\r\n");
            if (pb != NULL) //start operation
            {

                tmp_f = atof(&pb[0]);
                get_calib()->bp2 = tmp_f;
                get_flash_stru()->bp_status = get_flash_stru()->bp_status | 0x04;
                get_nozzle()->state = get_nozzle()->state | 0x200;
            }
            pb = analy_string("set_bp3=", "\r\n");
            if (pb != NULL) //start operation
            {

                tmp_f = atof(&pb[0]);
                get_calib()->bp3 = tmp_f;
                get_flash_stru()->bp_status = get_flash_stru()->bp_status | 0x08;
                get_nozzle()->state = get_nozzle()->state | 0x300;

            }

            pb = analy_string("set_bp4=", "\r\n");
            if (pb != NULL) //start operation
            {

                tmp_f = atof(&pb[0]);
                get_calib()->bp4 = tmp_f;
                get_flash_stru()->bp_status = get_flash_stru()->bp_status | 0x10;
                get_nozzle()->state = get_nozzle()->state | 0x400;//7fff  //0x7f  0x7fff
                // get_flash_stru()->bp_status = 1;

            }

            pb = analy_string("set_perror=", "\r\n");
            if (pb != NULL) //start operation
            {
                tmp_f = atof(&pb[0]);
                get_calib()->perror = tmp_f;
                get_nozzle()->state = get_nozzle()->state | 0x800;
                get_flash_stru()->perror_status = 1;
            }


            pb = analy_string("set_v1=", "\r\n");
            if (pb != NULL) //start operation
            {
                get_flash_stru()->v_status = 0;
                tmp_f = atof(&pb[0]);
                get_calib()->v1 = tmp_f;
                get_flash_stru()->v_status = get_flash_stru()->v_status | 1;
                get_nozzle()->state = get_nozzle()->state | 0x1000;
            }
            pb = analy_string("set_v2=", "\r\n");
            if (pb != NULL) //start operation
            {
                tmp_f = atof(&pb[0]);
                get_calib()->v2 = tmp_f;
                get_flash_stru()->v_status = get_flash_stru()->v_status | 2;
                get_nozzle()->state = get_nozzle()->state | 0x2000;
                //get_flash_stru()->v_status = 1;
            }
//            pb = analy_string("set_alpha=", "\r\n");
//            if (pb != NULL) //start operation
//            {
//                tmp_f = atof(&pb[0]);
//                get_calib()->alpha = tmp_f;
//                get_nozzle()->state = get_nozzle()->state | 0x4000;
//
//            }

            if ((get_nozzle()->state & 0xffff) >= 0x7ff)//400
            {
                flash_proc(WRITE, NOZZLE_SET);
                flash_proc(WRITE, PERROR_SET);
                get_nozzle()->state = 0;

            }
            if ((get_gas()->update_state & 0x3ff) == 0x3ff)
            {
                flash_proc(WRITE, GAS_SET);
                flash_proc(WRITE, PERROR_SET);
                get_gas()->update_state = 0;
            }
//            if (get_gas()->update_state & 0x80 == 0x80)
//            {
//                flash_proc(WRITE, PERROR_SET);
//                //get_gas()->update_state = 0;
//            }

        }

        //******************************************** end set nozzle******************
        pb = analy_string("cdg set=", ",");
        if (pb != NULL) //set cdg command
        {

            unsigned char i;
            unsigned char   *p;
            p = get_uart_recv_stru(UART_DEBUG)->uartRecBuff;

            i = 0;
            while (*p != '\r')
            {

                if (*p == '=')
                    get_cdg_config()->command = atoi((p + 1));
                else
                {
                    if (*p == ',')
                    {

                        if (i == 0)
                        {
                            i = 1;
                            get_cdg_config()->addr = atoi((p + 1));
                        }
                        else
                            get_cdg_config()->dat = atoi((p + 1));
                    }
                }
                p++;
            }

            get_cdg_config()->update = 1;
            config_dat.update = 0;
        }
        memset(p->uartRecBuff, 0, UART_REC_SIZE);
        p->uartRecFlag = 0;
    }
}


//void config_proc()
//{
//    uart_recv_stru *p, *pb;
//  if(COM_TYPE == RS_232)
//    p = get_uart_recv_stru(UART_DEBUG);
//  else
//    p = get_uart_recv_stru(UART_485);
//    if (p->uartRecFlag == 1)
//    {
//
//        p->uartRecFlag = 0;
//        if (string_analy("cmd=start\r\n", "\r\n", RE_CMD) == 0)//start operation
//        {
//            config_dat.update = 1;
//            config_dat.cmd = LINE_PURGED_START;
//            if (DEBUG_LOG1_ENABLE)
//                printf("LINE_PURGED_START\r\n");
//
//        }
//        else if (string_analy("cmd=debug\r\n", "\r\n",
//                              RE_CMD) == 0) //trigger machine state  manual
//        {
//            config_dat.update = 2;
//            config_dat.debug_cmd = DEBUG_STEADY;
//            if (DEBUG_LOG1_ENABLE)
//                printf("DEBUG_STEADY\r\n");
//
//        }
//
//        else if (string_analy("cmd=stop\r\n", "\r\n", RE_CMD) == 0) //stop operation
//        {
//            config_dat.update = 1;
//            config_dat.cmd = NORMAL_OPERATION;
//            if (DEBUG_LOG1_ENABLE)
//                printf("stop\r\n");
//
//        }
//        else if (string_analy("cmd=flow steady\r\n", "\r\n",
//                              RE_CMD) == 0) //
//        {
//            config_dat.update = 1;
//            config_dat.cmd = FLOW_SET_POINT_STEADY;
//            if (DEBUG_LOG1_ENABLE)
//                printf("config_dat.cmd = %d\r\n", config_dat.cmd);
//
//        }
//        else if (string_analy("cmd=pressure steady", "\r\n",
//                              RE_CMD) == 0) //
//        {
//            config_dat.update = 1;
//            config_dat.cmd = PRESSURE_STEADY_START;
//            if (DEBUG_LOG1_ENABLE)
//                printf("PRESSURE_STEADY_START\r\n");
//
//        }
//        else if (string_analy("flow_mode=high\r\n", "\r\n",
//                              RE_CMD) == 0) //sel high speed vavle
//        {
//            config_dat.flow_mode = HIGH_FLOW;
//            config_dat.update = 2;
//            if (DEBUG_LOG1_ENABLE)
//                printf("config_dat.flow_mode = %d\r\n", config_dat.flow_mode);
//
//        }
//        else if (string_analy("flow_mode=low\r\n", "\r\n",
//                              RE_CMD) == 0) //sel low speed vavle
//        {
//            config_dat.flow_mode = LOW_FLOW;
//            config_dat.update = 2;
//            if (DEBUG_LOG1_ENABLE)
//                printf("config_dat.flow_mode = %d\r\n", config_dat.flow_mode);
//
//        }
//        else if (string_analy("p20_error=", "\r\n",
//                              RE_FLOAT) == 0) //steady error set;range:0-1
//        {
//            config_dat.p20_pressure_error =  config_dat.res_float;
//            config_dat.update = 2;
//            if (DEBUG_LOG1_ENABLE)
//                printf("config_dat.p20_pressure_error = %f\r\n", config_dat.pressure_error);
//
//        }
//        else if (string_analy("error=", "\r\n",
//                              RE_FLOAT) == 0) //steady error set;range:0-1
//        {
//            config_dat.pressure_error =  config_dat.res_float;
//            config_dat.update = 2;
//            if (DEBUG_LOG1_ENABLE)
//                printf("config_dat.pressure_error = %f\r\n", config_dat.pressure_error);
//
//        }
//
//        else if (string_analy("vac_val=", "\r\n", RE_FLOAT) == 0)//vacuum value set
//        {
//            config_dat.vac_val = config_dat.res_float;
//            config_dat.update = 2;
//            if (DEBUG_LOG1_ENABLE)
//                printf("config_dat.vac_val = %f\r\n", config_dat.vac_val);
//
//        }
//        else if (string_analy("pressure val=", "\r\n",
//                              RE_FLOAT) == 0) //target pressure set
//        {
//            config_dat.pressure_val =  config_dat.res_float;
//            config_dat.update = 2;
//            if (DEBUG_LOG1_ENABLE)
//                printf("config_dat.pressure_val = %f\r\n", config_dat.pressure_val);
//
//        }
//        else if (string_analy("flow setpoint=", "\r\n",
//                              RE_FLOAT) == 0) //target flow set
//        {
//            config_dat.flow_setpoint =  config_dat.res_float;
//                get_calib()->cali_flow;
//            config_dat.update = 2;
//            if (DEBUG_LOG1_ENABLE)
//                printf("config_dat.flow_setpoint = %f\r\n", config_dat.flow_setpoint);
//
//        }
//        else if (string_analy("gas=", "\r\n",
//                              RE_CHAR) == 0) //target flow set
//        {
//
//                get_gas()->id= config_dat.res_char;
//            config_dat.update = 2;
//            if (DEBUG_LOG1_ENABLE)
//                printf("config_dat.flow_setpoint = %f\r\n", config_dat.flow_setpoint);
//
//        }
//        else if (string_analy("flow_mode?\r\n", "\r\n",
//                              RE_CMD) == 0) //sel low speed vavle
//        {
//                printf("config_dat.flow_mode = %d\r\n", config_dat.flow_mode);
//
//        }
//        else if (string_analy("pressure val?\r\n", "\r\n",
//                              RE_CMD) == 0) //sel low speed vavle
//        {
//                printf("config_dat.pressure_val = %f\r\n", config_dat.pressure_val);
//        }
//
//        else if (string_analy("cdg set=", ",", RE_CHAR) == 0) //set cdg command
//        {
//
//            unsigned char i;
//            unsigned char   *p;
//            p = get_uart_recv_stru(UART_DEBUG)->uartRecBuff;
//
//            i = 0;
//            while (*p != '\r')
//            {
//
//                if (*p == '=')
//                    get_cdg_config()->command = atoi((p + 1));
//                else
//                {
//                    if (*p == ',')
//                    {
//
//                        if (i == 0)
//                        {
//                            i = 1;
//                            get_cdg_config()->addr = atoi((p + 1));
//                        }
//                        else
//                            get_cdg_config()->dat = atoi((p + 1));
//
//                    }
//
//                }
//                p++;
//            }
//
//            get_cdg_config()->update = 1;
//            config_dat.update = 0;
//            if (DEBUG_LOG1_ENABLE)
//            {
//                printf("cdg command = %d  ", get_cdg_config()->command);
//                printf("cdg addr = %d  ", get_cdg_config()->addr);
//                printf("cdg dat = %d\r\n", get_cdg_config()->dat);
//
//            }
//
//        }
//        else if (string_analy("set v1=", "\r\n", RE_FLOAT) == 0) //v1 set
//
//        {
//            get_calib()->v1 =  config_dat.res_float;
//            config_dat.update = 0;
//            if (DEBUG_LOG1_ENABLE)
//            {
//                printf("v1 = %f  ", get_calib()->v1);
//
//            }
//
//        }
//        else if (string_analy("set v2=", "\r\n", RE_FLOAT) == 0) //v2 set
//        {
//
//            get_calib()->v2 =  config_dat.res_float;
//            config_dat.update = 0;
//            if (DEBUG_LOG1_ENABLE)
//            {
//                printf("v2 = %f  ", get_calib()->v2);
//
//            }
//
//        }
//        else if (string_analy("set R=", "\r\n", RE_FLOAT) == 0)//R set
//        {
//
//            get_calib()->R =  config_dat.res_float;
//            config_dat.update = 0;
//            if (DEBUG_LOG1_ENABLE)
//            {
//                printf("R = %f  ", get_calib()->R);
//
//            }
//
//        }
//        else if (string_analy("set p1=", "\r\n", RE_FLOAT) == 0)//p1 set
//        {
//
//            get_calib()->p1 =  config_dat.res_float;
//            config_dat.update = 0;
//            if (DEBUG_LOG1_ENABLE)
//            {
//                printf("p1 = %f  ", get_calib()->R);
//
//            }
//
//        }
//
//        else if (string_analy("set p20=", "\r\n", RE_FLOAT) == 0) //p20 set
//        {
//
//            get_calib()->p20 =  config_dat.res_float;
//            config_dat.update = 0;
//            if (DEBUG_LOG1_ENABLE)
//            {
//                printf("p20= %f  ", get_calib()->p20);
//
//            }
//
//        }
//
//        else
//        {
//            // p->uartRecFlag = 1;
//            config_dat.update = 0;
//        }
//        memset(p->uartRecBuff, 0, UART_REC_SIZE);
//
//
//
//
//
//    }
//}



