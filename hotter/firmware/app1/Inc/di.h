#ifndef DI_H_
#define DI_H_
#include "main.h"
#define DI_SIZE  16
//*****di status*********************

#define WATER_LEVEL				0
#define SEQ_PROTECT				1
#define BASIC_PUMP				2
#define SECONDARY_PUMP			3
#define SUPPLE_PUMP				4

#define BASIC_PUMP_MANUAL		5
#define SECONDARY_PUMP_MANUAL	6
#define SUPPLE_PUMP_MANUAL		7

#define BASIC_PUMP_FAULT		8
#define SECONDARY_PUMP_FAULT	9
#define SUPPLE_PUMP_FAULT		10


typedef struct
{
	unsigned int di_status;
	unsigned int di_No_buf[DI_SIZE];
	unsigned char di_index_conv[DI_SIZE];
} stru_di_stru;

stru_di_stru di_input_read(unsigned int diNO);
unsigned char di_proc(unsigned char diNO);
stru_di_stru *get_di_data(void);

#endif

