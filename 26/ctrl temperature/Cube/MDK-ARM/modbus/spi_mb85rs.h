#ifndef __SPI_MB85RS_H__
#define __SPI_MB85RS_H__
#include "main.h"

#define MB85RS_CMD_WREN 	0x06
#define MB85RS_CMD_WRDI		0x04
#define MB85RS_CMD_RDSR		0x05
#define MB85RS_CMD_WRSR		0x01
#define MB85RS_CMD_READ		0x03
#define MB85RS_CMD_WRITE	0x02
#define MB85RS_CMD_NULL   0x00
#define MB85RS_CMD_RDID		0x9f
///////////////////修改
//#define MB85_CS_SET		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)   //////////////////////////
//#define MB85_CS_CLR		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)///////////////////////////
#define MB85_CS_SET		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET)   //////////////////////////
#define MB85_CS_CLR		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET)///////////////////////////
/////////////////修改
#define MB85_WP_SET		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET)
#define MB85_WP_CLR		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET)


void MB85Ctrl_Init(void);
void MB85Ctrl_Read(uint16_t addr, uint8_t *buf, uint16_t len);
void MB85Ctrl_Write(uint16_t addr, uint8_t *buf, uint16_t len);
uint8_t SPI_ReadByte(uint8_t data);
uint8_t MB85Ctrl_Read_one(uint16_t addr);//写入一个字节到特定地址空间
void MB85Ctrl_Write_one(uint8_t data,uint16_t addr);//写入一个字节到特定地址空间

#endif /*__ SPI_MB85RS__ */

//存储地址表
//00-01  固定为0xaa  0xbb 检测通信
//02  RS485波特率  0：9600       1：115200
//03  RS232波特率  0：9600       1：115200
//04-19  AES密钥
//20-35  AES初始向量
//36-37  LORA密钥 高位 低位
