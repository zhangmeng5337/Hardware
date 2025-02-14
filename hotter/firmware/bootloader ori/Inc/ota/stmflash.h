#ifndef __STMFLASH_H__
#define __STMFLASH_H__

#include "main.h"
#include "stdio.h"

#define FLASH_BASE           0x08000000

#define FLASH_SECTOR_SIZE       0x4000U  
#define FLASH_BANK_SIZE         0x100000
 
#define PageSize				FLASH_PAGE_SIZE

/**
	*	第一部分：BootLoader 		起始地址:	0x08000000 	大小为:64K 占4 sectors
	*	第二部分：Application_1      起始地址:	0x08020000	大小为:256K 占2 sectors
	*	第三部分：Application_2      起始地址:	0x08060000 	大小为:256K 占2 sectors
	*/
#define BootLoader_Size				0x10000U						// BootLoader的区间大小  64K
#define Application_Size			0x3fc00//0x40000U					// Application的区间大小 256K

#define Application_1_Addr		0x08020000U				// 应用程序1的首地址占2 sectors
#define Application_2_Addr		0x08060000U				// 应用程序2的首地址占2 sectors

uint32_t GetPage(uint32_t Address);
int Erase_page(uint32_t pageaddr, uint32_t num);
void WriteFlash(uint32_t addr, uint8_t * buff, int buf_len);
void ReadFlash(uint32_t addr, uint8_t * buff, int buf_len);

#endif 

