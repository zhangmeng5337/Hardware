#ifndef FILTER_H_
#define FILTER_H_
#include "main.h"
#define N 10
#define N2 100

#define window_size 100            //�������ڳ���
#define PRESS_window_size 10            //�������ڳ���

float filter(unsigned char adc_num,float *p);
float  GetMedianNum(float * bArray, int iFilterLen);
float average_filter(float bArray);  

#endif


