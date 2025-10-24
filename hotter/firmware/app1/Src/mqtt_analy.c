#include "mqtt_analy.h"
#include "my_string.h"
#include "lte_hal.h"
//#include "json_para.h"

#include "ai_proc.h"
#include "di.h"
#include "modbus.h"
#include "uart.h"
#include "config.h"
#include "sys.h"
#include "pid.h"
#include "filter.h"
#include "schedule.h"
#include "rtc.h"
#include "hotter.h"
#include "FuzzyPID.h"
//#include <jansson.h>
#include "do.h"
#include "power_energy.h"
#include "malloc.h"

tsATCmds     mqtt_at_cmds;
teATCmdNum    mqtt_at_cmd_num;
teATStatus    mqtt_at_status;
tsLpuart1type *mqtt_recv;
char json_mqtt_send_buf[2048];

extern tsLpuart1type *lte_recv;


void json_plan_analy(unsigned char *p, unsigned char index);

//unsigned char mqtt_send_buf[128];


mqtt_payload_stru mqtt_payload_u;
unsigned char count = 0;
//json_t *root, *sensorSta, *emeterData, *airpumpData, *devParams;
//json_t *diSta, *aiSta, *doSta;
char *out, *out2;
void find_number_u(char *pb, short unsigned int *result)
{
    short int tmp_i;
    short unsigned int tmp_u;
    unsigned char buf[32];
    unsigned char i, j, k;
    j = 0;
    k = 0;
    i = 0;
    get_config()->tlen = 0;
    while (pb[i] != ']')
    {
        if (pb[i] != ',' && pb[i] != ']')
        {

            buf[j++] = pb[i];

        }

        else
        {

            tmp_i = atoi(buf);
            if (tmp_i < 0)
            {
                tmp_i = -tmp_i;
                tmp_u = tmp_i;
                tmp_u = ~tmp_u + 1;

            }
            else
                tmp_u = tmp_i;

            result[k++] = tmp_i;
            get_config()->tlen++;
            memset(buf, 0, 16);
            j = 0;
        }
        i++;
    }
    tmp_i = atoi(buf);
    if (tmp_i < 0)
    {
        tmp_i = -tmp_i;
        tmp_u = tmp_i;
        tmp_u = ~tmp_u + 1;

    }
    else
        tmp_u = tmp_i;

    result[k++] = tmp_i;

}
void find_number_f(char *pb, float *result)
{
    unsigned char buf[32];
    unsigned char i, j, k;
    j = 0;
    k = 0;
    i = 0;
    get_config()->tlen = 0;
    while (pb[i] != ']')
    {
        if (pb[i] != ',' && pb[i] != ']')
        {

            buf[j++] = pb[i];

        }

        else
        {
            float tmp_f;
            tmp_f = atof(buf);
            result[k++] = tmp_f;
            get_config()->tlen++;
            memset(buf, 0, 16);
            j = 0;
        }
        i++;
    }
}
char *find_json_string(char *pb_left, char *pb_right, unsigned char end_flag)
{


    char recv_buf[512];
    static unsigned char valid_dat;
    memset(recv_buf, 0, 512);
    if (end_flag == 0)
    {
        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, get_config()->user_id,
                        ",\r\n", recv_buf) == 1)
        {
            valid_dat = 1;
        }

    }
    else
    {

    }
    // if (valid_dat == 1)
    {
        memset(recv_buf, 0, 128);
        if (end_flag == 1)
            valid_dat = 0;
        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, pb_left, pb_right,
                        recv_buf) == 1)
            return recv_buf;
        else
            return NULL;

    }
    //    else
    //   return NULL;

}
void plan_analy(unsigned char *p, unsigned char index)
{
    //if (p != NULL)
    {
        //memset(dev_id, 0, 128);
        unsigned char buf[32];
        unsigned char buf2[PLAN_SIZE];
        unsigned int tmp, i, j, k;
        j = 0;
        k = 0;
        i = 0;
        //get_config()->tlen = 0;
        while (p [i] != ']')
        {
            if (p [i] != ',' && p [i] != ']')
                buf[j++] = p [i];
            else
            {
                tmp = atoi(buf);
                buf2[k++] = tmp;
                memset(buf, 0, 32);
                j = 0;
            }
            i++;

        }
        tmp = atoi(buf);
        buf2[k++] = tmp;
        i = 0;

        get_schedule()->buf[index].pwr_state = buf2[i++];
        get_schedule()->buf[index].temperature = buf2[i++];
        get_schedule()->buf[index].shour = buf2[i++];
        get_schedule()->buf[index].sminute = buf2[i++];
        get_schedule()->buf[index].sweekday = buf2[i++];
        get_schedule()->buf[index].ehour = buf2[i++];
        get_schedule()->buf[index].eminute = buf2[i++];
        get_schedule()->buf[index].eweekday = buf2[i++];
        get_schedule()->buf[index].enable = buf2[i++];
        get_schedule()->buf[index].index = index;

        get_schedule()->save_flag = 1;



    }

}
void json_plan_analy(unsigned char *p, unsigned char index)
{

    unsigned int i;

    i = 0;

    get_schedule()->buf[index].pwr_state = p[i++];
    get_schedule()->buf[index].temperature = p[i++];
    get_schedule()->buf[index].shour = p[i++];
    get_schedule()->buf[index].sminute = p[i++];
    get_schedule()->buf[index].sweekday = p[i++];
    get_schedule()->buf[index].ehour = p[i++];
    get_schedule()->buf[index].eminute = p[i++];
    get_schedule()->buf[index].eweekday = p[i++];
    get_schedule()->buf[index].enable = p[i++];
    get_schedule()->buf[index].index = index;
    get_schedule()->save_flag = 1;


}

void json_clear()
{

//    json_decref(root);
//    json_decref(sensorSta);
//    json_decref(emeterData);
//    json_decref(airpumpData);
//    json_decref(devParams);
//    json_decref(diSta);
//    json_decref(doSta);
//    free(out);



}
unsigned char *sprintf_array_f(float *p, unsigned int len)
{
    unsigned char buf_tmp[512];
    unsigned int i, j;
    memset(buf_tmp, 512, 0);
    if (len > 1)
    {
        sprintf(buf_tmp, "[%.3f,", p[0]);
        i = strlen(buf_tmp);
        for (j = 1; j < (len - 1); j++)
        {
            sprintf(&buf_tmp[i], "%.3f,", p[j]);
            i = strlen(buf_tmp);
        }
        sprintf(&buf_tmp[i], "%.3f]", p[j]);

    }
    else
    {
        if (len == 1)
        {
            sprintf(buf_tmp, "[%.4f]", p[0]);

        }
    }
    return buf_tmp;

}
unsigned char *sprintf_array_u(short unsigned int *p, unsigned int len,
                               unsigned char num_typ)
{
    unsigned char buf_tmp[1024];
    unsigned int i, j;
    for (i = 0; i < 1024; i++)
        buf_tmp[i] = 0;

    if (num_typ == 0)
    {
        if (len > 1)
        {
            sprintf(buf_tmp, "[%u,", p[0]);
            i = strlen(buf_tmp);
            for (j = 1; j < (len - 1); j++)
            {
                sprintf(&buf_tmp[i], "%u,", p[j]);
                i = strlen(buf_tmp);
            }
            sprintf(&buf_tmp[i], "%u]", p[j]);

        }
        else
        {
            if (len == 1)
            {
                sprintf(buf_tmp, "[%u]", p[0]);

            }
        }

    }
    else
    {
        if (len > 1)
        {
            sprintf(buf_tmp, "[%hd,", p[0]);
            i = strlen(buf_tmp);
            for (j = 1; j < (len - 1); j++)
            {
                sprintf(&buf_tmp[i], "%hd,", p[j]);
                i = strlen(buf_tmp);
            }
            sprintf(&buf_tmp[i], "%hd]", p[j]);

        }
        else
        {
            if (len == 1)
            {
                sprintf(buf_tmp, "[%hd]", p[0]);

            }
        }

    }

    return buf_tmp;

}


