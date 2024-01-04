#ifndef MODBUS_H_
#define MODBUS_H_

#define POWER_REQUEST		0x01  //
#define POWER_CALI          0x02




#define MODBUS_READ_CMD				0x03  //
#define MODBUS_WRITE_ONE_CMD        0x06


#define DEV_ADDR		0x01
#define FUNC_R		0x03
#define FUNC_W		0x02

#define REGH		0x00
#define REGL		0x39
#define DATAH		0x00
#define DATAL		0x34
#define CRCH		0x10
#define CRCL		0x94



typedef struct 
{
	unsigned char address;
	unsigned char func;
	unsigned int  reg;
	unsigned int  regCount;
	unsigned char payload[256];
	unsigned int crc;
}modbus_stru;
void modbus_trans(unsigned char mode);
void modbus_proc(unsigned char mode);
void modbus_pack(unsigned char mode, unsigned char *pb);
modbus_stru *get_recv_machine(void);
unsigned char rs485_recv(void);


#endif

