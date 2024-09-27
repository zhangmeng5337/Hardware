#ifndef KEY_H_
#define KEY_H_
#include "main.h"
#define KEY_DELAY_TIME 10

typedef struct
{
	unsigned char encode_Direction;
	unsigned int encode_cnt;
	unsigned char encode_update;//1:正转 2：反转
	unsigned char press_update;//按键按下
 	unsigned char ok_press_update;//确认按键按下 1：开关按下
     
	uint32_t  press_delay_time;
	unsigned char key_press_flag;
	unsigned char key1;
	unsigned char key2;
	unsigned char key3;
	
}key_stru;

void key_scan(void);
void key_tick(void);
key_stru *get_key_state(void);


#endif

