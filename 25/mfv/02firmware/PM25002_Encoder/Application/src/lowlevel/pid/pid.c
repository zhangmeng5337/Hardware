#include "main.h"
struct _pid
{
    float setpoint;                 // 定义设定值
    float pv;              // 定义实际值
    float error;                    // 定义偏差值
    float err_k_1;              // 定义偏差值(k-1)
    float err_k_2;              // 定义偏差值(k-2)
    float err;
    float Kp, Ki, Kd;               // 定义比例、积分、微分系数
    float umax;
    float umin;
    float crack2;   //积分分离阈值
    float crack1;//积分项
    float cv;
    float increment;
    float interal;
    unsigned char controller_type;//0:increment 1:position
    unsigned char inter_enable;
} pid;                              // 定义了一个结构名为_pid的结构变量pid

void PID_init()
{
    pid.setpoint = 0.0;
    pid.pv = 0.0;
    pid.error = 0.0;
    pid.err_k_2 = 0.0;
    pid.err_k_1 = 0.0;
    pid.err = 0.0;
    pid.Kp = 6;                   // 需要自行整定
    pid.Ki = 0.1;                   // 注意，和普通增量，以及积分分离类型相比，这里加大了积分环节的值
    pid.Kd = 0;
    pid.interal = 0;
    pid.crack2 = 0.001; //
    pid.crack1 = 0;
    pid.inter_enable = 0;
    pid.umax = 1;
    pid.umin = 0;
    pid.cv = 0;
    pid.controller_type = 1;
    pid.increment = 0;

}

float PID_realize(float speed)
{
    float incrementSpeed;


    pid.setpoint = speed;
    pid.error = pid.setpoint - pid.pv;

    if (pid.pv > pid.umax)      // 抗积分饱和过程
    {
        // abs()是用来求int型的绝对值，fabsf()是用来求float型的绝对值，fabs()是用来求double型的绝对值，要用库math.h
        if (fabsf(pid.error) > pid.crack2)      // 积分分离过程
        {
            pid.inter_enable = 0;
        }
        else
        {
            pid.inter_enable = 1;
            if (pid.error < 0)
            {
                pid.err = pid.error;
                pid.interal  = pid.interal + pid.err;
                pid.crack1 = pid.interal;
            }
            else
            {
                pid.err = 0.0;
                pid.interal = 0;
            }
        }
    }
    else if (pid.pv < pid.umin) // 抗积分饱和过程
    {
        if (fabsf(pid.error) > pid.crack2)      // 积分分离过程
        {
            pid.inter_enable = 0;
        }
        else
        {
            pid.inter_enable = 1;
            if (pid.error > 0)
            {
                pid.err = pid.error;
                pid.interal  = pid.interal + pid.err;
                pid.crack1 = pid.interal;
            }
            else
            {
                pid.err = 0.0;
                pid.interal = 0;
            }
        }
    }
    else
    {
        if (fabsf(pid.error) > pid.crack2)
        {
            pid.inter_enable = 0;
        }
        else
        {
            pid.inter_enable = 1;
            pid.err = pid.error;
            pid.interal  = pid.interal + pid.err;
            pid.crack1 = pid.interal;
        }
    }
    if (pid.controller_type == 0)
    {
        pid.increment = pid.Kp * (pid.error - pid.err_k_1) + pid.inter_enable * pid.Ki *
                        pid.err + pid.Kd * (pid.error - 2 * pid.err_k_1 + pid.err_k_2);
        pid.cv +=  pid.increment;

    }
    else
    {
        pid.increment = pid.Kp * pid.error  + pid.inter_enable * pid.Ki * pid.interal +
                        pid.Kd * (pid.error - pid.err_k_1);
        pid.cv = pid.increment;

    }
    if (pid.cv > pid.umax)
        pid.cv = pid.umax;
    if (pid.cv < pid.umin)
        pid.cv = pid.umin;

    pid.err_k_2 = pid.err_k_1;
    pid.err_k_1 = pid.error;

    return pid.cv;
}


int compare(const void a, const void b)
{
    return ((int)a - (int)b);
}

// 中位值滤波函数
void medianFilter(int *signal, int *filtered, int length, int windowSize)
{
    int halfWindow = windowSize / 2;
    for (int i = halfWindow; i < length - halfWindow; i++)
    {
        int window[windowSize];
        for (int j = 0; j < windowSize; j++)
        {
            window[j] = signal[i - halfWindow + j];
        }
        qsort(window, windowSize, sizeof(int), compare); // 对窗口内元素进行排序
        filtered[i] = window[windowSize / 2]; // 取中值
    }
    // 处理边界点（这里简单处理为复制边界值，实际应用中可根据需求调整）
    for (int i = 0; i < halfWindow; i++)
    {
        filtered[i] = signal[i];
    }
    for (int i = length - halfWindow; i < length; i++)
    {
        filtered[i] = signal[length - 1 - i + halfWindow]; // 镜像处理边界
    }
}
// 平均滤波函数
void averageFilter(int *signal, int *filtered, int length, int windowSize)
{
    int halfWindow = windowSize / 2;
    for (int i = halfWindow; i < length - halfWindow; i++)
    {
        int sum = 0;
        for (int j = 0; j < windowSize; j++)
        {
            sum += signal[i - halfWindow + j];
        }
        filtered[i] = sum / windowSize;
    }
    // 处理边界点（同样简单处理为复制边界值）
    for (int i = 0; i < halfWindow; i++)
    {
        filtered[i] = signal[i];
    }
    for (int i = length - halfWindow; i < length; i++)
    {
        filtered[i] = signal[length - 1 - i + halfWindow]; // 镜像处理边界
    }
}
// 中位值平均滤波函数
void medianAverageFilter(int *signal, int *filtered, int length, int windowSize)
{
    int tempSignal = (int)malloc(length * sizeof(int));
    if (tempSignal == NULL)
    {
        printf(“Memory allocation failed\n”);
        exit(1);
    }
    // 首先进行中位值滤波
    medianFilter(signal, tempSignal, length, windowSize);
    // 然后对中位值滤波后的信号进行平均滤波
    averageFilter(tempSignal, filtered, length, windowSize);
    free(tempSignal); // 释放内存
}
————————————————
版权声明：本文为CSDN博主「盼海」的原创文章，遵循CC 4.0 BY - SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/u013050118/article/details/143477480

