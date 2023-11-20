#ifndef __STMFLASH_H__
#define __STMFLASH_H__

#include "main.h"
#include "stdio.h"

#define PageSize							FLASH_PAGE_SIZE

/**
	*	��һ���֣�BootLoader 		��ʼ��ַ:	0x0800F000 	��СΪ:60K ռ30ҳ
	*	�ڶ����֣�Application_1 ��ʼ��ַ:	0x0800F000	��СΪ:90K ռ45ҳ
	*	�������֣�Application_2 ��ʼ��ַ:	0x08025800 	��СΪ:90K ռ45ҳ
	*/
#define BootLoader_Size				0xF000U						// BootLoader�������С  60K
#define Application_Size			0x16800U					// Application�������С 90K

#define Application_1_Addr		0x0800F000U				// Ӧ�ó���1���׵�ַ  30ҳ
#define Application_2_Addr		0x08025800U				// Ӧ�ó���2���׵�ַ  75ҳ

uint32_t GetPage(uint32_t Address);
int Erase_page(uint32_t pageaddr, uint32_t num);
void WriteFlash(uint32_t addr, uint8_t * buff, int buf_len);
void ReadFlash(uint32_t addr, uint8_t * buff, int buf_len);

#endif 

