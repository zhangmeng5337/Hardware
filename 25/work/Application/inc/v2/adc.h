#ifndef __ADC_H_
#define __ADC_H_

#include <stdbool.h>
#include "main.h"

#define PT_VREF 2.487
#define R2_V 2.0

#define R3_V 0.05

//#define V2_V  0.1188   // 0.1k
#define V2_V  0.0895 // 0.075k

#define R1_V 10.0
#define RF_V 100

#define V1_RATIO 10.0

#define V3_V  V1_RATIO*(RF_V + 1)

#define SQUARE_RATIO  0.3161
#define ONE_RATIO     23.842
#define COMS_RATIO    74.89

#define VOL_FACTOR  1.210

#define MAX_TEMP_VOL  46.985
#define MIN_TEMP_VOL  -2.208
#define MAX_TEMP  1100
#define MIN_TEMP  -50
#define ADC_REF  3

#define ROM_NUM		1220
#define T_ERROR		10

#define PT_SENSOR_TYPE

typedef struct {
  bool update;
  uint16_t adc_value[5];
  float T_value[4];
  float v_value[4];
  float R_value[4];
  float V2_V_offset; 
  float average_T;
  float average_P1_T;
  float T_offset;
  float ref;
} adc_struct;

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern adc_struct adcG4;

extern void init_SDADC(void);
extern void start_SDADC(void);
extern void adc_proc(void);

#endif
