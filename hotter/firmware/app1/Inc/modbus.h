#ifndef MODBUS_H_
#define MODBUS_H_

#include "config.h"
//#define MODBUS_RTU_COUNT            4
//#define MODBUS_RETRY_COUNT          4
#define CMD_SIZE		10
#define DEV_SIZE    5

#define PWR_INDEX				0
#define PID_INDEX				1
#define STATUS1_INDEX			2
#define STATUS2_INDEX			3


#define CONTROLLER_REG				0x0000
#define CONTROLLER_REG2				0x0040

#define TEMPERATURE_REG				0x0007
#define FAULT_REG				    0x0051



#define MODBUS_READ_CMD				0x03  //
#define MODBUS_WRITE_ONE_CMD        0x06
#define MODBUS_WRITE_MUL_CMD        0x10

#define CTRL_EN						1

#define DEV_ADDR  0xfe

#define RETRY_COUNT		5
#define FAULT_COUNT		4
typedef struct 
{
 unsigned char addr;
 unsigned char func;
 unsigned int reg;
 unsigned int regCount;
 unsigned char payload[64]; 

 unsigned char enable;// tx  poll by cpu
 unsigned char status;//to be tx 
 unsigned int crc; 
 unsigned char last_reg; 
}modbus_pump_cmd_stru;
typedef struct 
{
 modbus_pump_cmd_stru *pb;
 unsigned char cmd_seq; 
 unsigned char retry_count;
 unsigned char addr;
 
}modbus_cmd_pack_stru;




typedef struct 
{
    unsigned char ctrl_mode;//0:global pump ctrl 1:node pump 2:native mode
	unsigned char ctrl_func[10];
	unsigned char error_count[DEV_SIZE];
	unsigned char address;
	unsigned char func;
	unsigned int  reg;
	unsigned int  regCount;
	unsigned char payload[512];
	unsigned int  crc;
	unsigned char update;
	unsigned char retry_count;
	unsigned int  fault;
	unsigned char dev_addr_index;
	unsigned char recv_len;
	unsigned int last_reg;
	unsigned char tx_index;
	unsigned char dev_addr_mask;//tx to specified dev
}modbus_stru;

unsigned char modbus_trans(unsigned char addr,unsigned char func,unsigned int reg,unsigned char *payload,unsigned int reg_count,unsigned char len,unsigned char retry_times);
void modbus_proc(void);
void modbus_pack(unsigned char mode, unsigned char *pb);
modbus_stru *get_recv_machine(void);

void modbus_init(void);
modbus_stru *get_tx_machine(void);
void modbus_test(void);
#endif

