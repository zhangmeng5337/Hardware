#ifndef EEPROM_H_
#define EEPROM_H_
#include "stc8h1k.h"

unsigned char EEPROM_Read_Byte(unsigned int addr);
unsigned char EEPROM_Read_Word(u16 addr);
unsigned char EEPROM_Read_Inte(u16 addr);
void IapErase(int addr);
void IapProgram(int addr, char dat);
void EEPROM_Write_Word(u16 addr ,u32 dat);
void EEPROM_Write_Inte(u16 addr ,u16 dat);
void EEPROM_Write_Byte(u16 addr ,u8 dat);

#endif

