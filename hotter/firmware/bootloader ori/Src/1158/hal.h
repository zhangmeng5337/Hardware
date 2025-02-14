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

#ifndef HAL_H_
#define HAL_H_


//****************************************************************************
//
// Standard libraries
//
//****************************************************************************
#include <stdbool.h>
#include <stdint.h>
#include "main.h"

#define AI_SIZE   16

typedef  struct
{
	uint8_t data[4];
	unsigned char  chip_No;
	unsigned char ads1_update;
	unsigned char ads2_update;	
	unsigned char ads1_start;
	unsigned char ads2_start;
	uint32_t value;
	uint32_t adsvolt[AI_SIZE];
	uint32_t adsvolt2[AI_SIZE];
	float    ads_ma[AI_SIZE];	
	//uint32_t adsvolt2[AI_SIZE];
	uint32_t ADIref;
	uint32_t ADIgain;
	uint32_t ADItemp;
	uint32_t ADIvcc;
	uint32_t ADIoffset;
	uint8_t channel;
	float ft;
	float vcc;
	float gain;
	float offset;
	float temp;

    float wendu;	
}structChan;
 
//****************************************************************************
//
// Insert processor specific header file(s) here
//
//****************************************************************************

/*  --- TODO: INSERT YOUR CODE HERE --- */
/** Alias used for setting GPIOs pins to the logic "high" state */
#define HIGH                1

/** Alias used for setting GPIOs pins to the logic "low" state */
#define LOW                 0

#define ADS1158_CS1_LOW      HAL_GPIO_WritePin(MIBSPI1MCS1_GPIO_Port, MIBSPI1MCS1_Pin,GPIO_PIN_RESET);
#define ADS1158_CS1_HIGH	 HAL_GPIO_WritePin(MIBSPI1MCS1_GPIO_Port, MIBSPI1MCS1_Pin,GPIO_PIN_SET);

#define ADS1158_CS2_LOW      HAL_GPIO_WritePin(MIBSPI1MCS2_GPIO_Port, MIBSPI1MCS2_Pin,GPIO_PIN_RESET);
#define ADS1158_CS2_HIGH	 HAL_GPIO_WritePin(MIBSPI1MCS2_GPIO_Port, MIBSPI1MCS2_Pin,GPIO_PIN_SET);

#define START_H          HAL_GPIO_WritePin(MSTART_MCU_GPIO_Port,MSTART_MCU_Pin,GPIO_PIN_RESET);
#define START_L          HAL_GPIO_WritePin(MSTART_MCU_GPIO_Port,MSTART_MCU_Pin,GPIO_PIN_SET);


//*****************************************************************************
//
// Pin definitions (MSP432E401Y)
//
//*****************************************************************************

/*#define START_PORT          (GPIO_PORTQ_BASE)
#define START_PIN           (GPIO_PIN_1)

#define nDRDY_PORT          (GPIO_PORTP_BASE)
#define nDRDY_PIN           (GPIO_PIN_5)
#define nDRDY_INT           (INT_GPIOP5)

#define nCS_PORT            (GPIO_PORTM_BASE)
#define nCS_PIN             (GPIO_PIN_0)

#define nRESET_PORT         (GPIO_PORTM_BASE)
#define nRESET_PIN          (GPIO_PIN_6)

#define nPWDN_PORT          (GPIO_PORTP_BASE)
#define nPWDN_PIN           (GPIO_PIN_3)


#define CLKSEL_PORT         (GPIO_PORTD_BASE)
#define CLKSEL_PIN          (GPIO_PIN_4)
*/

//*****************************************************************************
//
// Function Prototypes
//
//*****************************************************************************

void    InitADCPeripherals(void);
void    delay_ms(uint32_t delay_time_ms);
void    delay_ns(uint32_t delay_time_us);
void setCS(unsigned char adc_No,bool state);
void    setSTART(bool state);
void    setPWDN(bool state);
void    toggleRESET(void);
void spiSendReceiveArrays(unsigned char adc_No,uint8_t DataTx[], uint8_t DataRx[], uint8_t byteLength);
uint8_t spiSendReceiveByte(uint8_t dataTx);
unsigned char waitForDRDYinterrupt(uint32_t ads_No,uint32_t timeout);

// Functions used for testing only
GPIO_PinState getCS(void);
//bool    getPWDN(void);
//bool    getRESET(void);
GPIO_PinState getSTART(void);
//void    setRESET(bool state);

void ads_data_update(unsigned char flag);
void ads1158_start(void);
void clear_update_flag(unsigned char chipNO,unsigned char bitset);
structChan *getAds_status(void);

#endif /* HAL_H_ */
