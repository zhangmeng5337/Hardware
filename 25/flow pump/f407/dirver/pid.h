#ifndef __BSP_PID_H
#define	__BSP_PID_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    float target_val;           //目标值
    float actual_val;        		//实际值
    float err;             			//定义偏差值
    float err_last;          		//定义上一个偏差值
    float Kp;
	float Ki;
	float Kd;          		//定义比例、积分、微分系数
	float pterm;
	float iterm;
	float dterm;
    float integral;          		//定义积分值
    float outh;
	float outL;
}_pid;

void  PID_param_init(void);
void  set_pid_target(float temp_val);
float get_pid_target(void);
void  set_p_i_d(float p, float i, float d);
float PID_realize(float actual_val);
void  time_period_fun(void);

#endif

