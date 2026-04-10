// kalman_filter.h
#ifndef __KALMAN_FILTER_H
#define __KALMAN_FILTER_H

#include <stdint.h>
#include <math.h>

typedef struct {
    float x;        // 状态估计值（滤波后温度）
    float p;        // 估计误差协方差
    float q;        // 过程噪声协方差（系统模型误差）
    float r;        // 测量噪声协方差（传感器噪声）
    float k;        // 卡尔曼增益
    float last_dt;  // 上次时间间隔（秒）
} KalmanFilter_t;

void Kalman_Init(KalmanFilter_t *kf, float initial_temp, float q, float r);
float Kalman_Update(KalmanFilter_t *kf, float measurement, float dt);
void Kalman_Set_Q(KalmanFilter_t *kf, float q);
void Kalman_Set_R(KalmanFilter_t *kf, float r);

#endif

