#ifndef PID_H_
#define PID_H_
#include "main.h"
#define PID_MAX   5000


typedef struct {
    float SetSpeed;            //�����趨ֵ
    float ActualSpeed;        //����ʵ��ֵ
    float err;                //����ƫ��ֵ
    float err_last;            //������һ��ƫ��ֵ
    float Kp,Ki,Kd;            //������������֡�΢��ϵ��
    float voltage;             //�������ִ�����ı���
    float integral;            //�������ֵ
    float umax;
    float umin;
}pid_stru;

float PID_realize(float speed);
void PID_init(void);


#endif

