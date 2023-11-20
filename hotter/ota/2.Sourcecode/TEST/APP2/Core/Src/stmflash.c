#include "stmflash.h"


/** @bieaf 	ͨ����ַ��ȡҳ��
	* @return page			����ֵ
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

/**	@bieaf 	����ҳ L431ÿҳ2K  ��127ҳ  256K
	* @param	TypeErase ��������Ϊҳ����
	* @param 	Banks		  ��оƬֻ����BANK_1
	* @param 	pageaddr  ��ʼ��ַ
	* @param	num       ������ҳ��
	* @return 1					����ֵ
	*/
int Erase_page(uint32_t pageaddr, uint32_t num)
{
	uint32_t PageError = 0;	//����PageError
	HAL_StatusTypeDef status;	//FLASH����״̬��
	
	__disable_irq();	//ʧ���ж�
	/* ���FLASH��־λ */
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR \
											 | FLASH_FLAG_OPTVERR | FLASH_FLAG_PGSERR | FLASH_FLAG_PROGERR | FLASH_FLAG_BSY);
	/* ����FLASH */
	HAL_FLASH_Unlock();
	
	/* ����FLASH */
	FLASH_EraseInitTypeDef FlashSet;
	FlashSet.TypeErase = FLASH_TYPEERASE_PAGES;
	FlashSet.Banks = FLASH_BANK_1;
	FlashSet.NbPages = GetPage(pageaddr);
	FlashSet.Page = num;
	status = HAL_FLASHEx_Erase(&FlashSet, &PageError);	//���ò�������
	if (status != HAL_OK)
  {
    printf("erase fail, PageError = %d\r\n", PageError);
  }	printf("erase success\r\n");
   
	/* ����FLASH */
	HAL_FLASH_Lock();
	
	__enable_irq();	//ʹ���ж�
	
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
    for( i= 0; i < buf_len; i+=8)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr+i, *(uint64_t*)(buff+i));
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
