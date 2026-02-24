#ifndef MACHINE_CTRL_H_
#define MACHINE_CTRL_H_
#include "main.h"

#define MANUAL 0
#define AUTO  1
typedef struct 
{
	unsigned char fault;
	unsigned char mode;	
}dev_ctrl_stru;

void dev_ctrl(void);

#endif


