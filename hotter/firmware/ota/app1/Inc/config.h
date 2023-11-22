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
   //mqtt
    char user[128];//mqtt server user
    char password[64]; //mqtt server password
    char user_id[128];//imei
    char mqtt_ip[128];//server ip
    char mqtt_port[64];//
    char mqtt_msubtopic[128];
    char mqtt_subtopic[128];
	char version[64];
  //http
    char http_ip[128];//server ip
    char http_port[64];//

	
    unsigned char temp[24];//�¶ȶ�
    unsigned char mode;// 0---���أ�1----Զ��
    unsigned char ai_config[32];
    unsigned char ao_config[1];
    unsigned char di_config[8];
    unsigned char do_config[20];

    //setting params
	unsigned char reboot;
	unsigned char machine;	
	unsigned char update_firm;	
	float set_tout;
	float set_tindoor;
	float set_up_period;	
	//char password[128];
	unsigned char update_setting;

} CONFIG_stru;




#endif


