#ifndef Protocol_C_H_
#define Protocol_C_H_

#include "main.h"
#include "loraHW.h"



#define ADDR_SIZE			0x08
#define START_ADDR_SIZE		0x02
#define PAYLOAD_SIZE		280
#define PAYLOAD_LEN_INDEX   0x14
#define UNSIGNED            0x00
#define SIGN_DOT1           0x81
#define UNSIGN_DOT1         0x01
#define UNSIGN_DOT2         0x02
#define UNSIGN_DOT3         0x03


enum FUNC_CODE
{
	gateway_Rcmd = 0x17,
	pc_cmd = 0x18,
	PC_readFrame = 0x30,
	auto_report_cmd = 0x51
};

enum DEV_CODE
{
	soilRH_DEV = 0x01,
	soilCON_DEV,	
	co2_DEV,
	con_DEV,
	sunShine_DEV,
	pressure_DEV = 0x08,
	flow_DEV,
	gateway_DEV = 0xfd
};
enum REG_EUIP_ADDR
{
	reg_period_set = 0xf001,
	reg_period_Hmin,
	reg_period_Lmin,
	reg_period_sec,
	reg_power,
	reg_quantity,
	reg_node_addr_H,
	reg_node_addr_M,
	reg_node_addr_LM,
	reg_node_addr_L,
	reg_lora_native_addr,
	reg_modbus_sensor_rate,
	reg_modbus_pc_rate,
	reg_longitude_H = 0xe00,
	reg_longitude_M,
	reg_longitude_LM,
	reg_longitude_L,
	reg_latitude_H = 0xe008,
	reg_latitude_M,
	reg_latitude_LM,
	reg_latitude_L,
	reg_soil_temp_5 = 0x1001,
	reg_soil_rh_5 ,	
	reg_soil_temp_4,
	reg_soil_rh_4 , 
	reg_soil_temp_3,
	reg_soil_rh_3, 
	reg_soil_temp_2,
	reg_soil_rh_2, 
	reg_soil_temp_1,
	reg_soil_rh_1, 
    reg_actutor1 = 0x1010,
	reg_actutor2,
	reg_actutor3,
	reg_clock_ym = 0x1100,
	reg_clock_dh,
	reg_clock_ms,
	reg_dest_gateway_addrH = 0x1200,
	reg_dest_gateway_addrM,
	reg_dest_gateway_addrLM,
	reg_dest_gateway_addrL,
	reg_dest_lora_addrH,
	reg_dest_lora_center_addr,
	reg_lora_air_rate,
	reg_lora_ch,
	reg_server_ipH,
	reg_server_ipM,
	reg_server_ipLM,
	reg_server_ipL,
	reg_server_port
};

enum sensor_params_enum
{
	temperature = 0x01,
	rh,
	light,
	soilT,
	soilW,
	pressure_air,
	pressure,
	co2,
	co1,
	so2,
	ph,
	ec,
	o2,
	rain,
	wind,
	direction,
	p_pa,
	P_kpa,
	P_Mpa,
	P_bar,
	P_Mbar,
	quantity = 0xf2,
	year_month,
	day_hour,
	min_sec,
	auto_report_time
};

typedef struct
{
  unsigned char DesAddr[ADDR_SIZE];
  unsigned char SrcAddr[ADDR_SIZE];
  unsigned char devCode;
  unsigned char funCode;
  unsigned char startAddr[START_ADDR_SIZE];
  unsigned char payloadLenH;
  unsigned char payloadLenL; 
  unsigned char payload[PAYLOAD_SIZE];
}Proctocol_C_stru;
typedef struct
{
  unsigned char timoutFLag;
  unsigned char tickFlag;
  uint32_t tickCount;
}loraModuleTimeout_stru;

void get_equipmentData(void);
unsigned char protocolCAnaly(unsigned char  *p);
void WrRead_equipment(LORAHW_stru *loraNo);
void equipmentProcess(void);
void wirelessTimoutStart(unsigned char p);
Proctocol_C_stru *protocolCstru(void);
unsigned char *ackmodeStatus(void);
void SendLoraData(LORAHW_stru *loraNo,unsigned char *p );
void SendPayloadPack(LORAHW_stru *loraNo,unsigned char ackMode);


#endif

