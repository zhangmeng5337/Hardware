#ifndef SETTINGS_H_
#define SETTINGS_H_
#define NORMAL		        0x00
#define SETTING_MODE		0x01


typedef struct {
  unsigned char diplay_mode;
  unsigned char setting_bit;
}params_stru;
params_stru *get_params_mode(void);

#endif
