/**
 * @file pressure_control.c
 * @brief 基于升压速率控制的压力调节 (STM32实现)
 * 
 * 控制逻辑：
 * 1. 升压阶段阀门全关 (u=0)，压力上升。
 * 2. 实时计算剩余时间 t_rem = (P_target - P_filt) / v_filt。
 * 3. 当 t_rem ≤ 5.0秒 且未进入控制模式时，进入速率控制模式，
 *    记录当前压力 P_entry 和速率 v_entry。
 * 4. 控制模式中，目标速率 v_target 从 v_entry 线性下降至 0，
 *    根据速率误差 e_v = v_target - v_filt 进行比例控制，
 *    输出阀门变化量 delta_u，并施加速率限制。
 * 5. 阀门指令 u 更新后输出。
 * 
 * 硬件要求：需实现 ReadPressure() 和 SetValve() 函数。
 * 定时器周期：0.04秒 (25ms)，在中断中调用 PressureControl_Task()。
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

/* ==================== 系统参数 ==================== */
#define K               1000.0f      // 系统增益 (Torr) - 阀门全关时的稳态压力
#define T               15.0f        // 时间常数 (秒)
#define L               1.0f         // 纯延迟 (秒) - 用于仿真，实际系统忽略
#define Ts              0.04f        // 控制周期 (秒)

/* ==================== 目标参数 ==================== */
#define P_START         50.0f        // 起始压力 (Torr)
#define P_TARGET        500.0f       // 目标压力 (Torr)

/* ==================== 控制器参数 ==================== */
#define CONTROL_START_TIME  5.0f     // 剩余时间阈值 (秒)
#define KP_RATE         0.03f        // 速率误差比例增益
#define DEAD_ZONE       0.1f         // 速率误差死区 (Torr/秒)
#define MAX_DELTA_U     0.002f       // 每周期最大阀门变化量
#define FILTER_ALPHA    0.03f        // 压力一阶低通滤波系数
#define V_FILTER_ALPHA  0.1f         // 速率一阶低通滤波系数
#define EPSILON         1e-3f        // 速率最小值阈值

/* ==================== 全局变量 ==================== */
static float y_filt = 0.0f;          // 滤波后压力
static float P_prev = 0.0f;          // 上一周期滤波压力
static float v_filt = 0.0f;          // 滤波后速率

static float u_out = 0.0f;           // 阀门指令输出 (0=关阀, 1=开阀)
static float last_u = 0.0f;          // 上一周期阀门指令

static bool control_active = false;  // 控制模式标志
static float P_entry = 0.0f;         // 进入控制时的压力
static float v_entry = 0.0f;         // 进入控制时的速率

/* ==================== 硬件抽象接口 ==================== */
// 用户需实现：读取当前压力 (单位：Torr)
extern float ReadPressure(void);

// 用户需实现：设置阀门开度 (0.0~1.0, 0=关阀, 1=开阀)
extern void SetValve(float u);

/* ==================== 初始化函数 ==================== */
void PressureControl_Init(void)
{
    // 初始化滤波值
    y_filt = ReadPressure();
    P_prev = y_filt;
    v_filt = 0.0f;
    u_out = 0.0f;          // 初始阀门全关
    last_u = u_out;
    control_active = false;
    SetValve(u_out);
}

/* ==================== 核心控制任务 (每Ts秒调用一次) ==================== */
void PressureControl_Task(void)
{
    // 1. 读取原始压力
    float y_raw = ReadPressure();
    
    // 2. 一阶低通滤波 (压力)
    y_filt = FILTER_ALPHA * y_raw + (1.0f - FILTER_ALPHA) * y_filt;
    
    // 3. 计算原始速率
    float v_raw = (y_filt - P_prev) / Ts;
    P_prev = y_filt;
    
    // 4. 对速率进行一阶低通滤波
    v_filt = V_FILTER_ALPHA * v_raw + (1.0f - V_FILTER_ALPHA) * v_filt;
    
    // 5. 控制逻辑
    float u_target = last_u;  // 默认保持
    
    if (!control_active)
    {
        // 尚未进入控制模式：计算剩余时间
        float t_rem;
        if (v_filt > EPSILON)
        {
            t_rem = (P_TARGET - y_filt) / v_filt;
        }
        else
        {
            t_rem = 1e6f;  // 无穷大
        }
        
        if (t_rem <= CONTROL_START_TIME)
        {
            // 进入控制模式
            control_active = true;
            P_entry = y_filt;
            v_entry = v_filt;
            // 可打印调试信息 (通过串口)
            // printf("Entry: P=%.1f, v=%.2f, t_rem=%.2f\n", P_entry, v_entry, t_rem);
        }
        else
        {
            // 未进入控制：阀门全关
            u_target = 0.0f;
        }
    }
    
    if (control_active)
    {
        // 计算目标速率 (线性下降)
        float v_target;
        if (y_filt >= P_TARGET)
        {
            v_target = 0.0f;
        }
        else
        {
            v_target = v_entry * (P_TARGET - y_filt) / (P_TARGET - P_entry);
        }
        
        // 速率误差
        float e_v = v_target - v_filt;
        
        // 死区处理
        float delta_u = 0.0f;
        if (fabsf(e_v) >= DEAD_ZONE)
        {
            // 比例控制，注意符号：实际过快(e_v<0)需开阀(u增大)
            delta_u = -KP_RATE * e_v * Ts;
        }
        
        // 限制每步变化量
        if (fabsf(delta_u) > MAX_DELTA_U)
        {
            delta_u = (delta_u > 0.0f) ? MAX_DELTA_U : -MAX_DELTA_U;
        }
        
        u_target = last_u + delta_u;
        
        // 限幅
        if (u_target < 0.0f) u_target = 0.0f;
        if (u_target > 1.0f) u_target = 1.0f;
    }
    
    // 6. 更新输出
    u_out = u_target;
    last_u = u_out;
    SetValve(u_out);
    
    // 可选：保存状态供调试
}


