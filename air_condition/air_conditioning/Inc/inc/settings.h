#ifndef SETTINGS_H_
#define SETTINGS_H_
#define NORMAL		        0x00
#define SETTING_MODE		0x01

#define MAX_MACHINE_FREQ    20
#define MIN_MACHINE_FREQ    10
#define MAX_WATER_LEVEL     5
#define MIN_WATER_LEVEL     1

#define COLD         1
#define HOT          2
#define COLD_HUM     3
#define FAST_COLD    4

#define FAN_RUN_TIME 2000


typedef struct {
  //unsigned char diplay_mode;
  unsigned char setting_bit;
  unsigned char cold_temper;
  unsigned char hot_temper;
  unsigned char humid;
  unsigned char present_mode;
  unsigned char present_status;
}params_stru;
params_stru *get_params_mode(void);
void run_process(void);

#endif
