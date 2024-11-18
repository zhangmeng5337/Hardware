#ifndef __STMFLASH_H__
#define __STMFLASH_H__

#include "main.h"
#include "stdio.h"

#define WRITE		0
#define READ		1
#define SUCESS		0
#define NOPARAMS		1

//#define FLASH_BASE           0x08000000

#define FLASH_SECTOR_SIZE       0x4000U  
#define FLASH_BANK_SIZE         0x100000
 
#define PageSize				FLASH_PAGE_SIZE

/**
	*	第一部分：BootLoader 		     起始地址:	0x0801fff0 	大小�?64K �? sectors
	*	第二部分：Application_1      起始地址:	0x08020000	大小�?256K �? sectors
	*	第三部分：Application_2      起始地址:	0x08060000 	大小�?256K �? sectors
	*	第三部分：参数区�?          起始地址:	0x08010000 	大小�?64K �? sectors	
	*/
#define APP1   0
#define APP2   1

#define APP3   3
#define OTA_UPDATE_TO   APP1
#define VERSION        "V1.0.40"


#define OTA_NUM_ADDR   0x801fff0

#if OTA_UPDATE_TO == APP2
#define APP_NUM		0x1a
#define VECT_OFFSET_USR	0x20000
#endif

#if OTA_UPDATE_TO == APP1
#define APP_NUM		0x2a
#define VECT_OFFSET_USR	0x60000
#endif
#if OTA_UPDATE_TO == APP3
#define APP_NUM		0x2a
#define VECT_OFFSET_USR	0x00000

#endif


	


#define BootLoader_Size				0x10000U						// BootLoader size 64K
#define Application_Size			0x40000U					// Application size 256K

#define Application_1_Addr		0x08020000U				// app1 base addr
#define Application_2_Addr		0x08060000U				// app2 base addr
#define SYS_PARAMS_Addr        Application_2_Addr + Application_Size

#define CONFIG_Addr		       0x08010000


uint32_t GetPage(uint32_t Address);
int Erase_page(uint32_t pageaddr, uint32_t num);
void WriteFlash(uint32_t addr, uint8_t * buff, int buf_len);
void ReadFlash(uint32_t addr, uint8_t * buff, int buf_len);
void WriteFlashBytes(uint32_t addr, uint8_t * buff, int buf_len);
#endif 

