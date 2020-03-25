#include "pid.h"

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

	// Limit the maximum output
	if (output > MAX_PID_OUTPUT)
	{
		output = MAX_PID_OUTPUT;
	}
	else if (output < -MAX_PID_OUTPUT)
	{
		output = -MAX_PID_OUTPUT;
	}

	pid->prevError = error;
  return output;
}