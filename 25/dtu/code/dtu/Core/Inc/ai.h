#ifndef AI_H_
#define AI_H_
#include "main.h"
#define AI_SIZE   8
#define ADC_VREF 3.0
#define VOL_RATIO 0.2
typedef struct
{
	//unsigned do_status;
	unsigned char press[3];
	//unsigned char press[3];
	unsigned char ai_curr[AI_SIZE];
	
} stru_AI_stru;
typedef struct
{

	uint32_t adc[AI_SIZE];
	float ai_buf[AI_SIZE];
} stru_adc_stru;

//void do_ctrl_proc(unsigned int do_NO_sel,unsigned char bit_set);

#endif

