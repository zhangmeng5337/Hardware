/**************************************************************************//**
 * @file     FlashPrg.c
 * @brief    Flash Programming Functions adapted for New Device Flash
 * @version  V1.0.0
 * @date     10. January 2018
 ******************************************************************************/
/*
 * Copyright (c) 2010-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "FlashOS.H"        // FlashOS Structures
#include "fm25w32.h"
#include "spi.h"
#include "gpio.h"
extern SPI_HandleTypeDef hspi1;
/* 
   Mandatory Flash Programming Functions (Called by FlashOS):
                int Init        (unsigned long adr,   // Initialize Flash
                                 unsigned long clk,
                                 unsigned long fnc);
                int UnInit      (unsigned long fnc);  // De-initialize Flash
                int EraseSector (unsigned long adr);  // Erase Sector Function
                int ProgramPage (unsigned long adr,   // Program Page Function
                                 unsigned long sz,
                                 unsigned char *buf);

   Optional  Flash Programming Functions (Called by FlashOS):
                int BlankCheck  (unsigned long adr,   // Blank Check
                                 unsigned long sz,
                                 unsigned char pat);
                int EraseChip   (void);               // Erase complete Device
      unsigned long Verify      (unsigned long adr,   // Verify Function
                                 unsigned long sz,
                                 unsigned char *buf);

       - BlanckCheck  is necessary if Flash space is not mapped into CPU memory space
       - Verify       is necessary if Flash space is not mapped into CPU memory space
       - if EraseChip is not provided than EraseSector for all sectors is called
*/


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */
extern void SystemClock_Config(void);
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

  /* Add your Code */
	int ret = 0;
	volatile int i;
	volatile unsigned char * ptr = (volatile unsigned char * )&hspi1;

	for (i = 0; i < sizeof(hspi1); i++) 
	{
		*ptr++ = 0U;
	}

	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_SPI1_Init();
//	MX_USART1_UART_Init();
	  
	ret= FM25W32_Init();

  return (0);                                  // Finished without Errors
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {

  /* Add your Code */
  return (0);                                  // Finished without Errors
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void) {

  /* Add your Code */
	FM25W32_EraseChip();
  return (0);                                  // Finished without Errors
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {

  /* Add your Code */
	FM25W32_EraseSector(adr);
  return (0);                                  // Finished without Errors
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {

  /* Add your Code */
	FM25W32_WritePage(buf, adr, sz);
  return (0);                                  // Finished without Errors
}

#define bufsize 4096

uint8_t aux_buf[bufsize];

unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf) 
{
	volatile int i=0, j=0;

	for(i = 0; i < sz/bufsize; i++)
	{
		FM25W32_Read(aux_buf, adr+bufsize*i, bufsize);
		for (j = 0; j< bufsize; j++) 
		{
			if (aux_buf[j] != buf[j+bufsize*i]) 
			return (adr + j + bufsize*i);                  
		}	
	}
	
	if(sz%bufsize)
	{
		//nst_flash_read(aux_buf,adr+bufsize*i,sz%bufsize);
		FM25W32_Read(aux_buf, adr+bufsize*i, bufsize);
		for (j = 0; j< sz%bufsize; j++) 
		{
			if (aux_buf[j] != buf[j+bufsize*i]) 
			return (adr + j + bufsize*i);                   
		}			
	}

    return (adr+sz);                     
}

int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat) 
{
	volatile int i=0, j=0;

	for(i = 0; i < sz/bufsize; i++)
	{
		//nst_flash_read(aux_buf, adr+bufsize*i, bufsize);
		FM25W32_Read(aux_buf, adr+bufsize*i, bufsize);
		for (j = 0; j< bufsize; j++) 
		{
			if (aux_buf[j] != pat) 
			return 1;                
		}	
	}
	
	if(sz%bufsize)
	{
		//nst_flash_read(aux_buf, adr+bufsize*i, sz%bufsize);
		FM25W32_Read(aux_buf, adr+bufsize*i, bufsize);
		for (j = 0; j< sz%bufsize; j++) 
		{
			if (aux_buf[j] != pat) 
			return 1;                
		}			
	}

    return 0;
}

