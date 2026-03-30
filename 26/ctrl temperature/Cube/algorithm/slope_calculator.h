#ifndef SLOPE_CALCULATOR_H
#define SLOPE_CALCULATOR_H

#include <stdint.h>
#include <stdbool.h>
#include "kalman.h"

/**
 * 斜率计算器
 * 计算温度变化速率，支持死区处理和可选卡尔曼滤波
 */
typedef struct {
    float prev_temp;            // 上一次温度
    float dt;                   // 采样周期（秒）
    float deadband;             // 斜率死区（℃/s）
    float slope;                // 当前斜率（℃/s）
    
    // 可选斜率滤波
    KalmanFilter_t slope_filter;
    bool enable_filter;
} SlopeCalculator_t;

/**
 * 斜率计算器配置参数
 */
typedef struct {
    float dt;                   // 采样周期（秒）
    float deadband;             // 斜率死区（℃/s）
    
    // 斜率滤波配置
    bool enable_slope_filter;
    KalmanConfig_t slope_kalman_cfg;
} SlopeConfig_t;

/**
 * 初始化斜率计算器
 * @param sc  斜率计算器实例指针
 * @param cfg 配置参数
 */
void Slope_Init(SlopeCalculator_t* sc, SlopeConfig_t* cfg);

/**
 * 更新斜率计算器
 * @param sc            斜率计算器实例指针
 * @param current_temp  当前温度（已滤波）
 * @return              当前斜率（℃/s）
 */
float Slope_Update(SlopeCalculator_t* sc, float current_temp);

#endif

