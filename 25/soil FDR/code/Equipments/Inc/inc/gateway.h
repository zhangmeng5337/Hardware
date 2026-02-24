#ifndef GATEWAY_H#define GATEWAY_H
#include "main.h"#define DATA_MAX_ADDR		40000#define DATA_MAX_COUNT		2000typedef struct{	unsigned int last_addr;}data_in_flash_stru;void Gateway_Process(void);void DataFrameSave(void);data_in_flash_stru *getDataFrameAddr(void);unsigned char* readDataFrame(void);#endif

