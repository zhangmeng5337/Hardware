#ifndef MODBUS_H
#define MODBUS_H
#include "main.h"



#define MIN_ADDR  1
#define MAX_ADDR  4





#define UINT_TYPE		0
#define FLOAT_TYPE		1

#define FUNC_WRITE	0x06

#define FUNC_READ	0x03

#define MODBUS_BSIZE	128


typedef struct{
unsigned char  RS485_Addr;//从机地址
unsigned char  func;
unsigned char  len;
unsigned int startaddr;
unsigned int regnum;
unsigned int datcount;
unsigned int crc;
unsigned char  datType;
unsigned char  ack;
unsigned char  modbus_data[32];//接收缓冲区2048字节
unsigned char  modbus_txdata[64];//接收缓冲区2048字节

}modbus_pack_stru;



unsigned char  RS485_Service(void);

modbus_pack_stru *get_modbus_recv_dat(void);
void modbus_pack(unsigned char addr,unsigned char*p,unsigned char len);

#endif