void jsson_pack(unsigned char mqtt_packNum)
{
    unsigned char buf_tmp[128];
    unsigned char buf_tmp_large[2048];

    unsigned int i, j, k, l;
    unsigned char *p;
    memset(buf_tmp, sizeof(buf_tmp), 0);
    sprintf(buf_tmp, "%s", get_config()->user_id);
    switch (mqtt_packNum)
    {
        case 0://ai di do
            float buf_f[AI_SIZE_T + AI_SIZE_P];
            // memset(buf_tmp2, sizeof(buf_tmp2), 0);
            memset(json_mqtt_send_buf, sizeof(json_mqtt_send_buf), 0);

            for (i = 0; i < AI_SIZE_T; i++)
            {
                buf_f[i] = get_ai_data()->temp[i];

            }
            for (i = AI_SIZE_T; i < (AI_SIZE_T + AI_SIZE_P); i++)
            {
                buf_f[i] = get_ai_data()->press[i - AI_SIZE_T];


            }
            p = sprintf_array_f(buf_f, AI_SIZE_T + AI_SIZE_P);

//            json_object_set_new(sensorSta, "diSta", json_integer(get_di_data()->di_status));
//            //do
//            json_object_set_new(sensorSta, "doSta", json_integer(get_do_status()));

//            unsigned char buf2[128];
//            buf2[i++] = getRtcDate()->Year;
//            buf2[i++] = getRtcDate()->Month;
//            buf2[i++] = getRtcDate()->Date;
//            buf2[i++] = getRtcDate()->WeekDay;
//            buf2[i++] = getRtcTime()->Hours;
//            buf2[i++] = getRtcTime()->Minutes;
//            buf2[i++] = getRtcTime()->Seconds;
//            json_object_set_new(devParams, "time", json_string(buf2));
//            json_object_set_new(root, "sensorSta", sensorSta);
            //            //devdevParams
            //            json_object_set_new(devParams, "ver", json_string(get_config()->version));
            //            json_object_set_new(devParams, "setOT", json_integer(get_config()->set_tout));
            //            json_object_set_new(devParams, "setIT",
            //                                json_integer(get_config()->set_tindoor));
            //            json_object_set_new(devParams, "mode", json_integer(get_config()->mode));
            //            json_object_set_new(devParams, "upt",
            //                                json_integer(get_config()->set_up_period));
            //\"Time\":%u-%u-%u,\r\n\
            //\"Date\":%u-%u-%u,\r\n\

            sprintf(json_mqtt_send_buf,
                    "{\r\n\
\"devid\":\"%s\",\r\n\
\"sensorSta\":{\r\n\
\"aiSta\":%s,\r\n\
\"diSta\":%u,\r\n\
\"doSta\":%lu\r\n\
},\r\n\
\"devParams\":{\r\n\
\"ver\":\"%s\",\r\n\
\"setOT\":%u,\r\n\
\"setIT\":%u,\r\n\
\"mode\":%u,\r\n\
\"upt\":%.1f,\r\n\
}\r\n\
}",
                    buf_tmp, p,
                    get_di_data()->di_status,
                    get_do_status(),
                    get_config()->version,
                    get_config()->set_tout,
                    get_config()->set_tindoor,
                    get_config()->mode,
                    get_config()->set_up_period);
//getRtcTime()->Hours,
//getRtcTime()->Minutes,
//getRtcTime()->Seconds,
//getRtcDate()->Year,
//getRtcDate()->Month,
//getRtcDate()->Date);
            break;
        case 1:
            k = 0;
            memset(buf_tmp_large, sizeof(buf_tmp_large), 0);
            for (i = 0; i < ENERGY_COUNT; i++)
            {
                unsigned int tmp_addr;
                tmp_addr = get_energy_data()->pb[i].addr;
                p = sprintf_array_f(&get_energy_data()->pb[i].payload[0], ENERGY_BUF_SIZE - 2);


                if (i == (ENERGY_COUNT - 1))
                {
                    sprintf(&buf_tmp_large[k], "{\r\n\
\"addr\":%u,\r\n\
\"data\":%s}", tmp_addr, p);

                }
                else
                {
                    sprintf(&buf_tmp_large[k], "{\r\n\
\"addr\":%u,\r\n\
\"data\":%s},", tmp_addr, p);
                    k = strlen(buf_tmp_large);

                }
            }
            sprintf(json_mqtt_send_buf,
                    "{\r\n\
\"devid\":\"%s\",\r\n\
\"emeterData\":[%s\r\n\
]\r\n\
}", buf_tmp, buf_tmp_large);

            break;
        default :
            k = 0;
            unsigned char tmpi;

            if (count < AIR_PUMP_SIZE)
                tmpi = count;
            else
                tmpi = count - AIR_PUMP_SIZE;
            for (i = tmpi; i < (1 + tmpi); i++)
            {


                // if (get_hotter(i + 1)->status[0] != 0)
                {
                    unsigned int tmp_addr;

                    if (count < (AIR_PUMP_SIZE))
                    {
                        short unsigned int *pb;
                        memset(buf_tmp_large, sizeof(buf_tmp_large), 0);

                        {
                            unsigned int tmp_addr;

                            pb = malloc((STATUS1_SIZE + STATUS2_SIZE));
                            for (l = 0; l < ((STATUS1_SIZE + STATUS2_SIZE)); l++)
                                pb[l] = 0;
                            tmp_addr = get_hotter(i)->status[0];
                            memcpy(pb, &get_hotter(i)->status[1], (STATUS1_SIZE - 1) * 2);

                            memcpy(pb + (STATUS1_SIZE - 1), &get_hotter(i)->status2[1],
                                   (STATUS2_SIZE - 1) * 2);
                            p = sprintf_array_u(pb, (STATUS1_SIZE + STATUS2_SIZE - 2), 1);
                            if (i < AIR_PUMP_SIZE)
                            {
                                sprintf(&buf_tmp_large[k], "{\r\n\
						\"addr\":%u,\r\n\
						\"data1\":%s}", tmp_addr, p);

                            }
                            else
                            {
                                sprintf(&buf_tmp_large[k], "{\r\n\
						\"addr\":%u,\r\n\
						\"data1\":%s},", tmp_addr, p);
                                k = strlen(buf_tmp_large);

                            }
                        }
                        sprintf(json_mqtt_send_buf,
                                "{\r\n\
						\"devid\":\"%s\",\r\n\
						\"airpumpData\":[%s\r\n\
						]\r\n\
						}", buf_tmp, buf_tmp_large);
                        free(pb);
                    }
                    else
                    {
                        memset(buf_tmp_large, sizeof(buf_tmp_large), 0);

                        {
                            unsigned int tmp_addr;
                            tmp_addr = get_hotter(i)->status[0];
                            p = sprintf_array_u(&(get_hotter(i)->status3[1]), (STATUS3_SIZE - 1), 1);
                            if (i < AIR_PUMP_SIZE)
                            {
                                sprintf(&buf_tmp_large[k], "{\r\n\
										 \"addr\":%u,\r\n\
										 \"data2\":%s}", tmp_addr, p);

                            }
                            else
                            {
                                sprintf(&buf_tmp_large[k], "{\r\n\
										 \"addr\":%u,\r\n\
										 \"data2\":%s},", tmp_addr, p);
                                k = strlen(buf_tmp_large);

                            }
                        }
                        sprintf(json_mqtt_send_buf,
                                "{\r\n\
										 \"devid\":\"%s\",\r\n\
										 \"airpumpData\":[%s\r\n\
										 ]\r\n\
										 }", buf_tmp, buf_tmp_large);


                    }

                }
            }
            count++;

            if (count >= 2 * AIR_PUMP_SIZE)
                count = 0;
            break;

    }
}
//void sprintf_pack(unsigned char mqtt_packNum)
//{
//    json_t  *addr, *tmp, *array_tmp;
//    json_t *arrary_value;
//    unsigned int  k, kl;
//    static unsigned char count = 0;
//    /* Build an empty JSON object */
//    root = json_object();
//    if (!root)
//    {
//        json_decref(root);
////      json_decref(sensorSta);
////      json_decref(emeterData);
////      json_decref(airpumpData);
////      json_decref(devParams);
////      json_decref(diSta);
////      json_decref(doSta);

//        return ;
//    }
//    json_object_set_new(root, "devid", json_string(get_config()->user_id));

//    switch (mqtt_packNum)
//    {
//        case 0:
//            //sensorSta
//            sensorSta = json_object();

//            if (!sensorSta)
//            {
//                json_decref(sensorSta);

//                return ;
//            }

//            //ai
//            aiSta = json_array();

//            if (!aiSta)
//            {
//                json_decref(aiSta);

//                return ;
//            }

//            unsigned char i, j;
//            for (i = 0; i < AI_SIZE_T; i++)
//            {

//                json_array_insert_new(aiSta, i, json_real(get_ai_data()->temp[i]));

//            }
//            for (i = AI_SIZE_T; i < (AI_SIZE_T + AI_SIZE_P); i++)
//            {
//                json_object_set_new(arrary_value, "arrary_value",
//                                    json_real(get_ai_data()->press[i - AI_SIZE_T]));
//                json_array_insert_new(aiSta, i, arrary_value);

//            }

//            json_object_set_new(sensorSta, "aiSta", aiSta);
//            //di
//            json_object_set_new(sensorSta, "diSta", json_integer(get_di_data()->di_status));
//            //do
//            json_object_set_new(sensorSta, "doSta", json_integer(get_do_status()));

//            //devdevParams
//            json_object_set_new(devParams, "ver", json_string(get_config()->version));
//            json_object_set_new(devParams, "setOT", json_integer(get_config()->set_tout));
//            json_object_set_new(devParams, "setIT",
//                                json_integer(get_config()->set_tindoor));
//            json_object_set_new(devParams, "mode", json_integer(get_config()->mode));
//            json_object_set_new(devParams, "upt",
//                                json_integer(get_config()->set_up_period));
//            unsigned char buf2[128];
//            buf2[i++] = getRtcDate()->Year;
//            buf2[i++] = getRtcDate()->Month;
//            buf2[i++] = getRtcDate()->Date;
//            buf2[i++] = getRtcDate()->WeekDay;
//            buf2[i++] = getRtcTime()->Hours;
//            buf2[i++] = getRtcTime()->Minutes;
//            buf2[i++] = getRtcTime()->Seconds;
//            json_object_set_new(devParams, "time", json_string(buf2));
//            json_object_set_new(root, "sensorSta", sensorSta);
//            //json_object_set_new(root, "devParams", devParams);


//            break;
//        case 1:
//            emeterData = json_array();
//            k = 0;
//            for (i = 0; i < ENERGY_COUNT; i++)
//            {
//                // if (get_energy_data()->pb[i].addr != 0)
//                {
//                    addr = json_object();
//                    if (!addr)
//                    {
//                        json_decref(addr);

//                        return ;
//                    }
//                    arrary_value = json_array();
//                    if (!arrary_value)
//                    {
//                        json_decref(arrary_value);

//                        return ;
//                    }
//                    array_tmp = json_array();
//                    if (!array_tmp)
//                    {
//                        json_decref(array_tmp);

//                        return ;
//                    }
//                    addr = json_object();
//                    if (!addr)
//                    {
//                        json_decref(addr);

//                        return ;
//                    }
//                    arrary_value = json_array();
//                    if (!arrary_value)
//                    {
//                        json_decref(arrary_value);

//                        return ;
//                    }
//                    unsigned int tmp_addr;
//                    tmp_addr = get_energy_data()->pb[i].addr;
//                    json_object_set_new(addr, "addr", json_integer(tmp_addr));
//                    for (j = 0; j < (ENERGY_BUF_SIZE - 2); j++)
//                    {
//                        json_array_insert_new(array_tmp, j,
//                                              json_real(get_energy_data()->pb[i].payload[j]));

//                    }

//                    json_object_set_new(addr, "data", array_tmp);
//                    //out2 = json_dumps(addr, JSON_REAL_PRECISION(6));

//                    json_array_insert_new(emeterData, k, addr);
//                    // out2 = json_dumps(emeterData, JSON_REAL_PRECISION(6));
//                    k++;
//                    //json_array_append_new(emeterData, array_tmp);

//                }
//            }
//            json_object_set_new(root, "emeterData", emeterData);
//            // out2 = json_dumps(root, JSON_REAL_PRECISION(6));

//            break;
//        default :
//            airpumpData = json_array();
//            if (!airpumpData)
//            {
//                json_decref(airpumpData);
//                return ;
//            }

//            k = 0;

//            for (i = count; i < (1 + count); i++)
//            {
//                kl = 0;
//                addr = json_object();
//                if (!addr)
//                {
//                    json_decref(addr);
//                    return ;
//                }
//                arrary_value = json_array();
//                if (!arrary_value)
//                {
//                    json_decref(arrary_value);
//                    return ;
//                }

//                array_tmp = json_array();
//                if (!array_tmp)
//                {
//                    json_decref(array_tmp);
//                    return ;
//                }

//                // if (get_hotter(i + 1)->status[0] != 0)
//                {
//                    unsigned int tmp_addr;
//                    tmp_addr = (get_hotter(i)->status[0]);
//                    json_object_set_new(addr, "addr", json_integer(tmp_addr));
//                    for (j = 1; j < STATUS1_SIZE; j++)
//                    {
//                        tmp = json_integer(get_hotter(i)->status[j]);
//                        json_array_insert_new(arrary_value, kl, tmp);
//                        kl++;

//                    }
//                    for (j = 1; j < STATUS2_SIZE; j++)
//                    {
//                        tmp = json_integer(get_hotter(i)->status2[j]);
//                        json_array_insert_new(arrary_value, kl, tmp);
//                        kl++;

//                    }
////                       for (j = 1; j < STATUS3_SIZE; j++)
////                       {
////                           tmp = json_integer(get_hotter(i)->status3[j]);
////                           json_array_insert_new(arrary_value, kl, tmp);
////                           kl++;
////
////                       }


//                    json_object_set_new(addr, "data", arrary_value);
//                    json_array_insert_new(airpumpData, k, addr);
//                    k++;
//                    // json_array_append_new(airpumpData, array_tmp);

//                }

//            }
//            json_object_set_new(root, "airpumpData", airpumpData);

//            count++;
//            if (count >= AIR_PUMP_SIZE)
//                count = 0;
//            break;
//    }
//    out = json_dumps(root, JSON_REAL_PRECISION(3));
//    strcpy(json_mqtt_send_buf, out);
//    free(out);


//    json_decref(arrary_value);
//    json_decref(addr);
//    json_decref(tmp);
//    json_decref(array_tmp);


//    json_decref(root);
//    json_decref(sensorSta);
//    json_decref(emeterData);
//    json_decref(airpumpData);
//    json_decref(devParams);
//    json_decref(diSta);
//    json_decref(doSta);



