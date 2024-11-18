#include "cJSON.h"
#include "main.h"
#include "json_para.h"
#include "mqtt_analy.h"
#include "config.h"

//https://blog.csdn.net/weixin_45030703/article/details/106208593
//https://blog.csdn.net/sinat_41785418/article/details/104635446
/*     cJSON *root,*fmt;
    root=cJSON_CreateObject();
    cJSON_AddStringToObject(root,"Address",sta.Address);
    cJSON_AddStringToObject(root,"EndpointId",sta.Endpointid);
    cJSON_AddItemToObject(root, "State", fmt=cJSON_CreateObject());
    cJSON_AddStringToObject(fmt,"State", sta.State);
    cJSON_AddStringToObject(root,"StateType",sta.StateType);
    cJSON_AddNumberToObject(root,"DeviceType",1101);

    char *out=cJSON_PrintUnformatted(root);
    if(out)
    {
       strncpy((char*)sta_buff,out,strlen(out));
       sta_flag=1;
       free(out);
    }

{
“Address”: “1234567890ABCDEF”,
“EndpointId”: “1”,
“State”: {
“State”: “1”
},
“StateType”: “020D”，
“DeviceType”: 1101
}*/

/*dev---->ser
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
}

/*鏈嶅姟绔?-->璁惧锛堝懆鏈熶笅鍙戯級
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

/*鏈嶅姟绔?-->璁惧
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

//sprintf(mqtt_send_buf,"{\\0D\\0A\\
\\22Dev ID\\22:
//% s, \\0D\\0A\\
//\\22Status Data\\22:
//{
//    \\0D\\0A\\
//\\22Out Tem\\22:
//    \\22 % f\\22, \\0D\\0A\\
//\\22In Tem\\22:
//    \\22 % f\\22, \\0D\\0A\\
//\\22Front Pressure\\22:
//    \\22 % f\\22, \\0D\\0A\\
//\\22After Pressure\\22:
//    \\22 % f\\22, \\0D\\0A\\
//\\22Status\\22:
//    \\22 % d\\22\\0D\\0A\\
//}, \\0D\\0A\\
//\\22Dev Params\\22:
//{
//    \\0D\\0A\\
//\\22Version\\22:
//    \\22 % s\\22, \\0D\\0A\\
//\\22Set Out Tem\\22:
//    \\22 % f\\22, \\0D\\0A\\
//\\22Set In Tem\\22:
//    \\22 % f\\22, \\0D\\0A\\
//\\22Upload Period(second)\\22:
//    \\222\\22\\0D\\0A\\
//}\\0D\\0A\\
//}",mqtt_payload_u.devid,
//mqtt_payload_u.data[TOUT_INDEX],
//mqtt_payload_u.data[TIN_INDEX],
//mqtt_payload_u.data[PUMP_F_INDEX],
//mqtt_payload_u.data[PUMP_E_INDEX],
//mqtt_payload_u.status[DEV_STATUS_INDEX],
//mqtt_payload_u.version,
//mqtt_payload_u.data[WATER_O_INDEX],
//mqtt_payload_u.data[WATER_IN_INDEX],
//mqtt_payload_u.data[UP_PERIOD_INDEX]);

cJSON *dev_pub, *pub_status, *pub_params;
char *data;

char *json_pack(mqtt_payload_stru *pb)
{
    static uint8_t number_data;
    cJSON *dev_pub, *pub_status, *pub_params;
    char *data;


    /*{
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
    number_data = number_data + 1; //模拟数字
    if (number_data > 100)
    {
        number_data = 0;
    }
	dev_pub = cJSON_CreateObject(); //创建根数据对象
    cJSON_AddStringToObject(dev_pub, "Dev ID", pb->devid);  //根节点下添加数字

    cJSON_AddItemToObject(dev_pub, "Status Data",
                          pub_status = cJSON_CreateObject());             //根节点下添加字符
    cJSON_AddNumberToObject(pub_status, "Out Tem",
                            (pb->data[TOUT_INDEX]));                 //根节点下添加汉字
    cJSON_AddNumberToObject(pub_status, "In Tem", (pb->data[TIN_INDEX]));
    cJSON_AddNumberToObject(pub_status, "Front Pressure", (pb->data[PUMP_F_INDEX]));
    cJSON_AddNumberToObject(pub_status, "After Pressure", (pb->data[PUMP_E_INDEX]));
    cJSON_AddNumberToObject(pub_status, "Status", (pb->data[DEV_STATUS_INDEX]));
	cJSON_AddNumberToObject(pub_status, "air pump_status", (pb->data[DEV_PUMP_STATUS_INDEX]));

    cJSON_AddItemToObject(dev_pub, "Dev Params", pub_params = cJSON_CreateObject());
    cJSON_AddStringToObject(pub_params, "Version", (pb->version));
    cJSON_AddNumberToObject(pub_params, "Set Out Tem", (pb->data[WATER_O_INDEX]));
    cJSON_AddNumberToObject(pub_params, "Set In Tem", (pb->data[WATER_IN_INDEX]));
    cJSON_AddNumberToObject(pub_params, "Upload Period(second)",
                            (pb->data[UP_PERIOD_INDEX]));
   /* dev_pub = cJSON_CreateObject(); //创建根数据对象
    cJSON_AddStringToObject(dev_pub, "Dev ID", pb->devid);  //根节点下添加数字

    cJSON_AddItemToObject(dev_pub, "Status Data",
                          pub_status = cJSON_CreateObject());             //根节点下添加字符
    cJSON_AddNumberToObject(pub_status, "Out Tem",
                            (pb->data[TOUT_INDEX]));                 //根节点下添加汉字
    cJSON_AddNumberToObject(pub_status, "In Tem", (pb->data[TIN_INDEX]));
    cJSON_AddNumberToObject(pub_status, "Front Pressure", (pb->data[PUMP_F_INDEX]));
    cJSON_AddNumberToObject(pub_status, "After Pressure", (pb->data[PUMP_E_INDEX]));
    cJSON_AddNumberToObject(pub_status, "Status", (pb->data[DEV_STATUS_INDEX]));
	cJSON_AddNumberToObject(pub_status, "air pump_status", (pb->data[DEV_PUMP_STATUS_INDEX]));

    cJSON_AddItemToObject(dev_pub, "Dev Params", pub_params = cJSON_CreateObject());
    cJSON_AddStringToObject(pub_params, "Version", (pb->version));
    cJSON_AddNumberToObject(pub_params, "Set Out Tem", (pb->data[WATER_O_INDEX]));
    cJSON_AddNumberToObject(pub_params, "Set In Tem", (pb->data[WATER_IN_INDEX]));
    cJSON_AddNumberToObject(pub_params, "Upload Period(second)\\22,
                            (pb->data[UP_PERIOD_INDEX]));*/

    return cJSON_PrintUnformatted(dev_pub);

}
void free_cjson()
{
    cJSON_Delete(dev_pub);

}
/*鏈嶅姟绔?-->璁惧锛堝懆鏈熶笅鍙戯級
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

/*鏈嶅姟绔?-->璁惧
{
   "Dev ID":  "866289037465624",
   "Dev Ctrl": {
               "Updat Frimware": "1",
               "Reboot Dev": "1",
               "Power ctrl": "1",
               "Set Out Temp": "45",
               "Set Room Temp": "23",
               "Set Upload Period(second)\\22: "13"
     }
}*/

