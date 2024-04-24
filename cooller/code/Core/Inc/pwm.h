#ifndef PWM_H
#define PWM_H
#include "main.h"
#define COOLER_1		1
#define COOLER_2		2
#define COOLER_3		3
#define COOLER_4		4

#define HEATER_1		5
#define HEATER_2		6
#define HEATER_3		7
#define HEATER_4		8

#define AIR_FREQ		9


typedef struct
{
  uint32_t air_freq;
  uint32_t air_start_tick;
	uint32_t air_counter;

}pwm_stru;


#endif