//}
uint64_t tmp_utc;
void anlysis_mqtt_recv()
{

    char *dev_id;
    unsigned char valid_flag, i, j, k;
    float tmp_f;
    unsigned int tt;

    //valid_flag = 0;
    get_config()->update_setting = 0;

    dev_id = find_json_string("\"Updat Frimware\": ", "\r\n", 0);
    if (dev_id != NULL)
    {
        // memset(dev_id, 0, 128);
        get_config()->update_setting = 1;
        sprintf(&get_config()->reboot, "%s", dev_id); //????
        tmp_f = atoi(&dev_id[0]);
        get_config()->reboot = tmp_f;

    }

    dev_id = find_json_string("\"Reboot Dev\": ", "\r\n", 0);
    if (dev_id != NULL)
    {
        //memset(dev_id, 0, 128);
        get_config()->update_setting = 1;
        sprintf(&get_config()->reboot, "%s", dev_id); //????
        tmp_f = atoi(&dev_id[0]);
        get_config()->reboot = tmp_f;
    }

//    dev_id = find_json_string("\"heatPump1\":", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        //memset(dev_id, 0, 128);
//        get_config()->update_setting = 1;
//        //sprintf(&get_config()->machine, "%s", dev_id); //????
//        tmp_f = atoi(&dev_id[0]);
//      if(tmp_f == 1)
//          get_config()->machine = get_config()->machine|0x0001;
//
//    }
    dev_id = find_json_string("\"heatPumpAll\" :", "\r\n", 0);
    if (dev_id != NULL)
    {
        // memset(dev_id, 0, 128);
        get_config()->update_setting = 1;
        //sprintf(&get_config()->machine, "%s", dev_id); //????
        tmp_f = atoi(&dev_id[0]);
//      if(tmp_f == 1)
        {
            get_tx_machine()->ctrl_mode = 0;
            get_config()->machine = get_config()->machine | 0x0001;

        }


    }
    dev_id = find_json_string("\"outmax\": ", "\r\n", 0);
    if (dev_id != NULL)
    {
        //memset(dev_id, 0, 128);
        get_config()->update_setting = 1;
        tmp_f = atof(&dev_id[0]);
        get_pid_params()->out_max = tmp_f;
    }
    dev_id = find_json_string("\"outmin\": ", "\r\n", 0);
    if (dev_id != NULL)
    {
        //memset(dev_id, 0, 128);
        get_config()->update_setting = 1;
        tmp_f = atof(&dev_id[0]);
        get_pid_params()->out_min = tmp_f;
    }

    dev_id = find_json_string("\"kp\": ", "\r\n", 0);
    if (dev_id != NULL)
    {
        //memset(dev_id, 0, 128);
        get_config()->update_setting = 1;
        tmp_f = atof(&dev_id[0]);
        get_pid_params()->kp_u = tmp_f;
    }
    dev_id = find_json_string("\"ki\": ", "\r\n", 0);
    if (dev_id != NULL)
    {
        //memset(dev_id, 0, 128);
        get_config()->update_setting = 1;
        tmp_f = atof(&dev_id[0]);
        get_pid_params()->ki_u = tmp_f;
    }

    dev_id = find_json_string("\"kd\": ", "\r\n", 0);
    if (dev_id != NULL)
    {
        //memset(dev_id, 0, 128);
        get_config()->update_setting = 1;
        tmp_f = atof(&dev_id[0]);
        get_pid_params()->kd_u = tmp_f;
    }
    dev_id = find_json_string("\"instru\":", "\r\n", 0);//native mode
    if (dev_id != NULL)
    {
        tmp_f = atof(&dev_id[0]);
        if (tmp_f == 0x00)
            get_config()->instru_num = DELI;
        else if (tmp_f == 0x01)
            get_config()->instru_num = ZT;
        get_config()->update_setting = 1;
    }


    dev_id = find_json_string("\"scheMode\":", "\r\n", 0);//native mode
    if (dev_id != NULL)
    {
        get_config()->mode = SCHE_MODE;
        get_config()->update_setting = 1;

    }

    dev_id = find_json_string("\"offlineMode\":", "\r\n", 0);//native mode
    if (dev_id != NULL)
    {
        get_config()->mode = OFF_MODE;
        get_config()->update_setting = 1;

    }
    dev_id = find_json_string("\"nativeMode\":", "\r\n", 0);//native mode
    if (dev_id != NULL)
    {
        get_config()->mode = NATIVE_MODE;
        get_config()->update_setting = 1;

    }
    dev_id = find_json_string("\"smartMode\":", "\r\n", 0);//native mode
    if (dev_id != NULL)
    {
        get_config()->mode = SMART_MODE;
        get_config()->update_setting = 1;

    }
    dev_id = find_json_string("\"tin_index\": ", "\r\n", 0);
    if (dev_id != NULL)
    {
        tt = atoi(&dev_id[0]);
        get_config()->tin_index = tt;
    }
    dev_id = find_json_string("\"to_index\": ", "\r\n", 0);
    if (dev_id != NULL)
    {
        tt = atoi(&dev_id[0]);
        get_config()->to_index = tt;
    }
    dev_id = find_json_string("\"pin_index\": ", "\r\n", 0);
    if (dev_id != NULL)
    {
        tt = atoi(&dev_id[0]);
        get_config()->pin_index = tt;
    }
    dev_id = find_json_string("\"po_index\": ", "\r\n", 0);
    if (dev_id != NULL)
    {
        tt = atoi(&dev_id[0]);
        get_config()->po_index = tt;
    }
    dev_id = find_json_string("\"dev_size\": ", "\r\n", 0);
    if (dev_id != NULL)
    {
        // memset(dev_id, 0, 128);
        get_config()->update_setting = 1;
        sprintf(&get_config()->reboot, "%s", dev_id); //????
        tmp_f = atoi(&dev_id[0]);
        get_config()->dev_size = tmp_f;

    }
    dev_id = find_json_string("\"private cmd\": [", "\r\n", 0);
    if (dev_id != NULL)
    {
        // memset(dev_id, 0, 128);
        short unsigned int buf[3];
        find_number_u(dev_id, buf);
        get_cmd_list(buf);
        get_config()->update_setting = 1;
        sprintf(&get_config()->reboot, "%s", dev_id); //????
        tmp_f = atoi(&dev_id[0]);
        get_config()->dev_size = tmp_f;

    }

    dev_id = find_json_string("\"heatPump1\":", "\r\n", 0);
    if (dev_id != NULL)
    {
        // memset(dev_id, 0, 128);
        get_config()->update_setting = 1;
        //sprintf(&get_config()->machine, "%s", dev_id); //????
        tmp_f = atoi(&dev_id[0]);
//      if(tmp_f == 1)
        {
            get_tx_machine()->ctrl_mode = 1;
            get_config()->machine = get_config()->machine | 0x0001;

        }


    }

    dev_id = find_json_string("\"heatPump2\":", "\r\n", 0);
    if (dev_id != NULL)
    {
        //  memset(dev_id, 0, 128);
        get_config()->update_setting = 1;
        //sprintf(&get_config()->machine, "%s", dev_id); //????
        tmp_f = atoi(&dev_id[0]);
//      if(tmp_f == 1)
        {
            get_tx_machine()->ctrl_mode = 1;
            get_config()->machine = get_config()->machine | 0x0002;

        }



    }

    dev_id = find_json_string("\"heatPump3\":", "\r\n", 0);
    if (dev_id != NULL)
    {
        //memset(dev_id, 0, 128);
        get_config()->update_setting = 1;
        //sprintf(&get_config()->machine, "%s", dev_id); //????
        tmp_f = atoi(&dev_id[0]);
//      if(tmp_f == 1)
        {
            get_tx_machine()->ctrl_mode = 1;
            get_config()->machine = get_config()->machine | 0x0004;

        }



    }

    dev_id = find_json_string("\"heatPump4\":", "\r\n", 0);
    if (dev_id != NULL)
    {
        //memset(dev_id, 0, 128);
        get_config()->update_setting = 1;
        //sprintf(&get_config()->machine, "%s", dev_id); //????
        tmp_f = atoi(&dev_id[0]);
//      if(tmp_f == 1)
        {
            get_tx_machine()->ctrl_mode = 1;
            get_config()->machine = get_config()->machine | 0x0008;

        }



    }

    dev_id = find_json_string("\"heatPump5\":", "\r\n", 0);
    if (dev_id != NULL)
    {
        //memset(dev_id, 0, 128);
        get_config()->update_setting = 1;
        //sprintf(&get_config()->machine, "%s", dev_id); //????
        tmp_f = atoi(&dev_id[0]);
//      if(tmp_f == 1)
        {
            get_tx_machine()->ctrl_mode = 1;
            get_config()->machine = get_config()->machine | 0x0010;

        }



    }

    dev_id = find_json_string("\"heatPump6\":", "\r\n", 0);
    if (dev_id != NULL)
    {
        //memset(dev_id, 0, 128);
        get_config()->update_setting = 1;
        //sprintf(&get_config()->machine, "%s", dev_id); //????
        tmp_f = atoi(&dev_id[0]);
//      if(tmp_f == 1)
        {
            get_tx_machine()->ctrl_mode = 1;
            get_config()->machine = get_config()->machine | 0x0020;

        }
    }



    dev_id = find_json_string("\"Set Out Temp\": ", "\r\n", 0);
    if (dev_id != NULL)
    {
        //memset(dev_id, 0, 128);
        get_config()->update_setting = 1;
        tmp_f = atof(&dev_id[0]);
        get_config()->set_tout = tmp_f;
        get_config()->set_tout_tmp = tmp_f;


    }

    dev_id = find_json_string("Set Room Temp: ", "\r\n", 0);
    if (dev_id != NULL)
    {
        //memset(dev_id, 0, 128);
        //  get_config()->update_setting = 2;
        tmp_f = atof(&dev_id[0]);
        get_config()->set_tindoor = tmp_f;

    }

    dev_id = find_json_string("timestamp", ",", 0);
    if (dev_id != NULL)
    {
        //  memset(dev_id, 0, 128);
        dev_id = find_json_string("byte,", "\r\n", 0);
        if (dev_id != NULL)
        {
            i = 0;
            tmp_utc = 0;
            while (dev_id[i] >= 0x30 && dev_id[i] <= 0x39)
            {
                j = dev_id[i];
                j = j - 0x30;
                // tmp_utc = 0;
                tmp_utc = tmp_utc * 10 + j;
                i++;

            }
            utcTortc(tmp_utc);

        }
    }
    dev_id = find_json_string("\"Set Upload Period(second)\": ", "\r\n", 0);
    if (dev_id != NULL)
    {
        //  memset(dev_id, 0, 128);
        // get_config()- w  >update_setting = 2;
        tmp_f = atof(&dev_id[0]);
        get_config()->set_up_period = tmp_f;
		get_config()->update_setting = 1;
    }
    dev_id = find_json_string("\"do_num\": ", "\r\n", 0);
    if (dev_id != NULL)
    {
        //memset(dev_id, 0, 128);
        long int tmp_i;
        tmp_i = atoi(&dev_id[0]);
        set_do_out()->do_num = tmp_i;
        //set_do_out()->do_update = 1;

    }
    dev_id = find_json_string("\"do_set\": ", "\r\n", 0);
    if (dev_id != NULL)
    {
        //memset(dev_id, 0, 128);
        long int tmp_i;
        tmp_i = atoi(&dev_id[0]);
        set_do_out()->do_out = tmp_i;
        set_do_out()->do_update = 1;

    }

    float *bufa;
    dev_id = find_json_string("\"Room Temp\":[", "Room", 1);//schedule paln
    if (dev_id != NULL)
    {
        //memset(dev_id, 0, 128);
        unsigned char buf[32];
        j = 0;
        k = 0;
        i = 0;
        get_config()->tlen = 0;
        while (dev_id[i] != ']')
        {
            if (dev_id[i] >= 0x2e)
            {
                get_config()->count = 0;

            }

            if (dev_id[i] != ',' && dev_id[i] != ']')
            {

                buf[j++] = dev_id[i];

            }

            else
            {

                tmp_f = atof(buf);
                get_config()->indoor_temperature[k++] = tmp_f;
                get_config()->tlen++;
                memset(buf, 0, 16);
                j = 0;
            }
            i++;

            get_config()->update_setting = 2;

        }

        tmp_f = atof(buf);
        get_config()->indoor_temperature[k++] = tmp_f;
#if CTRL_EN

        bufa = low_temperature_cal(get_config()->indoor_temperature,
                                   get_config()->tlen + 1);
        get_temp_cal(bufa);
        //fuzzy_proc(get_config()->mode);  //smart ctrl
        memset(get_config()->indoor_temperature, 0, 64);
#endif

    }
    else
    {
        dev_id = find_json_string("\"Temp\": ", ",\r\n", 1);//schedule paln
        if (dev_id != NULL)
        {
            if (dev_id[i] >= 0x2e)
            {
                get_config()->count = 0;

            }


            j = 0;
            k = 0;
            i = 0;
            get_config()->tlen = 0;

            tmp_f = atof(&dev_id[0]);
            get_config()->indoor_temperature[k++] = tmp_f;
#if CTRL_EN

            bufa = low_temperature_cal(get_config()->indoor_temperature,
                                       get_config()->tlen + 1);
            get_temp_cal(bufa);
            //fuzzy_proc(get_config()->mode);  //smart ctrl
            memset(get_config()->indoor_temperature, 0, 64);
#endif

        }

    }


    dev_id = find_json_string("\"Plan1\": [", "\r\n", 1);
    if (dev_id != NULL)
    {
        plan_analy(dev_id, 0);
    }
    dev_id = find_json_string("\"Plan2\": [", "\r\n", 1);
    if (dev_id != NULL)
    {
        plan_analy(dev_id, 1);
    }
    dev_id = find_json_string("\"Plan3\": [", "\r\n", 1);
    if (dev_id != NULL)
    {
        plan_analy(dev_id, 2);
    }
    dev_id = find_json_string("\"Plan4\": [", "\r\n", 1);
    if (dev_id != NULL)
    {
        plan_analy(dev_id, 3);
    }
    dev_id = find_json_string("\"Plan5\": [", "\r\n", 1);
    if (dev_id != NULL)
    {
        plan_analy(dev_id, 4);
    }
    dev_id = find_json_string("\"Plan6\": [", "\r\n", 1);
    if (dev_id != NULL)
    {
        plan_analy(dev_id, 5);
    }
    dev_id = find_json_string("\"Plan7\": [", "\r\n", 1);
    if (dev_id != NULL)
    {
        plan_analy(dev_id, 6);
    }
    dev_id = find_json_string("\"Plan8\": [", "\r\n", 1);
    if (dev_id != NULL)
    {
        plan_analy(dev_id, 7);
    }
    dev_id = find_json_string("\"Plan9\": [", "\r\n", 1);
    if (dev_id != NULL)
    {
        plan_analy(dev_id, 8);
    }
    dev_id = find_json_string("\"Plan10\": [", "\r\n", 1);
    if (dev_id != NULL)
    {
        plan_analy(dev_id, 9);
    }

    memset(mqtt_recv->Lpuart1RecBuff, 0, LPUART1_REC_SIZE);

    //  json_analysis((char *)mqtt_recv->Lpuart1RecBuff);






}


