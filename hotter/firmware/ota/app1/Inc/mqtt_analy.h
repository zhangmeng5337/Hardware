#ifndef MQTT_ANALY_H_
#define MQTT_ANALY_H_



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
#define WATER_IN_INDEX       11


#define HEAT_INDEX          0

typedef struct 
{
	float data[64];
	unsigned int status;
	char devid[64];
	char version[64];
	
}mqtt_payload_stru;


void anlysis_mqtt_recv(void);
void upload(void);

#endif

