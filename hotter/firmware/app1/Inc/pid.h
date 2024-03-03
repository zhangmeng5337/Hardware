#ifndef PID_H_
#define PID_H_

#define MACHINE_MAX_T 		58
#define MACHINE_MIN_T		10

#define PID_MAX 			1000
#define PID_MIN				-200

#define MACHINE_RATIO  (MACHINE_MAX_T-MACHINE_MIN_T)/(PID_MAX-PID_MIN)
#define MACHINE_RATIO_B MACHINE_MIN_T- MACHINE_MIN_T*MACHINE_RATIO

#define MACHINE_IN_T	25
typedef struct  
{
	float max_temp;
	float temp_error;
	float temp_average;
	float low_temp_percent;
	float output;
} indoor_temp_stru;
float get_pid_output(void);
void fuzzy_proc(unsigned char mode);
void pid_init(void);


#endif

