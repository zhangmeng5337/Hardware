#include "mqtt_analy.h"
#include "my_string.h"
#include "lte_hal.h"
#include "json_para.h"

#include "ai_proc.h"
#include "di.h"
#include "modbus.h"
#include "uart.h"
#include "config.h"
#include "sys.h"
#include "pid.h"
#include "filter.h"
tsATCmds     mqtt_at_cmds;
teATCmdNum    mqtt_at_cmd_num;
teATStatus    mqtt_at_status;
tsLpuart1type *mqtt_recv;

//unsigned char mqtt_send_buf[128];

mqtt_payload_stru mqtt_payload_u;
mqtt_payload_stru *get_mqtt_payload()
{
    return &mqtt_payload_u;
}
void anlysis_mqtt_recv()
{

    char dev_id[128];
    unsigned char valid_flag;
    float tmp_f;


    valid_flag = 0;
    get_config()->update_setting = 0;
    if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, get_config()->user_id,
                    ",\r\n", dev_id) == 1)
    {
        //if ((strcmp(dev_id, get_config()->user_id)) == 0)
        valid_flag = 1;

    }
    if (valid_flag == 1)
    {
        memset(dev_id, 0, 128);
        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "\"Updat Frimware\": ", ",",
                        dev_id) == 1)
        {
            get_config()->update_setting = 1;
            sprintf(&get_config()->update_firm, "%s", dev_id);//????
            tmp_f = atoi(&dev_id[0]);
            get_config()->update_firm = tmp_f;

        }
        memset(dev_id, 0, 128);
        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "\"Reboot Dev\": ", ",",
                        dev_id) == 1)
        {
            get_config()->update_setting = 1;
            sprintf(&get_config()->reboot, "%s", dev_id); //????
            tmp_f = atoi(&dev_id[0]);
            get_config()->reboot = tmp_f;


        }
        memset(dev_id, 0, 128);

        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "\"Power ctrl\": ", ",",
                        dev_id) == 1)
        {
            get_config()->update_setting = 1;
            //sprintf(&get_config()->machine, "%s", dev_id); //????
            tmp_f = atoi(&dev_id[0]);
            get_config()->machine = tmp_f;

        }
        memset(dev_id, 0, 128);

        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "\"Set Out Temp\": ", ",",
                        dev_id) == 1)
        {
            get_config()->update_setting = 1;
            tmp_f = atof(&dev_id[0]);
            get_config()->set_tout = tmp_f;
            get_config()->set_tout_tmp = tmp_f;
        }


        memset(dev_id, 0, 128);
        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "\"Set Room Temp\": ", ",",
                        dev_id) == 1)
        {
            get_config()->update_setting = 2;
            tmp_f = atof(&dev_id[0]);
            get_config()->set_tindoor = tmp_f;

        }


        memset(dev_id, 0, 128);
        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff,
                        "\"Set Upload Period(second)\": ", "\r\n",
                        dev_id) == 1)
        {
            get_config()->update_setting = 2;
            tmp_f = atof(&dev_id[0]);
            get_config()->set_up_period = tmp_f;


        }

        unsigned char buf[16], j, i, k;
        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "\"Room Temp\": [", "\r\n",
                        dev_id) == 1)
        {
            j = 0;
            k = 0;
            i=0;
            while (dev_id[i] != ']')
            {
                if (dev_id[i] != ','&&dev_id[i] != ']')
                    buf[j++] = dev_id[i];
                else
                {
                    tmp_f = atof(buf);
                    get_config()->indoor_temperature[k++] = tmp_f;
                    memset(buf, 0, 16);
                    j = 0;

                }
                i++;

                get_config()->update_setting = 2;

            }

            tmp_f = atof(buf);
            get_config()->indoor_temperature[k++] = tmp_f;



        }
        //  json_analysis((char *)mqtt_recv->Lpuart1RecBuff);

#if CTRL_EN
        float *buf;
        buf = low_temperature_cal(get_config()->indoor_temperature, ENVIRO_SIZE);
        get_temp_cal(buf);
        fuzzy_proc(0);  //????????
