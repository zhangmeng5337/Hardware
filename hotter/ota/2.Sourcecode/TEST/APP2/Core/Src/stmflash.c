#include "stmflash.h"


/** @bieaf 	通过地址获取页码
	* @return page			返回值
	*/
uint32_t GetPage(uint32_t Address)
{
    uint32_t page = 0;
    if (Address < (FLASH_BASE + FLASH_BANK_SIZE))
        page = (Address - FLASH_BASE) / FLASH_PAGE_SIZE;
    else
        page = (Address - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  return page;
}

/**	@bieaf 	擦除页 L431每页2K  共127页  256K
	* @param	TypeErase 擦除类型为页擦除
	* @param 	Banks		  本芯片只能是BANK_1
	* @param 	pageaddr  起始地址
	* @param	num       擦除的页数
	* @return 1					返回值
	*/
int Erase_page(uint32_t pageaddr, uint32_t num)
{
	uint32_t PageError = 0;	//设置PageError
	HAL_StatusTypeDef status;	//FLASH操作状态量
	
	__disable_irq();	//失能中断
	/* 清除FLASH标志位 */
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR \
											 | FLASH_FLAG_OPTVERR | FLASH_FLAG_PGSERR | FLASH_FLAG_PROGERR | FLASH_FLAG_BSY);
	/* 解锁FLASH */
	HAL_FLASH_Unlock();
	
	/* 擦除FLASH */
	FLASH_EraseInitTypeDef FlashSet;
	FlashSet.TypeErase = FLASH_TYPEERASE_PAGES;
	FlashSet.Banks = FLASH_BANK_1;
	FlashSet.NbPages = GetPage(pageaddr);
	FlashSet.Page = num;
	status = HAL_FLASHEx_Erase(&FlashSet, &PageError);	//调用擦除函数
	if (status != HAL_OK)
  {
    printf("erase fail, PageError = %d\r\n", PageError);
  }	printf("erase success\r\n");
   
	/* 上锁FLASH */
	HAL_FLASH_Lock();
	
	__enable_irq();	//使能中断
	
	return 1;
}

/** @bieaf 	写若干个数据,STM32L4xxx只能双字节写入
	* @param 	addr     		写入的地址
	* @param 	buff       	写入数据的起始地址
	* @param 	word_size  	长度
	* @return	none				返回值
 */
void WriteFlash(uint32_t addr, uint8_t * buff, int buf_len)
{
		int i = 0;
    /* 解锁FLASH */
    HAL_FLASH_Unlock();
	
		/* 对FLASH烧写 */
    for( i= 0; i < buf_len; i+=8)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr+i, *(uint64_t*)(buff+i));
    }
		
    /* 上锁FLASH */
    HAL_FLASH_Lock();
}

/**
	* @bieaf 读若干个数据
	* @param addr       读数据的地址
	* @param buff       读出数据的数组指针
	* @param buf_len  长度
	* @return
	*/
void ReadFlash(uint32_t addr, uint8_t * buff, int buf_len)
{
    uint32_t i;
    for(i = 0; i < buf_len; i++)
    {
        buff[i] = *(__IO uint8_t*)(addr + i);
    }
}
