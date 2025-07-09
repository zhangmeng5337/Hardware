#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "protocol.h"
#include "aes.h"
#include "nvmem.h"
#include "bootloader.h"
#include "streaming.h"

char *blver = (char*)BL_VERSION;
char fwver[9] = "XX.YY.ZZ";
char algver[9] = "xx.yy.zz";
char fwblver[9] = "XX.YY.ZZ";
char fwid[3] = FIRMWARE_ID;

extern uint8_t key[16];
extern uint8_t iv[16];
extern uint32_t rtbAddress;
uint32_t rtbReadAddress;

FLASH_OBProgramInitTypeDef obConfig; 
uint32_t RdpLevel;

extern void read_single_block(uint32_t address, rtb_struct *out);

#ifdef BOOTLOADER
static uint8_t cmd01w(uint8_t *cmdInfo) {
	command_info *cmdInfo_lcl = (command_info *)cmdInfo;
	static FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
  uint32_t PageError = 0;
	static uint32_t Address = FLASH_APP_START_ADDR;
	uint8_t *ptrData;
	//uint32_t src_addr;
	uint16_t i;
  uint64_t Data = 0x00;
	uint8_t decBuffer[512]; //

	// erase 2K blocks of code if address goes beyond the page boundary  
	if ((Address%FLASH_PAGE_SIZE)==0) {
#ifdef WATCHDOG_ENABLED 		
 		HAL_WWDG_Refresh(&hwwdg);
#endif	
    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();
     /* Clear OPTVERR bit set on initial samples */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGSERR);
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGAERR);
    /* Get the 1st page to erase */
    FirstPage = GetPage(Address);
    /* Get the number of pages to erase from 1st page */
    NbOfPages = 1; //GetPage(FLASH_USER_END_ADDR) - FirstPage + 1;       
    /* Get the bank */
    BankNumber = GetBank(Address);
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
    
	// decrypt 64 bytes at a time, all 512 bytes
	ptrData = &cmdInfo_lcl->data_array[2];
	for (i=0; i<2; i++) {
#ifdef WATCHDOG_ENABLED 		
 		HAL_WWDG_Refresh(&hwwdg);
#endif
			AES128_CBC_decrypt_buffer(&decBuffer[i*64], ptrData, 64, key, iv);
			ptrData+=64;
	}
    
	ptrData = decBuffer;
	for (i=0;i<128;i+=8) {
#ifdef WATCHDOG_ENABLED 		
 		HAL_WWDG_Refresh(&hwwdg);
#endif
      __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGSERR);
      __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGAERR);
      memcpy(&Data, ptrData, 8); 
      ptrData+=8;
			/* Program the user Flash area word by word
			(area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
			if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, Data) == HAL_OK)
			{
					Address = Address + 8;
			}
			else
			{ 
					/* Error occurred while writing data in Flash memory. 
					User can add here some code to deal with this error */
					return _FAIL;
			}
	}
	return _SUCCESS;
}

/**
    @brief update the header to finalize firmware download
*/
static uint8_t cmd02w(uint8_t *cmdInfo) {
    
	if (update_app_image() == 1) {
		return _SUCCESS;
	} else {
		return _FAIL;
	}
        
}
#endif

/**
    @brief erase the header section while application is running
*/
static uint8_t cmd03w(uint8_t *cmdInfo) {
  uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
  uint32_t PageError = 0;
	static FLASH_EraseInitTypeDef EraseInitStruct;
	
	// clear header block
#ifdef WATCHDOG_ENABLED 		
 		HAL_WWDG_Refresh(&hwwdg);
#endif
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
	}
	return _SUCCESS;        
}

#ifdef RDP_ENABLED
/**
  @brief  Read RDP level
*/
static void getRDPLevel(void) {
  //obConfig.Banks = FLASH_BANK_1;
  HAL_FLASHEx_OBGetConfig(&obConfig);	
  RdpLevel = obConfig.RDPLevel;
}

