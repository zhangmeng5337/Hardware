#ifndef APP_H
#define APP_H

#include "main.h"
#define VBAT_MAX_VALU    43
#define VBAT_MIN_VALU    32

typedef struct{
	uint32_t NightPeriod;
	uint32_t DayPeriod;
	uint32_t period;
	unsigned char Dhours;
	unsigned char Dminutes;
	unsigned char Nhours;
	unsigned char Nminutes;
	unsigned char vbat;
	unsigned char vbatT;

}systemParams_stru;
void HardwareInit(void);
void app_main(void);
void ParamsInit(void);
void test(void);
void systmeReconfig(void);

#endif

