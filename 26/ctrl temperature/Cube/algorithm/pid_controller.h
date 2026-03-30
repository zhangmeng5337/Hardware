#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * PID控制器
 * 支持输入归一化，使PID参数与量纲解耦
 */
typedef struct {
    // 参数
    float Kp, Ki, Kd;           // PID参数（无量纲，基于归一化误差）
    float setpoint;             // 目标值（原始量纲）
    float out_min, out_max;     // 输出限幅（0-1范围）
    float integral_sep_thresh;  // 积分分离阈值（归一化后的误差阈值）
    float integral_limit;       // 积分项限幅
    
    // 归一化参数
    float error_scale;          // 误差缩放因子（1/量程，将误差归一化到[-1,1]）
    float derivative_scale;     // 微分缩放因子（用于速率归一化）
    
    // 内部状态
    float integral;             // 积分累计值（归一化后）
    float prev_pv;              // 上一次测量值（原始量纲）
    float prev_error;           // 上一次误差（原始量纲）
    float last_output;          // 上一次输出
    
    // 配置标志
    uint8_t derivative_on_pv;   // 1=微分先行，0=标准微分
    bool anti_windup;           // 是否启用遇限削弱积分
    bool enable_normalization;  // 是否启用归一化
} PIDController_t;

/**
 * PID控制器配置参数
 */
typedef struct {
    float Kp, Ki, Kd;           // PID参数（无量纲）
    float setpoint;             // 目标值
    float out_min, out_max;     // 输出限幅（0-1范围）
    float integral_sep_thresh;  // 积分分离阈值（原始量纲，内部会归一化）
    float integral_limit_ratio; // 积分限幅比例（相对于输出范围）
    
    // 归一化配置
    bool enable_normalization;  // 是否启用归一化
    float error_range;          // 误差范围（用于归一化，例如温度范围100℃）
    float derivative_range;     // 微分范围（用于归一化，例如速率范围5℃/s）
    
    uint8_t derivative_on_pv;   // 微分方式：1=微分先行，0=标准微分
    bool anti_windup;           // 是否启用抗饱和
} PIDConfig_t;

void PID_Init(PIDController_t* pid, PIDConfig_t* cfg);
void PID_SetSetpoint(PIDController_t* pid, float setpoint);
void PID_ResetIntegral(PIDController_t* pid, float init_val);
void PID_Reset(PIDController_t* pid);
float PID_Update(PIDController_t* pid, float pv, float dt);
void PID_SetErrorRange(PIDController_t* pid, float error_range);
void PID_SetDerivativeRange(PIDController_t* pid, float derivative_range);

#endif

