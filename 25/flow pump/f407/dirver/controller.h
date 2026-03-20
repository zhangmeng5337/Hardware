#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "devinfo.h"

typedef struct
{

 	unsigned int mode ;////0:manual; 1:pulse 2:0-20ma 3:4-20ma 4:pulsectrl 5:cycBycycle 6:weekly
                       //7:manufacture  8:custom
	float fraction;//1 circule --->flow
	float setFlow;
	float realFlow;
	float dispFlow;
	
	counter_stru counterU;
	unsigned char maintaince;
	setting_stru settingU;
	uint32_t serPack;
	unsigned char serRestRecord;
	uint32_t serial;
	uint32_t prodNo;
	
}controller_stru;
void controlInit(void);
#endif 

