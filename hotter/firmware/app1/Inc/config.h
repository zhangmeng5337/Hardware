#ifndef CONFIG_H_
#define CONFIG_H_
#include "main.h"

#define NATIVE_MODE  1
#define SMART_MODE  2
#define SCHE_MODE   3
#define OFF_MODE   4
#define DELI		0
#define ZT			1



#define  UCHAR_TYP 		0
#define  UINT_TYP 		1
#define  FLOAT_TYP 	    2
#define  INT_TYP 		3
#define  STRING_TYP 	4
#define  UINT64_TYP 	5
#define  INT_ARRAY_TYP 	6

#define  MATCH_ID 	    1
#define  NO_MATCH_ID 	0
#define  UPDATE_SET 	1
#define  NOUPDATE_SET 	0
void delay_us(uint32_t nCount);
#include "main.h"



/*index = 0  "鍗囩�?: 1,
index = 1 "閲嶅�?: 1,
index = 2 "鏈虹粍寮€鍏虫満": 0,
index = 3 "璁剧疆鍑烘按娓╁�?: "value",
index = 4 "璁剧疆瀹ゅ唴娓╁害": "value",
index = 5 "鏁版嵁涓婁紶鍛ㄦ�?: "value"*/



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
    uint16_t seq;
	uint16_t seq_count;
	char http_download[128];//ftp url
    //unsigned char temp[24];//temperature
    unsigned char instru_num;
    unsigned char mode;// 1---smart  2----native no need net 4---customer 3ctrl schedule temp
    unsigned char dev_size;
	unsigned char energy_size;
	unsigned char dev_size_tmp;
    unsigned char ai_config[32];
    unsigned char ao_config[1];
    unsigned char di_config[8];
    unsigned char do_config[20];

    //setting params
    unsigned char count;
	unsigned char reboot;
	unsigned int machine;	  //pwrctrl:bit0---m1;bit1---m2;bit2---m3;
	                          //select:bit15 ---m1;bit14---m2;bit13---m3,only for pwr ctrl
	unsigned char update_firm;	//firmware update
	float indoor_temperature[ENVIRO_SIZE];
	unsigned char tlen;
	unsigned int set_tout;  //pid cal
	unsigned int set_tout_tmp;  //pum set

	unsigned int set_tindoor; //
	float set_up_period;//
	uint32_t fault_mask;	
	uint32_t fault_status;
	//char password[128];
	unsigned char update_setting; //setting update 
	int addr_count ;
	int reboot_flag ;
	int Erase_flag ;
	unsigned char to_index;
	unsigned char tin_index;
	unsigned char po_index;
	unsigned char pin_index;	
	uint32_t timeout;
	unsigned char valid_flag;
	unsigned char connectTimeOut;
	unsigned int private_reg;
	unsigned char energy_start_addr;
	//unsigned char sub_sring[3][64];

} CONFIG_stru;
extern  CONFIG_stru config_usr;
CONFIG_stru *get_config(void);
void config_save(void);
void config_init(void);



#endif


