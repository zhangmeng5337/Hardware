#ifndef GAS_H_
#define GAS_H_
#include "main.h"
#include "flash.h"


typedef struct
{
  unsigned int id;
  unsigned char name[16];
  float shr;
  float mw;
  float c1;
  float c2;
  float c3;//alpha
  float a;
  float b;
  float c;
  float d;
  unsigned int update_state;
  
}gas_datbase_stru;
typedef struct
{
  unsigned int id;
  float fullRange;
  float calibrationRatioBuf[GAS_MATCH_MKS_BUF_SIZE];
  float setpointBuf[GAS_MATCH_MKS_BUF_SIZE];
  
  unsigned int update_state;
}gas_datbase_matchMks_stru;



gas_datbase_stru *get_gas(void);
gas_datbase_matchMks_stru *get_gas_MKS(void);


#endif

