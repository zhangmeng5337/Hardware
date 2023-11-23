#include "stmflash.h"

/**
	* @bieaf 	通过地址获取页
	* @return page
	*/
uint32_t GetSectors(uint32_t Addr)
{
    uint32_t sectors = 0;
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {   /* Bank 1 */
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

        printf("Bank_1\r\n");
     
		
    }
   
  return sectors;

}




/**
	* @bieaf 	擦除页 L431每页2K  共127页  256K
	* @param 	pageaddr  起始地址
	* @param 	num       擦除的页数
	* @return 1
	*/
int Erase_page(uint32_t secaddr, uint32_t num)
{
    uint32_t PageError = 0;
    HAL_StatusTypeDef status;
    __disable_irq();
    //清除标志位，经测试 必要！！！
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
                           | FLASH_FLAG_PGSERR |  FLASH_FLAG_BSY);
    HAL_FLASH_Unlock();
    /* 擦除FLASH*/
    FLASH_EraseInitTypeDef FlashSet;  
	  FlashSet.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    FlashSet.Banks   = FLASH_BANK_1;	//STM32L431RCT6只有Bank1
    FlashSet.TypeErase = FLASH_TYPEERASE_SECTORS;//按页擦除
    FlashSet.Sector  = GetSectors(secaddr);//获取扇区位置
    FlashSet.NbSectors = num;  //擦除的扇区数
    printf("识别的初始扇区数:%d  共删除%d扇区\r\n",FlashSet.Sector,2);
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
    for( i= 0; i < buf_len; i+=4)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr+i, *(uint32_t*)(buff+i));
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
