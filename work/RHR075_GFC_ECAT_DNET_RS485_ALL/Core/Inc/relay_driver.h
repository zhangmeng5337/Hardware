#ifndef RELAY_DRIVER_H
#define RELAY_DRIVER_H
#include "main.h"
#define ON  0
#define OFF 1

#define ROOT_UP_RELAY           0
#define HIGHT_FLOW_UP_RELAY     1
#define LOW_FLOW_UP_RELAY       2
#define DOWN_RELAY              3

void relay_init(void);
void relay_ctrl(unsigned char relay_num, unsigned char status);


#endif