/**
  @brief LEVEL 1 chip protection. This should disable reads from tools such as JTAG. Prevents cloning without authorization.
*/
static void setRDPLevel(uint8_t level) {

  getRDPLevel();
  
  if ((RdpLevel == OB_RDP_LEVEL_0) && (level == 1)) {
    // program OB
    if (HAL_FLASH_OB_Unlock() == HAL_OK) {
      if (HAL_FLASH_Unlock() == HAL_OK) {
        // set to protection
        obConfig.OptionType = OPTIONBYTE_RDP;
        obConfig.RDPLevel = OB_RDP_LEVEL_1;
        while (HAL_FLASHEx_OBProgram(&obConfig) != HAL_OK) {
          HAL_Delay(10);
        }
        if (HAL_FLASH_OB_Launch() != HAL_OK) {
          // set status flag here
        }
        __nop();       
      }   
    }     
  } else if ((RdpLevel == OB_RDP_LEVEL_1) && (level == 0)) {
    // program OB
    if (HAL_FLASH_OB_Unlock() == HAL_OK) {
      if (HAL_FLASH_Unlock() == HAL_OK) {
        // set to protection
        obConfig.OptionType = OPTIONBYTE_RDP;
        obConfig.RDPLevel = OB_RDP_LEVEL_0;
        while (HAL_FLASHEx_OBProgram(&obConfig) != HAL_OK) {
          HAL_Delay(10);
        }
        if (HAL_FLASH_OB_Launch() != HAL_OK) {
          // set status flag here
        }
        __nop();       
      }   
    }  
  }
}

/**
    @brief lock device for release
*/
static uint8_t cmd04w(uint8_t *cmdInfo) {
  command_info *cmdInfo_lcl = (command_info *)cmdInfo;
  setRDPLevel(cmdInfo_lcl->data_array[0]);  
  return _SUCCESS; 
}

/**
    @brief lock device for release
*/
static uint8_t cmd04r(uint8_t *cmdInfo) {
  command_info *cmdInfo_lcl = (command_info *)cmdInfo;
  getRDPLevel();
  if (RdpLevel == OB_RDP_LEVEL_0) {
    cmdInfo_lcl->data_array[0] = 0;    
  } else if (RdpLevel == OB_RDP_LEVEL_1) {
    cmdInfo_lcl->data_array[0] = 1;  
  } else {
    cmdInfo_lcl->data_array[0] = 99;  // unknown state  
  }
  cmdInfo_lcl->data_size = 1;
  
  return _CONTINUE; 
}
#endif
/**
  @brief read firmware version formatted to "XX.YY.ZZ"
*/
static uint8_t cmd09r(uint8_t *cmdInfo) {
#ifdef BOOTLOADER
  sprintf(fwver, "%02d.%02d.%02d", FW_BL_REVISION_MAJOR, FW_BL_REVISION_MINOR, FW_BL_REVISION_PATCH);
#else  
  sprintf(fwver, "%02d.%02d.%02d", FW_REVISION_MAJOR, FW_REVISION_MINOR, FW_REVISION_PATCH);
#endif 
  return _CONTINUE; 
}

/**
  @brief read algorithm version formatted to "XX.YY.ZZ"
*/
static uint8_t cmd10r(uint8_t *cmdInfo) {
  
  sprintf(algver, "%02d.%02d.%02d", ALG_REVISION_MAJOR, ALG_REVISION_MINOR, ALG_REVISION_PATCH);
 
  return _CONTINUE; 
}

/**
    @brief reset flash to factory defaults
*/
static uint8_t cmd0Bw(uint8_t *cmdInfo) {
  command_info *cmdInfo_lcl = (command_info *)cmdInfo;
  int res = strcmp((char*)cmdInfo_lcl->data_array, "LudicrouS");
  
  if ( res == 0) {
#ifndef BOOTLOADER			
      load_default_values(&nvmem);
      return _SUCCESS;  
#else
      clearNvMem();
      return _SUCCESS;  			
#endif		
  }
  return _FAIL; 
}

