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

	if (output > MAX_SPEED)
	{
		output = MAX_SPEED;
	}
	else if (output < -MAX_SPEED)
	{
		output = -MAX_SPEED;
	}

	pid->prevError = error;
  return output;
}