void json_upload()
{
    if (mqtt_payload_u.mqtt_state == MQTT_READY &&
            lte_recv->Lpuart1RecFlag == 0)
    {
        jsson_pack(mqtt_payload_u.process_step);
        mqtt_payload_u.mqtt_state = MQTT_BUSY;
        mqtt_payload_u.process_step ++ ;

        if (mqtt_payload_u.process_step >= MAX_STEP)
        {
            mqtt_payload_u.process_step = 0;
        }

        mqtt_at_cmd_num = AT_MPUB;
    }
    else
    {
        if (lte_recv->Lpuart1RecFlag == 1)
            if (mqtt_at_cmd_num != AT_MPUB_RECV)

            {
                mqtt_at_cmd_num = AT_MPUB_RECV;
            }
    }
}
mqtt_payload_stru *get_mqtt_payload()
{
    return &mqtt_payload_u;
}


uint64_t tmp_utc;
//void anlysis_mqtt_recv()
//{
//
//    char *dev_id;
//    unsigned char valid_flag, i, j, k;
//    float tmp_f;
//    unsigned int tt;
//
//    //valid_flag = 0;
//    get_config()->update_setting = 0;
//
//    dev_id = find_json_string("\"Updat Frimware\": ", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        // memset(dev_id, 0, 128);
//        get_config()->update_setting = 1;
//        sprintf(&get_config()->reboot, "%s", dev_id); //????
//        tmp_f = atoi(&dev_id[0]);
//        get_config()->reboot = tmp_f;
//
//    }
//
//    dev_id = find_json_string("\"Reboot Dev\": ", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        //memset(dev_id, 0, 128);
//        get_config()->update_setting = 1;
//        sprintf(&get_config()->reboot, "%s", dev_id); //????
//        tmp_f = atoi(&dev_id[0]);
//        get_config()->reboot = tmp_f;
//    }
//
//    //    dev_id = find_json_string("\"heatPump1\":", "\r\n", 0);
//    //    if (dev_id != NULL)
//    //    {
//    //        //memset(dev_id, 0, 128);
//    //        get_config()->update_setting = 1;
//    //        //sprintf(&get_config()->machine, "%s", dev_id); //????
//    //        tmp_f = atoi(&dev_id[0]);
//    //      if(tmp_f == 1)
//    //          get_config()->machine = get_config()->machine|0x0001;
//    //
//    //    }
//    dev_id = find_json_string("\"heatPumpAll\":", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        // memset(dev_id, 0, 128);
//        get_config()->update_setting = 1;
//        //sprintf(&get_config()->machine, "%s", dev_id); //????
//        tmp_f = atoi(&dev_id[0]);
//        //      if(tmp_f == 1)
//        {
//            get_tx_machine()->ctrl_mode = 0;
//            get_config()->machine = get_config()->machine | 0x0001;
//
//        }
//
//
//    }
//    dev_id = find_json_string("\"outmax\": ", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        //memset(dev_id, 0, 128);
//        get_config()->update_setting = 1;
//        tmp_f = atof(&dev_id[0]);
//        get_pid_params()->out_max = tmp_f;
//    }
//    dev_id = find_json_string("\"outmin\": ", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        //memset(dev_id, 0, 128);
//        get_config()->update_setting = 1;
//        tmp_f = atof(&dev_id[0]);
//        get_pid_params()->out_min = tmp_f;
//    }
//
//    dev_id = find_json_string("\"kp\": ", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        //memset(dev_id, 0, 128);
//        get_config()->update_setting = 1;
//        tmp_f = atof(&dev_id[0]);
//        get_pid_params()->kp_u = tmp_f;
//    }
//    dev_id = find_json_string("\"ki\": ", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        //memset(dev_id, 0, 128);
//        get_config()->update_setting = 1;
//        tmp_f = atof(&dev_id[0]);
//        get_pid_params()->ki_u = tmp_f;
//    }
//
//    dev_id = find_json_string("\"kd\": ", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        //memset(dev_id, 0, 128);
//        get_config()->update_setting = 1;
//        tmp_f = atof(&dev_id[0]);
//        get_pid_params()->kd_u = tmp_f;
//    }
//    dev_id = find_json_string("\"instru\":", "\r\n", 0);//native mode
//    if (dev_id != NULL)
//    {
//        tmp_f = atof(&dev_id[0]);
//        if (tmp_f == 0x00)
//            get_config()->instru_num = DELI;
//        else if (tmp_f == 0x01)
//            get_config()->instru_num = ZT;
//        get_config()->update_setting = 1;
//    }
//
//
//    dev_id = find_json_string("\"scheMode\":", "\r\n", 0);//native mode
//    if (dev_id != NULL)
//    {
//        get_config()->mode = SCHE_MODE;
//        get_config()->update_setting = 1;
//
//    }
//
//    dev_id = find_json_string("\"offlineMode\":", "\r\n", 0);//native mode
//    if (dev_id != NULL)
//    {
//        get_config()->mode = OFF_MODE;
//        get_config()->update_setting = 1;
//
//    }
//    dev_id = find_json_string("\"nativeMode\":", "\r\n", 0);//native mode
//    if (dev_id != NULL)
//    {
//        get_config()->mode = NATIVE_MODE;
//        get_config()->update_setting = 1;
//
//    }
//    dev_id = find_json_string("\"smartMode\":", "\r\n", 0);//native mode
//    if (dev_id != NULL)
//    {
//        get_config()->mode = SMART_MODE;
//        get_config()->update_setting = 1;
//
//    }
//    dev_id = find_json_string("\"tin_index\": ", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        tt = atoi(&dev_id[0]);
//        get_config()->tin_index = tt;
//    }
//    dev_id = find_json_string("\"to_index\": ", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        tt = atoi(&dev_id[0]);
//        get_config()->to_index = tt;
//    }
//    dev_id = find_json_string("\"pin_index\": ", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        tt = atoi(&dev_id[0]);
//        get_config()->pin_index = tt;
//    }
//    dev_id = find_json_string("\"po_index\": ", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        tt = atoi(&dev_id[0]);
//        get_config()->po_index = tt;
//    }
//    dev_id = find_json_string("\"dev_size\": ", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        // memset(dev_id, 0, 128);
//        get_config()->update_setting = 1;
//        sprintf(&get_config()->reboot, "%s", dev_id); //????
//        tmp_f = atoi(&dev_id[0]);
//        get_config()->dev_size = tmp_f;
//
//    }
//
//    dev_id = find_json_string("\"heatPump1\":", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        // memset(dev_id, 0, 128);
//        get_config()->update_setting = 1;
//        //sprintf(&get_config()->machine, "%s", dev_id); //????
//        tmp_f = atoi(&dev_id[0]);
//        //      if(tmp_f == 1)
//        {
//            get_tx_machine()->ctrl_mode = 1;
//            get_config()->machine = get_config()->machine | 0x0001;
//
//        }
//
//
//    }
//
//    dev_id = find_json_string("\"heatPump2\":", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        //  memset(dev_id, 0, 128);
//        get_config()->update_setting = 1;
//        //sprintf(&get_config()->machine, "%s", dev_id); //????
//        tmp_f = atoi(&dev_id[0]);
//        //      if(tmp_f == 1)
//        {
//            get_tx_machine()->ctrl_mode = 1;
//            get_config()->machine = get_config()->machine | 0x0002;
//
//        }
//
//
//
//    }
//
//    dev_id = find_json_string("\"heatPump3\":", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        //memset(dev_id, 0, 128);
//        get_config()->update_setting = 1;
//        //sprintf(&get_config()->machine, "%s", dev_id); //????
//        tmp_f = atoi(&dev_id[0]);
//        //      if(tmp_f == 1)
//        {
//            get_tx_machine()->ctrl_mode = 1;
//            get_config()->machine = get_config()->machine | 0x0004;
//
//        }
//
//
//
//    }
//
//    dev_id = find_json_string("\"heatPump4\":", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        //memset(dev_id, 0, 128);
//        get_config()->update_setting = 1;
//        //sprintf(&get_config()->machine, "%s", dev_id); //????
//        tmp_f = atoi(&dev_id[0]);
//        //      if(tmp_f == 1)
//        {
//            get_tx_machine()->ctrl_mode = 1;
//            get_config()->machine = get_config()->machine | 0x0008;
//
//        }
//
//
//
//    }
//
//    dev_id = find_json_string("\"heatPump5\":", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        //memset(dev_id, 0, 128);
//        get_config()->update_setting = 1;
//        //sprintf(&get_config()->machine, "%s", dev_id); //????
//        tmp_f = atoi(&dev_id[0]);
//        //      if(tmp_f == 1)
//        {
//            get_tx_machine()->ctrl_mode = 1;
//            get_config()->machine = get_config()->machine | 0x0010;
//
//        }
//
//
//
//    }
//
//    dev_id = find_json_string("\"heatPump6\":", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        //memset(dev_id, 0, 128);
//        get_config()->update_setting = 1;
//        //sprintf(&get_config()->machine, "%s", dev_id); //????
//        tmp_f = atoi(&dev_id[0]);
//        //      if(tmp_f == 1)
//        {
//            get_tx_machine()->ctrl_mode = 1;
//            get_config()->machine = get_config()->machine | 0x0020;
//
//        }
//    }
//
//
//
//    dev_id = find_json_string("\"Set Out Temp\": ", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        //memset(dev_id, 0, 128);
//        get_config()->update_setting = 1;
//        tmp_f = atof(&dev_id[0]);
//        get_config()->set_tout = tmp_f;
//        get_config()->set_tout_tmp = tmp_f;
//
//
//    }
//
//    dev_id = find_json_string("\"Set Room Temp\": ", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        //memset(dev_id, 0, 128);
//        //  get_config()->update_setting = 2;
//        tmp_f = atof(&dev_id[0]);
//        get_config()->set_tindoor = tmp_f;
//
//    }
//
//    dev_id = find_json_string("timestamp", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        //  memset(dev_id, 0, 128);
//        dev_id = find_json_string("byte,", "\r\n", 0);
//        if (dev_id != NULL)
//        {
//            i = 0;
//            tmp_utc = 0;
//            while (dev_id[i] >= 0x30 && dev_id[i] <= 0x39)
//            {
//                j = dev_id[i];
//                j = j - 0x30;
//                // tmp_utc = 0;
//                tmp_utc = tmp_utc * 10 + j;
//                i++;
//
//            }
//            utcTortc(tmp_utc);
//
//        }
//    }
//    dev_id = find_json_string("\"Set Upload Period(second)\": ", "\r\n", 0);
//    if (dev_id != NULL)
//    {
//        //  memset(dev_id, 0, 128);
//        // get_config()- w  >update_setting = 2;
//        tmp_f = atof(&dev_id[0]);
//        get_config()->set_up_period = tmp_f;
//    }
//
//    float *bufa;
//    dev_id = find_json_string("\"Room Temp\":[", "Room", 1);//schedule paln
//    if (dev_id != NULL)
//    {
//        //memset(dev_id, 0, 128);
//        unsigned char buf[32];
//        j = 0;
//        k = 0;
//        i = 0;
//        get_config()->tlen = 0;
//        while (dev_id[i] != ']')
//        {
//            if (dev_id[i] >= 0x2e)
//            {
//                get_config()->count = 0;
//
//            }
//
//            if (dev_id[i] != ',' && dev_id[i] != ']')
//            {
//
//                buf[j++] = dev_id[i];
//
//            }
//
//            else
//            {
//
//                tmp_f = atof(buf);
//                get_config()->indoor_temperature[k++] = tmp_f;
//                get_config()->tlen++;
//                memset(buf, 0, 16);
//                j = 0;
//            }
//            i++;
//
//            get_config()->update_setting = 2;
//
//        }
//
//        tmp_f = atof(buf);
//        get_config()->indoor_temperature[k++] = tmp_f;
//#if CTRL_EN
//
//        bufa = low_temperature_cal(get_config()->indoor_temperature,
//                                   get_config()->tlen + 1);
//        get_temp_cal(bufa);
//        //fuzzy_proc(get_config()->mode);  //smart ctrl
//        memset(get_config()->indoor_temperature, 0, 64);
//#endif
//
//    }
//    else
//    {
//        dev_id = find_json_string("\"Temp\": ", ",\r\n", 1);//schedule paln
//        if (dev_id != NULL)
//        {
//            if (dev_id[i] >= 0x2e)
//            {
//                get_config()->count = 0;
//
//            }
//
//
//            j = 0;
//            k = 0;
//            i = 0;
//            get_config()->tlen = 0;
//
//            tmp_f = atof(&dev_id[0]);
//            get_config()->indoor_temperature[k++] = tmp_f;
//#if CTRL_EN
//
//            bufa = low_temperature_cal(get_config()->indoor_temperature,
//                                       get_config()->tlen + 1);
//            get_temp_cal(bufa);
//            //fuzzy_proc(get_config()->mode);  //smart ctrl
//            memset(get_config()->indoor_temperature, 0, 64);
//#endif
//
//        }
//
//    }
//
//
//    dev_id = find_json_string("\"Plan1\": [", "\r\n", 1);
//    if (dev_id != NULL)
//    {
//        plan_analy(dev_id, 0);
//    }
//    dev_id = find_json_string("\"Plan2\": [", "\r\n", 1);
//    if (dev_id != NULL)
//    {
//        plan_analy(dev_id, 1);
//    }
//    dev_id = find_json_string("\"Plan3\": [", "\r\n", 1);
//    if (dev_id != NULL)
//    {
//        plan_analy(dev_id, 2);
//    }
//    dev_id = find_json_string("\"Plan4\": [", "\r\n", 1);
//    if (dev_id != NULL)
//    {
//        plan_analy(dev_id, 3);
//    }
//    dev_id = find_json_string("\"Plan5\": [", "\r\n", 1);
//    if (dev_id != NULL)
//    {
//        plan_analy(dev_id, 4);
//    }
//    dev_id = find_json_string("\"Plan6\": [", "\r\n", 1);
//    if (dev_id != NULL)
//    {
//        plan_analy(dev_id, 5);
//    }
//    dev_id = find_json_string("\"Plan7\": [", "\r\n", 1);
//    if (dev_id != NULL)
//    {
//        plan_analy(dev_id, 6);
//    }
//    dev_id = find_json_string("\"Plan8\": [", "\r\n", 1);
//    if (dev_id != NULL)
//    {
//        plan_analy(dev_id, 7);
//    }
//    dev_id = find_json_string("\"Plan9\": [", "\r\n", 1);
//    if (dev_id != NULL)
//    {
//        plan_analy(dev_id, 8);
//    }
//    dev_id = find_json_string("\"Plan10\": [", "\r\n", 1);
//    if (dev_id != NULL)
//    {
//        plan_analy(dev_id, 9);
//    }
//
//    memset(mqtt_recv->Lpuart1RecBuff, 0, LPUART1_REC_SIZE);
//
//    //  json_analysis((char *)mqtt_recv->Lpuart1RecBuff);
//
//
//
//
//
//
//}

