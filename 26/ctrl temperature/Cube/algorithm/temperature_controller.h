#ifndef TEMPERATURE_CONTROLLER_H
#define TEMPERATURE_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>
#include "kalman.h"
#include "slope_calculator.h"
#include "pid_controller.h"

/**
 * 控制模式
 */
typedef enum {
    CTRL_MODE_FULL,     // 全速加热模式（温差大时）
    CTRL_MODE_RATE,     // 速率控制模式（升温阶段）
    CTRL_MODE_TEMP      // 温度PID模式（稳态阶段）
} ControlMode_t;

/**
 * 温区自适应参数
 */
typedef struct {
    float temp_low;         // 温区下限
    float temp_high;        // 温区上限
    float kp_factor;        // Kp系数
    float ki_factor;        // Ki系数
    float kd_factor;        // Kd系数
    float rate_factor;      // 目标速率系数
    float full_power_factor; // 全功率阈值系数
    float switch_factor;    // 切换阈值系数
} ZoneParam_t;

/**
 * 混合控制器
 */
typedef struct {
    // 卡尔曼滤波器
    KalmanFilter_t kalman_filter;
    
    // PID控制器
    PIDController_t rate_pid;       // 速率控制PID
    PIDController_t temp_pid;       // 温度控制PID
    
    // 斜率计算器
    SlopeCalculator_t slope_calc;
    
    // 目标参数
    float target_temp;
    float target_rate;
    float switch_threshold;
    float full_power_threshold;
    
    // 归一化范围
    float temp_error_range;         // 温度误差归一化范围（℃）
    float rate_error_range;         // 速率误差归一化范围（℃/s）
    
    // 基准参数
    float base_target_rate;
    float base_switch_threshold;
    float base_full_power_threshold;
    
    // 温区自适应
    ZoneParam_t zones[7];
    int current_zone;
    bool adaptive_enabled;
    
    // PID基准参数
    float base_kp_rate, base_ki_rate, base_kd_rate;
    float base_kp_temp, base_ki_temp, base_kd_temp;
    
    // 模式管理
    ControlMode_t mode;
    ControlMode_t last_mode;
    
    // 输出限制
    float output_rate_limit;
    float last_output;
    
    // 切换条件
    bool require_rate_zero;
    float rate_zero_thresh;
    float early_switch_factor;
    
    // 状态监控
    float filtered_temp;
    float current_rate;
    float temp_error;
    bool switch_ready;
    bool first_rate_entry;
    int rate_mode_entry_count;
    
    // 过冲预防
    float peak_temp;
    bool overshoot_detected;
    
    // 低温保护
    bool low_temp_protection;
    float min_heating_power;
    
    // 目标变化检测
    float last_target_temp;
    bool target_changed;
} HybridController_t;

/**
 * 配置参数
 */
typedef struct {
    // 目标参数
    float target_temp;
    float target_rate;
    float switch_threshold;
    float full_power_threshold;
    
    // 归一化范围
    float temp_error_range;         // 温度误差归一化范围（建议100℃）
    float rate_error_range;         // 速率误差归一化范围（建议5℃/s）
    
    // 温区自适应
    bool adaptive_enabled;
    ZoneParam_t zones[7];
    
    // PID基准参数（无量纲，基于归一化误差）
    float base_kp_rate;
    float base_ki_rate;
    float base_kd_rate;
    float base_kp_temp;
    float base_ki_temp;
    float base_kd_temp;
    
    // 基准目标参数
    float base_target_rate;
    float base_switch_threshold;
    float base_full_power_threshold;
    
    // 卡尔曼滤波器配置
    KalmanConfig_t kalman_cfg;
    
    // 速率PID配置（基准值）
    PIDConfig_t rate_pid_cfg;
    
    // 温度PID配置（基准值）
    PIDConfig_t temp_pid_cfg;
    
    // 斜率计算配置
    SlopeConfig_t slope_cfg;
    
    // 输出限制
    float output_rate_limit;
    
    // 切换条件
    bool require_rate_zero;
    float rate_zero_thresh;
    float early_switch_factor;
    
    // 低温保护
    bool low_temp_protection;
    float min_heating_power;
} HybridConfig_t;

// 函数声明
void Hybrid_Init(HybridController_t* hc, HybridConfig_t* cfg);
float Hybrid_Update(HybridController_t* hc, float pv_raw, float dt);
void Hybrid_SetTargetTemp(HybridController_t* hc, float target_temp);
void Hybrid_SetTargetRate(HybridController_t* hc, float target_rate);
void Hybrid_ResetStateMachine(HybridController_t* hc);
ControlMode_t Hybrid_GetMode(HybridController_t* hc);
const char* Hybrid_GetModeName(ControlMode_t mode);
float Hybrid_GetCurrentRate(HybridController_t* hc);
float Hybrid_GetFilteredTemp(HybridController_t* hc);
float Hybrid_GetTempError(HybridController_t* hc);
void Hybrid_SetAdaptiveEnabled(HybridController_t* hc, bool enabled);
int Hybrid_GetCurrentZone(HybridController_t* hc);
#include "main.h"
#define PERIOD_SAMPLE  0.1f
typedef struct
{
  float temperatureOri;
  float temperatureTarget;
  float pwmFraction;
  float periodMeter;
  
}temperatureStru;
void controllerInit(void);

#endif

