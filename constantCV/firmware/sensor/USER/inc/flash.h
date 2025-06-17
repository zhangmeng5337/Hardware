#ifndef FLASH_USER_H_
#define FLASH_USER_H_
#include "main.h"
#define READ_SIZE  512
#define START_ADDR	512*127	
#define END_ADDR    512*127	+ 511
void flash_write_byte(uint32_t WriteAddr, uint8_t *pWrBuf, uint16_t count);
void flash_read_byte(uint32_t ReadAddr, uint8_t *pWrBuf, uint16_t count);
void floatTouint32(float dat,unsigned char *pb);
float uint32Tofloat(unsigned char *buf);

#endif

