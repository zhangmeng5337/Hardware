#ifndef SREP_MOTOR_H
#define  SREP_MOTOR_H
#include "main.h"
#define MAX_STEP_YAW 		4350
#define MAX_STEP_PITCH 		1087
#define MOTOR_PHASE_DELAY       2
#define POSITIVE		1
#define PITCH			0
#define YAW			1
#define FORWARD			0
#define BACKWARD                1
#define DIRECTION_INDEX         0x02
#define MOTOR_NUM_INDEX         0x01
#define ANGLE_H_INDEX           0x03
#define ANGLE_L_INDEX           0x04
#define RGB_INDEX               0x05
#define RGB_RED_ON		0x11
#define RGB_BLUE_ON		0x22
#define RGB_GREEN_ON		0x33
#define RGB_ALL_OFF		0xff

#define CHECKSUM_INDEX          0x06
#define TAG_INDEX               0x07
#define TAG			0x55
#define HEADER		        0xaa

void rgb_control(unsigned char color);
void step_motor(float angle,unsigned char motor_no,unsigned char direction);
void Receive_command(unsigned char revdat);
void uart_analysis(void);
void uart_test(void);
#endif
/****************************************************
data format: Header    MotorNo             Diretion      Angle_H  Angle_L  rgbled   Checksum   tag
              0xaa    YAW/PITCH     FORWARD/BACKWARD

*****************************************************/
