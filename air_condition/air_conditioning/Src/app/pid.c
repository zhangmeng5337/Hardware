#include "pid.h"
#include "settings.h"
/* PID calculation routine */
int16_t calcPID(PID *pid, int16_t error)
{
	int16_t output;

	if (pid->Ki != 0)
	{
		pid->integrationError += error;
		// Limit the maximum integration error
		if (pid->integrationError > MAX_INTEGRATION_ERROR)
		{
			pid->integrationError = MAX_INTEGRATION_ERROR;
		}
		else if (pid->integrationError < -MAX_INTEGRATION_ERROR)
		{
			pid->integrationError = -MAX_INTEGRATION_ERROR;
		}
	}

	output = pid->Kp * error + pid->Ki * pid->integrationError + pid->Kd * (error - pid->prevError);

	// Limit the maximum output#define MAX_SPEED			5000

	if (output > MAX_PID_OUTPUT)
	{
		output = MAX_PID_OUTPUT;
	}

	else if(output < MIN_PID_OUTPUT)
	{
		output = MIN_PID_OUTPUT;
	}
	output=36*output/10+2000;

	pid->prevError = error;
  return output;
}
