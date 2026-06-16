/* adaptive_kalman.h */
#ifndef ADAPTIVE_KALMAN_H
#define ADAPTIVE_KALMAN_H

#include <stdint.h>
#include <math.h>

// 最大滑动窗口长度（可根据内存调整）
#define MAX_WINDOW 50

// 自适应卡尔曼滤波器结构体
typedef struct {
    // 状态
    float x;          // 当前估计值
    float P;          // 估计协方差
    
    // 固定参数
    float R;          // 测量噪声协方差（已知或标定）
    // 自适应参数
    float Q;          // 当前使用的Q值
    float std_min;    // 残差标准差下界（低于此值认为平稳）
    float std_max;    // 残差标准差上界（高于此值认为剧烈波动）
    float var_alpha;
	float Q_rate_max;
    float Q_min;      // 最小过程噪声（最平滑）
    float Q_max;      // 最大过程噪声（最快响应）	
    

    float norm;
	float Q_target;
	float S;
	float K;
	float P_est;
	float local_std;
	float residual_var;

} AdaptiveKalman;

// 初始化函数
void AdaptiveKalman_Init(AdaptiveKalman* kf, 
                         float init_x, float init_P,
                         float R, 
                         float Q_min, float Q_max,
                         float std_min, float std_max,
                         float Q_rate_max);

// 更新函数：输入测量值z，输出滤波后的估计值

void AdaptiveKalman_Update(AdaptiveKalman* kf, float input,float *out);

#endif

