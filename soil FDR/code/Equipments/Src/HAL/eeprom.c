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
uint32_t GetSector(uint32_t Address)
{
	uint32_t sector = 0;

	if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
	{
		sector = FLASH_Sector_0;
	}
	else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
	{
		sector = FLASH_Sector_1;
	}
	else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
	{
		sector = FLASH_Sector_2;
	}
	else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
	{
		sector = FLASH_Sector_3;
	}
	else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
	{
		sector = FLASH_Sector_4;
	}
	else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
	{
		sector = FLASH_Sector_5;
	}
	else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
	{
	sector = FLASH_Sector_6;
	}
	else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
	{
		sector = FLASH_Sector_7;
	}
	else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
	{
		sector = FLASH_Sector_8;
	}
	else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
	{
	sector = FLASH_Sector_9;
	}
	else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
	{
		sector = FLASH_Sector_10;
	}
	else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
	{
		sector = FLASH_Sector_11;
	}
	return sector;
}

/*******************************************************************************
function: user flash Init
input:no
output:no
*******************************************************************************/
void flash_init()
{
	/* Fill EraseInit structure*/
	  FLASH_EraseInitTypeDef  EraseInitStruct;
    uint32_t error;
	  unsigned char i;
	  StartSector = GetSector(FLASH_USER_START_ADDR); //获取FLASH的Sector编号
	  EndSector = GetSector(FLASH_USER_END_ADDR);
	  	EraseInitStruct.TypeErase   = FLASH_PROC_SECTERASE;
		EraseInitStruct.Sector = FLASH_USER_START_ADDR;
		EraseInitStruct.VoltageRange =FLASH_VOLTAGE_RANGE_3;
		EraseInitStruct.Banks = FLASH_BANK_1;
		EraseInitStruct.NbSectors     = (StartSector - EndSector)+1;	
	  //擦除FLASH
	  for (i = StartSector; i <= EndSector; i += 8) //每次FLASH编号增加8，可参考上边FLASH Sector的定义。
	  {
		  /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
		  be done by word */
		  if (HAL_FLASHEx_Erase(&EraseInitStruct, &error) != 0)
		  {
			  while (1)
			  {
			  }
		  }
	  }

}

static void FLASH_Program_Byte(uint32_t Address, uint8_t Data)
{
  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Address));
  
  /* If the previous operation is completed, proceed to program the new data */
  CLEAR_BIT(FLASH->CR, FLASH_CR_PSIZE);
  FLASH->CR |= FLASH_PSIZE_BYTE;
  FLASH->CR |= FLASH_CR_PG;

  *(__IO uint8_t*)Address = Data;
}

/*******************************************************************************
function: write data into flash
input:addr is the address;
*******************************************************************************/
void flash_write(uint32_t addr,unsigned char *data,uint16_t numtowrite)	
{
	int i;
	uint32_t Address;
	HAL_FLASH_Unlock();
    flash_init();
	Address=FLASH_USER_START_ADDR+4*addr;
	for(i=0;i<numtowrite;i++)
	{
		FLASH_Program_Byte(Address,(uint8_t)(data[i]));	
		Address =Address+1;
	}
	HAL_FLASH_Lock();
}

/*******************************************************************************
function: read data from flash
input:addr is the address;
*******************************************************************************/
void flash_read(uint32_t addr,unsigned char *pbuffer,uint16_t numtoread)	
{
	uint16_t i;
	uint32_t tmp;
	uint32_t Address;
	Address = FLASH_USER_START_ADDR+4*addr;
	for(i=0;i<numtoread;i++)
	{
		 tmp = *(__IO uint32_t *)Address;
		 pbuffer[i] = (unsigned char)tmp;	
		Address = Address+1;
	}
}
void flash_write2(uint32_t addr,unsigned char *data,uint16_t numtowrite)	
{
	int i;
	uint32_t Address;
	HAL_FLASH_Unlock();
    flash_init();
	Address=DATA_FLASH_USER_START_ADDR+4*addr;
	for(i=0;i<numtowrite;i++)
	{
		FLASH_Program_Byte(Address,(uint8_t)(data[i]));	
		Address =Address+1;
	}
	HAL_FLASH_Lock();
}

/*******************************************************************************
function: read data from flash
input:addr is the address;
*******************************************************************************/
void flash_read2(uint32_t addr,unsigned char *pbuffer,uint16_t numtoread)	
{
	uint16_t i;
	uint32_t tmp;
	uint32_t Address;
	Address = DATA_FLASH_USER_START_ADDR+4*addr;
	for(i=0;i<numtoread;i++)
	{
		 tmp = *(__IO uint32_t *)Address;
		 pbuffer[i] = (unsigned char)tmp;	
		Address = Address+1;
	}
}


