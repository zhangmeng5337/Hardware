#ifndef _DELAY_H_
#define _DELAY_H_

//#include<stdint.h>
#include "stm8l15x.h"
extern volatile  uint32_t TickCounter;
extern volatile  uint32_t ticktimer;

void Delay_Us (uint32_t delay);
void DelayMs(uint32_t x);
void HAL_Delay_nMS(uint32_t Delay );

#define HAL_GetTick()  TickCounter

#endif

