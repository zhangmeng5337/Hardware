#ifndef __STMFLASH_H__
#define __STMFLASH_H__

#include "main.h"
#include "stdio.h"

#define FLASH_BASE           0x08000000

#define FLASH_SECTOR_SIZE       0x4000U  
#define FLASH_BANK_SIZE         0x100000
 
#define PageSize				FLASH_PAGE_SIZE

/**
	*	��һ���֣�BootLoader 		��ʼ��ַ:	0x08000000 	��СΪ:64K ռ4 sectors
	*	�ڶ����֣�Application_1      ��ʼ��ַ:	0x08020000	��СΪ:256K ռ2 sectors
	*	�������֣�Application_2      ��ʼ��ַ:	0x08060000 	��СΪ:256K ռ2 sectors
	*/
#define BootLoader_Size				0x10000U						// BootLoader�������С  64K
#define Application_Size			0x3fc00//0x40000U					// Application�������С 256K

#define Application_1_Addr		0x08020000U				// Ӧ�ó���1���׵�ַռ2 sectors
#define Application_2_Addr		0x08060000U				// Ӧ�ó���2���׵�ַռ2 sectors

uint32_t GetPage(uint32_t Address);
int Erase_page(uint32_t pageaddr, uint32_t num);
void WriteFlash(uint32_t addr, uint8_t * buff, int buf_len);
void ReadFlash(uint32_t addr, uint8_t * buff, int buf_len);

#endif 