unsigned char json_analysis(char *out)
{

    cJSON *sub_ser, *sub_ctrl, *sub_dev_ctrl, *json_three;
    unsigned int items_size, i;
	json_three = cJSON_Parse(out); //将得到的字符串解析成json形式
    sub_ser = cJSON_GetObjectItem(json_three, "Room Temp"); //将得到的字符串解析成json形式
    if (sub_ser != NULL)
    {
        items_size = cJSON_GetArraySize(sub_ser);
        if (items_size == 0)
        {
            return 0;
        }
        cJSON *arr_items = sub_ser->child;//；
         get_config()->update_setting = 2;
        for (i = 0; i < items_size; i++)
        {
            get_config()->indoor_temperature[i] = (unsigned int)arr_items->valueint;
            arr_items = arr_items->next;//跳到下一个元素
        }
    }
    else
    {
        sub_ser = cJSON_GetObjectItem(json_three, "Dev Ctrl"); //将得到的字符串解析成json形式
       // if (sub_ser != NULL)
        {
            /****************************/
            /*    测试将JSON打印出来  */
            /***************************/
            //char *out_data = cJSON_Print(json);   //将json形式打印成正常字符串形式
            //printf(\\22%s",out_data);

            sub_dev_ctrl = cJSON_GetObjectItem(sub_ser,"Updat Frimware");    //从json获取键值内容
            get_config()->update_firm = sub_dev_ctrl->valueint;

            sub_dev_ctrl = cJSON_GetObjectItem(sub_ser,"Reboot Dev");   //从json获取键值内容
            get_config()->reboot = sub_dev_ctrl->valueint;

            sub_dev_ctrl = cJSON_GetObjectItem(sub_ser,"Power ctrl");   //从json获取键值内容
            get_config()->machine = sub_dev_ctrl->valueint;

            sub_dev_ctrl = cJSON_GetObjectItem(sub_ser,"Set Out Temp");     //从json获取键值内容
            get_config()->set_tout = sub_dev_ctrl->valueint;

            sub_dev_ctrl = cJSON_GetObjectItem(sub_ser,"Set Room Temp");   //从json获取键值内容
            get_config()->set_tindoor = sub_dev_ctrl->valueint;

            sub_dev_ctrl = cJSON_GetObjectItem(sub_ser,"Set Upload Period(second)");   //从json获取键值内容
            get_config()->set_up_period = sub_dev_ctrl->valueint;
		    get_config()->update_setting = 1;

        }
        //else
          //return 0;

    }
    cJSON_Delete(json_three);  //释放内存
}





