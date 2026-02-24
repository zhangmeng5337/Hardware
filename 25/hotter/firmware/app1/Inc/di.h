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

typedef enum
{
	di_index1 = 0x0001,
	di_index2 = 0x0002,
	di_index3 = 0x0004,
	di_index4 = 0x0008,
	di_index5 = 0x0010,
	di_index6 = 0x0020,
	di_index7 = 0x0040,
	di_index8 = 0x0080,
	di_index9 = 0x0100,
	di_index10 = 0x0200,
	di_index11 = 0x0400,
	di_index12 = 0x0800,
	di_index13 = 0x1000,
	di_index14 = 0x2000,
	di_index15 = 0x4000,
	di_index16 = 0x8000
}di_table;

typedef struct
{
	unsigned int di_status;
	//unsigned int di_No_buf[DI_SIZE];
	unsigned char di_index_conv[DI_SIZE];
} stru_di_stru;

stru_di_stru di_input_read(unsigned int diNO);
unsigned int di_proc(unsigned char diNO);
stru_di_stru *get_di_data(void);

#endif

