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
    "??1",
    "??2",
    "??3",
    "??4",
    "??5",
    "??6",
    "??7",
    "??8",
    "??9",
    "??10",
    "??11",
    "??12",
    "??13",
    "??14",
    "??15",
    "??16",
    "??17",
    "??18",
    "??19",
    "??20",
    "??21",
    "??22",
    "??23",
    "??24",
    "??25",
    "??26",
    "??27",
    "??28",
    "??29",
    "??30",
    "??31",
    "??32",
    "??33",
    "??34",
    "??35",
    "??36",
    "??37",
    "??38",
    "??39",
    "??40",
    "??41",
    "??42",
    "??43",
    "??44",
    "??45",
    "??46",
    "??47",
    "??48",
    "??49",
    "??50",
    "??51",
    "??52",
    "??53",
    "??54",
    "??55",
    "??56",
    "??57",
    "??58",
    "??59",
    "??60",
    "??61",
    "??62",
    "??63",
    "??64"
};
mqtt_payload_stru mqtt_payload_u;


/*
/*{
   "??ID": "47",
   "????": {
	   "??": 1,
	   "??": 1,
	   "?????": 0,
	   "??????": "value",
	   "??????": "value",
	   "??????": "value"
   }
}*/


void anlysis_mqtt_recv()
{
    /*{
       "??ID": "47",
       "????": {
           "??": 1,
           "??": 1,
           "?????": 0,
           "??????": "value",
           "??????": "value",
           "??????": "value"
       }
    }*/
    char dev_id[128];
    unsigned char valid_flag;
    float tmp_f;
    float tmp_i;
    unsigned char index;

    valid_flag = 0;
    get_config()->update_setting = 0;
    if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "??ID:  ", ",\0D\0A",
                    dev_id) == 1)
    {
        if ((strcmp(dev_id, get_config()->user_id)) == 0)
            valid_flag = 1;

    }
    if (valid_flag == 1)
    {
        memset(dev_id, 0, 128);
        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "??:  ", ",\0D\0A",
                        dev_id) == 1)
        {
            get_config()->update_setting = 1;
            sprintf(&get_config()->update_firm, "%s", dev_id);//????
            tmp_f = atoi(dev_id);
            get_config()->update_firm = tmp_f;

        }
        memset(dev_id, 0, 128);
        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "??: ", ",\0D\0A",
                        dev_id) == 1)
        {
            get_config()->update_setting = 1;
            sprintf(&get_config()->reboot, "%s", dev_id); //????
            tmp_f = atoi(dev_id);
            get_config()->reboot = tmp_f;


        }
        memset(dev_id, 0, 128);

        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "?????: ", ",\0D\0A",
                        dev_id) == 1)
        {
            get_config()->update_setting = 1;
            //sprintf(&get_config()->machine, "%s", dev_id); //????
            tmp_f = atoi(dev_id);
            get_config()->machine = tmp_f;

        }
        memset(dev_id, 0, 128);

        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "??????: ", ",\0D\0A",
                        dev_id) == 1)
        {
            get_config()->update_setting = 1;
            tmp_f = atof(dev_id);
            get_config()->set_tout = tmp_f;
            get_config()->set_tout_tmp = tmp_f;
        }


        memset(dev_id, 0, 128);
        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "??????: ", ",\0D\0A",
                        dev_id) == 1)
        {
            get_config()->update_setting = 2;
            tmp_f = atof(dev_id);
            get_config()->set_tindoor = tmp_f;

        }


        memset(dev_id, 0, 128);
        if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, "??????: ", ",\0D\0A",
                        dev_id) == 1)
        {
            get_config()->update_setting = 2;
            tmp_f = atof(dev_id);
            get_config()->set_up_period = tmp_f;


        }
        unsigned char i;
        for (i = 0; i < ENVIRO_SIZE; i++)
        {
            if (Find_string((char *)mqtt_recv->Lpuart1RecBuff, enviro_tem[i], ",\0D\0A",
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
        fuzzy_proc(0);  //????????
#endif
        valid_flag = 0;

    }







}
/*{
    "??ID": 47,
    "????": {
        "????": 25,
        "????": 36,
        "????": 42,
        "????": 4,
        "?????: 4
    },
    "????": {
        "????: 255,
        "??????": 5,
        "??????": 8,
        "??????": 1
    },
    "?????: {
        "??1": {
            "???: "??",
            "????: 254
        },
        "??2": {
            "???: "??"
        },
        "??3": {
            "???: "??"
        }
    }
}*/

/*{
    "??ID": 47,
    "????": {
        "????": 25,
        "????": 36,
        "????": 42,
        "????": 4,
        "?????: 4
    },
    "????": {
        "????: 255,
        "??????": 5,
        "??????": 8,
        "??????": 1
    }
}*/

/*else if (ATCmdNum == AT_MPUB)//????
{
	sprintf(send_buffer, "%s%s,%d\0D\0A",ATCmds[ATCmdNum].ATSendStr,
											 config_usr.mqtt_msubtopic, 0);
	HAL_UART_Transmit(&huart1, (uint8_t *)send_buffer, strlen(send_buffer), 0xFF);
	memset(send_buffer, 0x00, strlen(send_buffer));
	}*/
char mqtt_send_buf[1024];

//????
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

//   sprintf(mqtt_send_buf, "{\0D\0A\
//	   ??ID?? 123,\0D\0A\
//	   ????: {\0D\0A\
//		   ????:  123,\0D\0A\
//		   ????:  123,\0D\0A\
//		   ????:  123,\0D\0A\
//		   ????:  123,\0D\0A\
//		   ?????  123,\0D\0A\
//		   ??????: 123,\0D\0A\
//	   },\0D\0A\
//	   ????: {\0D\0A\
//		   ????  123,\0D\0A\
//		   ??????:  123,\0D\0A\
//		   ??????:  123,\0D\0A\
//		   ??????:  7123\0D\0A\
//	   }\0D\0A\
//   }");

//{
//                "??ID": 123,
//                "????": {
//                        "????": 12.000000,
//                        "????": 7.000000,
//                        "????": 5.000000,
//                        "????": 3.000000,
//                        "?????: 4,
//                        "???????: 5.0},
//                "????": {
//                        "????: 22,
//                        "??????": 1.000000,
//                        "??????": 2.000000,
//                        "??????: 3.000000
//                }
//        }


//    sprintf(mqtt_send_buf, "{\0D\0A\
//		??ID?? %s,\0D\0A\
//		?????? {\0D\0A\
//			?????? %f,\0D\0A\
//			?????? %f,\0D\0A\
//			?????? %f,\0D\0A\
//			?????? %f,\0D\0A\
//			?????? %x,\0D\0A\
//			???????%x,\0D\0A\
//		},\0D\0A\
//		?????? {\0D\0A\
//			????? %s,\0D\0A\
//			???????? %f,\0D\0A\
//			???????? %f,\0D\0A\
//			???????? %f\0D\0A\
//		}\0D\0A\
//	}",     mqtt_payload_u.devid,
//            mqtt_payload_u.data[TOUT_INDEX],
//            mqtt_payload_u.data[TIN_INDEX],
//            mqtt_payload_u.data[PUMP_F_INDEX],
//            mqtt_payload_u.data[PUMP_E_INDEX],
//            mqtt_payload_u.status[DEV_STATUS_INDEX],
//            mqtt_payload_u.status[DEV_MASK_INDEX],
//            mqtt_payload_u.version,
//            mqtt_payload_u.data[WATER_O_INDEX],
//            mqtt_payload_u.data[WATER_IN_INDEX],
//            mqtt_payload_u.data[UP_PERIOD_INDEX]);
    sprintf(mqtt_send_buf, "{\\0D\\0A\
		\22??ID\22 : %s,\\0D\\0A\
		\22 ????\22 : {\\0D\\0A\
			\22 ????\22 : %f,\\0D\\0A\
			\22 ????\22 : %f,\\0D\\0A\
			\22 ????\22 : %f,\\0D\\0A\
			\22 ????\22 : %f,\\0D\\0A\
			\22 ????\22 : %x,\\0D\\0A\
			\22 ??????\22 ??x,\\0D\\0A\
		},\\0D\\0A\
		\22 ????\22 : {\\0D\\0A\
			\22 ???\22 : %s,\\0D\\0A\
			\22 ??????\22 : %f,\\0D\\0A\
			\22 ??????\22 : %f,\\0D\\0A\
			\22 ??????\22 : %f\\0D\\0A\
		}\\0D\\0A\
	}",     mqtt_payload_u.devid,
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
						memset(mqtt_send_buf,0,1024);

						sprintf(mqtt_send_buf,"test mqtt");
		//mqtt_send_buf = “test mqtt??
    mqtt_at_cmd_num = AT_MPUB;

    //{
    //    "??ID": "47",
    //    "????": {
    //        "????": 1,
    //        "????": 2,
    //        "????": 3,
    //        "????": 4,
    //        "??????: 5
    //    },
    //    "????": {
    //        "????: 255,
    //        "??": 255,
    //        "??????": 6,
    //        "??????": 7,
    //        "??????": 8
    //    }
    //}


}
//void mqtt_msub()//????
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
	memset(buf,0,1024);
    switch (mqtt_at_cmd_num)
    {
        case AT_MCONFIG:
        {
            sprintf(buf, "AT+MCONFIG=%s,%s,%s\0D\0A",
                    get_config()->user_id, get_config()->user, get_config()->password);
            if (lte_Send_Cmd(buf, "OK", LTE_SHORT_DELAY)) //??AT
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
            if (lte_Send_Cmd(buf, "OK", LTE_SHORT_DELAY)) //??AT
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
            sprintf(buf, "AT+MIPSTART=%s,%s\r\n", get_config()->mqtt_ip,
                    get_config()->mqtt_port);
            if (lte_Send_Cmd(buf, "CONNECT", LTE_SHORT_DELAY)) //??AT
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
            if (lte_Send_Cmd("AT+MCONNECT=1,3000\r\n", "CONNACK OK", LTE_SHORT_DELAY)) //??AT
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
            sprintf(buf, "AT+MSUB=%s,%d\r\n", get_config()->mqtt_subtopic,
                    0);
            if (lte_Send_Cmd(buf, "SUBACK", LTE_SHORT_DELAY)) //??AT
            {
                mqtt_at_cmd_num = AT_MIPCLOSE;
            }
            else
            {
                mqtt_at_cmds.RtyNum = 0;
                mqtt_at_cmd_num = AT_MPUB_RECV;
				
			//memset(get_lte_recv()->Lpuart1RecBuff,0,sizeof(get_lte_recv()->Lpuart1RecBuff));
            }

        }
        break;
        case AT_MPUB://public msg
        {
					  
            sprintf(buf, "AT+MPUB=\\22%s\\22,%d,%d,\\22%s\\22\\0D\\0A", get_config()->mqtt_mpubtopic,
                    1,0, mqtt_send_buf);
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
   /* if (mqtt_at_cmds.RtyNum >= 3)
    {
        CAT1_Init();
        mqtt_at_cmds.net_status = TIME_OUT;

    }*/

    return mqtt_at_cmds.net_status;
}

void mqtt_proc()
{

    if (lte_Info_Show() == NET_CONNECT) //????
    {
        if (mqtt_Info_Show() == SUCCESS_REC) //mqtt??????
        {
            registerTick(MQTT_TX_TICK_NO, 10000);
            if (GetTickResult(MQTT_TX_TICK_NO) == 1) //10s
            {
                reset_registerTick(MQTT_TX_TICK_NO);
                upload();//????
                mqtt_Info_Show();

            }
            registerTick(MODBUS_TEMP_TX_TICK_NO, 180000);
        }

    }
    else //???????,????????
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

