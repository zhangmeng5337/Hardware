
#ifndef uSYSTEM_H
#define uSYSTEM_H
#include "stdint.h"
void TimingStop(unsigned char seq);
unsigned char TimingStart(unsigned char select,uint32_t tCount1,uint32_t tCount2,uint32_t tCount3);
unsigned char get_tick_end(unsigned char num);
void set_tick_end(unsigned char num);
#endif

