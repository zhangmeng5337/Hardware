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

/**
	* @bieaf ׁɴىٶ˽ߝ
	* @param addr       ׁ˽ߝքַ֘
	* @param buff       ׁԶ˽ߝք˽ةָ֫
	* @param buf_len  Ӥ׈
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
void uint32Touint32(uint32_t dat,unsigned char *buf)
{
	   //float a=3.14159;
	   uint32_t b;
	   b=dat;
	   
	   buf[0]=b>>0;
	   buf[1]=b>>8;
	   buf[2]=b>>16;
	   buf[3]=b>>24;


}

uint32_t uint8Touint32(unsigned char *buf)
{
	   //float a=3.14159;
	   uint32_t b;
	  
	   b=(buf[0]<<0)|(buf[1]<<8)|(buf[2]<<16)|(buf[3]<<24);
       return b;
}
uint32_t uint8Touint322(unsigned char *buf)
{
	   //float a=3.14159;
	   uint32_t b;
	  
	   b=(buf[0]<<24)|(buf[1]<<16)|(buf[2]<<8)|(buf[3]<<0);
       return b;
}


void floatTouint32(float dat,unsigned char *buf)
{
	   //float a=3.14159;
	   uint32_t b;
	   b=*(uint32_t*)&dat;
	   
	   buf[0]=b>>0;
	   buf[1]=b>>8;
	   buf[2]=b>>16;
	   buf[3]=b>>24;


}
float uint32Tofloat(unsigned char *buf)
{
	uint32_t tmp;
	tmp=(buf[0]<<0)|(buf[1]<<8)|(buf[2]<<16)|(buf[3]<<24);
	return *(float*)&tmp;

}