#endif
        // valid_flag = 0;


    }
}

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
char mqtt_send_buf[1024];

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
void upload()
{



    memset(mqtt_payload_u.devid, 0, sizeof(mqtt_payload_u.devid));
    memset(mqtt_payload_u.version, 0, sizeof(mqtt_payload_u.version));
    sprintf(mqtt_payload_u.devid, "%s", get_config()->user_id);//devid
    mqtt_payload_u.data[TOUT_INDEX] = get_ai_data()->temp[AI_WATER_T_O_INDEX];
    mqtt_payload_u.data[TIN_INDEX] =
        get_ai_data()->temp[AI_WATER_T_IN_INDEX]; //water IN
    mqtt_payload_u.data[PUMP_F_INDEX] =
        get_ai_data()->press[AI_PUMP_F_INDEX]; //pump front
    mqtt_payload_u.data[PUMP_E_INDEX] =
        get_ai_data()->press[AI_PUMP_E_INDEX]; //pump end

    uint32_t tmp;
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

    get_config()->fault_status = mqtt_payload_u.status[DEV_STATUS_INDEX]
                                 ;//fault status
    sprintf(mqtt_payload_u.version, "%s", get_config()->version);//devid
    mqtt_payload_u.data[WATER_O_INDEX] = get_config()->set_tout; //set tmp
    mqtt_payload_u.data[WATER_IN_INDEX] =
        get_config()->set_tindoor; //set indoor tmp
    mqtt_payload_u.data[UP_PERIOD_INDEX] = get_config()->set_up_period; //up period




#if 1

    sprintf(mqtt_send_buf, "{\\0D\\0A\\
            \\22Dev ID\\22: %s,\\0D\\0A\\
            \\22Status Data\\22: {\\0D\\0A\\
                                  \\22Out Tem\\22: %.0f,\\0D\\0A\\
                                  \\22In Tem\\22: %.0f,\\0D\\0A\\
                                  \\22Front Pressure\\22: %.2f,\\0D\\0A\\
                                  \\22After Pressure\\22: %.2f,\\0D\\0A\\
                                  \\22Status\\22: %u,\\0D\\0A\\
                                  \\22air pump_status\\22: %u\\0D\\0A\\
                                 },\\0D\\0A\\
            \\22Dev Params\\22: {\\0D\\0A\\
                                 \\22Version\\22: \\22%s\\22,\\0D\\0A\\
                                 \\22Set Out Tem\\22: %.0f,\\0D\\0A\\
                                 \\22Set In Tem\\22: %.0f,\\0D\\0A\\
                                 \\22Upload Period(second)\\22: %.0f\\0D\\0A\\
                                }\\0D\\0A\\
}", mqtt_payload_u.devid,
mqtt_payload_u.data[TOUT_INDEX],
                    mqtt_payload_u.data[TIN_INDEX],
                    mqtt_payload_u.data[PUMP_F_INDEX],
                    mqtt_payload_u.data[PUMP_E_INDEX],
                    mqtt_payload_u.status[DEV_STATUS_INDEX],
                    mqtt_payload_u.status[DEV_PUMP_STATUS_INDEX],
                    mqtt_payload_u.version,
                    mqtt_payload_u.data[WATER_O_INDEX],
                    mqtt_payload_u.data[WATER_IN_INDEX],
                    mqtt_payload_u.data[UP_PERIOD_INDEX]);


#else
    mqtt_send_buf = json_pack(&mqtt_payload_u);

#endif
mqtt_at_cmd_num = AT_MPUB;


}
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

}
uint8_t mqtt_Info_Show(void)
{
//    static unsigned char msub_count = 0;

    unsigned char buf[1024];
    memset(buf, 0, 1024);
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
            mqtt_at_cmd_num = AT_MSUB_1;
        }

    }
    case AT_MSUB_1:////subscribe msg
    {
        //dev_sub_temp_
        //            unsigned char str[128];
        // memcpy(str,&get_config()->sub_sring[1][0],strlen(&get_config()->sub_sring[1][0]));
        sprintf(buf, "AT+MSUB=\"%s%s\",%d\r\n", "dev_sub_temp_", get_config()->user_id,
                0);
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
                1, 0, mqtt_send_buf);
        //lte_Send_Cmd_mqtt(1,buf, "PUBACK", LTE_LONG_DELAY)
        if (lte_Send_Cmd(buf, "PUBACK", LTE_LONG_DELAY)) //??AT
        {
            mqtt_at_cmds.RtyNum = mqtt_at_cmds.RtyNum++;
            mqtt_at_cmd_num = AT_MIPCLOSE;
        }
        else
        {
            //mqtt_init();
            mqtt_at_cmds.RtyNum = 0;
            mqtt_at_cmd_num = AT_MPUB_RECV;

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
            anlysis_mqtt_recv();
        }
        else if (status == ERROR_STATUS)//????
        {
            mqtt_init();
        }

    }
    break;



defautl:
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
    //mqtt_send_buf = json_pack(&mqtt_payload_u);
    // printf("%s",mqtt_send_buf);
//free_cjson();
    if (lte_Info_Show() == NET_CONNECT) //????
    {
        if (mqtt_Info_Show() == SUCCESS_REC) //mqtt??????
        {
            if(get_config()->set_up_period>0&&get_config()->set_up_period<3600)
            {
                uint32_t tmp;
                tmp = get_config()->set_up_period*1000;
                registerTick(MQTT_TX_TICK_NO, tmp);
            }

            else
                registerTick(MQTT_TX_TICK_NO, 10000);
            if (GetTickResult(MQTT_TX_TICK_NO) == 1) //10s
            {
                reset_registerTick(MQTT_TX_TICK_NO);
                upload();//????
                mqtt_Info_Show();
                //free_cjson();

            }
            registerTick(MODBUS_MQTT_PID_TICK_NO, 180000);
        }

    }
    else //???????,????????
    {
        registerTick(MODBUS_MQTT_PID_TICK_NO, 180000);
        if (GetTickResult(MODBUS_MQTT_PID_TICK_NO) == 1) //180s
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

