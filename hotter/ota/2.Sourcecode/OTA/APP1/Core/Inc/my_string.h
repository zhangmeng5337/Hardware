#ifndef _MY_STRING_H
#define _MY_STRING_H

#include "stm32l4xx.h"
#include "string.h"
#include <ctype.h>
#include <stdlib.h>
#include "usart.h"

int Find_string(char *pcBuf,char *left,char *right, char *pcRes);
uint16_t crc16(unsigned char *addr, int num, uint16_t crc);
char * mid(char *dst,char *src, int n,int m);

#endif
