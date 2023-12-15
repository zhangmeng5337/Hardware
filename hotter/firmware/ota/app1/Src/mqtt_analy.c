#include "mqtt_analy.h"
#include "my_string.h"
#include "lte_hal.h"

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

unsigned char *enviro_tem[] =
{
    "温度1",
    "温度2",
    "温度3",
    "温度4",
    "温度5",
    "温度6",
    "温度7",
    "温度8",
    "温度9",
    "温度10",
    "温度11",
    "温度12",
    "温度13",
    "温度14",
    "温度15",
    "温度16",
    "温度17",
    "温度18",
    "温度19",
    "温度20",
    "温度21",
    "温度22",
    "温度23",
    "温度24",
    "温度25",
    "温度26",
    "温度27",
    "温度28",
    "温度29",
    "温度30",
    "温度31",
    "温度32",
    "温度33",
    "温度34",
    "温度35",
    "温度36",
    "温度37",
    "温度38",
    "温度39",
    "温度40",
    "温度41",
    "温度42",
    "温度43",
    "温度44",
    "温度45",
    "温度46",
    "温度47",
    "温度48",
    "温度49",
    "温度50",
    "温度51",
    "温度52",
    "温度53",
    "温度54",
    "温度55",
    "温度56",
    "温度57",
    "温度58",
    "温度59",
    "温度60",
    "温度61",
    "温度62",
    "温度63",
    "温度64"
};
mqtt_payload_stru mqtt_payload_u;


/*
/*{
   "设备ID": "47",
   "设备控制": {
	   "升级": 1,
	   "重启": 1,
	   "机组开关机": 0,
	   "设置出水温度": "value",
	   "设置室内温度": "value",
	   "数据上传周期": "value"
   }
}*/


void anlysis_mqtt_recv()
{
    /*{
       "设备ID": "47",
       "设备控制": {
           "升级": 1,
           "重启": 1,
           "机组开关机": 0,
           "设置出水温度": "value",
           "设置室内温度": "value",
           "数据上传周期": "value"
       }
    }*/
    char dev_id[128];
    unsigned char valid_flag;
    float tmp_f;
    float tmp_i;
    unsigned char index;

    valid_flag = 0;
    get_config()->update_setting = 0;
    if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "设备ID:  ", ",\r\n",
                    dev_id) == 1)
    {
        if ((strcmp(dev_id, get_config()->user_id)) == 0)
            valid_flag = 1;

    }
    if (valid_flag == 1)
    {
        memset(dev_id, 0, 128);
        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "升级:  ", ",\r\n",
                        dev_id) == 1)
        {
            get_config()->update_setting = 1;
            sprintf(&get_config()->update_firm, "%s", dev_id);//字符串
            tmp_f = atoi(dev_id);
            get_config()->update_firm = tmp_f;

        }
        memset(dev_id, 0, 128);
        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "重启: ", ",\r\n",
                        dev_id) == 1)
        {
            get_config()->update_setting = 1;
            sprintf(&get_config()->reboot, "%s", dev_id); //字符串
            tmp_f = atoi(dev_id);
            get_config()->reboot = tmp_f;


        }
        memset(dev_id, 0, 128);

        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "机组开关机: ", ",\r\n",
                        dev_id) == 1)
        {
            get_config()->update_setting = 1;
            //sprintf(&get_config()->machine, "%s", dev_id); //字符串
            tmp_f = atoi(dev_id);
            get_config()->machine = tmp_f;

        }
        memset(dev_id, 0, 128);

        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "设置出水温度: ", ",\r\n",
                        dev_id) == 1)
        {
            get_config()->update_setting = 1;
            tmp_f = atof(dev_id);
            get_config()->set_tout = tmp_f;
            get_config()->set_tout_tmp = tmp_f;
        }


        memset(dev_id, 0, 128);
        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "设置室内温度: ", ",\r\n",
                        dev_id) == 1)
        {
            get_config()->update_setting = 2;
            tmp_f = atof(dev_id);
            get_config()->set_tindoor = tmp_f;

        }


        memset(dev_id, 0, 128);
        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "数据上传周期: ", ",\r\n",
                        dev_id) == 1)
        {
            get_config()->update_setting = 2;
            tmp_f = atof(dev_id);
            get_config()->set_up_period = tmp_f;


        }
        unsigned char i;
        for (i = 0; i < ENVIRO_SIZE; i++)
        {
            if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, enviro_tem[i], ",\r\n",
                            dev_id) == 1)
            {
                //index = 0;
                get_config()->update_setting = 2;
                tmp_f = atof(dev_id);
                get_config()->indoor_temperature[i] = tmp_f;


            }

        }
