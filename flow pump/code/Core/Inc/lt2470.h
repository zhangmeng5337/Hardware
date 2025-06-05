#ifndef LT2470_H_
#define LT2470_H_
#include "main.h"
#define EN1				0x80
#define EN2				0x00
#define SPD_RATE1		0x00   //0:208sps  1:833sps
#define SPD_RATE2		0x10   //0:208sps  1:833sps

#define NAP_MODE		0x00
#define SLEEP_MODE		0x10

#define VREF 1.25
#define VR 49.9

typedef struct
{

	float adc_value;
	float current;
}ai_stru;

unsigned char ai_read(void);



#endif

