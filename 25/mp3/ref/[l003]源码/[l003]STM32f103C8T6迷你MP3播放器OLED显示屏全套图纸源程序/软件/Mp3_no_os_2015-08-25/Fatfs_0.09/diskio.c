/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* by grqd_xp                                                            */
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "stm32f10x.h"
#include "stm3210e_eval_sdio_sd.h"

/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */
/* Note that Tiny-FatFs supports only single drive and always            */
/* accesses drive number 0.                                              */

#define SECTOR_SIZE 512

//u32 buff2[512/4];
/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{	
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
 SD_Error Status = SD_OK;
 if(count==1)
 {
  Status = SD_ReadBlock((uint8_t *)(&buff[0]), sector << 9, SECTOR_SIZE);
  if (Status == SD_OK)
  {
   Status = SD_WaitReadOperation();							//等待DMA传输数据完成
  }
  if (Status == SD_OK)
  {
   while(SD_GetStatus() != SD_TRANSFER_OK);						//等待SDIO操作完成
  }
 }
 else
 {
  SD_ReadMultiBlocks((uint8_t *)(&buff[0]), sector << 9, SECTOR_SIZE, count);
  if (Status == SD_OK)
  {
   Status = SD_WaitReadOperation();							//等待DMA传输数据完成
  }
  if (Status == SD_OK)
  {
   while(SD_GetStatus() != SD_TRANSFER_OK);						//等待SDIO操作完成
  }
 }
 return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
 SD_Error Status = SD_OK;
 if(count==1)
 {
  SD_WriteBlock((uint8_t *)(&buff[0]), sector << 9, SECTOR_SIZE);
  if (Status == SD_OK)
  {
   Status = SD_WaitWriteOperation();							//等待DMA传输数据完成
  }
  if (Status == SD_OK)
  {
   while(SD_GetStatus() != SD_TRANSFER_OK);						//等待SDIO操作完成
  }
 }
 else
 {
  SD_WriteMultiBlocks((uint8_t *)(&buff[0]), sector << 9, SECTOR_SIZE, count);
  if (Status == SD_OK)
  {
   Status = SD_WaitWriteOperation();							//等待DMA传输数据完成
  }
  if (Status == SD_OK)
  {
   while(SD_GetStatus() != SD_TRANSFER_OK);						//等待SDIO操作完成
  }
 }
 return RES_OK;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{

	return RES_OK;
}

DWORD get_fattime(void){
	return 0;
}























