/**
    @filename   nvmem.c
    @brief      contains functions and drivers for handling non-volatile memory storage. Includes support for data flash redundancy
*/
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "main.h"
#include "nvmem.h"
#include "crc16.h"
#include "utilities.h"

#define STATUS_MEM_MAIN_BAD         0x01
#define STATUS_MEM_REDUNDANT_BAD    0x02

uint8_t Status_Mem = 0;
bool flash_it = false;
bool flash_busy = false;
uint8_t mem_loaded = 0;
uint16_t flashwritetimeout = 0;
uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
uint32_t Address = 0, PageError = 0;
uint32_t src_addr;
nvmem_state nvmem_state_machine = NVMEM_INIT;
nvmem_struct nvmem;

rtb_struct rtb;
uint32_t rtbAddress = 0;
uint8_t rtb_wr = 0;

/**
  @brief PVD interrupt occurs when VDD is below the PVD threshold. Stop all flash access it we get an interrupt to prevent
         Flash corruption
*/
void HAL_PWR_PVDCallback(void)
{
  HAL_FLASH_Lock(); // lock flash access
  nvmem_state_machine = NVMEM_IDLE; // put nvmem statmachine to IDLE so no further writes are initiated. This lock should happen before BOR(1.7V) occurs
}

/**
  @brief 
*/
void HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue) {
  flash_it = true;
}

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
uint32_t GetPage(uint32_t Addr)
{
  uint32_t page = 0;

  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }

  return page;
}

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
uint32_t GetBank(uint32_t Addr)
{
  uint32_t bank = 0;

  if (READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE) == 0)
  {
    /* No Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_1;
    }
    else
    {
      bank = FLASH_BANK_2;
    }
  }
  else
  {
    /* Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_2;
    }
    else
    {
      bank = FLASH_BANK_1;
    }
  }

  return bank;
}

/**
    @brief  retruns 1 if flash is busy, 0 if it is ready for a flash transaction.
*/
uint8_t is_flash_busy(void) {
    if (nvmem_state_machine <= NVMEM_IDLE) {
        return 0;
    } else {
        return 1;
    }
}

/**
    @brief start counting down before we write to flash. Timeout is in milliseconds
*/
void write_to_flash(uint16_t timeout) {
#ifndef _INCLUDE_CUNIT_TEST_    
    if (nvmem_state_machine <= NVMEM_WAITING) 
    { // make sure that there are no write in progress
      nvmem_state_machine = NVMEM_WAITING;
      flashwritetimeout = timeout/FLASH_THREAD_PERIOD;
    }
    
#endif    
}

/**
    @brief load default non-volatile parameters if flash blank
*/
void load_default_values(nvmem_struct *nv) {
    
  memset(nv, 0, sizeof(nvmem_struct));
  
  nv->magic_number = FLASH_MAGICNUMBER;
  strncpy((char *)nv->app.manufacturing.partnumber, "PNx", sizeof(nv->app.manufacturing.partnumber));
  strncpy((char *)nv->app.manufacturing.sensorserialnumber, "SN__", sizeof(nv->app.manufacturing.sensorserialnumber));
  strncpy((char *)nv->app.manufacturing.deviceserialnumber, "DV__", sizeof(nv->app.manufacturing.deviceserialnumber));
  strncpy((char *)nv->app.manufacturing.electronicsrev, "00", sizeof(nv->app.manufacturing.electronicsrev));
  
  nv->app.network.defaultbaudrate = 2;  // 19200
  nv->app.network.length = 1;
  nv->app.network.parity = 0;
  nv->app.network.stopbits = 1;
  nv->app.network.address = 1;
  nv->app.network.extproc = 0;
#ifndef BOOTLOADER    
  // DeviceNet objects
  nv->app.network.producer = 2;
  nv->app.network.consumer = 7;
  nv->app.manufacturing.usVendorId = 41;
  nv->app.manufacturing.usDeviceType = 39;  // EMFC
  nv->app.manufacturing.usProductCode = 34;
  memcpy(nv->app.manufacturing.abProductName, "GFV", 3);
  // Supervisor Object
  memcpy(nv->app.supObj.devtype, "EMFC", 4);
  memcpy(nv->app.supObj.semirev, "E54-0997", 8);
  memcpy(nv->app.supObj.manufacturer, "OPM", 3);
  memcpy(nv->app.supObj.model, "P21", 3);
  sprintf((char*)nv->app.supObj.fwrev, "%02d.%02d.%02d", FW_REVISION_MAJOR, FW_REVISION_MINOR, FW_REVISION_PATCH);
  memcpy(nv->app.supObj.hwrev, "001", 3);    
  memcpy(nv->app.supObj.sn, "210001", 6);  
  memcpy(nv->app.supObj.config, "P21-xxxx-xxxx", 13);  
  // SAS
  for (uint8_t i=0; i<3; i++) {
    nv->app.sasObj[i].dType = DNS_DATATYPE_INT;       // Int
    nv->app.sasObj[i].dUnits = DNS_DATAUNITS_COUNTS;    // counts
    nv->app.sasObj[i].fscounts = 0x6000;
    nv->app.sasObj[i].fsrange = 0;
    nv->app.sasObj[i].warn.eflag = 0;
    nv->app.sasObj[i].warn.tph = 1.0f;
    nv->app.sasObj[i].warn.tpl = 0.0f;
    nv->app.sasObj[i].warn.hyst = 0.01f;
    nv->app.sasObj[i].warn.SetTime = 100;    
    nv->app.sasObj[i].alarm.eflag = 0;
    nv->app.sasObj[i].alarm.tph = 1.0f;
    nv->app.sasObj[i].alarm.tpl = 0.0f;
    nv->app.sasObj[i].alarm.hyst = 0.01f;
    nv->app.sasObj[i].alarm.SetTime = 100;
  }
  nv->app.sasObj[0].fsrange = 100.0f; // 100 sccm
  nv->app.sasObj[1].fsrange = 100.0f; // 100 psi
  nv->app.sasObj[2].fsrange = 500.0f; // 500 K
  // SAA
  nv->app.saaObj.dType = DNS_DATATYPE_INT;            // Int
  nv->app.saaObj.dUnits = DNS_DATAUNITS_COUNTS;         //counts
  nv->app.saaObj.warn.eflag = 0;
  nv->app.saaObj.warn.tph = 1.0f;
  nv->app.saaObj.warn.tpl = 0.0f;
  nv->app.saaObj.warn.hyst = 0.01f;
  nv->app.saaObj.alarm.eflag = 0;
  nv->app.saaObj.alarm.tph = 1.0f;
  nv->app.saaObj.alarm.tpl = 0.0f;
  nv->app.saaObj.alarm.hyst = 0.01f;
  // SCC
  nv->app.sccObj.dType = DNS_DATATYPE_INT;            // Int
  nv->app.sccObj.dUnits = DNS_DATAUNITS_COUNTS;         // counts
  nv->app.sccObj.cMode = 0;
  nv->app.sccObj.warn.eflag = 0;
  nv->app.sccObj.warn.errBand = 0.01f;
  nv->app.sccObj.warn.SetTime = 100;
  nv->app.sccObj.alarm.eflag = 0;
  nv->app.sccObj.alarm.errBand = 0.01f;
  nv->app.sccObj.alarm.SetTime = 100;

  // SGC   
  nv->app.sgcObj[0].calGasNumber = 13;
  nv->app.sgcObj[0].sasInst = 1;
  memcpy(nv->app.sgcObj[0].gasSymbol, "N2", 2);
  nv->app.sgcObj[0].sFullScale.dUnits = DNS_DATAUNITS_SCCM;
  nv->app.sgcObj[0].sFullScale.fsrange = 100.0f;
  nv->app.sgcObj[0].caldate = 0;
  nv->app.sgcObj[0].calGasNumber = 13;
  nv->app.sgcObj[0].gasNumber = 13;
  
#endif

  sprintf((char *)nvmem.app.manufacturing.cfwversion, "%02d.%02d.%02d", FW_REVISION_MAJOR, FW_REVISION_MINOR, FW_REVISION_PATCH);
  write_to_flash(FLASH_TIMEOUT);
}

/**
*/
void interrogateFLT(float *data, float lowlim, float hilim, float defval) {
  if (isnan(*data) == 1) {
    *data = defval;
  } else if (isfinite(*data) == 0) {
    *data = defval;
  } else {
    *data = ((*data>hilim)||(*data<lowlim)) ? defval : *data;
  }
}

/**
*/
void interrogateU8(uint8_t *data, uint8_t lowlim, uint8_t hilim, uint8_t defval) {
  *data = ((*data>hilim)||(*data<lowlim)) ? defval : *data;
}

/**
*/
void interrogateU16(uint16_t *data, uint16_t lowlim, uint16_t hilim, uint16_t defval) {
  *data = ((*data>hilim)||(*data<lowlim)) ? defval : *data;
}

/**
*/
void interrogateU32(uint32_t *data, uint32_t lowlim, uint32_t hilim, uint32_t defval) {
  *data = ((*data>hilim)||(*data<lowlim)) ? defval : *data;
}

/**
*/
void post_fwupgrade(void) {
  char currentver[9] = "XX.YY.ZZ";
  char upperverlim[9] = "99.99.99";
  char lowerverlim[9] = "02.00.00";
  uint8_t upgd = 1;  // assume that we are upgrading
  
  if ((strcmp((const char *)nvmem.app.manufacturing.cfwversion, upperverlim) > 0) ||
      (strcmp((const char *)nvmem.app.manufacturing.cfwversion, lowerverlim) < 0)) {  
    
    sprintf((char *)nvmem.app.manufacturing.cfwversion, "%02d.%02d.%02d", FW_REVISION_MAJOR, FW_REVISION_MINOR, FW_REVISION_PATCH);
    write_to_flash(FLASH_TIMEOUT);
  } else {
    // get current firmware version from compiled code
    sprintf(currentver, "%02d.%02d.%02d", FW_REVISION_MAJOR, FW_REVISION_MINOR, FW_REVISION_PATCH);
    
    if (strcmp((const char *)nvmem.app.manufacturing.cfwversion, currentver) < 0) {
      upgd = 1; // we are upgrading
    } else   if (strcmp((const char *)nvmem.app.manufacturing.cfwversion, currentver) > 0) {
      upgd = 0; // we are downgrading
    } else {
      upgd = 3; // we are neither upgrading nor downgrading, do nothing
    }
    
    switch (upgd) {
      case 0:
        sprintf((char *)nvmem.app.manufacturing.cfwversion, "%02d.%02d.%02d", FW_REVISION_MAJOR, FW_REVISION_MINOR, FW_REVISION_PATCH);
        write_to_flash(FLASH_TIMEOUT);
        break;
      case 1:
//        if (strcmp((const char *)nvmem.app.cfwversion, "02.21.15") < 0) {
//          // enter flash inits here that is/are new to this firmware revision level
//          sprintf((char *)nvmem.app.cfwversion, "%02d.%02d.%02d", 2, 21, 15);
//        }        
//        if (strcmp((const char *)nvmem.app.cfwversion, "02.21.16") < 0) {
//          // enter flash inits here that is/are new to this firmware revision level
//          sprintf((char *)nvmem.app.cfwversion, "%02d.%02d.%02d", 2, 21, 16);
//        } 
        // ...
        // ...
        // keep on growing depending on flash init needs
        
        // update to current revision
        sprintf((char *)nvmem.app.manufacturing.cfwversion, "%02d.%02d.%02d", FW_REVISION_MAJOR, FW_REVISION_MINOR, FW_REVISION_PATCH);
        write_to_flash(FLASH_TIMEOUT);
        break;
      default:
        break;
    } 
  }
}

/**
    @brief  load_data_from_flash() handles loading flash parameters to RAM image durng bootup.
            
            Operation starts by loading both main and redundant page and calculating their crc16.
            If both pages are blank, we assume taht this is a brand new device. We load default values to both non-volatile memory
            If both pages are healty and they are identical, we proceed
            If main page is good and redundant page is bad, we copy the main page to the redundant page.
            If main page is bad and the redundant page is good, we copy the redundant page to main page and trigger a flash write to both main and redundant page
            If if boath main and redundant pages are bad, we DO NOT TOUCH THE CONTENTS AND FLAG THAT THERE IS A CATASTROPHIC ERROR
            Finally, if both main and redundant pages are good but they are not identical, main pageis assumed to be the correct page and will be copied to theredundant page
*/
void load_data_from_flash(void) {
  uint16_t lcl_crc16_main;
  uint16_t lcl_crc16_red;
  uint16_t flash_crc16_main;
  uint16_t flash_crc16_red;
  uint8_t page_stat = 0x00;
  
  // load main non-volatile memory page
  memcpy(&nvmem, (uint8_t*)FLASH_USER_START_ADDR, sizeof(nvmem));
  lcl_crc16_main = crc16_ccitt(FLASH_CRC_SEED, (uint8_t*)&nvmem, sizeof(nvmem)-2);  
  flash_crc16_main = nvmem.crc16;
  if (nvmem.magic_number == FLASH_MAGICNUMBER) {
    page_stat |= 0x01;  // primary page  has some values
  }
  // load redundant non-volatile memory page
  memcpy(&nvmem, (uint8_t*)FLASH_USER_REDUNDANT_START_ADDR, sizeof(nvmem));
  lcl_crc16_red = crc16_ccitt(FLASH_CRC_SEED, (uint8_t*)&nvmem, sizeof(nvmem)-2);  
  flash_crc16_red = nvmem.crc16;
  if (nvmem.magic_number == FLASH_MAGICNUMBER) {
    page_stat |= 0x02;  // primary page has some values
  }  
  
  if (page_stat == 0x00) {  // problem with both pages. lets start fresh
    load_default_values(&nvmem);
  } else {
    if ((page_stat & 0x01) && (lcl_crc16_main == flash_crc16_main)) {
      // load main non-volatile memory page
      memcpy(&nvmem, (uint8_t*)FLASH_USER_START_ADDR, sizeof(nvmem));
      if (lcl_crc16_red != flash_crc16_red) {
        #ifndef BOOTLOADER
        write_to_flash(FLASH_TIMEOUT);  // initiate a redundant page resotration
        #endif
      } else {
        nvmem_state_machine = NVMEM_IDLE;
      }
    } else if ((page_stat & 0x02) && (lcl_crc16_red == flash_crc16_red)) {
      // load redundant non-volatile memory page
      memcpy(&nvmem, (uint8_t*)FLASH_USER_REDUNDANT_START_ADDR, sizeof(nvmem));
      #ifndef BOOTLOADER
      write_to_flash(FLASH_TIMEOUT);  // initiate a primary page resotration
      #else
      nvmem_state_machine = NVMEM_IDLE;
      #endif
    } else {
      // load main non-volatile memory page (even if it is broken)
      memcpy(&nvmem, (uint8_t*)FLASH_USER_START_ADDR, sizeof(nvmem));
      nvmem_state_machine = NVMEM_IDLE; 
    }
    #ifndef BOOTLOADER
    // check for special firmware upgrade utilities
    post_fwupgrade();
    #endif
  } 
}

/**
    @brief handle retries
*/
nvmem_state handle_retries(nvmem_state current_state, nvmem_state next_state, uint8_t *retries, uint8_t max_retries) {
    
    if (*retries >= max_retries) {
        return NVMEM_ERROR;
    } else {
        *retries += 1;
        return next_state;
    }
}

/**
	@brief Utility to wipe out primary and redundant page
*/
void clearNvMem(void) {
	static FLASH_EraseInitTypeDef EraseInitStruct; 

	/* Unlock the Flash to enable the flash control register access *************/
	HAL_FLASH_Unlock();
   /* Clear OPTVERR bit set on initial samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGSERR);
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGAERR);
  /* Get the 1st page to erase */
  FirstPage = GetPage(FLASH_USER_START_ADDR);
  /* Get the number of pages to erase from 1st page */
  NbOfPages = GetPage(FLASH_USER_END_ADDR) - FirstPage + 1;       
  /* Get the bank */
  BankNumber = GetBank(FLASH_USER_START_ADDR);
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
	}
}

