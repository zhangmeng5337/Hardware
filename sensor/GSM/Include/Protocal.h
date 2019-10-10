#ifndef PROTOCAL_H_
#define PROTOCAL_H_
#include "stm8l15x.h"

#define NODE_TO_SERVERH  0xAB
#define NODE_TO_SERVERL  0xAB
#define SERVER_TO_NODEH  0xAA
#define SERVER_TO_NODEL  0xAA

#define CHECKSUM_INDEX   0x05

typedef struct{
  unsigned char id[2];

  unsigned char len;
  unsigned char checksum;
  unsigned char deepth[2];
  unsigned char deepth_percent;
  unsigned char vbat[2];
  unsigned char status;
  unsigned int deepth_calibration;
  uint32_t Warn_Thres;
  float target;
  float calibration;
}Data_Stru;
void module_process(void);
void params_init(void);
void module_prams_init(void);
#endif

