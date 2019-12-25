#ifndef PROTOCAL_H_
#define PROTOCAL_H_
#include "stm8l15x.h"

#define NODE_TO_SERVERH  0xAB
#define NODE_TO_SERVERL  0xAB
#define SERVER_TO_NODEH  0xAA
#define SERVER_TO_NODEL  0xAA

#define CHECKSUM_INDEX   0x05
#define SENSOR_FACTOR	 3.5/70
typedef struct{
  unsigned char id[2];

  unsigned char len;
  unsigned char checksum;
  unsigned char deepth[2];
  unsigned char deepth_percent;
  unsigned char vbat[2];
  unsigned char status;
  float deepth_calibration;
  float Warn_Thres;
  float vbatf;
  float deep;

}Data_Stru;
void module_process(void);
void module_prams_init(void);


#endif

