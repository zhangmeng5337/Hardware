#ifndef MODBUS_H_
#define MODBUS_H_
#define DEV_ADDR  0xfe

typedef struct 
{
	unsigned char address;
	unsigned char func;
	unsigned int  reg;
	unsigned int  regCount;
	unsigned char payload[128];
	unsigned int crc;
}modbus_stru;
typedef struct 
{
	unsigned char address;
	unsigned char func;
	unsigned int  reg;
	unsigned int  regCount;
	unsigned char payload[64];
	unsigned int crc;
	unsigned char update;
}modbus_tx_stru;
void modbus_trans(void);
modbus_tx_stru*modbus_pack(void);
void modbus_proc(void);

#endif

