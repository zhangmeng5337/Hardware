#include "time.h"

/*����ʱ��*/
void SetTime(tsTimeType *TimeType,uint32_t TimeInter)
{
	TimeType->TimeStart = HAL_GetTick();
  TimeType->TimeInter = TimeInter;
}

/*�Ƚ�ʱ��*/
uint32_t CompareTime(tsTimeType *TimeType)
{
   uint32_t t1;
   t1 = HAL_GetTick()-TimeType->TimeStart;
   if(t1 >= TimeType->TimeInter)
    return 1;
   else 
   	return 0;
}
