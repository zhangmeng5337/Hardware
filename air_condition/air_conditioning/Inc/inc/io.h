#ifndef IO_H_
#define IO_H_

#include "main.h"
#define FAN_HOT    0x01
#define FAN_COOL   0x02
#define PUMP       0x03
void device_ctrl(unsigned char devNo,unsigned char on_off);
GPIO_PinState get_io(unsigned char devNo);

#endif
