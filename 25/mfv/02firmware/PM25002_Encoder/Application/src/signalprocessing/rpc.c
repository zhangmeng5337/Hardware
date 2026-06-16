/*
 * RPC 算法 STM32 实现框架 (基于定时器中断)
 * 核心逻辑移植自 MATLAB 仿真代码
 */

#include "stm32f4xx_hal.h"  // 根据你的具体型号修改
#include <math.h>            // 用于 fmaxf, fminf 等

// ==================== 硬件抽象接口 ====================
// 需要根据你的硬件平台实现这些函数
extern float Read_Pressure_Sensor(void);   // 返回当前压力 (Torr)
extern void Set_Valve_Command(float u);    // 设置阀门指令 (0.0~1.0, 0=全开, 1=全关)
extern uint32_t Get_Tick(void);            // 获取系统时间戳 (毫秒)

// ==================== RPC 参数定义 ====================
// 这些参数直接从你调试好的仿真代码中复制
#define SP  (500.0f)          // 目标压力 (Torr)
#define MOVE_RATE (0.06f)     // 阀门最大动作速度 (%/秒)，对应仿真中 0.06
#define PREDICTION_TIME (22.0f) // 预测时间 (秒)
#define TAPER_BAND (380.0f)   // 锥形带宽度 (Torr)
#define KI (0.0f)             // 积分增益 (0 表示关闭)
#define INTEGRAL_LIMIT (0.0f) // 积分限幅
#define FILTER_ALPHA (0.08f)  // 一阶低通滤波系数
#define ALPHA_SMOOTH (0.3f)   // 预测值平滑系数 (对应仿真的 alpha_smooth)

// ==================== 全局变量 ====================
// 这些变量需要在中断服务函数和主循环间共享，注意加volatile
static volatile float y_meas_filtered = 0.0f;   // 滤波后的压力测量值
static volatile float last_u = 1.0f;             // 上一次的阀门指令 (初始全关)
static volatile float integral_error = 0.0f;      // 积分累积值
static volatile float P_pred_prev = 0.0f;         // 上一次的预测压力值
static volatile float y_prev = 0.0f;              // 上一次的测量值，用于计算变化率

/**
 * @brief RPC 算法核心计算 (应在定时器中断中调用)
 */
void RPC_Calculate(void) {
    // --- 1. 读取当前压力并滤波 ---
    float y_raw = Read_Pressure_Sensor();                 // 读取原始压力
    y_meas_filtered = FILTER_ALPHA * y_raw + (1.0f - FILTER_ALPHA) * y_meas_filtered; // 一阶低通滤波

    // --- 2. 计算压力变化率 (dP/dt) ---
    static float rate = 0.0f;
    rate = (y_meas_filtered - y_prev) / Ts;                 // 使用定时器周期 Ts
    y_prev = y_meas_filtered;

    // --- 3. 预测未来压力 (线性外推) ---
    float P_pred = y_meas_filtered + rate * PREDICTION_TIME;

    // --- 4. 对预测值进行平滑 (抑制抖动) ---
    P_pred = ALPHA_SMOOTH * P_pred + (1.0f - ALPHA_SMOOTH) * P_pred_prev;
    P_pred_prev = P_pred;

    // --- 5. 计算基础目标开度 u_base (核心RPC逻辑) ---
    float u_base = 0.5f; // 默认值
    if (fabsf(P_pred - SP) <= TAPER_BAND) {
        // 锥形带内：线性映射
        u_base = 0.5f + 0.5f * (SP - P_pred) / TAPER_BAND;
    } else {
        // 锥形带外：饱和输出
        if (P_pred < SP - TAPER_BAND) {
            u_base = 1.0f; // 预测压力过低 -> 关阀升压
        } else if (P_pred > SP + TAPER_BAND) {
            u_base = 0.0f; // 预测压力过高 -> 开阀降压
        } else {
            u_base = 0.5f;
        }
    }

    // 限幅
    if (u_base < 0.0f) u_base = 0.0f;
    if (u_base > 1.0f) u_base = 1.0f;

    // --- 6. 积分校正 (目前 Ki=0，保留框架) ---
    float current_error = SP - y_meas_filtered;
    integral_error += KI * current_error * Ts;
    if (integral_error > INTEGRAL_LIMIT) integral_error = INTEGRAL_LIMIT;
    if (integral_error < -INTEGRAL_LIMIT) integral_error = -INTEGRAL_LIMIT;

    float u_target = u_base + integral_error;
    if (u_target < 0.0f) u_target = 0.0f;
    if (u_target > 1.0f) u_target = 1.0f;

    // --- 7. 速率限制 (Move Rate) ---
    float max_change = MOVE_RATE * Ts; // 每个控制周期允许的最大变化量
    float delta_u = u_target - last_u;

    if (delta_u > max_change) {
        delta_u = max_change;
    } else if (delta_u < -max_change) {
        delta_u = -max_change;
    }

    float u_new = last_u + delta_u;
    if (u_new < 0.0f) u_new = 0.0f;
    if (u_new > 1.0f) u_new = 1.0f;

    // --- 8. 输出阀门指令 ---
    Set_Valve_Command(u_new);
    last_u = u_new;
}

/**
 * @brief 定时器中断回调 (以 0.04s 为周期)
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIMx) { // 替换为你的定时器
        RPC_Calculate();
    }
}

/**
 * @brief 初始化RPC模块 (在 main 中调用)
 */
void RPC_Init(void) {
    y_meas_filtered = Read_Pressure_Sensor(); // 初始化滤波值为当前压力
    y_prev = y_meas_filtered;
    P_pred_prev = y_meas_filtered;            // 初始化预测值
    last_u = 1.0f;                            // 初始阀门全关
    Set_Valve_Command(last_u);

    // 初始化并启动定时器 (Ts=0.04s)
    // ... 你的定时器初始化代码 ...
}

