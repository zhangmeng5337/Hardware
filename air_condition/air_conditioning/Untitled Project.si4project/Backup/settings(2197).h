#ifndef SETTINGS_H_
#define SETTINGS_H_
#define NORMAL		        0x00
#define SETTING_MODE		0x01
#define MAX_MODE_NO			0x05
#define WORK_ON				3
#define WORK_OFF			2
#define POWER_ON			1
#define POWER_OFF			0
#define MAX_SET_TEMPER		85
#define MIN_SET_TEMPER      10
#define MAX_ERROR_CA		5
#define MIDU_ERROR_CA		0.5
#define MID_ERROR_CA		0
#define MIND_ERROR_CA		-0.5

#define MAX_ERROR_CB	2
#define MID_ERROR_CB	1
#define MIN_ERROR_CB	0

#define MAX_MACHINE_FREQ    20
#define MIN_MACHINE_FREQ    10
#define MAX_WATER_LEVEL     5
#define MIN_WATER_LEVEL     1

#define COLD         1
#define HOT          2
#define COLD_HUM     3
#define FAST_COLD    4
#define RECYCLE_DISPLAY    5

#define FAN_RUN_TIME 2000


typedef struct {
  //unsigned char diplay_mode;
  unsigned char setting_bit;
  unsigned char target_set_temper;
  unsigned char target_environment_temper;
  unsigned char equip_environment_temper;
  unsigned char machine_air_temper;
  unsigned char target_wind_temper;

  unsigned char humid;
  unsigned char present_mode;
  unsigned char present_status;
}params_stru;

typedef struct {
  //unsigned char diplay_mode;
  unsigned char mode;
  unsigned char modeNo;
  unsigned char status;
  unsigned char last_mode_no;
}mode_stru;

params_stru *get_params_mode(void);
void run_process(void);

#endif
