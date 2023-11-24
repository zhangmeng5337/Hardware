#ifndef _TIME_H
#define _TIME_H

#include "main.h"

typedef struct
{
	uint32_t TimeStart;//开始时�?
	uint32_t TimeInter;//间隔时间
}tsTimeType;

void SetTime(tsTimeType *TimeType,uint32_t TimeInter);
uint32_t CompareTime(tsTimeType *TimeType);

#endif
