#ifndef FILTER_H_
#define FILTER_H_
#include "main.h"
#define N 250
#define N_2 50

#define window_size 300            //�������ڳ���
#define PRESS_window_size 10            //�������ڳ���

float filter(unsigned char adc_num,float *p);
float  GetMedianNum(float * bArray, int iFilterLen);
float sliding_average_filter(unsigned char adc_num,float value);
float average_filter(float bArray);
float average_filter_pressure(float bArray);

#endif

