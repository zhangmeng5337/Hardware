#ifndef EEPROM_H
#define EEPROM_H

#include "main.h"
#define FLASH_WAITETIME  50000          //FLASH等待超时时间

/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */






#define DATA_FLASH_USER_START_ADDR   0x080A0000   /* Start @ of user Flash area */
#define DATA_FLASH_USER_END_ADDR     0x080bffff   /* End @ of user Flash area */

#define FLASH_USER_START_ADDR   0x080C0000   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     0x080dffff   /* End @ of user Flash area */

void flash_write(uint32_t addr,uint32_t *data,uint32_t numtowrite)		;
void flash_read(uint32_t addr,uint32_t *pbuffer,uint16_t numtoread);
void flash_write_Data(uint32_t addr,unsigned char *data,uint16_t numtowrite)	;
void flash_read_Data(uint32_t addr,unsigned char *pbuffer,uint16_t numtoread);
void flash_init(unsigned char flashsector);



#endif

