/**
 * VAT TV阀双真空规自动切换算法（纯归一化实现）
 * 
 * 输入：当前压力归一化值 p_norm (0~1 对应 0~1000 Torr)
 * 输出：经切换算法融合后的归一化压力值 (0~1)
 * 
 * 支持三种模式：
 *   1. 软切换 (SOFT)   - 在交叠区线性加权融合，平滑过渡
 *   2. 硬切换 (HARD)   - 带迟滞的传感器切换，抗抖动
 *   3. 目标压力切换 (TARGET) - 根据目标压力固定选择传感器
 * 
 * 自动模式 (AUTO)：
 *   - 优先检测是否进入目标压力模式（目标已设且压力稳定在目标附近）
 *   - 否则检测两个传感器在交叠区的一致性，一致则用软切换，不一致则用硬切换
 */

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "cdgSwitch.h"


// 传感器读取函数（示例模拟，实际使用需替换）
void ReadSensorsNorm(VATController *ctrl, float *low_norm, float *high_norm);

// ======================== 函数实现 ========================

// 初始化控制器（显式初始化两个真空规）
void VAT_Init(VATController *ctrl) {
    // 低量程真空规初始化
    ctrl->low_gauge.fs_norm  = LOW_FS_NORM;
    ctrl->low_gauge.offset   = 0.0f;
    ctrl->low_gauge.gain     = 1.0f;
    ctrl->low_gauge.enabled  = true;

    // 高量程真空规初始化（满量程为1.0，覆盖整个0~1范围）
    ctrl->high_gauge.fs_norm = 1.0f;
    ctrl->high_gauge.offset  = 0.0f;
    ctrl->high_gauge.gain    = 1.0f;
    ctrl->high_gauge.enabled = true;

    // 软切换阈值
    ctrl->switch_start_norm = LOW_FS_NORM * SWITCH_START_RATIO;
    ctrl->switch_end_norm   = LOW_FS_NORM * SWITCH_END_RATIO;

    // 硬切换阈值
    ctrl->hard_up_norm   = LOW_FS_NORM * HARD_UP_RATIO;
    ctrl->hard_down_norm = LOW_FS_NORM * HARD_DOWN_RATIO;
	ctrl->last_pressure_norm = 0;
    ctrl->using_low = true;

    // 模式设置
    ctrl->mode = MODE_AUTO;
    ctrl->auto_enabled = true;

    // 目标压力
    ctrl->target_norm = 0.0f;

    // 一致性检测
    ctrl->consistency_threshold = CONSISTENCY_THRESHOLD;
    ctrl->sensors_consistent = true;

    // 压力变化率
    ctrl->last_pressure_norm = 0.0f;
    ctrl->last_time = 0.0f;
    ctrl->dpdt_threshold = 0.01f;   // 每秒变化1%满量程（可调）
}

// 设置目标压力（归一化值）
void VAT_SetTargetNorm(VATController *ctrl, float target_norm) {
    ctrl->target_norm = target_norm;
}

// 强制指定模式（会临时关闭自动模式）
void VAT_ForceMode(VATController *ctrl, SwitchMode mode) {
    ctrl->mode = mode;
    ctrl->auto_enabled = false;
}

// 重新开启自动模式
void VAT_EnableAuto(VATController *ctrl) {
    ctrl->auto_enabled = true;
}

// 软切换算法
float SoftSwitch(VATController *ctrl, float low_val, float high_val) {
    float p_low = low_val;
    if (p_low <= ctrl->switch_start_norm)
        return low_val;
    if (p_low >= ctrl->switch_end_norm)
        return high_val;
    float weight = (p_low - ctrl->switch_start_norm) / 
                   (ctrl->switch_end_norm - ctrl->switch_start_norm);
    return (1.0f - weight) * low_val + weight * high_val;
}


// 控制器结构体中增加上一时刻压力值（用于判断趋势）
// 注意：需要在 VATController 中添加 float last_pressure_norm;
// 并在每次调用硬切换前更新趋势（在 VAT_GetControlPressure 中维护）

