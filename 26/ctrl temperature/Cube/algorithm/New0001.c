#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* ============================================================
 *  硬件抽象接口 — 需根据实际平台实现
 * ============================================================ */
double read_pressure(void);          // 返回当前压力 (Torr)
void   set_valve(double opening);    // 设置阀门开度 0-100%
double get_current_time(void);       // 返回系统运行时间 (秒)
void   sleep_ms(int ms);             // 毫秒延时

/* ============================================================
 *  用户设定 (可在线修改)
 * ============================================================ */
static double P_target = 500.0;      // 目标压力 (Torr)
static double V_RAMP   = 5.0;        // 设定升压速率 (Torr/s) ，范围 1~10

/* ============================================================
 *  控制周期
 * ============================================================ */
#define TS                      0.02    // 20 ms

/* ============================================================
 *  阀门输出限幅
 * ============================================================ */
#define VALVE_MIN               0.0
#define VALVE_MAX               100.0

/* ============================================================
 *  动态减速区参数
 * ============================================================ */
#define DECEL_ACCEL             1.5     // 允许的最大减速度 (Torr/s2)
#define DECEL_MARGIN            5.0     // 安全余量 (Torr)
#define V_MIN                   0.5     // 最小速率截断 (Torr/s)

/* 压力下跌保护：至少保持由压力误差驱动的最小正向速率 */
#define KP_PROTECT              0.3     // 压力误差保护增益 (1/s)

/* ============================================================
 *  减速阶段输出冻结阈值
 * ============================================================ */
#define FREEZE_ERR              0.3     // Torr/s

/* ============================================================
 *  切换至压力 PID 的阈值
 * ============================================================ */
#define PRESSURE_TOLERANCE      0.3     // 压力误差 (Torr)
#define RATE_TOLERANCE          0.1     // 速率波动 (Torr/s)
#define RATE_STD_THRESHOLD      0.05    // 速率标准差阈值 (Torr/s)
#define STABLE_CNT_SWITCH       10      // 连续满足周期数

/* ============================================================
 *  速率环基准 PID 参数（在中等流量 8000 sccm, 5 Torr/s 下整定）
 * ============================================================ */
#define KP0                     4.0
#define KI0                     0.8

/* ============================================================
 *  增益限幅
 * ============================================================ */
#define KP_MIN                  1.5
#define KP_MAX                  15.0
#define KI_MIN                  0.05
#define KI_MAX                  4.0

/* ============================================================
 *  连续自适应参数
 * ============================================================ */
#define ALPHA_GAIN              0.5     // 偏差驱动增益增加速度
#define BETA_GAIN               4.0     // 振荡驱动增益减小速度 (远大于 alpha)
#define E_NOM_FACTOR            0.03    // E_nom = E_NOM_FACTOR * v_ramp + E_NOM_OFFSET
#define E_NOM_OFFSET            0.05
#define MAX_DKP_STEP            0.02    // 每周期最大 Kp 变化量 (相对于 KP0)

/* 快速自适应窗口：设定值改变后临时放大步长 */
#define FAST_ADAPT_TIME         3.0     // 快速自适应持续时间 (s)
#define FAST_MAX_DKP_FACTOR     3.0     // 步长放大因子

/* 误差指标滤波时间常数系数 */
#define GAMMA_E                 0.1     // E_avg 更新系数 (约0.2s)
#define GAMMA_V                 0.05    // V_osc 更新系数 (约0.4s)
#define GAMMA_LOW               0.1     // 误差低通滤波系数

/* ============================================================
 *  瞬时保护
 * ============================================================ */
#define INSTABILITY_THRESHOLD   3.0                     // 瞬时幅值保护 (Torr/s)
#define TRANSIENT_SUPPRESS_TIME 0.5                     // 设定值变化后屏蔽时间 (s)

/* ============================================================
 *  输出变化率限制
 * ============================================================ */
#define MAX_DU_DT               8.0     // %/s

/* ============================================================
 *  压力 PID 参数 (稳态微调，极弱)
 * ============================================================ */
