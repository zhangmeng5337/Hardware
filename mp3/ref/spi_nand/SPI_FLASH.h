#ifndef _SPI_FLASH_H
#define _SPI_FLASH_H

#include "mydefine.h"
//¶Ë¿Ú¶¨Òå
#define FLASH_CS_H()       PTT_PTT1 = 1
#define FLASH_CS_L()       PTT_PTT1 = 0

/* Private define ----------------------------------*/
#define W25N_WriteEnable		      0x06 
#define W25N_WriteDisable		      0x04 
#define W25N_ReadStatusReg		    0x05 
#define W25N_WriteStatusReg		    0x01 
#define W25N_ReadData			        0x03 
#define W25N_FastReadData		      0x0B 
#define W25N_FastReadDual		      0x3B 
#define W25N_PageProgram		      0x02 
#define W25N_BlockErase			      0xD8 
#define W25N_SectorErase		      0x20 
#define W25N_ChipErase			      0xC7 
#define W25N_PowerDown			      0xB9 
#define W25N_ReleasePowerDown	    0xAB 
#define W25N_DeviceID			        0xAB 
#define W25N_ManufactDeviceID   	0x90 
#define W25N_JedecDeviceID		    0x9F

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */
#define Dummy_Byte                0xFF

#define SPI_FLASH_PerWritePageSize      2048   //KB

UINT8 FLASH_INIT(void);
void SPI_FLASH_PageErase(UINT16 BlockAddr);
void SPI_FLASH_BufferRead(UINT8* pBuffer, UINT16 ReadAddr, UINT16 NumByteToRead);
void SPI_FLASH_PageWrite(UINT8* pBuffer, UINT16 WriteAddr, UINT16 NumByteToWrite);
void SPI_FLASH_SectorErase(UINT16 BlockAddr);
UINT16 W25NXX_ReadID(void);
void PageUnlock(void);
void ChangeSR2(void);
UINT8 SPI1_Flash_Send(UINT8 val);
#endif