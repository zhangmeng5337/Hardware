#include "pid_controller.h"
#include <math.h>

void PID_Init(PIDController_t* pid, PIDConfig_t* cfg)
{
    // 参数赋值
    pid->Kp = cfg->Kp;
    pid->Ki = cfg->Ki;
    pid->Kd = cfg->Kd;
    pid->setpoint = cfg->setpoint;
    
    // 输出限幅
    pid->out_min = (cfg->out_min < 0.0f) ? 0.0f : (cfg->out_min > 1.0f) ? 1.0f : cfg->out_min;
    pid->out_max = (cfg->out_max < 0.0f) ? 0.0f : (cfg->out_max > 1.0f) ? 1.0f : cfg->out_max;
    if (pid->out_min >= pid->out_max) {
        pid->out_max = pid->out_min + 0.01f;
    }
    
    // 归一化配置
    pid->enable_normalization = cfg->enable_normalization;
    if (pid->enable_normalization) {
        // 设置缩放因子：将误差归一化到[-1,1]范围
        pid->error_scale = 1.0f / cfg->error_range;
        pid->derivative_scale = 1.0f / cfg->derivative_range;
        
        // 积分分离阈值也进行归一化
        pid->integral_sep_thresh = cfg->integral_sep_thresh * pid->error_scale;
    } else {
        pid->error_scale = 1.0f;
        pid->derivative_scale = 1.0f;
        pid->integral_sep_thresh = cfg->integral_sep_thresh;
    }
    
    // 积分限幅
    pid->integral_limit = (pid->out_max - pid->out_min) * cfg->integral_limit_ratio;
    
    // 配置标志
    pid->derivative_on_pv = cfg->derivative_on_pv;
    pid->anti_windup = cfg->anti_windup;
    
    // 状态清零
    pid->integral = 0.0f;
    pid->prev_pv = 0.0f;
    pid->prev_error = 0.0f;
    pid->last_output = 0.0f;
}

void PID_SetSetpoint(PIDController_t* pid, float setpoint)
{
    pid->setpoint = setpoint;
}

void PID_ResetIntegral(PIDController_t* pid, float init_val)
{
    pid->integral = init_val;
    if (pid->integral > pid->integral_limit) pid->integral = pid->integral_limit;
    if (pid->integral < -pid->integral_limit) pid->integral = -pid->integral_limit;
}

void PID_Reset(PIDController_t* pid)
{
    pid->integral = 0.0f;
    pid->prev_pv = 0.0f;
    pid->prev_error = 0.0f;
    pid->last_output = 0.0f;
}

void PID_SetErrorRange(PIDController_t* pid, float error_range)
{
    if (error_range > 0.0f && pid->enable_normalization) {
        pid->error_scale = 1.0f / error_range;
    }
}

void PID_SetDerivativeRange(PIDController_t* pid, float derivative_range)
{
    if (derivative_range > 0.0f && pid->enable_normalization) {
        pid->derivative_scale = 1.0f / derivative_range;
    }
}

float PID_Update(PIDController_t* pid, float pv, float dt)
{
    if (dt <= 0.0f) {
        return pid->last_output;
    }
    
    // 计算原始误差
    float raw_error = pid->setpoint - pv;
    
    // 归一化误差
    float error = raw_error;
    if (pid->enable_normalization) {
        error = raw_error * pid->error_scale;
        // 限幅到[-1,1]范围
        if (error > 1.0f) error = 1.0f;
        if (error < -1.0f) error = -1.0f;
    }
    
    // 1. 比例项
    float Pout = pid->Kp * error;
    
    // 2. 积分项
    float Iout = pid->integral;
    bool integrate = (fabsf(error) < pid->integral_sep_thresh);
    
    if (integrate) {
        float delta_integral = pid->Ki * error * dt;
        
        if (pid->anti_windup) {
            float output_before_limit = Pout + pid->integral;
            bool is_saturating = (output_before_limit >= pid->out_max && error > 0) ||
                                  (output_before_limit <= pid->out_min && error < 0);
            if (!is_saturating) {
                pid->integral += delta_integral;
            }
        } else {
            pid->integral += delta_integral;
        }
        
        // 积分限幅
        if (pid->integral > pid->integral_limit) pid->integral = pid->integral_limit;
        if (pid->integral < -pid->integral_limit) pid->integral = -pid->integral_limit;
        Iout = pid->integral;
    }
    
    // 3. 微分项
    float Dout = 0.0f;
    if (pid->derivative_on_pv) {
        // 微分先行：只对PV求导
        float derivative_raw = (pv - pid->prev_pv) / dt;
        float derivative = derivative_raw;
        if (pid->enable_normalization) {
            derivative = derivative_raw * pid->derivative_scale;
            // 限幅
            if (derivative > 1.0f) derivative = 1.0f;
            if (derivative < -1.0f) derivative = -1.0f;
        }
        Dout = -pid->Kd * derivative;
        pid->prev_pv = pv;
    } else {
        // 标准微分：对误差求导
        float derivative_raw = (raw_error - pid->prev_error) / dt;
        float derivative = derivative_raw;
        if (pid->enable_normalization) {
            derivative = derivative_raw * pid->error_scale;
            if (derivative > 1.0f) derivative = 1.0f;
            if (derivative < -1.0f) derivative = -1.0f;
        }
        Dout = pid->Kd * derivative;
        pid->prev_error = raw_error;
    }
    
    // 4. 总输出
    float output = Pout + Iout + Dout;
    
    // 5. 输出限幅
    if (output > pid->out_max) output = pid->out_max;
    if (output < pid->out_min) output = pid->out_min;
    
    pid->last_output = output;
    return output;
}

