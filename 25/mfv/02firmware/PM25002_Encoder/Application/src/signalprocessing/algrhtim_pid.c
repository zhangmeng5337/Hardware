/**
 * @file pressure_control.c
 * @brief 完整压力控制模块（带默认分段表，含全面调试变量）
 *
 * 使用说明：
 *   1. 若不提供自定义分段表，则使用内置默认表（三个分段，适用于常见工况）。
 *   2. 也可传入自定义表以覆盖。
 *   3. 所有调试变量均保存在状态结构体中，便于监控。
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "algorithm_pid.h"
/**
 * @file pressure_control.c
 * @brief 完整压力控制模块（所有压力输入均为归一化值 0-1）
 * 
 * 使用说明：
 *   - 调用 PressureControl_Init(initial_valve, target_pressure_norm, rate_segments, num_segments)
 *   - 每个控制周期调用 PressureControl_Task(pv_norm, measure_norm)
 *   - 通过 PressureControl_GetOutput() 获取阀门指令 (0~1)
 *   - 用户需实现 SetValve() 驱动硬件
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ==================== 常数定义 ==================== */
#define PRESSURE_SCALE              1000.0f
#define INTEGRAL_SEPARATION_RATIO   2.0f


/* ==================== 默认速率分段表 ==================== */
static const RateSegmentParam DEFAULT_RATE_SEGMENTS[] = {
    { .rate_max_norm = 0.01f,   .steady_thresh_torr = 5.0f,  .Kp = 0.8f, .Ki = 0.1f, .Kd = 0.05f, .integral_limit = 0.03f, .max_delta_u = 0.005f },
    { .rate_max_norm = 0.03f,   .steady_thresh_torr = 10.0f, .Kp = 0.6f, .Ki = 0.15f, .Kd = 0.04f, .integral_limit = 0.05f, .max_delta_u = 0.005f },
    { .rate_max_norm = 0.10f,   .steady_thresh_torr = 15.0f, .Kp = 0.4f, .Ki = 0.2f, .Kd = 0.03f, .integral_limit = 0.07f, .max_delta_u = 0.005f },
    { .rate_max_norm = 1.0f,    .steady_thresh_torr = 20.0f, .Kp = 0.3f, .Ki = 0.25f, .Kd = 0.02f, .integral_limit = 0.10f, .max_delta_u = 0.005f }
};



/* ==================== 全局变量 ==================== */
static PressureControlParams g_params;
static PressureControlState g_state;
static const RateSegmentParam *g_rate_segments = NULL;
static int g_num_segments = 0;

/* ==================== 辅助函数 ==================== */
static inline float torr_to_norm(float torr) { return torr / PRESSURE_SCALE; }

static const RateSegmentParam* FindSegmentByRate(float rate_norm)
{
    if (g_rate_segments == NULL || g_num_segments == 0) return NULL;
    for (int i = 0; i < g_num_segments; i++) {
        if (rate_norm <= g_rate_segments[i].rate_max_norm)
            return &g_rate_segments[i];
    }
    return &g_rate_segments[g_num_segments - 1];
}

static void UpdateParamsFromSegment(const RateSegmentParam *seg)
{
    if (seg == NULL) return;
    g_params.p_err_steady_thresh_norm = torr_to_norm(seg->steady_thresh_torr);
    g_params.Kp_pid = seg->Kp;
    g_params.Ki_pid = seg->Ki;
    g_params.Kd_pid = seg->Kd;
    g_params.pid_integral_limit = seg->integral_limit;
    g_params.max_delta_u_pid = seg->max_delta_u;
    if (g_state.pid_active)
        g_state.integral_error = 0.0f;
}

/* ==================== 用户接口 ==================== */
void PressureControl_Init(float initial_valve, float target_pressure_norm,
                          const RateSegmentParam *rate_segments, int num_segments)
{
    // 分段表
    if (rate_segments == NULL || num_segments == 0) {
        g_rate_segments = DEFAULT_RATE_SEGMENTS;
        g_num_segments = DEFAULT_NUM_SEGMENTS;
    } else {
        g_rate_segments = rate_segments;
        g_num_segments = num_segments;
    }

    // 固定参数（归一化）
    g_params.control_period = 0.04f;
    g_params.Kv = 0.10f;
    g_params.Kp_rate = 0.05f;
    g_params.dead_zone_rate_norm = 0.0001f;         // 0.1 Torr/s
    g_params.max_delta_u_rate = 0.005f;
    g_params.prediction_time = 2.0f;
    g_params.Kp_pred_base = 0.02f;
    g_params.Kp_pred_rate_factor = 1.0f;
    g_params.pressure_filter_alpha = 0.03f;
    g_params.rate_filter_alpha = 0.10f;
    g_params.acc_filter_alpha = 0.20f;
    g_params.epsilon_norm = 0.000001f;              // 0.001 Torr/s
    g_params.t_steady_duration = 3.0f;
    g_params.exit_pid_threshold_ratio = 1.5f;

    g_params.target_pressure_norm = target_pressure_norm;
    g_params.p_err_steady_thresh_norm = torr_to_norm(10.0f);
    g_params.Kp_pid = 0.5f;
    g_params.Ki_pid = 0.1f;
    g_params.Kd_pid = 0.05f;
    g_params.pid_integral_limit = 0.05f;
    g_params.max_delta_u_pid = 0.005f;

    g_params.p_rate_start_thresh_norm = torr_to_norm(400.0f);   // 400 Torr
    g_params.v_rate_start_thresh_norm = 0.01f;                 // 10 Torr/s

    // 状态初始化
    g_state.y_filt_norm = 0.0f;
    g_state.P_prev_norm = 0.0f;
    g_state.v_filt_norm = 0.0f;
    g_state.a_filt_norm = 0.0f;
    g_state.prev_v_filt_norm = 0.0f;
    g_state.u_out = initial_valve;
    g_state.last_u = initial_valve;

    g_state.pid_active = false;
    g_state.steady_timer = 0.0f;
    g_state.integral_error = 0.0f;
    g_state.prev_y_filt_norm = 0.0f;
    g_state.current_seg_idx = -1;
    g_state.rate_stable_timer = 0.0f;
    g_state.pred_err_integral = 0.0f;

    memset(&g_state.debug_deltaP, 0, sizeof(g_state) - offsetof(PressureControlState, debug_deltaP));
}

