#include "stmflash.h"

/**
	* @bieaf 	ͨ����ַ��ȡҳ
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
	* @bieaf 	����ҳ L431ÿҳ2K  ��127ҳ  256K
	* @param 	pageaddr  ��ʼ��ַ
	* @param 	num       ������ҳ��
	* @return 1
	*/
int Erase_page(uint32_t secaddr, uint32_t num)
{
    uint32_t PageError = 0;
    HAL_StatusTypeDef status;
    __disable_irq();
    //�����־λ�������� ��Ҫ������
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
                           | FLASH_FLAG_PGSERR |  FLASH_FLAG_BSY);
    HAL_FLASH_Unlock();
    /* ����FLASH*/
    FLASH_EraseInitTypeDef FlashSet;  
	  FlashSet.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    FlashSet.Banks   = FLASH_BANK_1;	//STM32L431RCT6ֻ��Bank1
    FlashSet.TypeErase = FLASH_TYPEERASE_SECTORS;//��ҳ����
    FlashSet.Sector  = GetSectors(secaddr);//��ȡ����λ��
    FlashSet.NbSectors = num;  //������������
    printf("ʶ��ĳ�ʼ������:%d  ��ɾ��%d����\r\n",FlashSet.Sector,2);
    /*����PageError�����ò�������*/
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

    /* ��FLASH��д */
    for( i= 0; i < buf_len; i+=4)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr+i, *(uint32_t*)(buff+i));
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
