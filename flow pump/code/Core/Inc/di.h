#ifndef DI_H_
#define DI_H_
#include "main.h"
#define LOW_L	0x02
#define EMPITY	0x04

typedef struct
{
	unsigned char update;
	unsigned char status;
	uint32_t pulse_cnt;
	
}di_stru;

di_stru *get_di_status(void);
void di_init(void);
void pulse_exti(void);
void do_ctrl(unsigned char relay_num);
void Di_Do_proc(void);
void di_input_read(void);

#endif

