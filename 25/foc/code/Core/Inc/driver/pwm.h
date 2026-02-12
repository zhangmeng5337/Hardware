#ifndef PWM_H_
#define PWM_H_
#include "main.h"
#define START 0
#define STOP 1
#define UP_L  3
#define DPWN_L 2

void pwm_init(void);
void set_pwm_duty(float d_u, float d_v, float d_w);

void pwm_ctrl(unsigned char mode);

#endif

