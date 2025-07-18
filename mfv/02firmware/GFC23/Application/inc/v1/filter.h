#ifndef FILTER_H_
#define FILTER_H_
#include "main.h"
#define N 50
#define N_2 500

#define window_size 300            //滑动窗口长度
#define PRESS_window_size 10            //滑动窗口长度

float filter(unsigned char adc_num,float *p);
float  GetMedianNum(float * bArray, int iFilterLen);
float sliding_average_filter(unsigned char adc_num,float value);
float average_filter_pressure(float bArray);
float average_filter(float bArray, unsigned char num);

#endif

