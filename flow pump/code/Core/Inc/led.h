#ifndef LED_H_
#define LED_H_
#include "main.h"
#define LED_RUN   1
#define LED_WARN  2
#define LED_FAULT 3
#define LED_ALL   4


typedef struct
{
	unsigned char update;
	unsigned char status;
	uint32_t pulse_cnt;
	
}led_stru;


void led_ctrl(unsigned char led_num, unsigned char ctrl);
void led_init(void);

#endif

