#ifndef SENSOR_H_
#define SENSOR_H_
#include "main.h"

void getRH_T(void);
void getvbat(void);

void getGPS(void);
void equip_set(void);//����д����󣬶����ָ����о����ִ��
unsigned char getModeStatus(void);
void  actuator(unsigned int p,unsigned char len);

#endif

