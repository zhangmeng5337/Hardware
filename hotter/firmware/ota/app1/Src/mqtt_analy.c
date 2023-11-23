#include "mqtt_analy.h"
#include "my_string.h"
#include "cat1.h"
#include "unit_tests.h"
#include "di.h"

mqtt_payload_stru mqtt_payload_u;


/*
get_congfig()->payload[index]:
index = 0  "升级": 1,
index = 1 "重启": 1,
index = 2 "机组开关机": 0,
index = 3 "设置出水温度": "value",
index = 4 "设置室内温度": "value",
index = 5 "数据上传周期": "value"

#define UPDATE_INDEX		0
#define REBOOT_INDEX		1
#define MACHINE_INDEX		2
#define TOUT_INDEX			3
#define TINDOOR_INDEX		4
#define UP_PERIOD_INDEX		5

*/
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

    valid_flag = 0;
    get_congfig()->update_setting = 0;
    if (Find_string((char *)Lpuart1type.Lpuart1RecBuff, "设备ID:  ", "\r\n",
                    dev_id) == 1)
    {
        if ((strcmp(dev_id, get_congfig()->user_id)) == 0)
            valid_flag = 1;

    }
    if (valid_flag == 1)
    {
        memset(dev_id, 0, 128);
        if (Find_string((char *)Lpuart1type.Lpuart1RecBuff, "升级:  ", "\r\n",
                        dev_id) == 1)
        {
            get_congfig()->update_setting = 1;
            sprintf(&get_congfig()->update_firm, "%s", dev_id);//字符串
            tmp_f = atoi(dev_id);
            get_congfig()->update_firm = tmp_f;

        }
        memset(dev_id, 0, 128);
        if (Find_string((char *)Lpuart1type.Lpuart1RecBuff, "重启: ", "\r\n",
                        dev_id) == 1)
        {
            get_congfig()->update_setting = 1;
            sprintf(&get_congfig()->reboot, "%s", dev_id); //字符串
            tmp_f = atoi(dev_id);
            get_congfig()->reboot = tmp_f;


        }
        memset(dev_id, 0, 128);

        if (Find_string((char *)Lpuart1type.Lpuart1RecBuff, "机组开关机: ", "\r\n",
                        dev_id) == 1)
        {
            get_congfig()->update_setting = 1;
            sprintf(&get_congfig()->machine, "%s", dev_id); //字符串
            tmp_f = atoi(dev_id);
            get_congfig()->machine = tmp_f;

        }
        memset(dev_id, 0, 128);

        if (Find_string((char *)Lpuart1type.Lpuart1RecBuff, "设置出水温度: ", "\r\n",
                        dev_id) == 1)
        {
            get_congfig()->update_setting = 1;
            tmp_f = atof(dev_id);
            get_congfig()->set_tout = tmp_f;
        }


        memset(dev_id, 0, 128);
        if (Find_string((char *)Lpuart1type.Lpuart1RecBuff, "设置室内温度: ", "\r\n",
                        dev_id) == 1)
        {
            get_congfig()->update_setting = 1;
            tmp_f = atof(dev_id);
            get_congfig()->set_tindoor = tmp_f;

        }


        memset(dev_id, 0, 128);
        if (Find_string((char *)Lpuart1type.Lpuart1RecBuff, "数据上传周期: ", "\r\n",
                        dev_id) == 1)
        {
            get_congfig()->update_setting = 1;
            tmp_f = atof(dev_id);
            get_congfig()->set_up_period = tmp_f;


        }


    }







}
/*{
	"设备ID": "47",
	"运行数据": {
		"出水温度": "value",
		"回水温度": "value",
		"泵前压力": "value",
		"泵后压力": "value",
		"伴热带状态": "value"
	},
	"设备参数": {
		"版本号": 255,
		"故障": 255,
		"设置出水温度": "value",
		"设置室内温度": "value",
		"数据上传周期": "value"
	}
}
	}	*/
/*else if (ATCmdNum == AT_MPUB)//发布消息
{
	sprintf(send_buffer, "%s%s,%d\r\n",ATCmds[ATCmdNum].ATSendStr,
											 config_usr.mqtt_msubtopic, 0);
	HAL_UART_Transmit(&huart1, (uint8_t *)send_buffer, strlen(send_buffer), 0xFF);
	memset(send_buffer, 0x00, strlen(send_buffer));
	}*/

void upload()
{
    char mqtt_buf[512];

    memset(mqtt_payload_u.devid, 0, sizeof(mqtt_payload_u.devid));
    memset(mqtt_payload_u.version, 0, sizeof(mqtt_payload_u.version));
    sprintf(mqtt_payload_u.devid, "%s", get_congfig()->user_id);//devid
    mqtt_payload_u.data[TOUT_INDEX] = get_ai_data()->temp[AI_WATER_T_O_INDEX];
    mqtt_payload_u.data[TIN_INDEX] =  get_ai_data()->temp[AI_WATER_T_IN_INDEX]; //water IN
    mqtt_payload_u.data[PUMP_F_INDEX] = get_ai_data()->temp[AI_PUMP_F_INDEX]; //pump front
    mqtt_payload_u.data[PUMP_E_INDEX] = get_ai_data()->temp[AI_PUMP_E_INDEX]; //pump end
    mqtt_payload_u.status = get_di_data()->di_status; //dev status
    mqtt_payload_u.status = mqtt_payload_u.status << 20; //dev status
    mqtt_payload_u.status = mqtt_payload_u.status | get_ai_data()->channel_status; //dev status
    sprintf(mqtt_payload_u.version, "%s", get_congfig()->version);//devid
    mqtt_payload_u.data[WATER_O_INDEX] = get_congfig()->set_tout; //set tmp
    mqtt_payload_u.data[WATER_IN_INDEX] = get_congfig()->set_tindoor; //set indoor tmp
    mqtt_payload_u.data[UP_PERIOD_INDEX] = get_congfig()->set_up_period; //up period



    sprintf(mqtt_buf, "{\r\n\
		设备ID: %s,\r\n\
		运行数据: {\r\n\
			出水温度: %f,\r\n\
			回水温度: %f,\r\n\
			泵前压力: %f,\r\n\
			泵后压力: %f,\r\n\
			运行状态: %h\r\n\
		},\r\n\
		设备参数: {\r\n\
			版本号: %s,\r\n\
			设置出水温度: %f,\r\n\
			设置室内温度: %f,\r\n\
			数据上传周期: %f\r\n\
		}\r\n\
	}\r\n", mqtt_payload_u.devid, mqtt_payload_u.data[TOUT_INDEX], mqtt_payload_u.data[TIN_INDEX],
            mqtt_payload_u.data[PUMP_F_INDEX], mqtt_payload_u.data[PUMP_E_INDEX],
            mqtt_payload_u.status, mqtt_payload_u.version,
            mqtt_payload_u.data[WATER_O_INDEX], mqtt_payload_u.data[WATER_IN_INDEX],
            mqtt_payload_u.data[UP_PERIOD_INDEX]);

    MQTTSendData(mqtt_buf);

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


