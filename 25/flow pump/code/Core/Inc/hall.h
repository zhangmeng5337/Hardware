#ifndef HALL_H_
#define HALL_H_
#include "main.h"
typedef struct
{
	unsigned char state;
	float current;
	unsigned int current_dac;
	unsigned char reconfig;
	
}hall_stru;

uint8_t get_hall_state(void);


#endif

