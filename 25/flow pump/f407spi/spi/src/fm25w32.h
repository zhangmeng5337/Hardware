#ifndef __FM25W32_H
#define __FM25W32_H

#include "main.h"

// ---------------------------- 指令定义 ---------------------------------
#define FM25_CMD_WRITE_ENABLE       0x06
#define FM25_CMD_WRITE_DISABLE      0x04
#define FM25_CMD_READ_STATUS        0x05
#define FM25_CMD_WRITE_STATUS       0x01
#define FM25_CMD_READ_DATA          0x03
#define FM25_CMD_PAGE_PROGRAM       0x02
#define FM25_CMD_SECTOR_ERASE       0x20
#define FM25_CMD_BLOCK_ERASE_32K    0x52
#define FM25_CMD_BLOCK_ERASE_64K    0xD8
#define FM25_CMD_CHIP_ERASE         0xC7
#define FM25_CMD_READ_ID            0x9F
#define FM25_CMD_ENABLE_RESET       0x66
#define FM25_CMD_RESET_DEVICE       0x99

// ---------------------------- 芯片参数 ---------------------------------
#define FM25_PAGE_SIZE              256     // 页大小 (字节)
#define FM25_SECTOR_SIZE            4096    // 扇区大小 (4KB)
#define FM25_BLOCK_SIZE_32K         (32 * 1024)
#define FM25_BLOCK_SIZE_64K         (64 * 1024)
#define FM25_TOTAL_SIZE             (4 * 1024 * 1024)   // 4MB
#define FM25_BUSY_TIMEOUT_MAX       1000000             // 超时计数

// ---------------------------- 硬件接口宏 (根据实际连接修改) -----------------
// 假设 CS 接 PB12，SPI 使用 SPI1
#define FM25_CS_LOW()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)
#define FM25_CS_HIGH()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)

// ---------------------------- 对外API ------------------------------------
uint32_t FM25_ReadID(void);                                 // 读取芯片ID
uint8_t  FM25_EraseSector(uint32_t SectorAddr);             // 擦除扇区 (4KB)
uint8_t  FM25_EraseChip(void);                              // 整片擦除
void     FM25_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
uint8_t  FM25_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
void     FM25_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);

#endif