#if CTRL_EN
        float *buf;
        buf = low_temperature_cal(get_config()->indoor_temperature, ENVIRO_SIZE);
        get_temp_cal(buf);
        fuzzy_proc(0);  //根据室内温度控制
#endif
        valid_flag = 0;

    }







}
/*{
    "设备ID": 47,
    "运行数据": {
        "出水温度": 25,
        "回水温度": 36,
        "泵前压力": 42,
        "泵后压力": 4,
        "运行状态": 4
    },
    "设备参数": {
        "版本号": 255,
        "设置出水温度": 5,
        "设置室内温度": 8,
        "数据上传周期": 1
    },
    "机组状态": {
        "机组1": {
            "状态": "异常",
            "故障码": 254
        },
        "机组2": {
            "状态": "正常"
        },
        "机组3": {
            "状态": "正常"
        }
    }
}*/

/*{
    "设备ID": 47,
    "运行数据": {
        "出水温度": 25,
        "回水温度": 36,
        "泵前压力": 42,
        "泵后压力": 4,
        "运行状态": 4
    },
    "设备参数": {
        "版本号": 255,
        "设置出水温度": 5,
        "设置室内温度": 8,
        "数据上传周期": 1
    }
}*/

/*else if (ATCmdNum == AT_MPUB)//发布消息
{
	sprintf(send_buffer, "%s%s,%d\r\n",ATCmds[ATCmdNum].ATSendStr,
											 config_usr.mqtt_msubtopic, 0);
	HAL_UART_Transmit(&huart1, (uint8_t *)send_buffer, strlen(send_buffer), 0xFF);
	memset(send_buffer, 0x00, strlen(send_buffer));
	}*/
char mqtt_send_buf[512];