/**
    @brief start streaming data packets
*/
static uint8_t cmd0Fw(uint8_t *cmdInfo) {
  command_info *cmdInfo_lcl = (command_info *)cmdInfo;
  if (cmdInfo_lcl->data_array[0] == 1) {
    usbd_api.stream.enabled = 1;
  } else {
    usbd_api.stream.enabled = 0;
  }

	return _NORESP;
}

/**

*/
static uint8_t cmdInstall(uint8_t *cmdInfo) {
#ifndef BOOTLOADER
  command_info *cmdInfo_lcl = (command_info *)cmdInfo;  
  install_stream(cmdInfo_lcl->cmdSpecific - 92, cmdInfo_lcl->data_array[0]);
	return _SUCCESS;
#else
  return _FAIL;
  #endif
}

// 0xAA
const command_struct cmdFirmware[] = 
{
#ifdef BOOTLOADER
    {   0x0001,     _UINT8,     0,                  0x00,       0x00,    &cmd01w, _WRITE_ONLY   }, // not expecting a payload, set size to 0
    {   0x0002,     _UINT8,     0,                  0x00,       0x00,    &cmd02w, _WRITE_ONLY   }, // not expecting a payload, set size to 0
#endif
    {   3,     _UINT8,     0,                   0x00,       0x00,    &cmd03w, _WRITE_ONLY   }, // not expecting a payload, set size to 0
#ifdef RDP_ENABLED
    {   4,     _UINT8,     1,                   0x00,    &cmd04r,    &cmd04w,        0x00   },
#endif
    {   5,     _UINT8,     1,          &reset_device,       0x00,       0x00, _WRITE_ONLY   },
    {   6,     _UINT8,     1,              &fw_major,       0x00,       0x00, _READ_ONLY    },
    {   7,     _UINT8,     1,              &fw_minor,       0x00,       0x00, _READ_ONLY    },
    {   8,     _UINT8,     1,              &fw_patch,       0x00,       0x00, _READ_ONLY    },
    {   9,     _OCTETSTR,  8,                 &fwver,    &cmd09r,       0x00, _READ_ONLY    },      
    {  10,     _OCTETSTR,  8,                &algver,    &cmd10r,       0x00, _READ_ONLY    }, 

    {  12,     _OCTETSTR,  9,                   0x00,       0x00,    &cmd0Bw, _WRITE_ONLY   },

    {  16,     _OCTETSTR,  8,       (void*)BL_VERSION,       0x00,       0x00, _READ_ONLY    },
// 17..18
    {  19,     _UINT8,     1,           &nvmem.app.network.extproc,                 0x00,    0x00, _FLASH   },
    {  20,     _OCTETSTR,  3,                 &fwid,        0x00,       0x00,     _READ_ONLY   },
// 21..91
    
    {  92,     _UINT8,     1,                  0x00,       0x00,    &cmdInstall, _WRITE_ONLY   },
    {  93,     _UINT8,     1,                  0x00,       0x00,    &cmdInstall, _WRITE_ONLY   },
    {  94,     _UINT8,     1,                  0x00,       0x00,    &cmdInstall, _WRITE_ONLY   },
    {  95,     _UINT8,     1,                  0x00,       0x00,    &cmdInstall, _WRITE_ONLY   },
    {  96,     _UINT8,     1,                  0x00,       0x00,    &cmdInstall, _WRITE_ONLY   },
    {  97,     _UINT16,    2,  &usbd_api.stream.count, 0x00, 0x00, 0x00 },
    {  98,     _UINT16,    2,  &usbd_api.stream.spdivider, 0x00, 0x00, 0x00 },
    {  99,     _UINT8,     1,  &usbd_api.stream.enabled,    0x00,       cmd0Fw,   _WRITE_ONLY       }, 
    
    {0x0000,   0x00,    0x00,   0x00,   0x00,   0x00}
};
