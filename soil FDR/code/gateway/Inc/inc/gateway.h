#ifndef GATEWAY_H
#define GATEWAY_H
#include "main.h"
#define DATA_MAX_ADDR		40000
#define DATA_MAX_COUNT		2000
//#define DATA_MAX_ADDR		200
//#define DATA_MAX_COUNT		10
typedef struct
{
	unsigned int lastRead_addr;
	 uint32_t    lastWrite_addr;	
}data_in_flash_stru;
void Gateway_Process(void);
void DataFrameSave(void);
data_in_flash_stru *getDataFrameAddr(void);
unsigned char* readDataFrame(void);
#endif

