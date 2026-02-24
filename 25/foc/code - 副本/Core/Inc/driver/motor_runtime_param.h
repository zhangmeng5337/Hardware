#ifndef MOTOR_RUNTIME_PARAM_H_
#define MOTOR_RUNTIME_PARAM_H_

#include "main.h"

#include "config.h"



typedef struct
{
	float motor_i_u;
	float motor_i_v;
	float motor_i_w;

	float motor_i_d;
	float motor_i_q;
	float motor_speed;
	float motor_logic_angle;
	float encoder_angle;
	float rotor_zero_angle;
	
	float rotor_phy_angle;
	float rotor_logic_angle;
	int angle_raw;
    
	float motor_Ospeed;
    unsigned char status;//1:stop 2:down limitation 3:uplimitation
}motor_runtime_stru;

//extern float motor_i_u;
//extern float motor_i_v;
//extern float motor_i_d;
//extern float motor_i_q;
//extern float motor_speed;
//extern float motor_logic_angle; // 电机多圈角度
//extern float encoder_angle;     // 编码器直接读出的角度
//extern float rotor_zero_angle;  // 转子d轴与线圈d轴重合时的编码器角度
motor_runtime_stru *get_motor_runtime(void);
void adc_init(void);

#endif


