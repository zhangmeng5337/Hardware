#ifndef _STMFLASH_H_
#define _STMFLASH_H_
#include "main.h"
#include "stdio.h"

//#define FLASH_BASE           0x08000000

//#define FLASH_SECTOR_SIZE       0x4000U  
//#define FLASH_BANK_SIZE         0x100000
 
#define PageSize				FLASH_PAGE_SIZE
#define FLASH_BASE_ADDR         0x08040000U

#define FACTORY_ADDR			0x08040000U				// nozzle
#define FACTORY_END_ADDR         0x080407ffU


#define NOZZLE_ADDR			    0x08040800U				// nozzle
#define NOZZLE_END_ADDR         0x08040fffU

#define GAS_ADDR				0x08041000U				// gas
#define GAS_END_ADDR            0x08058fffU

#define PRE_END_ADDR            0x0807ffffU
#define PRE_ADDR                0x08059000U



//#define Application_2_Addr		0x080E0000U				// 应用程序2的首地址�? sectors




typedef struct
{
	unsigned char header;
	uint32_t used_len;
	uint32_t vailabe_len;
	uint32_t total_len;
	unsigned char usb_read_flag;
}flash_struct;


int Erase_page(uint32_t pageaddr, uint32_t num);
void WriteFlash(uint32_t addr, uint8_t * buff, int buf_len);
void ReadFlash(uint32_t addr, uint8_t * buff, int buf_len);
void floatTouint32(float dat,unsigned char *buf);
float uint32Tofloat(unsigned char *buf);
uint32_t GetPages(uint32_t Addr);

#endif 

