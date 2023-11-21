#include "mqtt_analy.h"
#include "my_string.h"
#include "cat1.h"
/*
get_params()->payload[index]:
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
{/*{
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
	get_params()->update = 1;
	if(Find_string((char *)Lpuart1type.Lpuart1RecBuff, "升级:  ", "\r\n", 
		        &get_params()->payload[UPDATE_INDEX])==0)
		{
				;
				}
	else if(Find_string((char *)Lpuart1type.Lpuart1RecBuff, "重启: ", "\r\n", 
		        &get_params()->payload[UPDATE_INDEX])==0)
					;
	else if(Find_string((char *)Lpuart1type.Lpuart1RecBuff, "机组开关机: ", "\r\n", 
		        &get_params()->payload[UPDATE_INDEX])==0)
					;
	else if(Find_string((char *)Lpuart1type.Lpuart1RecBuff, "设置出水温度: ", "\r\n", 
		        &get_params()->payload[UPDATE_INDEX])==0)
		{
				//sprintf(&get_params()->payload_f[TOUT_INDEX], "%f,", get_params()->payload[UPDATE_INDEX]);//user id
				

				}
	else if(Find_string((char *)Lpuart1type.Lpuart1RecBuff, "设置室内温度: ", "\r\n", 
		        &get_params()->payload[UPDATE_INDEX])==0)
		        ;
	else if(Find_string((char *)Lpuart1type.Lpuart1RecBuff, "数据上传周期: ", "\r\n", 
		        &get_params()->payload[UPDATE_INDEX])==0)
		        ;
	else 
		get_params()->update = 0;

				

}

void upload()
{
;

}


