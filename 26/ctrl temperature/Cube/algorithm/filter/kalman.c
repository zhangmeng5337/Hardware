#include "kalman_filter.h"
//#include "main.h"
//#define PERIOD_SAMPLE  0.1f
//typedef struct
//{
//  float temperatureOri;
//  float temperatureTarget;
//  float pwmFraction;
//  float periodMeter;
//  
//}temperatureStru;
//void controllerInit(void);

void Kalman_Init(KalmanFilter_t* kf, KalmanConfig_t* cfg)
{
    kf->A = 1.0f;           // 状态转移矩阵（温度变化缓慢）
    kf->H = 1.0f;           // 观测矩阵（直接观测温度）
    kf->Q = cfg->Q;
    kf->R = cfg->R;
    kf->x = cfg->init_x;
    kf->P = cfg->init_P;
}

float Kalman_Update(KalmanFilter_t* kf, float measurement, float dt)
{
    // ---------- 预测步骤 ----------
    // 状态预测: x_pred = A * x
    float x_pred = kf->A * kf->x;
    
    // 协方差预测: P_pred = A * P * A + Q * dt
    float P_pred = kf->A * kf->P * kf->A + kf->Q * dt;
    
    // ---------- 更新步骤 ----------
    // 卡尔曼增益: K = P_pred * H / (H * P_pred * H + R)
    float K = P_pred * kf->H / (kf->H * P_pred * kf->H + kf->R);
    
    // 状态更新: x = x_pred + K * (measurement - H * x_pred)
    kf->x = x_pred + K * (measurement - kf->H * x_pred);
    
    // 协方差更新: P = (1 - K * H) * P_pred
    kf->P = (1.0f - K * kf->H) * P_pred;
    
    return kf->x;
}

