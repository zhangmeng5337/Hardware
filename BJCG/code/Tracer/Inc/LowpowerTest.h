#include "main.h"
#ifndef LOWPOWER_H
#define LOWPOWER_H
void gpioLowpowerTest(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t GpioMode,uint32_t PullMode);
void gpioLowpowerIOSET(GPIO_TypeDef* GPIOx);

#endif 

