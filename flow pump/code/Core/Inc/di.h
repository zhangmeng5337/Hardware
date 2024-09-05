#ifndef DI_H_
#define DI_H_
#include "main.h"
typedef struct
{
	unsigned char update;
	unsigned char status;
	uint32_t pulse_cnt;
	
}di_stru;

di_stru *get_di_status(void);
void di_init(void);
di_stru di_input_read(void);
void pulse_exti(void);
void do_ctrl(unsigned char relay_num);

#endif

