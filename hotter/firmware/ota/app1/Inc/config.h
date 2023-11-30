#ifndef CONFIG_H_
#define CONFIG_H_
#include "main.h"
void delay_us(uint32_t nCount);
#include "main.h"
/*index = 0  "鍗囩�?: 1,
index = 1 "閲嶅�?: 1,
index = 2 "鏈虹粍寮€鍏虫満": 0,
index = 3 "璁剧疆鍑烘按娓╁�?: "value",
index = 4 "璁剧疆瀹ゅ唴娓╁害": "value",
index = 5 "鏁版嵁涓婁紶鍛ㄦ�?: "value"*/

#define UPDATE_INDEX		0
#define REBOOT_INDEX		1
#define MACHINE_INDEX		2
#define TOUT_INDEX			3
#define TINDOOR_INDEX		4
#define UP_PERIOD_INDEX		5

#define ENVIRO_SIZE    64
typedef struct
{
   //mqtt
    char user[128];//mqtt server user
    char password[64]; //mqtt server password
    char user_id[128];//imei
    char mqtt_ip[128];//server ip
    char mqtt_port[64];//
    char mqtt_mpubtopic[128];
    char mqtt_subtopic[128];
	char version[64];
  //http
    char http_ip[128];//server ip
    char http_port[64];//

	
    unsigned char temp[24];//temperature
    unsigned char mode;// 0---native�?----remote
    unsigned char ai_config[32];
    unsigned char ao_config[1];
    unsigned char di_config[8];
    unsigned char do_config[20];

    //setting params
	unsigned char reboot;
	unsigned int machine;	  //pwrctrl:bit0---m1;bit1---m2;bit2---m3;
	                          //select:bit15 ---m1;bit14---m2;bit13---m3,only for pwr ctrl
	unsigned char update_firm;	//firmware update
	float indoor_temperature[ENVIRO_SIZE];
	float set_tout;  //
	float set_tindoor; //
	float set_up_period;//
	uint32_t fault_mask;	
	uint32_t fault_status;
	//char password[128];
	unsigned char update_setting; //setting update 
	int addr_count ;
	int reboot_flag ;
	int Erase_flag ;

} CONFIG_stru;

CONFIG_stru *get_config(void);
void config_save(void);
void config_init(void);



#endif


