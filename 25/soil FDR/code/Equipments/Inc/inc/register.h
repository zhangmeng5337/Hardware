#ifndef REGISTER_H_
#define REGISTER_H_
#include "main.h"






#define x1_ADDR_START    0x1001
#define x1_ADDR_END      0x1102

#define x12_ADDR_START   0x1200
#define x12_ADDR_END     0x1209

#define x4_ADDR_START    0x4001
#define x4_ADDR_END      0x43FC

#define x44_ADDR_START   0x44f1
#define x44_ADDR_END     0x460F

#define xE_ADDR_START    0xe000
#define xE_ADDR_END      0xe00F

#define xF_ADDR_START    0xf001
#define xF_ADDR_END      0xf00e

#define x1_ADDR_SIZE      x1_ADDR_END-x1_ADDR_START
#define x12_ADDR_SIZE     x12_ADDR_END-x12_ADDR_START
#define x4_ADDR_SIZE      x44_ADDR_END-x44_ADDR_START
#define x44_ADDR_SIZE     x44_ADDR_END-x44_ADDR_START
#define xE_ADDR_SIZE      xE_ADDR_END-xE_ADDR_START
#define xF_ADDR_SIZE      xF_ADDR_END-xF_ADDR_START

typedef struct{
 unsigned char value[1024*2];
 unsigned char bindCount;
}REG_val_stru;

typedef struct
{
 unsigned int regAddr;
 unsigned char len;
 unsigned char wrStatus;
}REG_STATUS_stru;

unsigned char *ReadRegister(unsigned int p);
REG_STATUS_stru *readRigsterStatus(void);
unsigned int uchar2uint(unsigned char *p);
unsigned int ReadRegisterOffsetAddr(unsigned int p);

void WriteOneRegisterForce(unsigned int p,unsigned int regVal);


unsigned char *getRegisterProc(unsigned char p);
unsigned char WriteRegisteSet(unsigned int p,unsigned char *reg,unsigned char len);
void equip_bind(unsigned char *p);
unsigned char array_comp(unsigned char *p1,unsigned char *p2,unsigned char len);
REG_val_stru *getRegAddr(void);
void register_init(void);
unsigned char equip_bind_analy(unsigned char *p);
void loratestInit(void);
#endif

