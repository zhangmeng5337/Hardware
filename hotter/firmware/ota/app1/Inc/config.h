#ifndef CONFIG_H_
#define CONFIG_H_
#include "main.h"
void delay_us(uint32_t nCount);
#include "main.h"
/*index = 0  "升级": 1,
index = 1 "重启": 1,
index = 2 "机组开关机": 0,
index = 3 "设置出水温度": "value",
index = 4 "设置室内温度": "value",
index = 5 "数据上传周期": "value"*/

#define UPDATE_INDEX		0
#define REBOOT_INDEX		1
#define MACHINE_INDEX		2
#define TOUT_INDEX			3
#define TINDOOR_INDEX		4
#define UP_PERIOD_INDEX		5


typedef struct
{
    char user[128];
    char password[128];
    char user_id[128];
    char mqtt_ip[128];
    char mqtt_port[128];
    char mqtt_msubtopic[128];
    char mqtt_subtopic[128];
	char version[32];
    unsigned char temp[24];//�¶ȶ�
    unsigned char mode;// 0---���أ�1----Զ��
    unsigned char ai_config[32];
    unsigned char ao_config[1];
    unsigned char di_config[8];
    unsigned char do_config[20];
	unsigned char payload[256];
	unsigned char update;
	float payload_f[32];
} CONFIG_stru;




#endif


