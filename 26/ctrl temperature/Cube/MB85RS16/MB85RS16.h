#ifndef INC_MB85RS16_H_
#define INC_MB85RS16_H_
#include "main.h"

/*To define operation code*/
#define WREN 0x06    //Set Write Enable Latch
#define WRDI 0x04    //Reset Write Enable Latch
#define RDSR 0x05    //Read Status Register
#define WRSR 0x01    //Write Status Register
#define READ 0x03    //Read Memory Code
#define WRITE 0x02   //Write Memory Code
#define RDID 0x9F    //Read Device ID

#define MB85RS16_ID 0x01017F04

uint32_t MB85RS16_ReadID(void);
uint8_t MB85RS16_Init(void);
void MB85RS16_Set_Write_Enable_Latch(void);
void MB85RS16_Reset_Write_Enable_Latch(void);
void MB85RS16_Write_Status_Register(uint8_t SRV);
uint8_t MB85RS16_Read_Status_Register(void);
void MB85RS16_Write_Memory(uint8_t * wd, uint16_t addr, uint32_t len);
void MB85RS16_Read_Memory(uint8_t * rd, uint16_t addr, uint32_t len);

#endif /* INC_MB85RS16_H_ */
