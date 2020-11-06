#ifndef SENSOR_H_
#define SENSOR_H_
#include "main.h"

void getRH_T(void);
void getvbat(void);

void getGPS(void);
void equip_set(void);//网关写命令后，对相关指令进行具体的执行
unsigned char getModeStatus(void);

#endif

