#ifndef _PID_H_
#define _PID_H_

#include "main.h"

#define MAX_PID_OUTPUT					950
#define MAX_INTEGRATION_ERROR		100

typedef struct {
	int16_t targetValue;

	int16_t Kp;
	int16_t Ki;
	int16_t Kd;

	int16_t integrationError;
	int16_t prevError;
} PID;

int16_t  calcPID(PID *pid, int16_t error);

#endif
