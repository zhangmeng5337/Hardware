#ifndef _FLASH_HH__
#define _FLASH_HH__
#include "stmflash.h"
#include "main.h"
flash_struct *get_flash_status(void);
void flash_init(void);
void flash_save(void);

#endif 

