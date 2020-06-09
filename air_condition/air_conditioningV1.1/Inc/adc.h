#ifndef _ADC_H_
#define _ADC_H_
#include "main.h"
#define MAX_TEMPER  4000
#define MIN_TEMPER  0
#define TEMPERATURE 0x00
#define HUMIDTYPE   0x01
//typedef unsigned char u8;
//typedef unsigned int u16;

typedef struct{
	float adc_result[5];
	unsigned char io_status;
    unsigned int fault_status;
}adc_io_str;
void Get_Adc_Average(unsigned char times);
unsigned int adc_process(void);

#endif
