#ifndef FM25W32_H
#define FM25W32_H
#include "main.h"
// ------------------ FM25W32 命令定义 (参考 FM25W32 数据手册) ------------------
// --- 芯片基本信息 ---
#define FM25W32_PAGE_SIZE          256   // 页大小：256字节
#define FM25W32_SECTOR_SIZE        4096  // 扇区大小：4KB
#define FM25W32_BLOCK_SIZE_32K     (32 * 1024) // 32KB块大小
#define FM25W32_BLOCK_SIZE_64K     (64 * 1024) // 64KB块大小
#define FM25W32_TOTAL_SIZE         (4 * 1024 * 1024) // 总容量：4MB


// --- 常用指令定义 (参考FM25W32数据手册) ---
#define FM25W32_CMD_WRITE_ENABLE     0x06
#define FM25W32_CMD_WRITE_DISABLE    0x04
#define FM25W32_CMD_READ_STATUS      0x05
#define FM25W32_CMD_WRITE_STATUS     0x01
#define FM25W32_CMD_READ_DATA        0x03
#define FM25W32_CMD_PAGE_PROGRAM     0x02
#define FM25W32_CMD_SECTOR_ERASE     0x20
#define FM25W32_CMD_BLOCK_ERASE_32K  0x52
#define FM25W32_CMD_BLOCK_ERASE_64K  0xD8
#define FM25W32_CMD_CHIP_ERASE       0xC7
#define FM25W32_CMD_READ_ID          0x9F


#define FM25W32_CS_HIGH()  HAL_GPIO_WritePin(GPIOB, FLASH_nCS1_Pin, GPIO_PIN_SET)
#define FM25W32_CS_LOW()    HAL_GPIO_WritePin(GPIOB, FLASH_nCS1_Pin, GPIO_PIN_RESET)
/*Configure GPIO pin Output Level */
uint8_t FM25W32_Init(void);
uint8_t FM25W32_ReadID(uint8_t *pMID, uint16_t *pDID);
uint8_t FM25W32_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
uint8_t FM25W32_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
uint8_t FM25W32_EraseSector(uint32_t SectorAddr);
uint8_t FM25W32_EraseChip(void);
void FM25W32_WaitForWriteEnd(void);

#endif


