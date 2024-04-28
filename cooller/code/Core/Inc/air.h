#ifndef AIR_H
#define AIR_H
#include "main.h"

#define PID_OUT_MAX   45000
#define PID_OUT_MIN		8000
#define FREQ_RATIO		9000.0/(PID_OUT_MAX-PID_OUT_MIN)
#define FREQ_B		(1000-FREQ_RATIO*8000.0)

typedef struct
{
	
	uint32_t    freq;
	float    pid_out;
	float    heat_pid_out;

    unsigned char update;
}air_stru;





#endif

