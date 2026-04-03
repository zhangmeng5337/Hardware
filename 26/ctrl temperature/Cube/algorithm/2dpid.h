
#ifndef DPID_H_
#define DPID_H_
#include "main.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ======================== 用户配置 ======================== */
#define CONTROL_PERIOD_SEC    0.1f     // 控制周期 100ms
#define SAMPLE_PERIOD_SEC     0.01f    // 采样周期 10ms
#define OVERSAMPLE_RATE       10       // 过采样率 = 控制周期/采样周期
#define TEMP_MIN              -150.0f
#define TEMP_MAX              200.0f

/* ======================== 数据结构 ======================== */

// 欧姆龙二自由度 PID
typedef struct {
    float Kp, Ti, Td, alpha;
    float setpoint, pv, last_pv, last_setpoint;
    float error[2];
    float integral;
    float p_term, i_term, d_term;
    float mv, mv_min, mv_max;
    float integral_limit;
    float dt;
	  float suppressed_out;
	  float pred_comp;
	  float after_pred;
	  float under_extra;
	  float final_out;
	  float raw_out;
	float after_under;
} OmronPID_t;

// 过采样滤波器 (去极值滑动平均)
typedef struct {
    float buffer[OVERSAMPLE_RATE];
    uint8_t index;
    float filtered_value;
} OversampleFilter_t;

// 增益调度表项
typedef struct {
    float temp_low, temp_high;
    float Kp, Ti, Td;
} GainScheduleEntry_t;

// 老化补偿 (可选)
typedef struct {
    float target_slope, actual_slope, compensation, filter_alpha;
    float last_temp;
    uint32_t last_time_ms;
    bool enabled;
} AgingComp_t;

// 超调抑制 (升温速率限制、设定值下降、超温关断)
typedef struct {
    float max_heating_rate;
    float cool_deadband;
    float overshoot_stop_thres;
    bool setpoint_decreasing;
    float last_setpoint;
	  float error;
	  float max_output;
	  float limited;
	  float factor;
} OvershootSuppress_t;

// 过冷抑制 (基于斜率的提前补偿)
typedef struct {
    bool enable;
    float rate_threshold;       // 下降速率阈值 (℃/s)，低于此值触发补偿
    float K_under;              // 补偿系数
    float max_extra_output;     // 最大额外输出 (0~1)
    float last_temp;
    uint32_t last_time_ms;
    float current_slope;        // 当前温度变化率 (℃/s)
} UnderhootSuppress_t;
// ==================== 温度预测器 ====================
typedef struct {
    float dt_predict;
    float K_pred;
    float max_comp;
    float v;
    float v_last;
    float a;
    float temp_history[3];
    uint8_t hist_index;
    uint32_t last_time_ms;
    float predicted_temp;
} TemperaturePredictor_t;
void TemperatureControl_Init(void);
 void ControlTask_Run(unsigned char channel ,float setpoint) ;
 void sysTickGet(unsigned char channel);
void updatePIDperiod(void);
#endif

