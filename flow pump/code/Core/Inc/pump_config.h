#ifndef PUMP_CONFIG_H
#define PUMP_CONFIG_H
#include "main.h"
typedef struct
{
	unsigned char mode;
	float set_flow;
	float sum_flow;//�����ݻ�
	uint32_t add_time[2];//h:mm
	unsigned char unit;//ml L
	uint32_t add_time[16][14];//��ţ��ݻ���unit,mm,ss,hh,mm,1-7
	uint32_t warn[10][6];
	uin32_t time[6];
}
#endif

