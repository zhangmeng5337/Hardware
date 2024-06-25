#include "stmflash.h"
#include "main.h"
uint32_t GetPages(uint32_t Addr)
{
    uint32_t pages = 0;
    if (Addr>=0x08000000&&Addr<=0x0807FFFF)
    {
       
        pages = Addr-FLASH_BASE_ADDR;
		pages = pages/PageSize;
		
    }

    return pages;

}




/**
	* @bieaf 	?Խҳ L431ÿҳ2K  ٲ127ҳ  256K
	* @param 	pageaddr  ǰʼ?ַ
	* @param 	num       ?Խ?ҳ˽
	* @return 1
	*/
int Erase_page(uint32_t start, uint32_t len)
{
	  FLASH_EraseInitTypeDef EraseInitStruct;
	  uint32_t PageError = 0;
      unsigned int status;
       
	  EraseInitStruct.TypeErase	= FLASH_TYPEERASE_PAGES;	//ɾ����ʽ
	  EraseInitStruct.Page		  = GetPages(start);					  //��ʼҳ��
	  EraseInitStruct.NbPages	  = len;					  //ҳ������
	  EraseInitStruct.Banks 	  = FLASH_BANK_2;					  //bank��
	  
	  HAL_FLASH_Unlock();		  //��������׼������FLASH����
	  status=HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);	  //����
	  HAL_FLASH_Lock();
      HAL_FLASH_Lock();
    return status;
}

/** @bieaf 	дɴ?ٶ˽?,STM32L4xxxֻ?˫??дɫ
	* @param 	addr     		дɫ??ַ
	* @param 	buff       	дɫ˽??ǰʼ?ַ
	* @param 	word_size  	Ӥ?
	* @return	none				׵?ֵ
 */
void WriteFlash(uint32_t addr, uint8_t * buff, int buf_len)
{

    int i = 0;
	uint32_t wr_addr;
	HAL_StatusTypeDef status;
	wr_addr =  addr;
    HAL_FLASH_Unlock();
	FLASH_WaitForLastOperation(50000);    //添加?�代�?
	__HAL_FLASH_DATA_CACHE_DISABLE();//FLASH?�作??��?必须?止数�        /* Enable data cache */
    __HAL_FLASH_DATA_CACHE_ENABLE();//开?	 __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | \
                         FLASH_FLAG_PGAERR |  FLASH_FLAG_PGSERR);

    for( i= 0; i < buf_len; i+=8)
    {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, wr_addr+i, *(__IO uint64_t*)(buff+i));
        while(status != HAL_OK)
        {
           //while(status != HAL_OK)
			status = Erase_page(wr_addr, PageSize);
		   i = 0;
		}
			
	} 
    HAL_FLASH_Lock();
}

/**
	* @bieaf ?ɴ?ٶ˽?
	* @param addr       ?˽???ַ
	* @param buff       ?Զ˽??˽ةָ֫
	* @param buf_len  Ӥ?
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

