#ifndef REGISTER_H_
#define REGISTER_H_
#include "main.h"

#define xF_ADDR_START    0xf001
#define xE_ADDR_START    0xe000
#define x1_ADDR_START    0x1001
#define x12_ADDR_START   0x1200
#define x4_ADDR_START    0x4001
#define x44_ADDR_START   0x44f1

typedef struct
{
 unsigned char  bindEquip[200*8];
 unsigned char value[512*2];
}REG_VALUE_stru;

unsigned char *getRegisterProc(unsigned char p);

#endif

