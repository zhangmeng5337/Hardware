#include "kalman.h"

// ==================== 卡尔曼滤波器模块 ====================
void Kalman_Init(KalmanFilter_t* kf, KalmanConfig_t* cfg) {
    // 设置系统模型（假设温度变化缓慢，A=1）
    kf->A = 1.0f;
    kf->H = 1.0f;
    
    // 设置噪声协方差
    kf->Q = cfg->Q;
    kf->R = cfg->R;
    
    // 初始化状态估计
    kf->x = cfg->init_x;
    kf->P = cfg->init_P;
}

float Kalman_Update(KalmanFilter_t* kf, float measurement, float dt) {
    // 预测步骤
    // 状态预测: x_pred = A * x
    float x_pred = kf->A * kf->x;
    
    // 协方差预测: P_pred = A * P * A + Q
    // 注意：这里考虑了时间间隔dt对过程噪声的影响
    float P_pred = kf->A * kf->P * kf->A + kf->Q * dt;
    
    // 更新步骤
    // 卡尔曼增益: K = P_pred * H / (H * P_pred * H + R)
    float K = P_pred * kf->H / (kf->H * P_pred * kf->H + kf->R);
    
    // 状态更新: x = x_pred + K * (measurement - H * x_pred)
    kf->x = x_pred + K * (measurement - kf->H * x_pred);
    
    // 协方差更新: P = (1 - K * H) * P_pred
    kf->P = (1.0f - K * kf->H) * P_pred;
    
    return kf->x;
}


