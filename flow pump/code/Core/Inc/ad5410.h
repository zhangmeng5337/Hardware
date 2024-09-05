#ifndef AD5410_H_
#define AD5410_H_
#include "main.h"
#include "BspAD5410.h"

uint8_t AD5410xInit(unsigned char rangeC);
uint16_t AD5410xValueToCurrent(float value, float MaxValue);
uint8_t AD5410xWriteDataReg( uint16_t data);
 uint8_t AD5410xReadReg( uint8_t RegAddr, uint16_t *value);



#endif

