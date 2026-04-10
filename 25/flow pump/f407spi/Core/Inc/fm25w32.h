#ifndef FM25W322_H
#define FM25W322_H
#include "main.h"
// 定义片选控制宏
#define FM25W32_CS_LOW()  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET)
#define FM25W32_CS_HIGH() HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET)


// FM25W32 指令码
#define FM25W32_CMD_WREN      0x06  // 写使能
#define FM25W32_CMD_WRDI      0x04  // 写禁止
#define FM25W32_CMD_RDSR1     0x05  // 读状态寄存器1
#define FM25W32_CMD_RDSR2     0x07  // 读状态寄存器2（部分器件支持）
#define FM25W32_CMD_WRSR      0x01  // 写状态寄存器
#define FM25W32_CMD_READ      0x03  // 读数据
#define FM25W32_CMD_FAST_READ 0x0B  // 快速读（可选）
#define FM25W32_CMD_PP        0x02  // 页编程（写数据）
#define FM25W32_CMD_SE        0x20  // 扇区擦除 (4KB)
#define FM25W32_CMD_BE        0xD8  // 块擦除 (64KB)
#define FM25W32_CMD_CE        0xC7  // 全片擦除
#define FM25W32_CMD_RDID      0x9F  // 读器件ID

// 状态寄存器1 位定义
#define FM25W32_SR1_BUSY      (1 << 0)  // 忙标志
#define FM25W32_SR1_WEL       (1 << 1)  // 写使能锁存
#define FM25W32_SR1_BP0       (1 << 2)  // 块保护位0
#define FM25W32_SR1_BP1       (1 << 3)  // 块保护位1
#define FM25W32_SR1_BP2       (1 << 4)  // 块保护位2 (部分器件)
#define FM25W32_SR1_TB        (1 << 5)  // 顶部/底部保护
#define FM25W32_SR1_SRP       (1 << 7)  // 状态寄存器保护

//#define FM25W32_PAGE_SIZE         256   // 字节
//#define FM25W32_SECTOR_SIZE       (4 * 1024)   // 4KB
//#define FM25W32_BLOCK_SIZE        (64 * 1024)  // 64KB
//#define FM25W32_TOTAL_SIZE        (4 * 1024 * 1024)  // 4MB (32Mbit)

#define DEVICE_NAME "FM25W32" // 存储器名称
#define DEVICE_START_ADDR 0x90000000 // 存储器起始地址（可自定义）
#define MEMORY_FLASH_SIZE (4 * 1024 * 1024) // 4MB大小
#define MEMORY_PAGE_SIZE 256 // 页大小
#define MEMORY_SECTOR_SIZE (64 * 1024) // 块大小
 uint8_t SPI_TransmitByte(uint8_t data);

// 发送多个字节
 void SPI_TransmitBytes(uint8_t *pData, uint16_t size);

// 接收多个字节
 void SPI_ReceiveBytes(uint8_t *pData, uint16_t size);
#endif