/*
dev---->ser
{
    "Dev ID": "866289037465624",
    "Status Data": {
        "Out Tem": "45",
        "In Tem": "26",
        "Front Pressure": "0.5",
        "After Pressure": "0.5",
        "Status": "4"
    },
    "Dev Params": {
        "Version": "V3.0.1",
        "Set Out Tem": "45",
        "Set In Tem": "23",
        "Upload Period(second)": "10"
    }
}*/

/*服务�?-->设备（周期下发）
{
   "Dev ID":  "866289037465624",
   "Room Temp": {
               "Room 1 Temp": "15",
               "Room 2 Temp": "18",
               "Room 3 Temp": "16",
               "Room 4 Temp": "17",
               "Room 5 Temp": "15",
               "Room 6 Temp": "13",
               "Room 7 Temp": "12",
               "Room 8 Temp": "21",
               "Room 9 Temp": "20",
               "Room 10 Temp": "15"
     }
}*

/*服务�?-->设备
{
   "Dev ID":  "866289037465624",
   "Dev Ctrl": {
               "Updat Frimware": "1",
               "Reboot Dev": "1",
               "Power ctrl": "1",
               "Set Out Temp": "45",
               "Set Room Temp": "23",
               "Set Upload Period(second)": "13"
     }
}*/

/*服务�?-->设备（周期下发）
{
   "Dev ID":  "866289037465624",
   "Room Temp": {
               "Room 1 Temp": "15",
               "Room 2 Temp": "18",
               "Room 3 Temp": "16",
               "Room 4 Temp": "17",
               "Room 5 Temp": "15",
               "Room 6 Temp": "13",
               "Room 7 Temp": "12",
               "Room 8 Temp": "21",
               "Room 9 Temp": "20",
               "Room 10 Temp": "15"
     }
}*/

/*else if (ATCmdNum == AT_MPUB)//????
{
    sprintf(send_buffer, "%s%s,%d\0D\0A",ATCmds[ATCmdNum].ATSendStr,
                                             config_usr.mqtt_msubtopic, 0);
    HAL_UART_Transmit(&huart1, (uint8_t *)send_buffer, strlen(send_buffer), 0xFF);
    memset(send_buffer, 0x00, strlen(send_buffer));
    }*/
//char *mqtt_send_buf;


