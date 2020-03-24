#ifndef _ADC_H_
#define _ADC_H_
#include "main.h"
#define MAX_TEMPER  30
#define MIN_TEMPER  0
typedef struct{
	uint32_t adc_result[5];
	unsigned char io_status;
    unsigned char fault_status;
}adc_io_str;
void Get_Adc_Average(unsigned char times);
unsigned int adc_process(void);

#endif
