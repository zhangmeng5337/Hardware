#ifndef PROTOCAL_H_
#define PROTOCAL_H_
#include "stm8l15x.h"

#define NODE_TO_SERVERH  0xAB
#define NODE_TO_SERVERL  0xAB
#define SERVER_TO_NODEH  0xAA
#define SERVER_TO_NODEL  0xAA

#define LOCK_ON          1
#define LOCK_OFF         2
#define GPS_ON           3
#define GPS_OFF          4
#define GSM_ON           5
#define GSM_OFF          6

#define CHECKSUM_INDEX   0x05
#define SENSOR_FACTOR	 3.5/70
#define VOLTAGE_FACTOR	22275
#define vol_offset	60.6
typedef struct{
  unsigned char id[4];
  unsigned char len;
  unsigned char vbat[2];
  unsigned char status;
}Data_Stru;
void module_process(void);
void module_prams_init(void);

typedef struct {
  unsigned char cal_flag;
  unsigned int  pulse_period;
}Flow_stru;
unsigned char uart_analy(void);
void lock_ctrl_process(void);
#endif

