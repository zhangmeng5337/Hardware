#ifndef DI_H_
#define DI_H_
#include "main.h"
#define DI_SIZE  8
typedef struct
{
	unsigned char di_status;
	unsigned char di_No_buf[DI_SIZE];
} stru_di_stru;

stru_di_stru di_input_read(unsigned int diNO);
unsigned char di_proc(unsigned char diNO);
stru_di_stru *get_di_data(void);

#endif

