#ifndef __ADC_H
#define __ADC_H

#include "main.h"


void ADCx_Mode_Init(void);
uint8_t Get_Temperature(void);
float Get_Precent(void);
unsigned char adc_process(unsigned char flag);



#endif /* __ADC_H */

