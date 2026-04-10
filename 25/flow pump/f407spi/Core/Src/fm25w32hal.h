#ifndef FM25W32_H
#define FM25W32_H
#include "main.h"
void FM25W32_WaitForWriteEnd(void) ;
void FM25W32_WriteEnable(void);
void FM25W32_PageProgram(uint32_t address, uint8_t *pData, uint16_t size);
void FM25W32_ReadData(uint32_t address, uint8_t *pBuffer, uint16_t size);
void FM25W32_SectorErase(uint32_t address);
void FM25W32_Test(void);
int FM25W32_ChipErase(void);



#endif