void PressureControl_SetTargetPressureNorm(float target_norm)
{
    g_params.target_pressure_norm = target_norm;
    g_state.pid_active = false;
    g_state.steady_timer = 0.0f;
    g_state.integral_error = 0.0f;
}

float PressureControl_GetOutput(void)
{
    return g_state.u_out;
}

/* ==================== 核心控制任务（输入已归一化） ==================== */
void PressureControl_Task(float pv_norm, float measure_norm)
{
    const float Ts = g_params.control_period;

    // 更新目标压力（若变化）
    if (fabsf(pv_norm - g_params.target_pressure_norm) > 0.0001f) {
        g_params.target_pressure_norm = pv_norm;
        g_state.pid_active = false;
        g_state.steady_timer = 0.0f;
        g_state.integral_error = 0.0f;
    }

    // 1. 压力滤波
    g_state.y_filt_norm = g_params.pressure_filter_alpha * measure_norm
                          + (1.0f - g_params.pressure_filter_alpha) * g_state.y_filt_norm;

    // 2. 速率计算与滤波
    float v_raw = (g_state.y_filt_norm - g_state.P_prev_norm) / Ts;
    g_state.P_prev_norm = g_state.y_filt_norm;
    g_state.v_filt_norm = g_params.rate_filter_alpha * v_raw
                          + (1.0f - g_params.rate_filter_alpha) * g_state.v_filt_norm;

    // 3. 加速度计算与滤波
    g_state.debug_a_raw = (g_state.v_filt_norm - g_state.prev_v_filt_norm) / Ts;
    g_state.a_filt_norm = g_params.acc_filter_alpha * g_state.debug_a_raw
                          + (1.0f - g_params.acc_filter_alpha) * g_state.a_filt_norm;
    g_state.prev_v_filt_norm = g_state.v_filt_norm;

    // 4. 速率自适应（仅在速率控制阶段）
    if (!g_state.pid_active && g_rate_segments && g_num_segments > 0)
    {
        const RateSegmentParam *seg = FindSegmentByRate(g_state.v_filt_norm);
        if (seg)
        {
            int idx = (int)(seg - g_rate_segments);
            if (idx != g_state.current_seg_idx)
            {
                g_state.rate_stable_timer += Ts;
                if (g_state.rate_stable_timer >= 0.5f)
                {
                    UpdateParamsFromSegment(seg);
                    g_state.current_seg_idx = idx;
                    g_state.rate_stable_timer = 0.0f;
                }
            }
        }
    }
    else
    {
        g_state.rate_stable_timer = 0.0f;
    }

    // 5. 强制退出 PID（偏差过大）
    if (g_state.pid_active)
    {
        float err_abs = fabsf(g_params.target_pressure_norm - g_state.y_filt_norm);
        if (err_abs > g_params.p_err_steady_thresh_norm * g_params.exit_pid_threshold_ratio)
        {
            g_state.pid_active = false;
            g_state.steady_timer = 0.0f;
            g_state.integral_error = 0.0f;
        }
    }

    float u_target = g_state.last_u;

    if (!g_state.pid_active)
    {
        // ========== 阶段1：根据压力和速率决定是否全关 ==========
        if (g_state.y_filt_norm < g_params.p_rate_start_thresh_norm)
        {
            if (g_state.v_filt_norm >= g_params.v_rate_start_thresh_norm)
            {
                u_target = 0.0f;
                g_state.u_out = u_target;
                g_state.last_u = u_target;
                return;
            }
        }

        // 速率控制 + 二阶预测修正
        g_state.debug_deltaP = g_params.target_pressure_norm - g_state.y_filt_norm;
        g_state.debug_v_target = (g_state.debug_deltaP > 0.0f) ? (g_params.Kv * g_state.debug_deltaP) : 0.0f;
        g_state.debug_e_v = g_state.debug_v_target - g_state.v_filt_norm;

        g_state.debug_delta_u_base = 0.0f;
        if (fabsf(g_state.debug_e_v) >= g_params.dead_zone_rate_norm)
            g_state.debug_delta_u_base = -g_params.Kp_rate * g_state.debug_e_v * Ts;

        g_state.debug_P_pred = g_state.y_filt_norm
                               + g_state.v_filt_norm * g_params.prediction_time
                               + 0.5f * g_state.a_filt_norm * g_params.prediction_time * g_params.prediction_time;
        g_state.debug_overshoot = g_state.debug_P_pred - g_params.target_pressure_norm;

        g_state.debug_delta_u_pred = 0.0f;
        if (g_state.debug_overshoot > 0.0f)
        {
            g_state.debug_rate_factor = fminf(1.0f, g_state.v_filt_norm / 0.05f);
            g_state.debug_Kp_adaptive = g_params.Kp_pred_base * (1.0f + g_params.Kp_pred_rate_factor * g_state.debug_rate_factor);
            g_state.debug_delta_u_pred = g_state.debug_Kp_adaptive * g_state.debug_overshoot;

            float pred_err = g_state.debug_P_pred - g_state.y_filt_norm;
            g_state.pred_err_integral += 0.005f * pred_err * Ts;
            if (g_state.pred_err_integral > 0.01f) g_state.pred_err_integral = 0.01f;
            if (g_state.pred_err_integral < -0.01f) g_state.pred_err_integral = -0.01f;
            g_state.debug_delta_u_pred += g_state.pred_err_integral;

            if (g_state.debug_delta_u_pred > g_params.max_delta_u_rate)
                g_state.debug_delta_u_pred = g_params.max_delta_u_rate;
            if (g_state.debug_delta_u_pred < 0.0f) g_state.debug_delta_u_pred = 0.0f;
        }

        g_state.debug_delta_u = g_state.debug_delta_u_base + g_state.debug_delta_u_pred;
        if (g_state.debug_delta_u > g_params.max_delta_u_rate)   g_state.debug_delta_u = g_params.max_delta_u_rate;
        if (g_state.debug_delta_u < -g_params.max_delta_u_rate)  g_state.debug_delta_u = -g_params.max_delta_u_rate;

        u_target = g_state.last_u + g_state.debug_delta_u;
        u_target = fmaxf(0.0f, fminf(1.0f, u_target));

        // 切换 PID 条件
        float err_abs = fabsf(g_params.target_pressure_norm - g_state.y_filt_norm);
        if (err_abs <= g_params.p_err_steady_thresh_norm)
        {
            g_state.steady_timer += Ts;
            if (g_state.steady_timer >= g_params.t_steady_duration)
            {
                g_state.pid_active = true;
                g_state.integral_error = 0.0f;
                g_state.prev_y_filt_norm = g_state.y_filt_norm;
            }
        }
        else
            g_state.steady_timer = 0.0f;
    }
    else
    {
        // ========== 阶段2：PID 控制 ==========
        g_state.debug_error = g_params.target_pressure_norm - g_state.y_filt_norm;

        g_state.debug_P_out = g_params.Kp_pid * g_state.debug_error;

        g_state.debug_integral_sep_thresh = g_params.p_err_steady_thresh_norm * INTEGRAL_SEPARATION_RATIO;
        bool allow_integral = true;
        if (fabsf(g_state.debug_error) > g_state.debug_integral_sep_thresh)
            allow_integral = false;

        float delta_u_integral = g_params.Ki_pid * g_state.debug_error * Ts;
        if ((g_state.last_u >= 0.99f && delta_u_integral > 0) ||
            (g_state.last_u <= 0.01f && delta_u_integral < 0))
            allow_integral = false;

        g_state.debug_allow_integral = allow_integral ? 1 : 0;

        if (allow_integral)
        {
            g_state.integral_error += g_state.debug_error * Ts;
            if (g_state.integral_error > g_params.pid_integral_limit)
                g_state.integral_error = g_params.pid_integral_limit;
            if (g_state.integral_error < -g_params.pid_integral_limit)
                g_state.integral_error = -g_params.pid_integral_limit;
        }
        g_state.debug_I_out = g_params.Ki_pid * g_state.integral_error;

        float derivative = (g_state.y_filt_norm - g_state.prev_y_filt_norm) / Ts;
        g_state.debug_D_out = -g_params.Kd_pid * derivative;
        g_state.prev_y_filt_norm = g_state.y_filt_norm;

        g_state.debug_delta_u = (g_state.debug_P_out + g_state.debug_I_out + g_state.debug_D_out) * Ts;
        if (g_state.debug_delta_u > g_params.max_delta_u_pid)
            g_state.debug_delta_u = g_params.max_delta_u_pid;
        if (g_state.debug_delta_u < -g_params.max_delta_u_pid)
            g_state.debug_delta_u = -g_params.max_delta_u_pid;

        u_target = g_state.last_u + g_state.debug_delta_u;
        u_target = fmaxf(0.0f, fminf(1.0f, u_target));
    }

    g_state.u_out = u_target;
    g_state.last_u = u_target;
}

