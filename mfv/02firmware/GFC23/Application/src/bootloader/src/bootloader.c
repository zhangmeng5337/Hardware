/**
  ******************************************************************************
  * @file    bootloader.c 
  * @author  BrainQube LLC
  * @version see .h
  * @date    
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "nvmem.h"
#include "bootloader.h"
#include "protocol.h"
#include "crc16.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
    @brief  Image CRC calculator for validating the image before booting
*/
uint8_t validate_app_image(void)
{
    uint16_t header_crc, application_crc;
    app_header application;
    
    // get the application header
    memcpy((uint8_t*)&application, (uint8_t*)FLASH_APP_HEADER_ADDR, sizeof(app_header));
    
    // step 1: verify the application header crc
    header_crc = crc16_ccitt(0, (uint8_t*)&application, sizeof(app_header) - sizeof(application.header_crc16_ccitt));
    if (header_crc != application.header_crc16_ccitt) {
        return 0; // failed to validate image
    }
    
    // step 2: verify that the magic number exists
    if (application.magicnumber != FLASH_APP_MAGICNUMBER) {
        return 0; // failed to validate image
    }
    
    // step 3: verify app image crc
    
    application_crc = crc16_ccitt(0, (uint8_t*)FLASH_APP_START_ADDR, application.app_size);
    if (application_crc != application.app_crc16_ccitt) 
		{
			return 0; // failed to validate image
    }
 
    // final step: 
    /* Test if user code is programmed starting from address "APPLICATION_ADDRESS" */
    /* i.e. does the stack pointer address look like a stack pointer address?  */
    if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000) {
        return 1; // image is valid and read to boot
    } else {
        return 0; // failed to validate image
    }
}

/**
    @brief This function updates the application header block after the firmware gets downloaded
*/
uint8_t update_app_image(void)
{
	uint64_t Data = 0x0000000000000000;
	uint64_t *DataRam;
	app_header application;
	static FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
  uint32_t Address = 0, PageError = 0;
  //uint32_t src_addr;
  uint32_t errcnt=0;
  
	/* Unlock the Flash to enable the flash control register access *************/
	HAL_FLASH_Unlock();
   /* Clear OPTVERR bit set on initial samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGSERR);
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGAERR);
  /* Get the 1st page to erase */
  FirstPage = GetPage(FLASH_APP_HEADER_ADDR);
  /* Get the number of pages to erase from 1st page */
  NbOfPages = GetPage(FLASH_APP_HEADER_ENDADDR) - FirstPage + 1;       
  /* Get the bank */
  BankNumber = GetBank(FLASH_APP_HEADER_ADDR);
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks       = BankNumber;
  EraseInitStruct.Page        = FirstPage;
  EraseInitStruct.NbPages     = NbOfPages;
  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK) {
		; //handle error here
    errcnt++;
	}
  
	// update basic application information
	application.magicnumber = FLASH_APP_MAGICNUMBER;
	application.app_size = FLASH_APP_END_ADDR + 1 - FLASH_APP_START_ADDR;
    
	// calculate application CRC
	application.app_crc16_ccitt = crc16_ccitt(0, (uint8_t*)FLASH_APP_START_ADDR, application.app_size);
    
	// calculate header CRC
	application.header_crc16_ccitt = crc16_ccitt(0, (uint8_t*)&application, sizeof(app_header) - sizeof(application.header_crc16_ccitt));
    
	Address = FLASH_APP_HEADER_ADDR; // set the start address
	DataRam = (uint64_t*)&application;
 
	// loop to the entire header size
	while (Address < (FLASH_APP_HEADER_ADDR + sizeof(app_header))) {
    Data = (uint64_t)*DataRam;
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGSERR);
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGAERR);
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, Data) == HAL_OK)
    {
      Address += 8;
      DataRam++;
    }
    else
    { 
      /* Error occurred while writing data in Flash memory. 
      User can add here some code to deal with this error */
      return 0; // bad programming
    }
	}
	HAL_FLASH_Lock(); 
	return 1; // all good
}

/**
	@brief Jump to application fimrware
*/
void runApplicationFirmware(void) {
  pFunction Jump_To_Application;
  uint32_t JumpAddress;

  SCB->VTOR = FLASH_APP_START_ADDR;

	// disable all interrupts
	for(int i=0; i<256;i++) {
		HAL_NVIC_DisableIRQ((IRQn_Type)i);
	}
	
	/* Jump to user application */
	JumpAddress = *(__IO uint32_t*) (FLASH_APP_START_ADDR + 4);
	Jump_To_Application = (pFunction) JumpAddress;  

	/* Initialize user application's Stack Pointer */
	__set_MSP(*(__IO uint32_t*) FLASH_APP_START_ADDR);

	__set_CONTROL(0);		
	
	/* Jump to application */
	Jump_To_Application();	

}

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
