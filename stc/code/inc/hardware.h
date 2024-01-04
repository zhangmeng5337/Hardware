#ifndef HARDWARE_H_
#define HARDWARE_H_
#include "stc8h1k.h"


#define FALSE 0
#define TRUE  1

#define CD4052_A0   	P10
#define CD4052_A1   	P37

#define TM7780_SET   	P34
#define TM7780_CFA   	P35
#define TM7780_CF    	P36


#define CURRENT_M		0
#define VOL_M		    1


void mcu_init();
void tm7780_measure_ctrl(unsigned char ctrl);
void cd4052_driver(unsigned char channel);
void irq_init(void);

#endif

