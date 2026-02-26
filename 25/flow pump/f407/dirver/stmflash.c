#include "stmflash.h"
#include "main.h"
uint32_t GetSectors(uint32_t Addr)
{
    uint32_t sectors = 0;
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
        /* Bank 1 */
        if(Addr<=0x08003FFF&&Addr>=0x08000000)
        {
            sectors = FLASH_SECTOR_0;
        }
        else if(Addr<=0x08007FFF)
        {
            sectors = FLASH_SECTOR_1;
        }
        else if(Addr<=0x0800BFFF)
        {
            sectors = FLASH_SECTOR_2;
        }
        else if(Addr<= 0x0800FFFF)
        {
            sectors = FLASH_SECTOR_3;
        }
        else if(Addr<=0x0801FFFF)
        {
            sectors = FLASH_SECTOR_4;
        }
        else if(Addr<= 0x0803FFFF)
        {
            sectors = FLASH_SECTOR_5;
        }
        else if(Addr<= 0x0805FFFF)
        {
            sectors = FLASH_SECTOR_6;
        }
        else if(Addr<= 0x0807FFFF)
        {
            sectors = FLASH_SECTOR_7;
        }
        else if(Addr<=0x0809FFFF)
        {
            sectors = FLASH_SECTOR_8;
        }
        else if(Addr<= 0x080bFFFF)
        {
            sectors = FLASH_SECTOR_9;
        }
        else if(Addr<= 0x080dFFFF)
        {
            sectors = FLASH_SECTOR_10;
        }
        else if(Addr<= 0x080FFFFF)
        {
            sectors = FLASH_SECTOR_11;
        }

        //printf("Bank_1\r\n");


    }

    return sectors;

}




/**
	* @bieaf 	ӁԽҳ L431ÿҳ2K  ٲ127ҳ  256K
	* @param 	pageaddr  ǰʼַ֘
	* @param 	num       ӁԽքҳ˽
	* @return 1
	*/
int Erase_page(uint32_t secaddr, uint32_t num)
{

    uint32_t PageError = 0;
    HAL_StatusTypeDef status;
    /* ӁԽFLASH*/	
						 __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | \
                         FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
    FLASH_EraseInitTypeDef FlashSet;
    HAL_FLASH_Unlock();
	FLASH_WaitForLastOperation(50000);    //添加的代�?
    /* Fill EraseInit structure*/
  __HAL_FLASH_DATA_CACHE_DISABLE();//FLASH操作期间，必须禁止数据缓�?
        /* Enable data cache */
    __HAL_FLASH_DATA_CACHE_ENABLE();//开启数据缓�?


    /* Fill EraseInit structure*/
    FlashSet.TypeErase = FLASH_TYPEERASE_SECTORS;
    FlashSet.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    FlashSet.Banks   = FLASH_BANK_1;	//STM32L431RCT6ֻԐBank1
    FlashSet.Sector = GetSectors(secaddr);
    FlashSet.NbSectors = num;
    status = HAL_FLASHEx_Erase(&FlashSet, &PageError);
	//FLASH_WaitForLastOperation(50000);    //添加的代�?
    /* Lock the Flash to disable the flash control register access (recommended
    to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Lock();
    return status;
}

/** @bieaf 	дɴىٶ˽ߝ,STM32L4xxxֻŜ˫ؖޚдɫ
	* @param 	addr     		дɫքַ֘
	* @param 	buff       	дɫ˽ߝքǰʼַ֘
	* @param 	word_size  	Ӥ׈
	* @return	none				׵ܘֵ
 */
void WriteFlash(uint32_t addr, uint8_t * buff, int buf_len)
{

    int i = 0;
	HAL_StatusTypeDef status;
    HAL_FLASH_Unlock();
	FLASH_WaitForLastOperation(50000);    //添加的代�?
	__HAL_FLASH_DATA_CACHE_DISABLE();//FLASH操作期间，必须禁止数据缓�?
        /* Enable data cache */
    __HAL_FLASH_DATA_CACHE_ENABLE();//开启数据缓�?
	 __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | \
                         FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

    for( i= 0; i < buf_len; i+=4)
    {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr+i, *(__IO uint32_t*)(buff+i));
        while(status != HAL_OK)
        {
           while(status != HAL_OK)
			status = Erase_page(addr, 2);
		   i = 0;
		}
			
	}
    HAL_FLASH_Lock();
}
void WriteFlashBytes(uint32_t addr, uint8_t * buff, int buf_len)
{

    int i = 0;
	HAL_StatusTypeDef status;
    HAL_FLASH_Unlock();
	FLASH_WaitForLastOperation(50000);    //添加的代�?
	__HAL_FLASH_DATA_CACHE_DISABLE();//FLASH操作期间，必须禁止数据缓�?
        /* Enable data cache */
    __HAL_FLASH_DATA_CACHE_ENABLE();//开启数据缓�?
	 __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | \
                         FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

    for( i= 0; i < buf_len; i+=1)
    {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr+i, *(__IO uint32_t*)(buff+i));
        while(status != HAL_OK)
        {
           while(status != HAL_OK)
			status = Erase_page(addr, 1);
		   i = 0;
		}
			
	}
    HAL_FLASH_Lock();
}
nvmem_state nvmem_state_machine = NVMEM_INIT;

_Bool flash_busy = false;
_Bool flash_it = false;
uint16_t flashwritetimeout = 0;
uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
uint32_t Address = 0, PageError = 0;
uint32_t src_addr;

nvmem_struct nvmem;
void HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue) {
  flash_it = true;
}
nvmem_state nvmem_stat_machine(void) {     
  static uint16_t delaycycles = 0;
  static uint8_t retries = 0;
  uint16_t lcl_crc16;
  static FLASH_EraseInitTypeDef EraseInitStruct; 
  __IO uint64_t *src;
  
  switch (nvmem_state_machine) {
    case NVMEM_INIT:
     // load_data_from_flash();
      HAL_FLASH_Unlock();
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
        //HAL_FLASH_Unlock();      
         /* Clear OPTVERR bit set on initial samples */
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGSERR);
        /* Get the 1st page to erase */
        FirstPage = GetSectors(FLASH_USER_START_ADDR);
        /* Get the number of pages to erase from 1st page */
        NbOfPages = GetSectors(FLASH_USER_END_ADDR);       
        /* Get the bank */
       
        /* Fill EraseInit structure*/
        EraseInitStruct.TypeErase   = FLASH_TYPEERASE_SECTORS; //FLASH_TYPEERASE_MASSERASE; //FLASH_TYPEERASE_PAGES;
        EraseInitStruct.Banks       = FLASH_BANK_1;
        EraseInitStruct.Sector        = GetSectors(FLASH_USER_START_ADDR);
        EraseInitStruct.NbSectors    = NbOfPages;   
        flash_it = false;

       if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) == HAL_OK) {
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
      }else {
        retries = 0;
        nvmem_state_machine = NVMEM_PAGE_VALIDATE;
      }
      break;
    case NVMEM_PAGE_WRITE_WAIT_IT:
      if (flash_it) {
        Address = Address + sizeof(uint64_t);
        src_addr = src_addr + sizeof(uint64_t);
        retries = 0;
        nvmem_state_machine = NVMEM_RED_FINALIZE;
      }
      break;
          case NVMEM_RED_FINALIZE:
        //HAL_FLASH_Lock();        
        nvmem_state_machine = NVMEM_IDLE;  
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGSERR);      
        flash_busy = false;
        break; 
    default:
      break;
  }
  
  return nvmem_state_machine;
}


