#ifndef GAS_H_
#define GAS_H_
#include "main.h"

typedef struct
{
  unsigned int id;
  unsigned char name[16];
  float shr;
  float mw;
  float c1;
  float c2;
  float c3;//a
  float a;
  float b;
  float c;
  float d;
  unsigned int update_state;
  
}gas_datbase_stru;



gas_datbase_stru *get_gas(void);


#endif

