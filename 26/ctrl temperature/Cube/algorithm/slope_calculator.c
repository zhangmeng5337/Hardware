#include "slope_calculator.h"
#include <math.h>

void Slope_Init(SlopeCalculator_t* sc, SlopeConfig_t* cfg)
{
    sc->prev_temp = 0.0f;
    sc->dt = (cfg->dt > 0.001f) ? cfg->dt : 0.001f;
    sc->deadband = cfg->deadband;
    sc->slope = 0.0f;
    
    // 初始化斜率卡尔曼滤波器
    sc->enable_filter = cfg->enable_slope_filter;
    if (sc->enable_filter) {
        Kalman_Init(&sc->slope_filter, &cfg->slope_kalman_cfg);
    }
}

float Slope_Update(SlopeCalculator_t* sc, float current_temp)
{
    // 计算原始斜率
    float raw_slope = (current_temp - sc->prev_temp) / sc->dt;
    sc->prev_temp = current_temp;
    
    // 应用死区（消除小波动）
    float deadband_slope;
    if (fabsf(raw_slope) < sc->deadband) {
        deadband_slope = 0.0f;
    } else {
        deadband_slope = raw_slope;
    }
    
    // 可选卡尔曼滤波
    if (sc->enable_filter) {
        sc->slope = Kalman_Update(&sc->slope_filter, deadband_slope, sc->dt);
    } else {
        sc->slope = deadband_slope;
    }
    
    return sc->slope;
}