#define KP_PRESS                0.5
#define KI_PRESS                0.01
#define KD_PRESS                0.0

/* 切换后临时加强参数 */
#define PID_BOOST_TIME          5.0
#define KP_PRESS_BOOST          (KP_PRESS * 3.0)
#define KI_PRESS_BOOST          (KI_PRESS * 3.0)

/* ============================================================
 *  压力微调增益 (零速率保持时使用)
 * ============================================================ */
#define KP_FINE                 0.5     // %/Torr

/* ============================================================
 *  速率误差窗口（仅用于切换时速率标准差计算）
 * ============================================================ */
#define WIN_TIME                0.5
#define WIN_LEN                 (int)(WIN_TIME / TS)

/* ============================================================
 *  安全压力上限
 * ============================================================ */
#define PRESSURE_LIMIT          550.0

/* ============================================================
 *  状态机
 * ============================================================ */
typedef enum {
    STAGE_RAMP = 0,
    STAGE_PID  = 1
} ControlStage;

/* ============================================================
 *  全局变量
 * ============================================================ */
static ControlStage stage = STAGE_RAMP;

// 速率环状态
static double v_actual = 0.0;
static double v_des = 0.0;           // 滤波后的期望速率
static double integral_rate = 0.0;
static double Kp = KP0, Ki = KI0;
static double Ki_old = KI0;
static double Kp_target = KP0;
static double Ki_target = KI0;

// 压力环状态
static double integral_press = 0.0;
static double pid_switch_time = 0.0;

// 输出记录
static double u_last = 0.0;
static double P_prev = 0.0;

// 连续自适应指标
static double E_avg = 0.0;
static double error_low = 0.0;
static double V_osc = 0.0;

// 减速/制动标志
static bool   decel_active = false;
static bool   zero_rate_hold = false;
static double zero_rate_start_time = 0.0;

// 输出冻结
static bool   output_frozen = false;
static double frozen_u = 0.0;

// 切换计数器
static int stable_cnt = 0;

// 瞬时保护抑制与快速自适应窗口
static double transient_suppress_until = 0.0;
static double fast_adapt_until = 0.0;

// 速率误差环形缓冲区（仅用于切换时的速率标准差判定）
static double error_buffer[WIN_LEN];
static int    buf_idx = 0;
static int    buf_count = 0;

// 动态减速区宽度
static double decel_zone = 30.0;

/* ============================================================
 *  工具函数
 * ============================================================ */
