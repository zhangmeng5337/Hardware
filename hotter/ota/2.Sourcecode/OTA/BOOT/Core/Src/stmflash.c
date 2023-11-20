#include "stmflash.h"

/**
	* @bieaf 	通过地址获取页
	* @return page
	*/
uint32_t GetPage(uint32_t Addr)
{
    uint32_t page = 0;
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {   /* Bank 1 */
        page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
        printf("Bank_1\r\n");
    }
    else
    {   /* Bank 2 */
        page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
        printf("Bank_2\r\n");
    }
    return page;
}
uint32_t GetSectorsCount(unsigned int des_addr, unsigned int byte_size, )
{
    uint32_t page = 0;
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {   /* Bank 1 */
        page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
        printf("Bank_1\r\n");
    }
    else
    {   /* Bank 2 */
        page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
        printf("Bank_2\r\n");
    }
    return page;
}

/**
	* @bieaf 	擦除页 L431每页2K  共127页  256K
	* @param 	pageaddr  起始地址
	* @param 	num       擦除的页数
	* @return 1
	*/
int Erase_page(uint32_t pageaddr, uint32_t num)
{
    uint32_t PageError = 0;
    HAL_StatusTypeDef status;
    __disable_irq();
    //清除标志位，经测试 必要！！！
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR \
                           | FLASH_FLAG_OPTVERR | FLASH_FLAG_PGSERR | FLASH_FLAG_PROGERR | FLASH_FLAG_BSY);
    HAL_FLASH_Unlock();
    /* 擦除FLASH*/
    FLASH_EraseInitTypeDef FlashSet;
    FlashSet.Banks   = FLASH_BANK_1;	//STM32L431RCT6只有Bank1
    FlashSet.TypeErase = FLASH_TYPEERASE_PAGES;//按页擦除
    FlashSet.Page  = GetPage(pageaddr);//获取页位置
    FlashSet.NbPages = num;  //擦除的页数
    printf("识别的初始页数:%d  共删除%d页\r\n",FlashSet.Page,num);
    /*设置PageError，调用擦除函数*/
    status = HAL_FLASHEx_Erase(&FlashSet, &PageError);
    if (status != HAL_OK)
    {
        printf("HAL_FLASHEx_Erase  ERROR\r\n");
    }
    printf("Erase sucessfully!\r\n");
    HAL_FLASH_Lock();
    __enable_irq();
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
