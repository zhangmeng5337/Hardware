#ifndef MODBUS_H_
#define MODBUS_H_

#define DEV_SIZE    3
#define MODBUS_RTU_COUNT            4
#define MODBUS_RETRY_COUNT          4

#define CONTROLLER_REG				0x0000
#define TEMPERATURE_REG				0x0007
#define FAULT_REG				    0x0051



#define MODBUS_READ_CMD				0x03  //
#define MODBUS_WRITE_ONE_CMD        0x06
#define MODBUS_WRITE_MUL_CMD        0x10

#define CTRL_EN						0

#define DEV_ADDR  0xfe


typedef struct 
{
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
}modbus_stru;

unsigned char modbus_trans(unsigned char addr,unsigned char func,unsigned int reg,unsigned char *payload,unsigned int reg_count,unsigned char len,unsigned char retry_times);
void modbus_proc(void);
void modbus_pack(unsigned char mode, unsigned char *pb);
modbus_stru *get_recv_machine(void);


#endif

