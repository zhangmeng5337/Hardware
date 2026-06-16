#ifndef CDGSWITCH_H
#define CDGSWITCH_H
#include "main.h"
// ======================== 用户配置参数 ========================
#define PRESSURE_MAX_TORR 1000.0f     // 最大压力 (Torr)，用于归一化参考，实际计算中不直接使用

// 低量程真空规的满量程 (归一化值，例如 10 Torr / 1000 Torr = 0.01)
#define LOW_FS_NORM  0.01f

// 软切换区间 (相对于低量程满量程的比例)
#define SWITCH_START_RATIO 0.9f   // 90% 开始切换
#define SWITCH_END_RATIO   1.0f   // 100% 结束切换

// 硬切换迟滞阈值 (相对于低量程满量程的比例)
#define HARD_UP_RATIO   0.95f     // 上升至 95% 切换到高量程
#define HARD_DOWN_RATIO 0.85f     // 下降至 85% 切回低量程

// 传感器一致性检测阈值 (相对偏差)
#define CONSISTENCY_THRESHOLD 0.05f   // 5%

// 自动模式中进入目标压力模式的条件：当前压力与目标压力偏差小于此比例 (相对于低量程满量程)
#define TARGET_MODE_DEADBAND 0.1f
// 动态阈值：例如目标压力小于低量程满量程的10倍？或者直接使用固定值0.1
#define TARGET_LOW_THRESHOLD  (LOW_FS_NORM)   // 例如低量程满量程0.01 -> 阈值0.1
#define ACTUAL_HIGH_THRESHOLD (LOW_FS_NORM)

// ======================== 数据类型定义 ========================
// 工作模式
typedef enum {
    MODE_AUTO = 0,   // 自动选择
    MODE_SOFT,       // 强制软切换
    MODE_HARD,       // 强制硬切换
    MODE_TARGET      // 目标压力模式
} SwitchMode;

// 真空规结构体（支持校准参数）
typedef struct {
    float fs_norm;   // 满量程归一化值 (0~1)
    float offset;    // 零点偏移 (归一化)
    float gain;      // 增益系数
    bool  enabled;   // 是否启用
} VacuumGauge;
// 硬切换状态机：根据压力趋势（升压/降压）选择不同阈值
typedef enum {
    PRESSURE_RISING,   // 升压
    PRESSURE_FALLING,  // 降压
    PRESSURE_STABLE    // 稳定（可忽略）
} PressureTrend;

// 控制器结构体
typedef struct {
    VacuumGauge low_gauge;   // 低量程真空规
    VacuumGauge high_gauge;  // 高量程真空规

    float switch_start_norm; // 软切换起始压力 (归一化)
    float switch_end_norm;   // 软切换结束压力 (归一化)

    SwitchMode mode;         // 当前模式
    bool auto_enabled;       // 是否启用自动模式选择

    // 硬切换状态
    float hard_up_norm;      // 上升切换阈值 (归一化)
    float hard_down_norm;    // 下降切换阈值 (归一化)
    bool using_low;          // 当前使用的传感器 (true=低量程)

    // 目标压力模式
    float target_norm;       // 目标压力 (归一化)

    // 传感器一致性检测
    float consistency_threshold;
    bool sensors_consistent; // 上一次检测的一致性结果

    // 压力变化率检测 (用于自动模式中判断是否稳定)
    float last_pressure_norm;
    float last_time;
    float dpdt_threshold;    // 归一化压力变化率阈值 (1/s)
} VATController;

// ======================== 函数声明 ========================
void VAT_Init(VATController *ctrl);
void VAT_SetTargetNorm(VATController *ctrl, float target_norm);
void VAT_ForceMode(VATController *ctrl, SwitchMode mode);
void VAT_EnableAuto(VATController *ctrl);
float SoftSwitch(VATController *ctrl, float low_val, float high_val);
float HardSwitch(VATController *ctrl, float low_val, float high_val);
float TargetSwitch(VATController *ctrl, float low_val, float high_val);
bool CheckConsistency(VATController *ctrl, float low_val, float high_val);
SwitchMode AutoSelectMode(VATController *ctrl, float low_val, float high_val, float current_time);
//float VAT_GetControlPressure(VATController *ctrl, float p_norm, float current_time);
//void cdgSwitchProc(void);
int cdgInit(void);
//void cdgSwitchProc(float lowP,float highP);
void cdgSwitchProc(float lowP,float highP,float *out);


#endif

