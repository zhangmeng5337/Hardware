#ifndef SETTINGS_H_
#define SETTINGS_H_
#define NORMAL		        0x00
#define SETTING_MODE		0x01
#define MAX_MODE_NO			RECYCLE_DISPLAY

#define MAX_SPEED			5000
#define MIN_SPEED			3000
#define MAX_SET_TEMPER		85
#define MIN_SET_TEMPER      10
#define MAX_SET_HUM		85
#define MIN_SET_HUM      10
#define MAX_MACHINE_AIRT  70

#define INIT_SET_TEMPER      MIN_SET_TEMPER
#define INIT_SET_HUM         MIN_SET_HUM

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

#define WORK_ON				3
#define POWER_ON			2
#define WORK_OFF			1
#define POWER_OFF			0

#define STOP_MODE        0
#define STAND_MODE       1
#define COLD             2
#define HOT              3
#define COLD_HUM_COLD    4
#define COLD_HUM_HUM     5
#define FAST_COLD        6
#define FAST_COLD_HUM    7
#define RECYCLE_DISPLAY  8


#define FAN_RUN_TIME       2000
#define TX_COMMAND_TIME    60*1000


typedef struct {
  //unsigned char diplay_mode;
  unsigned char setting_bit;
  float tar_set_temper[4];//0:target cold temp;1:target hot temp;2:target cold and humid temp;3:fast colde temp; 
  float tar_env_temper;//4:target cold and humid humid 5:fast humid
  float equip_env_temper;
  float machine_air_temper;
  float target_wind_temper;
  unsigned int  speed;
  unsigned int  current;
  unsigned char humid[2];
  unsigned char present_mode;
  unsigned char present_status;
}params_stru;

typedef struct {
  //unsigned char diplay_mode;
  unsigned char mode;//normal:setting
  unsigned char modeNo;
  unsigned char status;//power on power off work on work off
  unsigned char manual;
  unsigned char last_mode_no;
  unsigned char flag_mark;
}mode_stru;

mode_stru *get_params_mode(void);
void run_process(void);
void init_params(void);

#endif
