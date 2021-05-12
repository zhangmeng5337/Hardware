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
#define FLASH_SECTOR_0     0U  /*!< Sector Number 0   */
#define FLASH_SECTOR_1     1U  /*!< Sector Number 1   */
#define FLASH_SECTOR_2     2U  /*!< Sector Number 2   */
#define FLASH_SECTOR_3     3U  /*!< Sector Number 3   */
#define FLASH_SECTOR_4     4U  /*!< Sector Number 4   */
#define FLASH_SECTOR_5     5U  /*!< Sector Number 5   */
#define FLASH_SECTOR_6     6U  /*!< Sector Number 6   */
#define FLASH_SECTOR_7     7U  /*!< Sector Number 7   */
#define FLASH_SECTOR_8     8U  /*!< Sector Number 8   */
#define FLASH_SECTOR_9     9U  /*!< Sector Number 9   */
#define FLASH_SECTOR_10    10U /*!< Sector Number 10  */
#define FLASH_SECTOR_11    11U /*!< Sector Number 11  */

uint32_t GetSector(uint32_t Address)
{
	uint32_t sector = 0;

	if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
	{
		sector = FLASH_SECTOR_0;
	}
	else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
	{
		sector = FLASH_SECTOR_1;
	}
	else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
	{
		sector = FLASH_SECTOR_2;
	}
	else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
	{
		sector = FLASH_SECTOR_3;
	}
	else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
	{
		sector = FLASH_SECTOR_4;
	}
	else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
	{
		sector = FLASH_SECTOR_5;
	}
	else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
	{
	sector = FLASH_SECTOR_6;
	}
	else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
	{
		sector = FLASH_SECTOR_7;
	}
	else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
	{
		sector = FLASH_SECTOR_8;
	}
	else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
	{
	sector = FLASH_SECTOR_9;
	}
	else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
	{
		sector = FLASH_SECTOR_10;
	}
	else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
	{
		sector = FLASH_SECTOR_11;
	}
	return sector;
}

/*******************************************************************************
function: user flash Init
input:no
output:no
*******************************************************************************/
void flash_init(unsigned char flashsector)
{
	/* Fill EraseInit structure*/
	FLASH_EraseInitTypeDef  EraseInitStruct;
	uint32_t error=0;
	HAL_FLASH_Unlock();             //解锁	
	if(flashsector == 0)
	{
		StartSector = GetSector(FLASH_USER_START_ADDR); //获取FLASH的Sector编号
		EndSector = GetSector(FLASH_USER_END_ADDR);

	}
	else
	{
		StartSector = GetSector(DATA_FLASH_USER_START_ADDR); //获取FLASH的Sector编号
		EndSector = GetSector(DATA_FLASH_USER_END_ADDR);

	}
    


    FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成
	EraseInitStruct.Banks = FLASH_BANK_1;
	EraseInitStruct.NbPages = 1;
	EraseInitStruct.Page = StartSector;
	EraseInitStruct.TypeErase = FLASH_PROC_PAGE_ERASE;

	if(HAL_FLASHEx_Erase(&EraseInitStruct,&error)!=HAL_OK) 
	{
			FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成
	}
	
 
}


/*******************************************************************************
function: write data into flash
input:addr is the address;
*******************************************************************************/
void flash_write(uint32_t addr,uint32_t *data,uint32_t numtowrite)	
{
	unsigned int i,k;
	uint32_t Address;
	
    
	Address=FLASH_USER_START_ADDR+addr;
	for(i=0;i<numtowrite;i++)
	{
		if(i>=2590)
			k++;
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_FAST,Address,(uint64_t)(*data));
		Address =Address+1;
	}
	FLASH_WaitForLastOperation(FLASH_WAITETIME); 
	HAL_FLASH_Lock();
}

/*******************************************************************************
function: read data from flash
input:addr is the address;
*******************************************************************************/
void flash_read(uint32_t addr,uint32_t *pbuffer,uint16_t numtoread)	
{
	uint16_t i;
	uint32_t tmp;
	uint32_t Address;
	Address = FLASH_USER_START_ADDR+addr;
	for(i=0;i<numtoread;i++)
	{
		 tmp = *(__IO uint32_t *)Address;
		// pbuffer[i] = (unsigned char)tmp;	
		Address = Address+1;
	}
}
//void flash_write_Data(uint32_t addr,unsigned char *data,uint16_t numtowrite)	
//{
//	int i;
//	uint32_t Address;
//	//HAL_FLASH_Unlock();
//    
//	Address=DATA_FLASH_USER_START_ADDR+addr;
//	for(i=0;i<numtowrite;i++)
//	{
//		FLASH_Program_Byte(Address,(data[i]));	
//		Address =Address+1;
//		FLASH_WaitForLastOperation(FLASH_WAITETIME);
//	}
//	
//	//HAL_FLASH_Lock();
//}

///*******************************************************************************
//function: read data from flash
//input:addr is the address;
//*******************************************************************************/
//void flash_read_Data(uint32_t addr,unsigned char *pbuffer,uint16_t numtoread)	
//{
//	uint16_t i;
//	uint32_t tmp;
//	uint32_t Address;
//	Address = DATA_FLASH_USER_START_ADDR+addr;
//	for(i=0;i<numtoread;i++)
//	{
//		 tmp = *(__IO uint32_t *)Address;
//		 pbuffer[i] = (unsigned char)tmp;	
//		Address = Address+1;
//	}
//}