//????
/********************************************************************
tmp = get_ai_data()->channel_status;
    mqtt_payload_u.status[DEV_STATUS_INDEX] = get_di_data()->di_status; //8 bit di
    mqtt_payload_u.status[DEV_STATUS_INDEX] =
        mqtt_payload_u.status[DEV_STATUS_INDEX] << 20; //dev status
    mqtt_payload_u.status[DEV_STATUS_INDEX] =
        mqtt_payload_u.status[DEV_STATUS_INDEX] | tmp; //20bit ai but 8bit used
    mqtt_payload_u.status[DEV_STATUS_INDEX] =
        mqtt_payload_u.status[DEV_STATUS_INDEX] ;
    mqtt_payload_u.status[DEV_MASK_INDEX] = get_config()->fault_mask;
    mqtt_payload_u.status[DEV_PUMP_STATUS_INDEX] = get_recv_machine()->fault;



mqtt_payload_u.status[DEV_STATUS_INDEX]:

bit 31-bit24:di7-di0
bit 19-bit8:pt100_11-pt100_0  bit8:out temperature;bit9:in temperature
bit 7-bit0: press_7-pt100_0   bit0:front pressure; bit1:after pressure


mqtt_payload_u.status[DEV_PUMP_STATUS_INDEX]
bit 15-bit8:dev addr
bit 7-bit0: dev error code

*********************************************************************/
//void upload()
//{
//
//    unsigned char buf2[256];
//
//    memset(mqtt_payload_u.devid, 0, sizeof(mqtt_payload_u.devid));
//    memset(mqtt_payload_u.version, 0, sizeof(mqtt_payload_u.version));
//    sprintf(mqtt_payload_u.devid, "%s", get_config()->user_id);//devid
//
//    mqtt_payload_u.data[TOUT_INDEX] = get_ai_data()->temp[get_config()->to_index];
//
//    if (get_config()->mode == SMART_MODE)
//        mqtt_payload_u.data[TIN_INDEX] = get_indoor_temp();
//    else
//        // mqtt_payload_u.data[TIN_INDEX] =
//        //   get_ai_data()->temp[get_config()->tin_index]; //water IN
//        mqtt_payload_u.data[TIN_INDEX] = get_indoor_temp();
//
//    mqtt_payload_u.data[PUMP_F_INDEX] = *get_power();
//    //get_ai_data()->press[get_config()->pin_index]; //pump front
//    mqtt_payload_u.data[PUMP_E_INDEX] =
//        get_ai_data()->press[get_config()->po_index]; //pump end
//
//    uint32_t tmp;
//    tmp = get_ai_data()->channel_status;
//    mqtt_payload_u.status[DEV_STATUS_INDEX] = get_di_data()->di_status; //16 bit di
//    mqtt_payload_u.status[DEV_STATUS_INDEX] =
//        mqtt_payload_u.status[DEV_STATUS_INDEX] << 11; //dev status
//    mqtt_payload_u.status[DEV_STATUS_INDEX] =
//        mqtt_payload_u.status[DEV_STATUS_INDEX] | tmp; //11bit
//    mqtt_payload_u.status[DEV_STATUS_INDEX] =
//        mqtt_payload_u.status[DEV_STATUS_INDEX] & 0x07ffffff ;
//
//
//    mqtt_payload_u.status[DEV_MASK_INDEX] = get_config()->fault_mask;
//    mqtt_payload_u.status[DEV_PUMP_STATUS_INDEX] = get_recv_machine()->fault;
//
//    get_config()->fault_status = mqtt_payload_u.status[DEV_STATUS_INDEX]
//                                 ;//fault status
//    sprintf(mqtt_payload_u.version, "%s", get_config()->version);//devid
//    if (get_config()->mode == SMART_MODE)
//        mqtt_payload_u.data[WATER_O_INDEX] = get_pid_output(); //set tmp
//    else
//        mqtt_payload_u.data[WATER_O_INDEX] = get_config()->set_tout; //set tmp
//    if (get_config()->mode == SMART_MODE)
//        mqtt_payload_u.data[WATER_IN_INDEX] = get_config()->set_tout;
//    else
//        mqtt_payload_u.data[WATER_IN_INDEX] = get_config()->set_tindoor;
//    mqtt_payload_u.data[WATER_IN_INDEX] =
//        get_ai_data()->temp[get_config()->tin_index];
//    //get_config()->set_tindoor; //set indoor tmp
//    mqtt_payload_u.data[UP_PERIOD_INDEX] = get_config()->set_up_period; //up period
//    if (mqtt_payload_u.data[WATER_O_INDEX] >= 1000)
//        mqtt_payload_u.data[WATER_O_INDEX] = 1000;
//
//
//
//#if 1
//    memset(buf2, 0, sizeof(buf2));
//    unsigned char buf3[128], numberBuffer[4], i;
//    i = 0;
//
//    buf2[i++] = getRtcDate()->Year;
//    buf2[i++] = getRtcDate()->Month;
//    buf2[i++] = getRtcDate()->Date;
//    buf2[i++] = getRtcDate()->WeekDay;
//    buf2[i++] = getRtcTime()->Hours;
//    buf2[i++] = getRtcTime()->Minutes;
//    buf2[i++] = getRtcTime()->Seconds;
//
//    sprintf(buf3, "%u-%u-%u-%u %u:%u:%u%", buf2[0]
//            , buf2[1]
//            , buf2[2]
//            , buf2[3]
//            , buf2[4]
//            , buf2[5]
//            , buf2[6]
//           );
//
//    i = 0;
//    static unsigned char addr = 1, status_out = 1;
//
//
//    //  sprintf(buf2, "[%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u]",
//    //          get_hotter(addr)->status[0],
//    //          get_hotter(addr)->status[1],
//    //          get_hotter(addr)->status[2],
//    //          get_hotter(addr)->status[3],
//    //          get_hotter(addr)->status[4],
//    //          get_hotter(addr)->status[5],
//    //          get_hotter(addr)->status[6],
//    //          get_hotter(addr)->status[7],
//    //          get_hotter(addr)->status[8],
//    //          get_hotter(addr)->status[9],
//    //          get_hotter(addr)->status[10],
//    //          get_hotter(addr)->status[11],
//    //          get_hotter(addr)->status[12],
//    //          get_hotter(addr)->status[13],
//    //          get_hotter(addr)->status[14],
//    //          get_hotter(addr)->status[15],
//    //          get_hotter(addr)->status[16],
//    //          get_hotter(addr)->status[17],
//    //          get_hotter(addr)->status[18],
//    //          get_hotter(addr)->status[19],
//    //          get_hotter(addr)->status[20],
//    //          get_hotter(addr)->status[21],
//    //          get_hotter(addr)->status[22]);
//
//
//
//
//    memset(buf2, 0, sizeof(buf2));
//    if (status_out == 1) //0x0000-0x0007
//    {
//        buf2[0] = '[';
//        for (i = 0; i < 8; i++)
//        {
//            sprintf(buf2 + strlen(buf2), "%hd,", get_hotter(addr)->status[i]);
//
//        }
//        sprintf(buf2 + strlen(buf2), "%hd", get_hotter(addr)->status[i]);
//        buf2[strlen(buf2)] = ']';
//        //   sprintf(buf2, "[%u,%u,%u,%u,%u,%u,%u,%u,%u,%u]",
//        //         get_hotter(addr)->status[0],
//        //         get_hotter(addr)->status[1],
//        //         get_hotter(addr)->status[2],
//        //         get_hotter(addr)->status[3],
//        //         get_hotter(addr)->status[4],
//        //         get_hotter(addr)->status[5],
//        //         get_hotter(addr)->status[6],
//        //         get_hotter(addr)->status[7],
//        //         get_hotter(addr)->status[8],
//        //         get_hotter(addr)->status[9]);
//
//    }
//    else if (status_out == 2) //0-20
//    {
//
//        buf2[0] = '[';
//        sprintf(buf2 + strlen(buf2), "%hd,", get_hotter(addr)->status2[0]);
//        //get_hotter(addr)->status2[0] = 15;
//        for (i = 1; i < 18; i++) //1-18
//        {
//            sprintf(buf2 + strlen(buf2), "%hd,", get_hotter(addr)->status2[i]);
//
//        }
//        sprintf(buf2 + strlen(buf2), "%hd", get_hotter(addr)->status2[i]);
//        buf2[strlen(buf2)] = ']';
//
//
//    }
//    else if (status_out == 3) //19-36
//    {
//        buf2[0] = '[';
//        sprintf(buf2 + strlen(buf2), "%hd,", get_hotter(addr)->status2[0]);
//        for (i = 19; i < 36; i++)
//        {
//            sprintf(buf2 + strlen(buf2), "%hd,", get_hotter(addr)->status2[i]);
//
//        }
//        sprintf(buf2 + strlen(buf2), "%hd", get_hotter(addr)->status2[i]);
//        buf2[strlen(buf2)] = ']';
//
//
//    }
//    else if (status_out == 4) //37-54
//    {
//        buf2[0] = '[';
//        sprintf(buf2 + strlen(buf2), "%hd,", get_hotter(addr)->status2[0]);
//        for (i = 37; i < 54; i++)
//        {
//            sprintf(buf2 + strlen(buf2), "%hd,", get_hotter(addr)->status2[i]);
//
//        }
//        sprintf(buf2 + strlen(buf2), "%hd", get_hotter(addr)->status2[i]);
//        buf2[strlen(buf2)] = ']';
//
//
//    }
//
//    else if (status_out == 5) //55-72
//    {
//        buf2[0] = '[';
//        sprintf(buf2 + strlen(buf2), "%hd,", get_hotter(addr)->status2[0]);
//        for (i = 55; i < 72; i++)
//        {
//            sprintf(buf2 + strlen(buf2), "%hd,", get_hotter(addr)->status2[i]);
//
//        }
//        sprintf(buf2 + strlen(buf2), "%hd", get_hotter(addr)->status2[i]);
//        buf2[strlen(buf2)] = ']';
//
//        memset(get_hotter(addr)->status2, 0, STATUS2_SIZE);
//        memset(get_hotter(addr)->status, 0, STATUS1_SIZE);
//    }
//
//
//
//
//
//    if (status_out == 1)
//    {
//        sprintf(mqtt_send_buf, "{\\0D\\0A\\
//                \\22Dev ID\\22: %s,\\0D\\0A\\
//                \\22Status Data\\22: {\\0D\\0A\\
//                                      \\22Out Tem\\22: %.1f,\\0D\\0A\\
//                                      \\22In Tem\\22: %.1f,\\0D\\0A\\
//                                      \\22FP\\22: %.2f,\\0D\\0A\\
//                                      \\22AP\\22: %.2f,\\0D\\0A\\
//                                      \\22Dsta\\22: %u,\\0D\\0A\\
//                                      \\22Sta1\\22: %s\\0D\\0A\\
//                                     },\\0D\\0A\\
//                \\22Dev Params\\22: {\\0D\\0A\\
//                                     \\22St\\22: %u,\\0D\\0A\\
//                                     \\22SOt\\22: %.1f,\\0D\\0A\\
//                                     \\22SIt\\22: %.1f,\\0D\\0A\\
//                                     \\22p\\22: %.0f,\\0D\\0A\\
//                                     \\22i\\22: %.3f,\\0D\\0A\\
//                                     \\22d\\22: %.3f\\0D\\0A\\
//                                    }\\0D\\0A\\
//    }", mqtt_payload_u.devid,
//    mqtt_payload_u.data[TOUT_INDEX],
//                        mqtt_payload_u.data[TIN_INDEX],
//                        mqtt_payload_u.data[PUMP_F_INDEX],
//                        mqtt_payload_u.data[PUMP_E_INDEX],
//                        mqtt_payload_u.status[DEV_STATUS_INDEX],
//                        buf2,
//                        get_config()->set_tout,
//                        mqtt_payload_u.data[WATER_O_INDEX],
//                        mqtt_payload_u.data[WATER_IN_INDEX],
//                        get_pid_params()->kp_u,
//                        get_pid_params()->ki_u,
//                        get_pid_params()->kd_u);
//
//
//
//}
//else if (status_out == 2)
//{
//
//    sprintf(mqtt_send_buf, "{\\0D\\0A\\
//            \\22Dev ID\\22: %s,\\0D\\0A\\
//            \\22Status Data\\22: {\\0D\\0A\\
//                                  \\22Out Tem\\22: %.1f,\\0D\\0A\\
//                                  \\22In Tem\\22: %.1f,\\0D\\0A\\
//                                  \\22FroP\\22: %.2f,\\0D\\0A\\
//                                  \\22AftP\\22: %.2f,\\0D\\0A\\
//                                  \\22Status\\22: %u,\\0D\\0A\\
//                                  \\22air pump_status2\\22: %s\\0D\\0A\\
//                                 },\\0D\\0A\\
//            \\22Dev Params\\22: {\\0D\\0A\\
//                                 \\22Version\\22: \\22%s\\22,\\0D\\0A\\
//                                 \\22Set Out Tem\\22: %.1f,\\0D\\0A\\
//                                 \\22Set In Tem\\22: %.1f,\\0D\\0A\\
//                                 \\22Upload Period(second)\\22: %.0f\\0D\\0A\\
//                                }\\0D\\0A\\
//}", mqtt_payload_u.devid,
//mqtt_payload_u.data[TOUT_INDEX],
//                    mqtt_payload_u.data[TIN_INDEX],
//                    mqtt_payload_u.data[PUMP_F_INDEX],
//                    mqtt_payload_u.data[PUMP_E_INDEX],
//                    mqtt_payload_u.status[DEV_STATUS_INDEX],
//                    buf2,
//                    mqtt_payload_u.version,
//                    mqtt_payload_u.data[WATER_O_INDEX],
//                    mqtt_payload_u.data[WATER_IN_INDEX],
//                    mqtt_payload_u.data[UP_PERIOD_INDEX]);
//}
//else if (status_out == 3)
//{
//
//sprintf(mqtt_send_buf, "{\\0D\\0A\\
//        \\22Dev ID\\22: %s,\\0D\\0A\\
//        \\22Status Data\\22: {\\0D\\0A\\
//                              \\22Out Tem\\22: %.1f,\\0D\\0A\\
//                              \\22In Tem\\22: %.1f,\\0D\\0A\\
//                              \\22Front Pressure\\22: %.2f,\\0D\\0A\\
//                              \\22After Pressure\\22: %.2f,\\0D\\0A\\
//                              \\22Status\\22: %u,\\0D\\0A\\
//                              \\22air pump_status3\\22: %s\\0D\\0A\\
//                             },\\0D\\0A\\
//        \\22Dev Params\\22: {\\0D\\0A\\
//                             \\22Version\\22: \\22%s\\22,\\0D\\0A\\
//                             \\22Set Out Tem\\22: %.1f,\\0D\\0A\\
//                             \\22Set In Tem\\22: %.1f,\\0D\\0A\\
//                             \\22Upload Period(second)\\22: %.0f\\0D\\0A\\
//                            }\\0D\\0A\\
//}", mqtt_payload_u.devid,
//mqtt_payload_u.data[TOUT_INDEX],
//mqtt_payload_u.data[TIN_INDEX],
//mqtt_payload_u.data[PUMP_F_INDEX],
//mqtt_payload_u.data[PUMP_E_INDEX],
//mqtt_payload_u.status[DEV_STATUS_INDEX],
//buf2,
//mqtt_payload_u.version,
//mqtt_payload_u.data[WATER_O_INDEX],
//mqtt_payload_u.data[WATER_IN_INDEX],
//mqtt_payload_u.data[UP_PERIOD_INDEX]);
//}
//else if (status_out == 4)
//{
//
//sprintf(mqtt_send_buf, "{\\0D\\0A\\
//        \\22Dev ID\\22: %s,\\0D\\0A\\
//        \\22Status Data\\22: {\\0D\\0A\\
//                              \\22Out Tem\\22: %.1f,\\0D\\0A\\
//                              \\22In Tem\\22: %.1f,\\0D\\0A\\
//                              \\22Front Pressure\\22: %.2f,\\0D\\0A\\
//                              \\22After Pressure\\22: %.2f,\\0D\\0A\\
//                              \\22Status\\22: %u,\\0D\\0A\\
//                              \\22air pump_status4\\22: %s\\0D\\0A\\
//                             },\\0D\\0A\\
//        \\22Dev Params\\22: {\\0D\\0A\\
//                             \\22Version\\22: \\22%s\\22,\\0D\\0A\\
//                             \\22Set Out Tem\\22: %.1f,\\0D\\0A\\
//                             \\22Set In Tem\\22: %.1f,\\0D\\0A\\
//                             \\22Upload Period(second)\\22: %.0f\\0D\\0A\\
//                            }\\0D\\0A\\
//}", mqtt_payload_u.devid,
//mqtt_payload_u.data[TOUT_INDEX],
//mqtt_payload_u.data[TIN_INDEX],
//mqtt_payload_u.data[PUMP_F_INDEX],
//mqtt_payload_u.data[PUMP_E_INDEX],
//mqtt_payload_u.status[DEV_STATUS_INDEX],
//buf2,
//mqtt_payload_u.version,
//mqtt_payload_u.data[WATER_O_INDEX],
//mqtt_payload_u.data[WATER_IN_INDEX],
//mqtt_payload_u.data[UP_PERIOD_INDEX]);
//}
//
//
//
//else
//{
//
//sprintf(mqtt_send_buf, "{\\0D\\0A\\
//        \\22Dev ID\\22: %s,\\0D\\0A\\
//        \\22Status Data\\22: {\\0D\\0A\\
//                              \\22Out Tem\\22: %.1f,\\0D\\0A\\
//                              \\22In Tem\\22: %.1f,\\0D\\0A\\
//                              \\22Front Pressure\\22: %.2f,\\0D\\0A\\
//                              \\22After Pressure\\22: %.2f,\\0D\\0A\\
//                              \\22Status\\22: %u,\\0D\\0A\\
//                              \\22air pump_status5\\22: %s\\0D\\0A\\
//                             },\\0D\\0A\\
//        \\22Dev Params\\22: {\\0D\\0A\\
//                             \\22Version\\22: \\22%s\\22,\\0D\\0A\\
//                             \\22Set Out Tem\\22: %.1f,\\0D\\0A\\
//                             \\22Set In Tem\\22: %.1f,\\0D\\0A\\
//                             \\22Upload Period(second)\\22: %.0f\\0D\\0A\\
//                            }\\0D\\0A\\
//}", mqtt_payload_u.devid,
//mqtt_payload_u.data[TOUT_INDEX],
//mqtt_payload_u.data[TIN_INDEX],
//mqtt_payload_u.data[PUMP_F_INDEX],
//mqtt_payload_u.data[PUMP_E_INDEX],
//mqtt_payload_u.status[DEV_STATUS_INDEX],
//buf2,
//mqtt_payload_u.version,
//mqtt_payload_u.data[WATER_O_INDEX],
//mqtt_payload_u.data[WATER_IN_INDEX],
//mqtt_payload_u.data[UP_PERIOD_INDEX]);
//
//
//}
//
//
//if (status_out < 5)
//{
//
//status_out++;
//
//}
//else
//{
//status_out = 1;
//if (addr < get_config()->dev_size)
//    {
//
//        addr++;
//
//    }
//    else
//        addr = 1;
//
//}
//
//
//
//#else
//    mqtt_send_buf = json_pack(&mqtt_payload_u);
//
//#endif
//mqtt_at_cmd_num = AT_MPUB;
//
//
//}
//void mqtt_msub()//????
//{
//  MQTTSendData(1,mqtt_buf);

