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
typedef struct PID {
double NextPoint; // �趨Ŀ��Desired value

double SetPoint; // �趨Ŀ��Desired value
double Proportion; // ��������Proportional Const
double Integral; // ���ֳ���Integral Const
double Derivative; // ΢�ֳ���Derivative Const
double LastError; // Error[-1]
double result;
double  iError;	//��ǰ���
    double Current_Error;//��ǰ���
    double Last_Error;//��һ�����
    double Previous_Error;//���ϴ����
double PrevError; // Error[-2]
double SumError; // Sums of Errors
	double	Increase;	//���ó���ʵ������
	unsigned char dir_cnt_pos;
	unsigned char dir_cnt_neg;
} PID;
uint32_t pid_proc(double rIn);
void pid_init(float setvalue);
void PIDInit (PID *pp);
void pid_init_flow(float setvalue);
void pid_init2(float setvalue);
uint32_t pid_proc_pump(double rIn);



#endif