float HardSwitchWithTrend(VATController *ctrl, float low_val, float high_val,float current_pressure) {
    // 判断压力趋势（简单比较当前值和上一次值）
    // 首先根据当前压力绝对值决定是否强制切换
    if (low_val >= ctrl->hard_up_norm) {
        // 压力已经超过上升阈值，强制使用高量程
        ctrl->using_low = false;
    } else if (low_val <= ctrl->hard_down_norm) {
        // 压力已经低于下降阈值，强制使用低量程
        ctrl->using_low = true;
    } else {
        // 压力在迟滞区间内，根据趋势决定
        PressureTrend trend = PRESSURE_STABLE;
        if (current_pressure > ctrl->last_pressure_norm + 0.0001f)
            trend = PRESSURE_RISING;
        else if (current_pressure < ctrl->last_pressure_norm - 0.0001f)
            trend = PRESSURE_FALLING;
        
        if (trend == PRESSURE_RISING && ctrl->using_low && low_val >= ctrl->hard_up_norm) {
            ctrl->using_low = false;
        } else if (trend == PRESSURE_FALLING && !ctrl->using_low && low_val <= ctrl->hard_down_norm) {
            ctrl->using_low = true;
        }
        // 其他情况保持原状态
    }
    
    ctrl->last_pressure_norm = current_pressure;
    return ctrl->using_low ? low_val : high_val;

}

/**
 * 目标压力切换（改进版）
 * 逻辑：
 *   1. 如果目标压力 < TARGET_LOW_THRESHOLD（例如 0.1） 且 当前实际压力 > ACTUAL_HIGH_THRESHOLD（例如 0.1），
 *      则返回高量程读数（实际压力），避免低量程饱和。
 *   2. 否则，按原规则：若目标压力 < 低量程满量程，则用低量程，否则用高量程。
 */


float TargetSwitch(VATController *ctrl, float low_val, float high_val) {
    // 获取当前实际压力（以高量程为准，因为它量程大）
    float actual_pressure = high_val;
    
    // 条件：目标压力很小，但实际压力很大
    if (ctrl->target_norm < TARGET_LOW_THRESHOLD && actual_pressure > ACTUAL_HIGH_THRESHOLD) {
        return actual_pressure;   // 显示实际压力
    }
    
    // 否则使用原逻辑：根据目标压力与低量程满量程的关系选择传感器
    return (ctrl->target_norm < ctrl->low_gauge.fs_norm) ? low_val : high_val;
}

// 传感器一致性检测（仅在交叠区域内检测）
bool CheckConsistency(VATController *ctrl, float low_val, float high_val) {
    // 交叠区：低量程读数在 [switch_start, switch_end] 之间
    if (low_val >= ctrl->switch_start_norm && low_val <= ctrl->switch_end_norm) {
        float diff = fabsf(low_val - high_val);
        float rel = diff / low_val;
        return (rel <= ctrl->consistency_threshold);
    }
    // 不在交叠区时，保持上一次的状态（避免频繁变化）
    return ctrl->sensors_consistent;
}

// 自动模式选择逻辑
SwitchMode AutoSelectMode(VATController *ctrl, float low_val, float high_val, float current_time) {
    // 1. 优先检查是否应该进入目标压力模式
    if (ctrl->target_norm > 0.0f) {
        float error = fabsf(low_val - ctrl->target_norm);
        if (error < TARGET_MODE_DEADBAND * ctrl->low_gauge.fs_norm) {
            // 可选：增加稳定性判断，这里简化处理，直接认为可以进入目标模式
            return MODE_TARGET;
        }
    }

    // 2. 检测传感器一致性
    ctrl->sensors_consistent = CheckConsistency(ctrl, low_val, high_val);
    return ctrl->sensors_consistent ? MODE_SOFT : MODE_HARD;
}

// 读取两个真空规的归一化值（应用校准参数）
// 注意：此函数为模拟实现，实际应用中需要替换为硬件读取代码
void ReadSensorsNorm(VATController *ctrl, float *low_norm, float *high_norm) {
    // 模拟压力从 0.001 上升到 0.02（超过低量程满量程）再循环
    static float sim_pressure = 0.001f;
    sim_pressure *= 1.02f;
    if (sim_pressure > 0.02f) sim_pressure = 0.001f;

    // 原始传感器读数（未校准）
    float raw_low  = sim_pressure;
    float raw_high = sim_pressure;

    // 添加模拟噪声（低量程在低压时噪声相对较大）
    raw_low  += ((float)rand() / RAND_MAX - 0.5f) * 0.0005f;
    raw_high += ((float)rand() / RAND_MAX - 0.5f) * 0.0005f;
    if (raw_low  < 0.0f) raw_low  = 0.0f;
    if (raw_high < 0.0f) raw_high = 0.0f;
    if (raw_low  > 1.0f) raw_low  = 1.0f;
    if (raw_high > 1.0f) raw_high = 1.0f;

    // 应用校准参数 (offset, gain)
    *low_norm  = ctrl->low_gauge.gain  * (raw_low  + ctrl->low_gauge.offset);
    *high_norm = ctrl->high_gauge.gain * (raw_high + ctrl->high_gauge.offset);

    // 限制输出范围
    if (*low_norm  < 0.0f) *low_norm  = 0.0f;
    if (*low_norm  > 1.0f) *low_norm  = 1.0f;
    if (*high_norm < 0.0f) *high_norm = 0.0f;
    if (*high_norm > 1.0f) *high_norm = 1.0f;
}