//周期上传
void upload()
{



    memset(mqtt_payload_u.devid, 0, sizeof(mqtt_payload_u.devid));
    memset(mqtt_payload_u.version, 0, sizeof(mqtt_payload_u.version));
    sprintf(mqtt_payload_u.devid, "%s", get_config()->user_id);//devid
    mqtt_payload_u.data[TOUT_INDEX] = get_ai_data()->temp[AI_WATER_T_O_INDEX];
    mqtt_payload_u.data[TIN_INDEX] =  get_ai_data()->temp[AI_WATER_T_IN_INDEX]; //water IN
    mqtt_payload_u.data[PUMP_F_INDEX] = get_ai_data()->temp[AI_PUMP_F_INDEX]; //pump front
    mqtt_payload_u.data[PUMP_E_INDEX] = get_ai_data()->temp[AI_PUMP_E_INDEX]; //pump end

    uint32_t tmp;
    tmp = get_ai_data()->channel_status;
    mqtt_payload_u.status[DEV_STATUS_INDEX] = get_di_data()->di_status; //8 bit di
    mqtt_payload_u.status[DEV_STATUS_INDEX] = mqtt_payload_u.status[DEV_STATUS_INDEX] << 8; //dev status
    mqtt_payload_u.status[DEV_STATUS_INDEX] = mqtt_payload_u.status[DEV_STATUS_INDEX] | tmp; //20bit ai but 8bit used
    mqtt_payload_u.status[DEV_STATUS_INDEX] = mqtt_payload_u.status[DEV_STATUS_INDEX] << 20;
    mqtt_payload_u.status[DEV_STATUS_INDEX] = mqtt_payload_u.status[DEV_STATUS_INDEX] |
        get_recv_machine()->fault;
    mqtt_payload_u.status[DEV_MASK_INDEX] = get_config()->fault_mask;

    get_config()->fault_status = mqtt_payload_u.status[DEV_STATUS_INDEX] ;//fault status
    sprintf(mqtt_payload_u.version, "%s", get_config()->version);//devid
    mqtt_payload_u.data[WATER_O_INDEX] = get_config()->set_tout; //set tmp
    mqtt_payload_u.data[WATER_IN_INDEX] = get_config()->set_tindoor; //set indoor tmp
    mqtt_payload_u.data[UP_PERIOD_INDEX] = get_config()->set_up_period; //up period

//   sprintf(mqtt_send_buf, "{\r\n\
//	   “设备ID”: 123,\r\n\
//	   运行数据: {\r\n\
//		   出水温度:  123,\r\n\
//		   回水温度:  123,\r\n\
//		   泵前压力:  123,\r\n\
//		   泵后压力:  123,\r\n\
//		   运行状态:  123,\r\n\
//		   设备使能状态： 123,\r\n\
//	   },\r\n\
//	   设备参数: {\r\n\
//		   版本号:  123,\r\n\
//		   设置出水温度:  123,\r\n\
//		   设置室内温度:  123,\r\n\
//		   数据上传周期:  7123\r\n\
//	   }\r\n\
//   }");

//{
//                "设备ID": 123,
//                "运行数据": {
//                        "出水温度": 12.000000,
//                        "回水温度": 7.000000,
//                        "泵前压力": 5.000000,
//                        "泵后压力": 3.000000,
//                        "运行状态": 4,
//                        "设备使能状态": 5.0},
//                "设备参数": {
//                        "版本号": 22,
//                        "设置出水温度": 1.000000,
//                        "设置室内温度": 2.000000,
//                        "数据上周期": 3.000000
//                }
//        }


    sprintf(mqtt_send_buf, "{\r\n\
		“设备ID”: %s,\r\n\
		“运行数据”: {\r\n\
			“出水温度”: %f,\r\n\
			“回水温度”: %f,\r\n\
			“泵前压力”: %f,\r\n\
			“泵后压力”: %f,\r\n\
			“运行状态”: %x,\r\n\
			“设备使能状态”：%x,\r\n\
		},\r\n\
		“设备参数”: {\r\n\
			“版本号”: %s,\r\n\
			“设置出水温度”: %f,\r\n\
			“设置室内温度”: %f,\r\n\
			“数据上传周期”: %f\r\n\
		}\r\n\
	}",       mqtt_payload_u.devid,
            mqtt_payload_u.data[TOUT_INDEX],
            mqtt_payload_u.data[TIN_INDEX],
            mqtt_payload_u.data[PUMP_F_INDEX],
            mqtt_payload_u.data[PUMP_E_INDEX],
            mqtt_payload_u.status[DEV_STATUS_INDEX],
            mqtt_payload_u.status[DEV_MASK_INDEX],
            mqtt_payload_u.version,
            mqtt_payload_u.data[WATER_O_INDEX],
            mqtt_payload_u.data[WATER_IN_INDEX],
            mqtt_payload_u.data[UP_PERIOD_INDEX]);

    mqtt_at_cmd_num = AT_MPUB;

    //{
    //    "设备ID": "47",
    //    "运行数据": {
    //        "出水温度": 1,
    //        "回水温度": 2,
    //        "泵前压力": 3,
    //        "泵后压力": 4,
    //        "伴热带状态": 5
    //    },
    //    "设备参数": {
    //        "版本号": 255,
    //        "故障": 255,
    //        "设置出水温度": 6,
    //        "设置室内温度": 7,
    //        "数据上传周期": 8
    //    }
    //}


}
//void mqtt_msub()//订阅消息
//{
//	MQTTSendData(1,mqtt_buf);

