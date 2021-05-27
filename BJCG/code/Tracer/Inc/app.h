#ifndef APP_H
#define APP_H

#include "main.h"

typedef struct{
	uint32_t NightPeriod;
	uint32_t DayPeriod;
	uint32_t period;
	unsigned char Dhours;
	unsigned char Dminutes;
	unsigned char Nhours;
	unsigned char Nminutes;

}systemParams_stru;
void HardwareInit(void);
void app_main(void);
void ParamsInit(void);
void test(void);
void systmeReconfig(void);

#endif

