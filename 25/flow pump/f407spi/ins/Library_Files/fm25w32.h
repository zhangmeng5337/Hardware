#ifndef __FM25W32_SPI_H
#define __FM25W32_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

//========================= 引脚定义 =========================
#define FM25W32_CS_PIN    GPIO_PIN_4
#define FM25W32_CS_PORT   GPIOA

//========================= 指令集 =========================
#define FM25W32_WREN      0x06  // 写使能
#define FM25W32_WRDI      0x04  // 写失能
#define FM25W32_RDSR      0x05  // 读状态寄存器
#define FM25W32_WRSR      0x01  // 写状态寄存器
#define FM25W32_READ      0x03  // 读数据
#define FM25W32_WRITE     0x02  // 写数据

//========================= 函数声明 =========================
uint8_t FM25W32_SPI_ReadWriteByte(uint8_t txData);

// 烧录算法标准接口 (必须声明)
int Init(void);
void UnInit(void);
int EraseChip(void);
int EraseSector(unsigned long adr);
int ProgramPage(unsigned long adr, unsigned long sz, unsigned char *buf);
int Read(unsigned long adr, unsigned long sz, unsigned char *buf);

#ifdef __cplusplus
}
#endif

#endif /* __FM25W32_SPI_H */