//}
void mqtt_recv_proc()
{

    if (get_config()->update_firm == '1' || get_config()->reboot == '1')
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
    unsigned char buf[1024];
    switch (mqtt_at_cmd_num)
    {
        case AT_MCONFIG:
        {
            sprintf(buf, "AT+MCONFIG=%s,%s,%s\r\n",
                    get_config()->user_id, get_config()->user, get_config()->password);
            if (lte_Send_Cmd(buf, "OK", 20)) //查询AT
            {
                mqtt_at_cmds.RtyNum = mqtt_at_cmds.RtyNum++;
            }
            else
            {
                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MIPSTART;
				
			memset(get_lte_recv()->Lpuart1RecBuff,0,sizeof(get_lte_recv()->Lpuart1RecBuff));
            }

        }
        break;
        case AT_MIPSTART:
        {
            sprintf(buf, "AT+MIPSTART=%s,%s", get_config()->mqtt_ip,
                    get_config()->mqtt_port);
            if (lte_Send_Cmd(buf, "CONNECT OK", 20)) //查询AT
            {
                mqtt_at_cmds.RtyNum = mqtt_at_cmds.RtyNum++;
            }
            else
            {
                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MCONNECT;
				
			memset(get_lte_recv()->Lpuart1RecBuff,0,sizeof(get_lte_recv()->Lpuart1RecBuff));
            }

        }
        break;
        case AT_MCONNECT:
        {
            if (lte_Send_Cmd("AT+MCONNECT=1,300", "OK", 20)) //查询AT
            {
                mqtt_at_cmds.RtyNum = mqtt_at_cmds.RtyNum++;
            }
            else
            {
                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MSUB;
				
			memset(get_lte_recv()->Lpuart1RecBuff,0,sizeof(get_lte_recv()->Lpuart1RecBuff));
            }

        }
        break;

        case AT_MSUB://订阅消息
        {
            sprintf(buf, "AT+MSUB=%s,%d", get_config()->mqtt_subtopic,
                    0);
            if (lte_Send_Cmd(buf, "SUBACK", 20)) //查询AT
            {
                mqtt_at_cmds.RtyNum = mqtt_at_cmds.RtyNum++;
            }
            else
            {
                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MPUB_RECV;
				
			memset(get_lte_recv()->Lpuart1RecBuff,0,sizeof(get_lte_recv()->Lpuart1RecBuff));
            }

        }
        break;
        case AT_MPUB://发布消息
        {
            sprintf(buf, "AT+MPUB=%s,%d,%d,%s", get_config()->mqtt_mpubtopic,
                    0, 0, mqtt_send_buf);
            if (lte_Send_Cmd(buf, "OK", 20)) //查询AT
            {
                mqtt_at_cmds.RtyNum = mqtt_at_cmds.RtyNum++;
            }
            else
            {
                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MPUB_RECV;
				
			memset(get_lte_recv()->Lpuart1RecBuff,0,sizeof(get_lte_recv()->Lpuart1RecBuff));
            }

        }
        break;
        case AT_MPUB_RECV://接收消息
        {

            unsigned char status;
            status  = ATRec("+MSUB:");
            mqtt_at_cmds.net_status = SUCCESS_REC;
            if (status == SUCCESS_REC) //查询AT
            {
                anlysis_mqtt_recv();
            }
            else if (status == ERROR_STATUS)//异常值
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
    if (mqtt_at_cmds.RtyNum >= 3)
    {
        CAT1_Init();
        mqtt_at_cmds.net_status = TIME_OUT;

    }

    return mqtt_at_cmds.net_status;
}

void mqtt_proc()
{

    if (lte_Info_Show() == NET_CONNECT) //订阅主题
    {
        if (mqtt_Info_Show() == SUCCESS_REC) //mqtt初始化测成功
        {
            registerTick(MQTT_TX_TICK_NO, 10000);
            if (GetTickResult(MQTT_TX_TICK_NO) == 1) //10s
            {
                reset_registerTick(MQTT_TX_TICK_NO);
                upload();//发布消息
                mqtt_Info_Show();

            }
            registerTick(MODBUS_TEMP_TX_TICK_NO, 180000);
        }

    }
    else //无网络本地控制，根据回水温度控制
    {
        registerTick(MODBUS_TEMP_TX_TICK_NO, 180000);
        if (GetTickResult(MODBUS_TEMP_TX_TICK_NO) == 1) //180s
        {
            //	 reset_registerTick(MODBUS_TEMP_TX_TICK_NO);
#if CTRL_EN
            fuzzy_proc(1);
#endif
            //get_pid_output(void);

        }


    }


}

