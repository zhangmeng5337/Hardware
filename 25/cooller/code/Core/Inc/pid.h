#ifndef PID_H
#define PID_H
#define PID_TYPE  2
#define PID_O_MAX   100

#define PID_I_MAX   150
#define PID_SI_MAX   8500000
#define PID_SI_MIN   -300
#define MAX_OUTPUT  45000
#define MIN_OUTPUT  0
#include<main.h>
typedef struct PID
{
    double NextPoint; // 设定目标Desired value

    double SetPoint; // 设定目标Desired value
    double Proportion; // 比例常数Proportional Const
    double Integral; // 积分常数Integral Const
    double Derivative; // 微分常数Derivative Const
    double LastError; // Error[-1]
    double result;
    double  iError; //当前误差
    double Current_Error;//当前误差
    double Last_Error;//上一次误差
    double Previous_Error;//上上次误差
    double PrevError; // Error[-2]
    double SumError; // Sums of Errors
    double  Increase;   //最后得出的实际增量
    unsigned char dir_cnt_pos;
    unsigned char dir_cnt_neg;

	double rOut,rOut_pump; // PID Response (Output)
	double rIn_PID,rIn_PID_pump; // PID Feedback (Input)


	
} PID;
uint32_t pid_proc(double rIn);
void pid_init(float setvalue, float heat_T);

void PIDInit(PID *pp);

uint32_t pid_proc_fan_heat(unsigned char heater_num,double rIn);


#endif