//}
void mqtt_recv_proc()
{

    if (get_config()->update_firm == 1 || get_config()->reboot == 1)
        HAL_NVIC_SystemReset();

}
void mqtt_init()
{
    //mqtt_at_cmd_num = AT_MCONFIG;
    mqtt_at_cmd_num = AT_MIPSTART;
    mqtt_at_cmds.RtyNum = 0;
    mqtt_recv = get_lte_recv();
    mqtt_at_cmds.net_status = NO_REC;

    mqtt_payload_u.mqtt_state = MQTT_READY;
    mqtt_payload_u.process_step = 0;
    mqtt_payload_u.txEnable = 0;
}
unsigned char get_mqtt_status(void)
{
    return mqtt_at_cmd_num;
}
uint8_t mqtt_Json_Info_Show(void)
{
    //    static unsigned char msub_count = 0;

    unsigned char buf[256];
    //buf = malloc(256);
    for (unsigned int i = 0; i < 256; i++)
        buf[i] = 0;

    switch (mqtt_at_cmd_num)
    {
        case AT_MCONFIG:
        {
            sprintf(buf, "AT+MCONFIG=%s,%s,%s\0D\0A",
                    get_config()->user_id, get_config()->user, get_config()->password);
            if (lte_Send_Cmd(buf, "OK", LTE_SHORT_DELAY_MQTT)) //??AT
            {
                mqtt_at_cmds.RtyNum = mqtt_at_cmds.RtyNum++;
            }
            else
            {
                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MIPSTART;

                //memset(get_lte_recv()->Lpuart1RecBuff,0,sizeof(get_lte_recv()->Lpuart1RecBuff));
            }

        }
        break;
        case AT_MIPCLOSE:
        {
            mqtt_at_cmds.net_status = ERROR_STATUS;
            sprintf(buf, "AT+MIPCLOSE\r\n");
            if (lte_Send_Cmd(buf, "OK", LTE_SHORT_DELAY_MQTT)) //??AT
            {
                mqtt_at_cmds.RtyNum = mqtt_at_cmds.RtyNum++;
            }
            else
            {
                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MIPSTART;

                //memset(get_lte_recv()->Lpuart1RecBuff,0,sizeof(get_lte_recv()->Lpuart1RecBuff));
            }

        }
        case AT_MIPSTART:
        {
            sprintf(buf, "AT+MIPSTART=\"%s\",\"%s\"\r\n", get_config()->mqtt_ip,
                    get_config()->mqtt_port);
            if (lte_Send_Cmd(buf, "CONNECT", LTE_SHORT_DELAY_MQTT)) //??AT
            {
                mqtt_at_cmd_num = AT_MIPCLOSE;
            }
            else
            {
                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MCONNECT;

                //memset(get_lte_recv()->Lpuart1RecBuff,0,sizeof(get_lte_recv()->Lpuart1RecBuff));
            }

        }
        break;
        case AT_MCONNECT:
        {
            if (lte_Send_Cmd("AT+MCONNECT=1,3000\r\n", "CONNACK OK",
                             LTE_SHORT_DELAY_MQTT)) //??AT
            {
                mqtt_at_cmd_num = AT_MIPCLOSE;
            }
            else
            {
                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MSUB;

                //memset(get_lte_recv()->Lpuart1RecBuff,0,sizeof(get_lte_recv()->Lpuart1RecBuff));
            }

        }
        break;

        case AT_MSUB:////subscribe msg
        {
            //dev_sub_temp_
            //            unsigned char str[128];
            // memcpy(str,&get_config()->sub_sring[1][0],strlen(&get_config()->sub_sring[1][0]));
            sprintf(buf, "AT+MSUB=\"%s%s\",%d\r\n", "dev_sub_ctrl_", get_config()->user_id,
                    0);
            if (lte_Send_Cmd(buf, "SUBACK", LTE_SHORT_DELAY_MQTT)) //??AT
            {
                mqtt_at_cmd_num = AT_MIPCLOSE;
            }
            else
            {

                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MSUB_2;
            }

        }
        //case AT_MSUB_1:////subscribe msg
        //{
        //  //dev_sub_temp_
        //  //            unsigned char str[128];
        //  // memcpy(str,&get_config()->sub_sring[1][0],strlen(&get_config()->sub_sring[1][0]));
        //  sprintf(buf, "AT+MSUB=\"%s\",%d\r\n", "sensor_pub_083a8d40fa8e",0);
        //  if (lte_Send_Cmd(buf, "SUBACK", LTE_SHORT_DELAY_MQTT)) //??AT
        //  {
        //      mqtt_at_cmd_num = AT_MIPCLOSE;
        //  }
        //  else
        //  {
        //
        //      mqtt_at_cmds.RtyNum = 0;
        //      mqtt_at_cmd_num = AT_MSUB_2;
        //  }
        //
        //}

        //    case AT_MSUB_1:////subscribe msg
        //    {
        //        //dev_sub_temp_
        //        //            unsigned char str[128];
        //        // memcpy(str,&get_config()->sub_sring[1][0],strlen(&get_config()->sub_sring[1][0]));
        //        sprintf(buf, "AT+MSUB=\"%s%s\",%d\r\n", "dev_sub_temp_", get_config()->user_id,
        //                0);
        //        if (lte_Send_Cmd(buf, "SUBACK", LTE_SHORT_DELAY_MQTT)) //??AT
        //        {
        //            mqtt_at_cmd_num = AT_MIPCLOSE;
        //        }
        //        else
        //        {
        //
        //            mqtt_at_cmds.RtyNum = 0;
        //            mqtt_at_cmd_num = AT_MSUB_2;
        //        }
        //
        //    }
        case AT_MSUB_2:////subscribe msg
        {
            //dev_sub_temp_
            //            unsigned char str[128];
            // memcpy(str,&get_config()->sub_sring[1][0],strlen(&get_config()->sub_sring[1][0]));
            sprintf(buf, "AT+MSUB=\"%s\",%d\r\n", "dev_sub_ctrl_all", 0);
            if (lte_Send_Cmd(buf, "SUBACK", LTE_SHORT_DELAY_MQTT)) //??AT
            {
                mqtt_at_cmd_num = AT_MIPCLOSE;
            }
            else
            {

                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MSUB_3;
            }

        }

        break;
        case AT_MSUB_3:////subscribe msg
        {
            //dev_sub_temp_
            //            unsigned char str[128];
            // memcpy(str,&get_config()->sub_sring[1][0],strlen(&get_config()->sub_sring[1][0]));
            sprintf(buf, "AT+MSUB=\"%s\",%d\r\n", "timestamp", 0);
            if (lte_Send_Cmd(buf, "SUBACK", LTE_SHORT_DELAY_MQTT)) //??AT
            {
                mqtt_at_cmd_num = AT_MIPCLOSE;
            }
            else
            {

                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MPUB_RECV;
            }

        }

        break;

        case AT_MPUB://public msg
        {
            if (mqtt_payload_u.txEnable == 1)
            {
                unsigned int len;
                len = strlen(json_mqtt_send_buf);
                sprintf(buf, "AT+MPUBEX=\"%s%s\",%d,%d,%d\r\n", get_config()->mqtt_mpubtopic,
                        get_config()->user_id,
                        1, 0, len);
                //lte_Send_Cmd_mqtt(1,buf, "PUBACK", LTE_LONG_DELAY)
                if (lte_Send_Cmd(buf, ">", MQTT_LTE_LONG_DELAY)) //??AT
                {
                    mqtt_at_cmds.RtyNum = mqtt_at_cmds.RtyNum++;
                    mqtt_at_cmd_num = AT_MIPCLOSE;
                    mqtt_payload_u.mqtt_state = MQTT_READY;
                    json_clear();
                }
                else
                {
                    //mqtt_init();
                    mqtt_at_cmds.RtyNum = 0;
                    mqtt_at_cmd_num = AT_MPUBEX;
                    //memset(get_lte_recv()->Lpuart1RecBuff,0,sizeof(get_lte_recv()->Lpuart1RecBuff));
                }


            }



        }
        break;
        case AT_MPUBEX://public msg
        {

            //lte_Send_Cmd_mqtt(1,buf, "PUBACK", LTE_LONG_DELAY)
            if (lte_Send_Cmd(json_mqtt_send_buf, "OK", MQTT_LTE_LONG_DELAY)) //??AT
            {
                mqtt_at_cmds.RtyNum = mqtt_at_cmds.RtyNum++;
                mqtt_at_cmd_num = AT_MPUBEX2;
                mqtt_payload_u.txEnable = 0;
                //json_clear();
            }
            else
            {
                //mqtt_init();
                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MPUB_RECV;
                mqtt_payload_u.mqtt_state = MQTT_READY;
                json_clear();
                mqtt_payload_u.txEnable = 0;
                //memset(get_lte_recv()->Lpuart1RecBuff,0,sizeof(get_lte_recv()->Lpuart1RecBuff));
            }




        }
        break;
        case AT_MPUBEX2://public msg
        {
            mqtt_at_cmd_num = AT_MIPCLOSE;

            mqtt_payload_u.mqtt_state = MQTT_READY;
            json_clear();
            mqtt_payload_u.txEnable = 0;
            mqtt_at_cmds.net_status = ERROR_STATUS;

        }
        break;

        case AT_MPUB_RECV://????
        {

            unsigned char status;
            mqtt_at_cmds.recv_status  = ATRec("+MSUB:");
            mqtt_at_cmds.net_status = SUCCESS_REC;
            if (mqtt_at_cmds.recv_status == SUCCESS_REC) //??AT
            {
                anlysis_mqtt_recv();
                clear_uart_buf(0);
                //  json_para();
                //anlysis_mqtt_recv();
            }
            else if (mqtt_at_cmds.recv_status == ERROR_STATUS)//????
            {
                mqtt_init();
            }
            else
            {
                ;//
            }

        }
        break;

        default:
        {
            CAT1_Init();
        }
        break;


    }
    //free(buf);
    if (mqtt_at_cmds.RtyNum >= 10)
    {
        CAT1_Init();
        mqtt_at_cmds.net_status = TIME_OUT;

    }

    return mqtt_at_cmds.net_status;
}

