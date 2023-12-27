#include "stmflash.h"
#include "main.h"
/**
	* @bieaf 	ͨ����ַ��ȡҳ
	* @return page
	*/
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
	* @bieaf 	����ҳ L431ÿҳ2K  ��127ҳ  256K
	* @param 	pageaddr  ��ʼ��ַ
	* @param 	num       ������ҳ��
	* @return 1
	*/
int Erase_page(uint32_t secaddr, uint32_t num)
{
    uint32_t PageError = 0;
    HAL_StatusTypeDef status;
    /* ����FLASH*/
    FLASH_EraseInitTypeDef FlashSet;
    HAL_FLASH_Unlock();
    /* Fill EraseInit structure*/

    /* Fill EraseInit structure*/
    FlashSet.TypeErase = FLASH_TYPEERASE_SECTORS;
    FlashSet.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    FlashSet.Banks   = FLASH_BANK_1;	//STM32L431RCT6ֻ��Bank1
    FlashSet.Sector = GetSectors(secaddr);
    FlashSet.NbSectors = num;
    HAL_FLASHEx_Erase(&FlashSet, &PageError);

    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
    you have to make sure that these data are rewritten before they are accessed during code
    execution. If this cannot be done safely, it is recommended to flush the caches by setting the
    DCRST and ICRST bits in the FLASH_CR register. */
    __HAL_FLASH_DATA_CACHE_DISABLE();
    __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

    __HAL_FLASH_DATA_CACHE_RESET();
    __HAL_FLASH_INSTRUCTION_CACHE_RESET();

    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
    __HAL_FLASH_DATA_CACHE_ENABLE();

    /* Lock the Flash to disable the flash control register access (recommended
    to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Lock();
    return 1;
}

/** @bieaf 	д���ɸ�����,STM32L4xxxֻ��˫�ֽ�д��
	* @param 	addr     		д��ĵ�ַ
	* @param 	buff       	д�����ݵ���ʼ��ַ
	* @param 	word_size  	����
	* @return	none				����ֵ
 */
void WriteFlash(uint32_t addr, uint8_t * buff, int buf_len)
{

    int i = 0;
    /* ����FLASH */
    HAL_FLASH_Unlock();
    __HAL_FLASH_DATA_CACHE_DISABLE();
    __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

    __HAL_FLASH_DATA_CACHE_RESET();
    __HAL_FLASH_INSTRUCTION_CACHE_RESET();

    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
    __HAL_FLASH_DATA_CACHE_ENABLE();

    for( i= 0; i < buf_len; i+=1)

    {
       // FLASH_Program_Byte(addr+i, *(buff+i));
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr+i, *(buff+i));
    }


    /* ����FLASH */
    HAL_FLASH_Lock();
}

/**
	* @bieaf �����ɸ�����
	* @param addr       �����ݵĵ�ַ
	* @param buff       �������ݵ�����ָ��
	* @param buf_len  ����
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
