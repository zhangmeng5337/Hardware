#ifndef CONFIG_H_
#define CONFIG_H_
#include "main.h"
void delay_us(uint32_t nCount);
#include "main.h"
/*index = 0  "鍗囩骇": 1,
index = 1 "閲嶅惎": 1,
index = 2 "鏈虹粍寮€鍏虫満": 0,
index = 3 "璁剧疆鍑烘按娓╁害": "value",
index = 4 "璁剧疆瀹ゅ唴娓╁害": "value",
index = 5 "鏁版嵁涓婁紶鍛ㄦ湡": "value"*/

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

	
    unsigned char temp[24];//temperature
    unsigned char mode;// 0---native；1----remote
    unsigned char ai_config[32];
    unsigned char ao_config[1];
    unsigned char di_config[8];
    unsigned char do_config[20];

    //setting params
	unsigned char reboot;
	unsigned char machine;	
	unsigned char update_firm;	//firmware update
	float set_tout;  //
	float set_tindoor; //
	float set_up_period;//
	uint32_t fault_mask;	
	uint32_t fault_status;
	//char password[128];
	unsigned char update_setting; //setting update 

} CONFIG_stru;

CONFIG_stru *get_congfig(void);
void config_save(void);
void config_init(void);



#endif


