/* --COPYRIGHT--,BSD
 * Copyright (c) 2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

#ifndef UNIT_TESTS_H_
#define UNIT_TESTS_H_

// Includes
#include <stdbool.h>
#include <stdint.h>
#include "ads1258.h"
#include "hal.h"

#define ADC1_PT_SIZE    0//4
#define ADC1_PT_INDEX   0

#define ADC2_PT_SIZE    6
#define ADC2_PT_INDEX   0

#define ADC1_PR_INDEX   11
#define PR_RATIO		0.08

#define MAX_TEMP        100
#define MIN_TEMP        -60

#define MAX_PRESS      1
#define MIN_PRESS      0

#define GOOD		0
#define BAD			1


#define AI_WATER_T_O_INDEX   	0
#define AI_WATER_T_IN_INDEX   	1

#define AI_PUMP_F_INDEX   		0
#define AI_PUMP_E_INDEX   		1


#define PRESS_RATIO        641     //0.393 --- 0   1.997----1

#define PRESS_B            -250
#define PT100_PR          75

#define MAX_FAILUE 100 
typedef struct 
{
	float data_ai[CHANNEL_SIZE];
	float ref1;
	float ref2;
    float press[8];	//pressure
    float temp[12]; //temperature
    float resis[21];
	float data2_ai[CHANNEL_SIZE];
	unsigned int failure_count[32];
	unsigned int last_gather[32];	
	uint32_t channel_status; //fault status
	unsigned char update;
	
}data_ai_stru;


//
// Function prototypes
//


void ads1158_init(void);
void ads1158_config(unsigned char adc_No);
unsigned char adc_read_data(void);
void ai_proc(void);
data_ai_stru *get_ai_data(void);

#endif /* ADS1258_UNIT_TESTS_H_ */
