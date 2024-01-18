#ifndef IO_H_
#define IO_H_

#include "main.h"
#define FAN_HOT    0x01
#define FAN_COOL   0x02
#define PUMP       0x03
#define HUMID      0x04
#define GATE       0x05

#define DEV_ALL    0x06
void device_ctrl(unsigned char devNo,unsigned char on_off);
void get_io(void);

#endif
