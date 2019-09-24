#ifndef TEA_H
#define TEA_H
#include "stm8l15x.h"
/TEA加密函数
void btea_encrypt( unsigned char* buf, unsigned char* key );
//TEA解密函数
void btea_decrpyt( unsigned char* buf, unsigned char* key );
#endif