uint8_t mqtt_Info_Show(void)
{
    //    static unsigned char msub_count = 0;

    unsigned char buf[2048];
    memset(buf, 0, 2048);
    switch (mqtt_at_cmd_num)
    {
        case AT_MCONFIG:
        {
            sprintf(buf, "AT+MCONFIG=%s,%s,%s\0D\0A",
                    get_config()->user_id, get_config()->user, get_config()->password);
            if (lte_Send_Cmd(buf, "OK", LTE_SHORT_DELAY_MQTT)) //??AT
            {
                mqtt_at_cmds.RtyNum = mqtt_at_cmds.RtyNum++;
            }
            else
            {
                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MIPSTART;

                //memset(get_lte_recv()->Lpuart1RecBuff,0,sizeof(get_lte_recv()->Lpuart1RecBuff));
            }

        }
        break;
        case AT_MIPCLOSE:
        {
            sprintf(buf, "AT+MIPCLOSE\r\n");
            if (lte_Send_Cmd(buf, "OK", LTE_SHORT_DELAY_MQTT)) //??AT
            {
                mqtt_at_cmds.RtyNum = mqtt_at_cmds.RtyNum++;
            }
            else
            {
                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MIPSTART;

                //memset(get_lte_recv()->Lpuart1RecBuff,0,sizeof(get_lte_recv()->Lpuart1RecBuff));
            }

        }
        case AT_MIPSTART:
        {
            sprintf(buf, "AT+MIPSTART=\"%s\",\"%s\"\r\n", get_config()->mqtt_ip,
                    get_config()->mqtt_port);
            if (lte_Send_Cmd(buf, "CONNECT", LTE_SHORT_DELAY_MQTT)) //??AT
            {
                mqtt_at_cmd_num = AT_MIPCLOSE;
            }
            else
            {
                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MCONNECT;

                //memset(get_lte_recv()->Lpuart1RecBuff,0,sizeof(get_lte_recv()->Lpuart1RecBuff));
            }

        }
        break;
        case AT_MCONNECT:
        {
            if (lte_Send_Cmd("AT+MCONNECT=1,3000\r\n", "CONNACK OK",
                             LTE_SHORT_DELAY_MQTT)) //??AT
            {
                mqtt_at_cmd_num = AT_MIPCLOSE;
            }
            else
            {
                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MSUB;

                //memset(get_lte_recv()->Lpuart1RecBuff,0,sizeof(get_lte_recv()->Lpuart1RecBuff));
            }

        }
        break;

        case AT_MSUB:////subscribe msg
        {
            //dev_sub_temp_
            //            unsigned char str[128];
            // memcpy(str,&get_config()->sub_sring[1][0],strlen(&get_config()->sub_sring[1][0]));
            sprintf(buf, "AT+MSUB=\"%s%s\",%d\r\n", "dev_sub_ctrl_", get_config()->user_id,
                    0);
            if (lte_Send_Cmd(buf, "SUBACK", LTE_SHORT_DELAY_MQTT)) //??AT
            {
                mqtt_at_cmd_num = AT_MIPCLOSE;
            }
            else
            {

                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MSUB_2;
            }

        }
        //case AT_MSUB_1:////subscribe msg
        //{
        //  //dev_sub_temp_
        //  //            unsigned char str[128];
        //  // memcpy(str,&get_config()->sub_sring[1][0],strlen(&get_config()->sub_sring[1][0]));
        //  sprintf(buf, "AT+MSUB=\"%s\",%d\r\n", "sensor_pub_083a8d40fa8e",0);
        //  if (lte_Send_Cmd(buf, "SUBACK", LTE_SHORT_DELAY_MQTT)) //??AT
        //  {
        //      mqtt_at_cmd_num = AT_MIPCLOSE;
        //  }
        //  else
        //  {
        //
        //      mqtt_at_cmds.RtyNum = 0;
        //      mqtt_at_cmd_num = AT_MSUB_2;
        //  }
        //
        //}

        //    case AT_MSUB_1:////subscribe msg
        //    {
        //        //dev_sub_temp_
        //        //            unsigned char str[128];
        //        // memcpy(str,&get_config()->sub_sring[1][0],strlen(&get_config()->sub_sring[1][0]));
        //        sprintf(buf, "AT+MSUB=\"%s%s\",%d\r\n", "dev_sub_temp_", get_config()->user_id,
        //                0);
        //        if (lte_Send_Cmd(buf, "SUBACK", LTE_SHORT_DELAY_MQTT)) //??AT
        //        {
        //            mqtt_at_cmd_num = AT_MIPCLOSE;
        //        }
        //        else
        //        {
        //
        //            mqtt_at_cmds.RtyNum = 0;
        //            mqtt_at_cmd_num = AT_MSUB_2;
        //        }
        //
        //    }
        case AT_MSUB_2:////subscribe msg
        {
            //dev_sub_temp_
            //            unsigned char str[128];
            // memcpy(str,&get_config()->sub_sring[1][0],strlen(&get_config()->sub_sring[1][0]));
            sprintf(buf, "AT+MSUB=\"%s\",%d\r\n", "dev_sub_ctrl_all", 0);
            if (lte_Send_Cmd(buf, "SUBACK", LTE_SHORT_DELAY_MQTT)) //??AT
            {
                mqtt_at_cmd_num = AT_MIPCLOSE;
            }
            else
            {

                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MSUB_3;
            }

        }

        break;
        case AT_MSUB_3:////subscribe msg
        {
            //dev_sub_temp_
            //            unsigned char str[128];
            // memcpy(str,&get_config()->sub_sring[1][0],strlen(&get_config()->sub_sring[1][0]));
            sprintf(buf, "AT+MSUB=\"%s\",%d\r\n", "timestamp", 0);
            if (lte_Send_Cmd(buf, "SUBACK", LTE_SHORT_DELAY_MQTT)) //??AT
            {
                mqtt_at_cmd_num = AT_MIPCLOSE;
            }
            else
            {

                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MPUB_RECV;
            }

        }

        break;

        case AT_MPUB://public msg
        {

            sprintf(buf, "AT+MPUB=\"%s%s\",%d,%d,\"%s\"\r\n", get_config()->mqtt_mpubtopic,
                    get_config()->user_id,
                    1, 0, json_mqtt_send_buf);
            //lte_Send_Cmd_mqtt(1,buf, "PUBACK", LTE_LONG_DELAY)
            if (lte_Send_Cmd(buf, "PUBACK", LTE_LONG_DELAY)) //??AT
            {
                mqtt_at_cmds.RtyNum = mqtt_at_cmds.RtyNum++;
                mqtt_at_cmd_num = AT_MIPCLOSE;
                mqtt_payload_u.mqtt_state = MQTT_READY;
                json_clear();
            }
            else
            {
                //mqtt_init();
                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MPUB_RECV;
                mqtt_payload_u.mqtt_state = MQTT_READY;
                json_clear();

                //memset(get_lte_recv()->Lpuart1RecBuff,0,sizeof(get_lte_recv()->Lpuart1RecBuff));
            }

        }
        break;
        case AT_MPUB_RECV://????
        {

            unsigned char status;
            status  = ATRec("+MSUB:");
            mqtt_at_cmds.net_status = SUCCESS_REC;
            if (status == SUCCESS_REC) //??AT
            {
                //json_para();
                anlysis_mqtt_recv();
            }
            else if (status == ERROR_STATUS)//????
            {
                mqtt_init();
            }
        }
        break;

        default:
        {
            CAT1_Init();
        }
        break;


    }
    if (mqtt_at_cmds.RtyNum >= 10)
    {
        CAT1_Init();
        mqtt_at_cmds.net_status = TIME_OUT;

    }

    return mqtt_at_cmds.net_status;
}

void mqtt_proc()
{
    unsigned char status;
    //mqtt_send_buf = json_pack(&mqtt_payload_u);
    // printf("%s",mqtt_send_buf);
    //free_cjson();
    if (lte_Info_Show() == NET_CONNECT) //????
    {
        if (mqtt_Json_Info_Show() == SUCCESS_REC)  //mqtt??????
        {
            if (get_config()->set_up_period > 0 && get_config()->set_up_period < 3600)
            {
                uint32_t tmp;
                tmp = get_config()->set_up_period * 1000;
                //tmp = 100;
                registerTick(MQTT_TX_TICK_NO, tmp);
            }

            else

                registerTick(MQTT_TX_TICK_NO, 500);
            if (GetTickResult(MQTT_TX_TICK_NO) == 1) //10s
            {
                reset_registerTick(MQTT_TX_TICK_NO);
                //upload();//????
                mqtt_payload_u.txEnable = 1;
                json_upload();
                //mqtt_Json_Info_Show();
                //free_cjson();

            }
            registerTick(MODBUS_MQTT_PID_TICK_NO, PID_TICK_TIME);

        }

    }
    else //???????,????????
    {
        registerTick(MODBUS_MQTT_PID_TICK_NO, PID_TICK_TIME);
        if (GetTickResult(MODBUS_MQTT_PID_TICK_NO) == 1
                && get_config()->connectTimeOut >= 10) //180s
        {
            //   reset_registerTick(MODBUS_TEMP_TX_TICK_NO);
#if CTRL_EN
            fuzzy_proc(1);
#endif
            //get_pid_output(void);
        }


    }

    mqtt_recv_proc();




}

