#ifndef __STMFLASH_H__
#define __STMFLASH_H__

#include "main.h"
#include "stdio.h"

#define PageSize							FLASH_PAGE_SIZE

/**
	*	第一部分：BootLoader 		起始地址:	0x0800F000 	大小为:60K 占30页
	*	第二部分：Application_1 起始地址:	0x0800F000	大小为:90K 占45页
	*	第三部分：Application_2 起始地址:	0x08025800 	大小为:90K 占45页
	*/
#define BootLoader_Size				0xF000U						// BootLoader的区间大小  60K
#define Application_Size			0x16800U					// Application的区间大小 90K

#define Application_1_Addr		0x0800F000U				// 应用程序1的首地址  30页
#define Application_2_Addr		0x08025800U				// 应用程序2的首地址  75页

uint32_t GetPage(uint32_t Address);
int Erase_page(uint32_t pageaddr, uint32_t num);
void WriteFlash(uint32_t addr, uint8_t * buff, int buf_len);
void ReadFlash(uint32_t addr, uint8_t * buff, int buf_len);

#endif 

