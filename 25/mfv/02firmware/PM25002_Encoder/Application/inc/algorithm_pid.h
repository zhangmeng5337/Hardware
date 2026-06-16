#ifndef ALGORITHM_PID_H
#define ALGORITHM_PID_H
#include "main.h"

/* ==================== 常数定义 ==================== */
#define PRESSURE_SCALE              1000.0f
#define INTEGRAL_SEPARATION_RATIO   2.0f

/* ==================== 速率分段参数结构体 ==================== */
/* ==================== 速率分段参数结构体 ==================== */
typedef struct {
    float rate_max_norm;                // 该段最大速率 (归一化, 1/s)
    float steady_thresh_torr;           // 稳态压力误差阈值 (Torr)  -- 仍以Torr存储，内部转换
    float Kp;                           // PID 比例增益
    float Ki;                           // PID 积分增益
    float Kd;                           // PID 微分增益
    float integral_limit;               // 积分限幅 (阀门变化量)
    float max_delta_u;                  // PID 阶段每周期最大阀门变化量
} RateSegmentParam;


#define DEFAULT_NUM_SEGMENTS   (sizeof(DEFAULT_RATE_SEGMENTS) / sizeof(DEFAULT_RATE_SEGMENTS[0]))
/* ==================== 内部参数结构体 ==================== */
typedef struct {
    float control_period;               // 控制周期 (秒)
    float Kv;                           // 速率比例系数 (v_target = Kv * ΔP_norm)
    float Kp_rate;                      // 速率误差比例增益
    float dead_zone_rate_norm;          // 速率误差死区 (归一化, 1/s)
    float max_delta_u_rate;             // 速率控制阶段每周期最大阀门变化量
    float prediction_time;              // 预测时间 (秒)
    float Kp_pred_base;                 // 预测修正基础增益
    float Kp_pred_rate_factor;          // 预测增益速率调节系数
    float pressure_filter_alpha;        // 压力滤波系数
    float rate_filter_alpha;            // 速率滤波系数
    float acc_filter_alpha;             // 加速度滤波系数
    float epsilon_norm;                 // 速率最小值阈值 (归一化, 1/s)
    float t_steady_duration;            // 稳定持续时间 (秒)
    float exit_pid_threshold_ratio;     // 退出 PID 的误差倍数

    float target_pressure_norm;         // 目标压力 (归一化)
    float p_err_steady_thresh_norm;     // 稳态误差阈值 (归一化，动态)
    float Kp_pid;                       // PID 比例增益
    float Ki_pid;                       // PID 积分增益
    float Kd_pid;                       // PID 微分增益
    float pid_integral_limit;           // 积分限幅
    float max_delta_u_pid;              // PID 阶段最大阀门变化量

    float p_rate_start_thresh_norm;     // 启动速率控制的压力阈值 (归一化)
    float v_rate_start_thresh_norm;     // 启动速率控制的速率阈值 (归一化, 1/s)
} PressureControlParams;

/* ==================== 状态结构体（含调试变量） ==================== */
typedef struct {
    // 核心状态
    float y_filt_norm;                  // 滤波后压力
    float P_prev_norm;                  // 上一周期压力
    float v_filt_norm;                  // 滤波后速率
    float a_filt_norm;                  // 滤波后加速度 (1/s^2)
    float prev_v_filt_norm;             // 上一周期速率

    float u_out;                        // 当前阀门指令
    float last_u;                       // 上一周期阀门指令

    bool pid_active;                    // true: PID 激活
    float steady_timer;                 // 稳态计时器

    float integral_error;               // PID 积分项
    float prev_y_filt_norm;             // 上一周期压力 (用于微分)

    int current_seg_idx;                // 当前使用的分段索引
    float rate_stable_timer;            // 速率稳定计时器

    float pred_err_integral;            // 预测误差积分补偿

    // 调试变量
    float debug_deltaP;
    float debug_v_target;
    float debug_e_v;
    float debug_delta_u_base;
    float debug_P_pred;
    float debug_overshoot;
    float debug_rate_factor;
    float debug_Kp_adaptive;
    float debug_delta_u_pred;
    float debug_a_raw;
    float debug_error;
    float debug_integral_sep_thresh;
    uint8_t debug_allow_integral;
    float debug_delta_u;
    float debug_P_out;
    float debug_I_out;
    float debug_D_out;
} PressureControlState;



void PressureControl_Init(float initial_valve, float target_pressure_norm,
                          const RateSegmentParam *rate_segments, int num_segments);

void PressureControl_Task(float pv,float measure);


#endif