// 主接口：输入当前压力归一化值 p_norm（0~1）和时间戳（秒），输出用于阀门控制的归一化压力
float VAT_GetControlPressure(VATController *ctrl, float low_val,float high_val) {
    // 读取两个真空规的归一化读数（已应用校准）
//    float low_val, high_val;
//    ReadSensorsNorm(ctrl, &low_val, &high_val);

    // 如果某个传感器被禁用，则使用另一个传感器的值（简化处理，此处假设都启用）
    if (!ctrl->low_gauge.enabled) return high_val;
    if (!ctrl->high_gauge.enabled) return low_val;

    // 自动模式选择
    if (ctrl->auto_enabled) {
        ctrl->mode = AutoSelectMode(ctrl, low_val, high_val, 0);
    }
   
    float current_pressure = high_val;
    // 根据选定模式计算输出压力
    float out_norm;
    switch (ctrl->mode) {
        case MODE_SOFT:
            out_norm = SoftSwitch(ctrl, low_val, high_val);
            break;
        case MODE_HARD:
            out_norm = HardSwitchWithTrend(ctrl, low_val, high_val,current_pressure);
            break;
        case MODE_TARGET:
            out_norm = TargetSwitch(ctrl, low_val, high_val);
            break;
        default:
            out_norm = low_val;
            break;
    }

    // 输出限制在 [0,1]
    if (out_norm < -1.3f) out_norm = 0.0f;
    if (out_norm > 1.0f) out_norm = 1.0f;
    return out_norm;
}

// ======================== 演示程序 ========================
VATController vat;

int cdgInit() {
    
    VAT_Init(&vat);
}
void cdgSwitchProc(float lowP,float highP,float *out)
{
    
	 *out = VAT_GetControlPressure(&vat, lowP,highP);

}
void setTarget(float setpoint)
{
	VAT_SetTargetNorm(&vat, setpoint);

}
	

    // 设置目标压力为 5 Torr (归一化 0.005)
//    VAT_SetTargetNorm(&vat, 0.005f);

//    printf("=== VAT TV阀双真空规自动切换算法演示（纯归一化）===\n");
//    printf("低量程满量程: %.4f (对应 %.1f Torr)\n", LOW_FS_NORM, LOW_FS_NORM * PRESSURE_MAX_TORR);
//    printf("软切换区间: %.4f ~ %.4f (归一化)\n", vat.switch_start_norm, vat.switch_end_norm);
//    printf("硬切换阈值: 上升 %.4f, 下降 %.4f\n", vat.hard_up_norm, vat.hard_down_norm);
//    printf("目标压力: %.4f (%.1f Torr)\n\n", vat.target_norm, vat.target_norm * PRESSURE_MAX_TORR);
//
//    printf("时间(s)\t输入压力\t低量程读数\t高量程读数\t当前模式\t输出压力\n");

//    float time = 0.0f;
//    for (int i = 0; i < 200; i++) {
//        // 模拟输入压力（从 0.001 缓慢上升至 0.02 再循环）
//        float p_in_norm = 0.001f * powf(1.01f, i);
//        if (p_in_norm > 0.02f) p_in_norm = 0.001f;

        
//        // 获取当前传感器读数用于显示
//        float low_val, high_val;
//        ReadSensorsNorm(&vat, &low_val, &high_val);
//
//        const char* mode_str = "";
//        switch (vat.mode) {
//            case MODE_SOFT:   mode_str = "软切换"; break;
//            case MODE_HARD:   mode_str = "硬切换"; break;
//            case MODE_TARGET: mode_str = "目标压力"; break;
//            default:          mode_str = "自动"; break;
//        }
//
//        printf("%.1f\t%.6f\t%.6f\t%.6f\t%s\t%.6f\n",
//               time, p_in_norm, low_val, high_val, mode_str, p_out_norm);
//
//        time += 0.1f;
//    }
//
//    return 0;
//}