/**
    @brief  nvmem_stat_machine() function runs the main non-volatile state machine.
            When a flash write is triggered, both main and redundant pages contents are handled here, making sure that the pages are identical
*/
nvmem_state nvmem_stat_machine(void) {     
  static uint16_t delaycycles = 0;
  static uint8_t retries = 0;
  uint16_t lcl_crc16;
  static FLASH_EraseInitTypeDef EraseInitStruct; 
  __IO uint64_t *src;
  
  switch (nvmem_state_machine) {
    case NVMEM_INIT:
      load_data_from_flash();
      break;
    case NVMEM_IDLE:
      break;
    case NVMEM_WAITING:
      if (flash_busy) {
        break;
      }
      // wait until no more requests to write into flash
      if (flashwritetimeout > 0) {
        flashwritetimeout--;
      } else {
        retries = 0;
        flash_busy = true;
        nvmem_state_machine = NVMEM_PAGE_UNLOCK;
      }
      break;
    case NVMEM_PAGE_UNLOCK:
        /* Unlock the Flash to enable the flash control register access *************/
        HAL_FLASH_Unlock();      
         /* Clear OPTVERR bit set on initial samples */
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGSERR);
        /* Get the 1st page to erase */
        FirstPage = GetPage(FLASH_USER_START_ADDR);
        /* Get the number of pages to erase from 1st page */
        NbOfPages = GetPage(FLASH_USER_END_ADDR) - FirstPage + 1;       
        /* Get the bank */
        BankNumber = GetBank(FLASH_USER_START_ADDR);
        /* Fill EraseInit structure*/
        EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES; //FLASH_TYPEERASE_MASSERASE; //FLASH_TYPEERASE_PAGES;
        EraseInitStruct.Banks       = BankNumber;
        EraseInitStruct.Page        = FirstPage;
        EraseInitStruct.NbPages     = NbOfPages;   
        flash_it = false;
        if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) == HAL_OK) {
          nvmem.crc16 = crc16_ccitt(FLASH_CRC_SEED, (uint8_t*)&nvmem, sizeof(nvmem)-2);							
          src_addr = (uint32_t)&nvmem.magic_number;
          Address = FLASH_USER_START_ADDR;
          nvmem_state_machine = NVMEM_PAGE_WRITE;
        }
      break;
    case NVMEM_PAGE_WRITE:
      if (Address < (FLASH_USER_START_ADDR + sizeof(nvmem))) {
        flash_it = false;
        src = (__IO uint64_t*)src_addr;
        HAL_FLASH_Program_IT(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, *(__IO uint64_t*)src_addr);       
        nvmem_state_machine = NVMEM_PAGE_WRITE_WAIT_IT;          
      } else {
        retries = 0;
        nvmem_state_machine = NVMEM_PAGE_VALIDATE;
      }
      break;
    case NVMEM_PAGE_WRITE_WAIT_IT:
      if (flash_it) {
        Address = Address + sizeof(uint64_t);
        src_addr = src_addr + sizeof(uint64_t);
        retries = 0;
        nvmem_state_machine = NVMEM_PAGE_WRITE;
      }
      break;
    case NVMEM_PAGE_VALIDATE:
        lcl_crc16 = crc16_ccitt(FLASH_CRC_SEED, (uint8_t*)FLASH_USER_START_ADDR, sizeof(nvmem)-2);
        if (lcl_crc16 == nvmem.crc16) {
          nvmem_state_machine = NVMEM_FINALIZE;
        } else {
          if (retries == 3) {
            #ifndef BOOTLOADER
            //awe.device.error |= NVMEM_PAGEVALIDATE_ERROR;
            #endif
            nvmem_state_machine = NVMEM_ERROR;
          } else {
            nvmem_state_machine = handle_retries(NVMEM_PAGE_VALIDATE, NVMEM_PAGE_UNLOCK, &retries, MAX_FLASH_RETRIES);  
          } 
        }
      break;
    case NVMEM_FINALIZE:
      HAL_FLASH_Lock();        
        nvmem_state_machine = NVMEM_PAGE_RED_UNLOCK;    
        delaycycles = 500; // 500 software loops
        break;
      case NVMEM_PAGE_RED_UNLOCK:
        if (delaycycles-- > 0)
          break;
        /* Unlock the Flash to enable the flash control register access *************/
        HAL_FLASH_Unlock();      
         /* Clear OPTVERR bit set on initial samples */
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGSERR);
        /* Get the 1st page to erase */
        FirstPage = GetPage(FLASH_USER_REDUNDANT_START_ADDR);
        /* Get the number of pages to erase from 1st page */
        NbOfPages = GetPage(FLASH_USER_REDUNDANT_END_ADDR) - FirstPage + 1;       
        /* Get the bank */
        BankNumber = GetBank(FLASH_USER_REDUNDANT_START_ADDR);
        /* Fill EraseInit structure*/
        EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
        EraseInitStruct.Banks       = BankNumber;
        EraseInitStruct.Page        = FirstPage;
        EraseInitStruct.NbPages     = NbOfPages;   
        flash_it = false;
        if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) == HAL_OK) {
          nvmem.crc16 = crc16_ccitt(FLASH_CRC_SEED, (uint8_t*)&nvmem, sizeof(nvmem)-2);							
          src_addr = (uint32_t)&nvmem.magic_number;
          Address = FLASH_USER_REDUNDANT_START_ADDR;
          nvmem_state_machine = NVMEM_PAGE_RED_WRITE;
        }
        break;
      case NVMEM_PAGE_RED_ERASE:
        break;
      case NVMEM_PAGE_RED_WRITE:
        if (Address < (FLASH_USER_REDUNDANT_START_ADDR + sizeof(nvmem))) {
          flash_it = false;
          src = (__IO uint64_t*)src_addr;
          HAL_FLASH_Program_IT(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, *src);
          nvmem_state_machine = NVMEM_PAGE_RED_WRITE_WAIT_IT;          
        } else {
          retries = 0;
          nvmem_state_machine = NVMEM_PAGE_RED_VALIDATE;
        }
        break;
      case NVMEM_PAGE_RED_WRITE_WAIT_IT:
        if (flash_it) {
          Address = Address + sizeof(uint64_t);
          src_addr = src_addr + sizeof(uint64_t);
          retries = 0;
          nvmem_state_machine = NVMEM_PAGE_RED_WRITE;
        }
        break;
      case NVMEM_PAGE_RED_VALIDATE:
        lcl_crc16 = crc16_ccitt(FLASH_CRC_SEED, (uint8_t*)FLASH_USER_REDUNDANT_START_ADDR, sizeof(nvmem)-2);
        if (lcl_crc16 == nvmem.crc16) {
          nvmem_state_machine = NVMEM_RED_FINALIZE;
        } else {
          if (retries == 3) {
            #ifndef BOOTLOADER
            //awe.device.error |= NVMEM_REDPAGEVALIDATE_ERROR;
            #endif
            nvmem_state_machine = NVMEM_ERROR;
          } else {
            nvmem_state_machine = handle_retries(NVMEM_PAGE_RED_VALIDATE, NVMEM_PAGE_RED_UNLOCK, &retries, MAX_FLASH_RETRIES);  
          } 
        }
        break;
      case NVMEM_RED_FINALIZE:
        HAL_FLASH_Lock();        
        nvmem_state_machine = NVMEM_IDLE;  
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGSERR);      
        flash_busy = false;
        break; 
    case NVMEM_ERROR:
      #ifndef BOOTLOADER
      //awe.device.error |= NVMEM_FAILURE;
      #endif
      break;
    default:
      break;
  }
  
  return nvmem_state_machine;
}
