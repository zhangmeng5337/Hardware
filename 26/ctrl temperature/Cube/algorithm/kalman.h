// kalman_filter.h
#ifndef __KALMAN_FILTER_H
#define __KALMAN_FILTER_H

#include <stdint.h>
#include <math.h>
// ==================== 卡尔曼滤波器模块 ====================
// 一维卡尔曼滤波器，适用于温度信号滤波
typedef struct {
    // 状态估计
    float x;        // 状态估计值（滤波后的温度）
    float P;        // 估计误差协方差
    
    // 系统参数
    float Q;        // 过程噪声协方差（描述系统动态不确定性）
    float R;        // 测量噪声协方差（描述传感器噪声）
    
    // 系统模型参数（假设温度变化模型）
    float A;        // 状态转移矩阵（默认1.0）
    float H;        // 观测矩阵（默认1.0）
} KalmanFilter_t;

// 卡尔曼滤波器配置参数
typedef struct {
    float Q;        // 过程噪声协方差（典型值：0.01~1.0）
    float R;        // 测量噪声协方差（典型值：0.1~10.0）
    float init_x;   // 初始状态估计（初始温度）
    float init_P;   // 初始估计误差协方差（典型值：1.0~100.0）
} KalmanConfig_t;

void Kalman_Init(KalmanFilter_t* kf, KalmanConfig_t* cfg);
float Kalman_Update(KalmanFilter_t* kf, float measurement, float dt);

#endif

