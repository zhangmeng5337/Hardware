#ifndef __STMFLASH_H__
#define __STMFLASH_H__

#include "main.h"
#include "stdio.h"

#define WRITE       0
#define READ        1
#define SUCESS      0
#define NOPARAMS        1

//#define FLASH_BASE           0x08000000

#define FLASH_SECTOR_SIZE       0x40001
#define FLASH_BANK_SIZE         0x100000
#define FLASH_PAGE_SIZE                 0x20000U  /* 2 KB */


#define PageSize                FLASH_PAGE_SIZE
#define FLASH_USER_START_ADDR   0X80E0000
#define FLASH_USER_END_ADDR     0X80FFFFF

typedef enum {
    NVMEM_INIT,
    NVMEM_IDLE,
    NVMEM_WAITING,
    NVMEM_PAGE_UNLOCK,
    NVMEM_PAGE_WRITE,
    NVMEM_PAGE_WRITE_WAIT_IT,
    NVMEM_PAGE_VALIDATE,
    NVMEM_FINALIZE,
    NVMEM_PAGE_RED_UNLOCK,
    NVMEM_PAGE_RED_ERASE,
    NVMEM_PAGE_RED_WRITE,
    NVMEM_PAGE_RED_WRITE_WAIT_IT,
    NVMEM_PAGE_RED_VALIDATE,
    NVMEM_RED_FINALIZE,
    NVMEM_ERROR
} nvmem_state;


typedef struct
{
   float a;
} app_data;

typedef struct
{
    uint32_t magic_number;
    app_data app;
} nvmem_struct;

uint32_t GetPage(uint32_t Address);
int Erase_page(uint32_t pageaddr, uint32_t num);
void WriteFlash(uint32_t addr, uint8_t *buff, int buf_len);
void ReadFlash(uint32_t addr, uint8_t *buff, int buf_len);
void WriteFlashBytes(uint32_t addr, uint8_t *buff, int buf_len);


#endif