static double clamp(double val, double lo, double hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

static double fmin_local(double a, double b) {
    return (a < b) ? a : b;
}

/* ============================================================
 *  动态减速区计算
 * ============================================================ */
static double compute_decel_zone(double v_ramp, double total_delta_P) {
    double zone_by_rate = (v_ramp * v_ramp) / (2.0 * DECEL_ACCEL) + DECEL_MARGIN;
    double zone_by_max = total_delta_P * 0.5;
    double zone = fmin_local(zone_by_rate, zone_by_max);
    if (zone < 1.0) zone = 1.0;
    if (zone > total_delta_P) zone = total_delta_P;
    return zone;
}

/* ============================================================
 *  初始化
 * ============================================================ */
void control_init(void) {
    P_prev = read_pressure();
    u_last = 0.0;
    v_actual = 0.0;
    v_des = 0.0;
    integral_rate = 0.0;
    integral_press = 0.0;
    Kp = KP0;
    Ki = KI0;
    Ki_old = KI0;
    Kp_target = KP0;
    Ki_target = KI0;
    stage = STAGE_RAMP;
    stable_cnt = 0;
    decel_active = false;
    zero_rate_hold = false;
    zero_rate_start_time = 0.0;
    output_frozen = false;
    frozen_u = 0.0;

    E_avg = 0.0;
    error_low = 0.0;
    V_osc = 0.0;

    transient_suppress_until = 0.0;
    fast_adapt_until = 0.0;
    buf_idx = 0;
    buf_count = 0;

    double total_delta = P_target - P_prev;
    if (total_delta < 0) total_delta = 0;
    decel_zone = compute_decel_zone(V_RAMP, total_delta);
}

/* ============================================================
 *  设定值变化时调用：重置瞬态抑制、启动快速自适应
 * ============================================================ */
void update_target_pressure(double new_target) {
    P_target = new_target;
    double total_delta = new_target - read_pressure();
    if (total_delta < 0) total_delta = 0;
    decel_zone = compute_decel_zone(V_RAMP, total_delta);
    double now = get_current_time();
    transient_suppress_until = now + TRANSIENT_SUPPRESS_TIME;
    fast_adapt_until = now + FAST_ADAPT_TIME;
}

void update_ramp_rate(double new_rate) {
    V_RAMP = new_rate;
    double total_delta = P_target - read_pressure();
    if (total_delta < 0) total_delta = 0;
    decel_zone = compute_decel_zone(new_rate, total_delta);
    double now = get_current_time();
    transient_suppress_until = now + TRANSIENT_SUPPRESS_TIME;
    fast_adapt_until = now + FAST_ADAPT_TIME;
}

/* ============================================================
 *  速率计算与滤波
 * ============================================================ */
static double compute_rate(double p_current) {
    double v_raw = (p_current - P_prev) / TS;
    P_prev = p_current;
    const double alpha = 0.2;
    v_actual = alpha * v_raw + (1.0 - alpha) * v_actual;
    return v_actual;
}

/* ============================================================
 *  期望速率生成 (含动态减速区、零速率保持、压力下跌保护)
 * ============================================================ */
static double generate_desired_rate(double delta_P) {
    double raw;
    if (delta_P > decel_zone) {
        raw = V_RAMP;
        decel_active = false;
        zero_rate_hold = false;
    } else {
        decel_active = true;
        double raw_dec = V_RAMP * (delta_P / decel_zone);
        double raw_protect = KP_PROTECT * delta_P;
        raw = (raw_dec > raw_protect) ? raw_dec : raw_protect;
        if (raw < V_MIN) raw = 0.0;
    }

    const double alpha_v = 0.2;
    v_des = alpha_v * raw + (1.0 - alpha_v) * v_des;

    if (v_des < 0.01 && decel_active && !zero_rate_hold) {
        zero_rate_hold = true;
        zero_rate_start_time = get_current_time();
    } else if (!decel_active) {
        zero_rate_hold = false;
    }

    if (zero_rate_hold) {
        v_des = 0.0;
    }

    return v_des;
}

/* ============================================================
 *  速率 PI 控制器 (含输出冻结)
 * ============================================================ */
static double rate_pi_control(double error, bool decel_or_zero) {
    if (decel_or_zero && fabs(error) <= FREEZE_ERR) {
        if (!output_frozen) {
            frozen_u = u_last;
            output_frozen = true;
        }
        return frozen_u;
    } else {
        output_frozen = false;
    }

    double Pout = Kp * error;

    if (!((u_last >= VALVE_MAX && error > 0) ||
          (u_last <= VALVE_MIN && error < 0))) {
        integral_rate += error * TS;
    }
    double Iout = Ki * integral_rate;

    double u_raw = Pout + Iout;

    double max_du = MAX_DU_DT * TS;
    double du = u_raw - u_last;
    if (du > max_du)       u_raw = u_last + max_du;
    else if (du < -max_du) u_raw = u_last - max_du;

    return clamp(u_raw, VALVE_MIN, VALVE_MAX);
}

/* ============================================================
 *  误差缓冲区更新（仅用于切换时的速率标准差计算）
 * ============================================================ */
static void update_error_buffer(double error) {
    error_buffer[buf_idx] = error;
    buf_idx = (buf_idx + 1) % WIN_LEN;
    if (buf_count < WIN_LEN) buf_count++;
}

/* ============================================================
 *  连续自适应指标更新（内部使用）
 * ============================================================ */
static void update_adapt_metrics(double error) {
    error_low = (1.0 - GAMMA_LOW) * error_low + GAMMA_LOW * error;
    double error_high = error - error_low;
    E_avg = (1.0 - GAMMA_E) * E_avg + GAMMA_E * fabs(error);
    V_osc = (1.0 - GAMMA_V) * V_osc + GAMMA_V * (error_high * error_high);
}

/* ============================================================
 *  自适应：计算目标增益（连续梯度，无分段，保留原函数名）
 * ============================================================ */
static void perform_adaptation(double v_des, double error, double now) {
    // 先更新连续指标（这样外部只需调用 perform_adaptation）
    update_adapt_metrics(error);

    if (decel_active || zero_rate_hold) {
        double decay = BETA_GAIN * V_osc;
        if (decay > 0.0) {
            double dKp_dec = -decay * TS;
            double max_step = MAX_DKP_STEP * KP0;
            if (dKp_dec < -max_step) dKp_dec = -max_step;
            Kp_target = clamp(Kp_target + dKp_dec, KP_MIN, KP_MAX);
            double ratio = Ki / Kp;
            Ki_target = Kp_target * ratio;
            Ki_target = clamp(Ki_target, KI_MIN, KI_MAX);
        }
        return;
    }

    double max_step = MAX_DKP_STEP * KP0;
    if (now < fast_adapt_until) {
        max_step *= FAST_MAX_DKP_FACTOR;
    }

    double E_nom = E_NOM_FACTOR * v_des + E_NOM_OFFSET;
    double dKp = (ALPHA_GAIN * (E_avg - E_nom) - BETA_GAIN * V_osc) * TS;

    if (dKp > max_step) dKp = max_step;
    else if (dKp < -max_step) dKp = -max_step;

    Kp_target += dKp;

    double ratio = Ki / Kp;
    Ki_target = Kp_target * ratio;

    Kp_target = clamp(Kp_target, KP_MIN, KP_MAX);
    Ki_target = clamp(Ki_target, KI_MIN, KI_MAX);
}

/* ============================================================
 *  增益平滑逼近与无扰切换 (每个控制周期调用，在 PI 控制之前)
 * ============================================================ */
static void smooth_gain_update(double error) {
    if (fabs(Kp_target - Kp) > 0.001 || fabs(Ki_target - Ki) > 0.001) {
        double max_dKp = 0.5 * TS;
        double max_dKi = 0.1 * TS;
        double dKp = Kp_target - Kp;
        double dKi = Ki_target - Ki;

        if (dKp > max_dKp) dKp = max_dKp;
        else if (dKp < -max_dKp) dKp = -max_dKp;
        if (dKi > max_dKi) dKi = max_dKi;
        else if (dKi < -max_dKi) dKi = -max_dKi;

        double new_Kp = Kp + dKp;
        double new_Ki = Ki + dKi;

        if (fabs(new_Ki) > 1e-9) {
            integral_rate = (Kp * error + Ki * integral_rate - new_Kp * error) / new_Ki;
        } else {
            integral_rate = 0.0;
        }

        Kp = new_Kp;
        Ki = new_Ki;
        Ki_old = Ki;
    }
}

/* ============================================================
 *  瞬时保护 (灾难性振荡时直接降增益，含瞬态抑制)
 * ============================================================ */
static void instant_protection(double error, double now) {
    if (now < transient_suppress_until) return;

    if (fabs(error) > INSTABILITY_THRESHOLD) {
        double old_Kp = Kp, old_Ki = Ki;
        Kp = clamp(Kp * 0.7, KP_MIN, KP_MAX);
        Ki = clamp(Ki * 0.7, KI_MIN, KI_MAX);
        if (fabs(Ki) > 1e-9) {
            integral_rate = (old_Kp * error + old_Ki * integral_rate - Kp * error) / Ki;
        } else {
            integral_rate = 0.0;
        }
        Ki_old = Ki;
        Kp_target = Kp;
        Ki_target = Ki;
    }
}

/* ============================================================
 *  压力 PID 无扰初始化
 * ============================================================ */
static void init_pressure_pid(double current_u, double delta_P) {
    double e_p = delta_P;
    if (fabs(KI_PRESS) > 1e-9) {
        integral_press = (current_u - KP_PRESS * e_p) / KI_PRESS;
    } else {
        integral_press = 0.0;
    }
    integral_press = clamp(integral_press, VALVE_MIN / KI_PRESS, VALVE_MAX / KI_PRESS);
    u_last = current_u;
}

/* ============================================================
 *  压力 PID 控制 (支持切换后临时加强)
 * ============================================================ */
static double pressure_pid_control(double error_p, double now) {
    double kp_now = KP_PRESS;
    double ki_now = KI_PRESS;
    if ((now - pid_switch_time) < PID_BOOST_TIME) {
        kp_now = KP_PRESS_BOOST;
        ki_now = KI_PRESS_BOOST;
    }

    double Pout = kp_now * error_p;
    double u_tmp = Pout + ki_now * integral_press;
    if (!((u_tmp >= VALVE_MAX && error_p > 0) ||
          (u_tmp <= VALVE_MIN && error_p < 0))) {
        integral_press += error_p * TS;
    }
    double u = Pout + ki_now * integral_press;
    return clamp(u, VALVE_MIN, VALVE_MAX);
}

/* ============================================================
 *  主控制循环
 * ============================================================ */
int main(void) {
    control_init();

    while (1) {
        double now = get_current_time();
        double P_current = read_pressure();

        if (P_current >= PRESSURE_LIMIT) {
            set_valve(0.0);
            u_last = 0.0;
            output_frozen = false;
            sleep_ms((int)(TS * 1000));
            continue;
        }

        double v_actual_ = compute_rate(P_current);
        double delta_P = P_target - P_current;

        if (stage == STAGE_RAMP) {
            double v_des_curr = generate_desired_rate(delta_P);
            double error_rate = v_des_curr - v_actual_;

            update_error_buffer(error_rate);
            // 连续自适应决策（内部自动更新指标）
            perform_adaptation(v_des_curr, error_rate, now);

            smooth_gain_update(error_rate);

            bool decel_or_zero = decel_active || zero_rate_hold;
            double u;

            if (zero_rate_hold && fabs(delta_P) < 0.3) {
                double error_p = P_target - P_current;
                u = u_last + KP_FINE * error_p;
                u = clamp(u, VALVE_MIN, VALVE_MAX);
                output_frozen = false;
            } else {
                u = rate_pi_control(error_rate, decel_or_zero);
            }

            u_last = u;

            instant_protection(error_rate, now);

            bool rate_stable = false;
            if (zero_rate_hold && buf_count >= WIN_LEN) {
                double sum_e = 0.0, sum_e2 = 0.0;
                for (int i = 0; i < WIN_LEN; i++) {
                    double e = error_buffer[i];
                    sum_e += e;
                    sum_e2 += e * e;
                }
                double mean_e = sum_e / WIN_LEN;
                double var_e = (sum_e2 / WIN_LEN) - (mean_e * mean_e);
                if (var_e < 0.0) var_e = 0.0;
                double std_e = sqrt(var_e);
                rate_stable = (std_e < RATE_STD_THRESHOLD);
            }

            if (fabs(delta_P) < PRESSURE_TOLERANCE &&
                fabs(v_actual_) < RATE_TOLERANCE &&
                zero_rate_hold && rate_stable) {
                stable_cnt++;
                if (stable_cnt >= STABLE_CNT_SWITCH) {
                    init_pressure_pid(u, delta_P);
                    pid_switch_time = now;
                    stage = STAGE_PID;
                    stable_cnt = 0;
                    zero_rate_hold = false;
                    output_frozen = false;
                }
            } else {
                stable_cnt = 0;
            }

            set_valve(u);
        }
        else if (stage == STAGE_PID) {
            double error_p = P_target - P_current;
            double u = pressure_pid_control(error_p, now);
            u_last = u;
            set_valve(u);
        }

        sleep_ms((int)(TS * 1000));
    }

    return 0;
}
