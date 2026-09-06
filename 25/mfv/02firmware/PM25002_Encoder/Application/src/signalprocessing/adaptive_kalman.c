/* adaptive_kalman.c */
#include "adaptive_kalman.h"
#include <string.h>   // for memset

// 辅助函数：计算滑动窗口内数据的标准差
static float sliding_std(float* buffer, int len, int filled) {
    if (len <= 0) return 0.0f;
    int actual_len = filled ? len : (len - (MAX_WINDOW - filled)); // 但这里简化：直接使用filled计数
    // 更简单：遍历有效数据
    int n = (filled == len) ? len : (len - (MAX_WINDOW - filled));
    if (n < 2) return 0.0f;
    
    float sum = 0.0f;
    for (int i = 0; i < n; i++) {
        sum += buffer[i];
    }
    float mean = sum / n;
    float sq_sum = 0.0f;
    for (int i = 0; i < n; i++) {
        float diff = buffer[i] - mean;
        sq_sum += diff * diff;
    }
    return sqrtf(sq_sum / (n - 1)); // 样本标准差
}

void AdaptiveKalman_Init(AdaptiveKalman* kf, 
                         float init_x, float init_P,
                         float R, 
                         float Q_min, float Q_max,
                         float std_min, float std_max,
                         float Q_rate_max) {
    kf->x = init_x;
    kf->P = init_P;
    kf->R = R;
    kf->Q_min = Q_min;//0.0001
    kf->Q_max = Q_max;//0.5
    kf->Q = Q_min;   // 初始使用最小Q
    kf->std_min = std_min;//0.00001
    kf->std_max = std_max;//0.5 
    kf->Q_rate_max = Q_rate_max;
//    kf->smoothing = smoothing;
//    kf->window_len = (window_len > MAX_WINDOW) ? MAX_WINDOW : window_len;
//    kf->window_idx = 0;
//    kf->buffer_filled = 0;
//    memset(kf->residual_buffer, 0, sizeof(kf->residual_buffer));
}

void AdaptiveKalman_Update(AdaptiveKalman* kf, float input,float *out) {


    // 1. 预测
    float x_pred = kf->x;
    float P_pred = kf->P + kf->Q;
    
    // 2. 新息
     float residual = input - x_pred;


	// Update 函数中
	// 更新残差方差（指数加权）
	float residual_sq = residual * residual;
	kf->residual_var = kf->var_alpha * kf->residual_var + (1.0f - kf->var_alpha) * residual_sq;
	 kf->local_std = sqrtf(kf->residual_var);

 
    
    // 3. 瞬时自适应调整 Q（无窗口，无延迟）
    float std_meas = sqrtf(kf->R);          // 测量噪声标准差
    float abs_res = fabsf(residual);
    // 归一化：残差相对于测量噪声的倍数
    float ratio = abs_res / std_meas;
    // 当 ratio < 3 时认为正常噪声，Q 取最小；ratio >= 10 时 Q 取最大；中间线性映射
     kf->norm = (kf->local_std - kf->std_min) / (kf->std_max - kf->std_min);
    if (kf->norm < 0.0f) kf->norm = 0.0f;
    if (kf->norm > 1.0f) kf->norm = 1.0f;

	// smoothstep: f(x) = 3x^2 - 2x^3，使过渡更平滑
	 float norm_smooth = kf->norm * kf->norm * (3.0f - 2.0f * kf->norm);
	 kf->Q_target = kf->Q_min + (kf->Q_max - kf->Q_min) * norm_smooth;
	 
	 // 5. 对 Q 的变化施加速率限制（关键：防止 Q 突变）
	 float Q_delta = kf->Q_target - kf->Q;
	 if (Q_delta > kf->Q_rate_max) Q_delta = kf->Q_rate_max;
	 if (Q_delta < -kf->Q_rate_max) Q_delta = -kf->Q_rate_max;
	 kf->Q = kf->Q + Q_delta;
	 // 限幅
	 if (kf->Q < kf->Q_min) kf->Q = kf->Q_min;
	 if (kf->Q > kf->Q_max) kf->Q = kf->Q_max;

	
//    kf->Q_target = kf->Q_min + (kf->Q_max - kf->Q_min) * kf->norm;
    // 平滑更新（可选）
//    kf->Q = kf->smoothing * kf->Q_target + (1.0f - kf->smoothing) * kf->Q;
    
    // 4. 卡尔曼更新
     kf->S = P_pred + kf->R;
     kf->K = P_pred / kf->S;
    float x_est = x_pred + kf->K * residual;
     kf->P_est = (1.0f - kf->K) * P_pred;
    
    kf->x = x_est;
    kf->P = kf->P_est;
    *out = x_est;
}

