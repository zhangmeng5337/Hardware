#ifndef APP_H
#define APP_H

#include "main.h"
typedef struct{
	uint32_t period;
}systemParams_stru;
void HardwareInit(void);
void app_main(void);

#endif

