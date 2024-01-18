#ifndef FILTER_H
#define FILTER_H

#include "main.h"

#define N 48  
#define AN 50

float filter(uint32_t seq);  
typedef struct{
  	float aN;
	float aF;
	float aG;
	float aH;
	float aI;
	float aB;
	float aC;
	float aD;
	float a0;
	float a1;
	float a2;
	unsigned char status;
	unsigned char calibrationFlag;
	unsigned int samplecount;
}ratio_stru;
ratio_stru *CalibrationRatio(float ahumid,float chumid);
ratio_stru *getRatio(void);
//uint32_t average_filter(uint32_t *pb) ;
float average_filter(float *pb,unsigned int count);  //��ֵ�˲�+��ֵ�˲�
float average_filterFloat(float *pb,unsigned int sampleCount);  //��ֵ�˲�+��ֵ�˲�;

#endif

