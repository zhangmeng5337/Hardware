#ifndef POWER_ENERGY_H_
#define POWER_ENERGY_H_
#include "main.h"

#define ENERGY_BUF_SIZE  16
#define ENERGY_COUNT   5

#define INSTR_DELI_SINDEX		5  //0x700
#define INSTR_DELI_EINDEX		6  //0x73c
#define INSTR_ZT_SINDEX		    7  // 0x0006
#define INSTR_ZT_EINDEX		    9 //0x401e
#define INSTR_DELI_INDEX       0//for debug
#define  INSTR_ZT_INDEX        2//for debug

#define INSTR_DELI_SIZE		    2
#define INSTR_ZT_SIZE		    3


typedef struct 
{
    unsigned char addr;
	unsigned char energy_typ; //0:zhengtai  1:deli
	
	float payload[ENERGY_BUF_SIZE];
}energy_stru;
typedef struct 
{
    unsigned char dev_size;
	energy_stru *pb;
}energylist_stru;

void power_cal(unsigned char index,unsigned char *p);

energylist_stru *get_energy_data(void);
void energy_init(void);


#endif

