#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * 一维卡尔曼滤波器
 * 适用于温度信号滤波，能够自适应地估计真实值
 */
typedef struct {
    // 状态估计
    float x;        // 状态估计值（滤波后的值）
    float P;        // 估计误差协方差
    
    // 系统参数
    float Q;        // 过程噪声协方差
    float R;        // 测量噪声协方差
    float A;        // 状态转移矩阵
    float H;        // 观测矩阵
} KalmanFilter_t;

/**
 * 卡尔曼滤波器配置参数
 */
typedef struct {
    float Q;        // 过程噪声协方差（典型值：0.001~0.1）
    float R;        // 测量噪声协方差（典型值：0.1~5.0）
    float init_x;   // 初始状态估计
    float init_P;   // 初始估计误差协方差（典型值：1.0~100.0）
} KalmanConfig_t;

/**
 * 初始化卡尔曼滤波器
 * @param kf  滤波器实例指针
 * @param cfg 配置参数
 */
void Kalman_Init(KalmanFilter_t* kf, KalmanConfig_t* cfg);

/**
 * 更新卡尔曼滤波器
 * @param kf           滤波器实例指针
 * @param measurement  测量值
 * @param dt           采样时间间隔（秒）
 * @return             滤波后的估计值
 */
float Kalman_Update(KalmanFilter_t* kf, float measurement, float dt);

#endif

