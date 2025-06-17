#ifndef KEY_H_
#define KEY_H_
#include "main.h"
#define KEY_DELAY_TIME 10

typedef struct
{
	unsigned char encode_Direction;//0:forward
	unsigned char encode_last_Direction;//0:forward	
	int32_t detal_cnt;
	int32_t encode_nodivcnt;
	int32_t encode_cnt;	
	int32_t overnumber;
	uint32_t encode_couter;
	unsigned char encode_update;//1:��ת 2����ת
	unsigned char press_update;//��������
 	unsigned char ok_press_update;//ȷ�ϰ������� 1�����ذ���
     
	uint32_t  press_delay_time;
	unsigned char key_press_flag;
	unsigned char key1;
	unsigned char key2;
	unsigned char key3;
	int32_t lastCount;
	int32_t totalCount;
	
}key_stru;

void key_scan(void);
void key_tick(void);
key_stru *get_key_state(void);
void key_init(void);


#endif

