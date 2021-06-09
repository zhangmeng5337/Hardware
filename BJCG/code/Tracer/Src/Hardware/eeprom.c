// ========================================================
/// @file       FLASH.c
/// @brief      FLASH Configuration and WR Operation
/// @version    V1.0
/// @date       2016/08/19
/// @company    WooZoom Co., Ltd.
/// @website    http://www.woozoom.net
/// @author     ZhangMeng
/// @mobile     +86-13804023611
// ========================================================
#include "EEPROM.h"
uint32_t StartSector ;
uint32_t EndSector;
uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;


static uint32_t GetPage(uint32_t Addr)
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
static uint32_t GetBank(uint32_t Addr)
{
  return FLASH_BANK_1;
}
static FLASH_EraseInitTypeDef EraseInitStruct;

/*******************************************************************************
function: user flash Init
input:no
output:no
*******************************************************************************/
void flash_init(unsigned char flashsector)
{
	/* Fill EraseInit structure*/

	uint32_t PAGEError=0;
	 
	 /* Unlock the Flash to enable the flash control register access *************/
	 HAL_FLASH_Unlock();
	 
	 /* Erase the user Flash area
	   (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
	 
	 /* Get the 1st page to erase */
	 FirstPage = GetPage(FLASH_USER_START_ADDR);
	 
	 /* Get the number of pages to erase from 1st page */
	 NbOfPages = GetPage(FLASH_USER_END_ADDR) - FirstPage + 1;
	 
	 /* Get the bank */
	 BankNumber = GetBank(FLASH_USER_START_ADDR);
	 
	 /* Fill EraseInit structure*/
	 EraseInitStruct.TypeErase	 = FLASH_TYPEERASE_PAGES;
	 EraseInitStruct.Banks		 = BankNumber;
	 EraseInitStruct.Page		 = FirstPage;
	 EraseInitStruct.NbPages	 = NbOfPages;
	 
	 /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
		you have to make sure that these data are rewritten before they are accessed during code
		execution. If this cannot be done safely, it is recommended to flush the caches by setting the
		DCRST and ICRST bits in the FLASH_CR register. */
	 if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	 {
			FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成
	}



	
 
}


/*******************************************************************************
function: write data into flash
input:addr is the address;
*******************************************************************************/
void flash_write(uint32_t addr,uint32_t *data,uint32_t numtowrite)	
{
	unsigned int i,k;
	uint32_t Address;
	uint64_t data_tmp;
    

	
  HAL_FLASH_Unlock();
	Address=addr*8;
	Address=FLASH_USER_START_ADDR+Address;	
	for(i=0;i<numtowrite;i++)
	{
		 data_tmp = data[i];
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, (data_tmp)) == HAL_OK)
		{
		  Address = Address + 8;
		}
	}
	FLASH_WaitForLastOperation(FLASH_WAITETIME); 
	HAL_FLASH_Lock();
}

/*******************************************************************************
function: read data from flash
input:addr is the address;
*******************************************************************************/
void flash_read(uint32_t addr,uint32_t *pbuffer,uint16_t numtoread)	
{
	uint16_t i;
	uint64_t tmp;
	uint32_t Address;
	Address = FLASH_USER_START_ADDR+addr*8;
	for(i=0;i<numtoread;i++)
	{
		 tmp = *(__IO uint32_t *)Address;
		 pbuffer[i] =(uint32_t) tmp;	
		Address = Address+8;
	}
}
//void flash_write_Data(uint32_t addr,unsigned char *data,uint16_t numtowrite)	
//{
//	int i;
//	uint32_t Address;
//	//HAL_FLASH_Unlock();
//    
//	Address=DATA_FLASH_USER_START_ADDR+addr;
//	for(i=0;i<numtowrite;i++)
//	{
//		FLASH_Program_Byte(Address,(data[i]));	
//		Address =Address+1;
//		FLASH_WaitForLastOperation(FLASH_WAITETIME);
//	}
//	
//	//HAL_FLASH_Lock();
//}

///*******************************************************************************
//function: read data from flash
//input:addr is the address;
//*******************************************************************************/
//void flash_read_Data(uint32_t addr,unsigned char *pbuffer,uint16_t numtoread)	
//{
//	uint16_t i;
//	uint32_t tmp;
//	uint32_t Address;
//	Address = DATA_FLASH_USER_START_ADDR+addr;
//	for(i=0;i<numtoread;i++)
//	{
//		 tmp = *(__IO uint32_t *)Address;
//		 pbuffer[i] = (unsigned char)tmp;	
//		Address = Address+1;
//	}
//}



