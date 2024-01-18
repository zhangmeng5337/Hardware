#ifndef MQTT_ANALY_H_
#define MQTT_ANALY_H_
#include "main.h"

//data
#define UPDATE_INDEX		0
#define REBOOT_INDEX		1
#define MACHINE_INDEX		2
#define TOUT_INDEX			3
#define TINDOOR_INDEX		4
#define UP_PERIOD_INDEX		5
#define DEV_ID_INDEX        6
#define TIN_INDEX           7
#define PUMP_F_INDEX        8
#define PUMP_E_INDEX        9
#define WATER_O_INDEX       10
#define WATER_IN_INDEX      11
//#define WATER_IN_INDEX      11

//status
#define DEV_STATUS_INDEX        0
#define DEV_MASK_INDEX          1
#define DEV_FAULT_INDEX         2
#define DEV_PUMP_STATUS_INDEX         3


typedef struct 
{
	float data[64];
	uint32_t status[64];
	//uint32_t fault;
	char devid[64];
	char version[64];
	
}mqtt_payload_stru;


void anlysis_mqtt_recv(void);
void upload(void);
void mqtt_recv_proc(void);


void mqtt_proc(void);
void mqtt_init(void);

#endif

