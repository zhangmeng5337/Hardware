#ifndef TEA_H
#define TEA_H
#include "stm8l15x.h"
void btea_encrypt( unsigned char* buf, unsigned char* key ,unsigned char len);
void btea_decrpyt( unsigned char* buf, unsigned char* key ,unsigned char len);

#endif

