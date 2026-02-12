#ifndef PID_H_
#define PID_H_
#include "main.h"
#define PID_MAX   5000


typedef struct {
    float SetSpeed;            //定义设定值
    float ActualSpeed;        //定义实际值
    float err;                //定义偏差值
    float err_last;            //定义上一个偏差值
    float Kp,Ki,Kd;            //定义比例、积分、微分系数
    float voltage;             //定义控制执行器的变量
    float integral;            //定义积分值
    float umax;
    float umin;
}pid_stru;

float PID_realize(float speed);
void PID_init(void);


#endif

