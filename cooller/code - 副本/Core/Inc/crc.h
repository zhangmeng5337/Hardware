#ifndef CRC_H
#define CRC_H
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "main.h"
unsigned char sum_Compute(unsigned char  *puchMsg, unsigned int usDataLen);

unsigned int CRC_Compute(unsigned char *puchMsg, unsigned int usDataLen);
#endif

