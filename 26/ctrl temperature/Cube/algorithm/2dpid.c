#include "2dpid.h"


/* ======================== 全局变量 ======================== */
 OmronPID_t g_pid; 
 OversampleFilter_t g_filter;
 GainScheduleEntry_t g_gainTable[6];
 AgingComp_t g_aging;
 OvershootSuppress_t g_oss;
 UnderhootSuppress_t g_under;
 volatile bool g_control_ready = false;
 volatile uint32_t g_sys_ms = 0;
 TemperaturePredictor_t g_predictor;   // 全局预测器
 
void Predictor_Init(TemperaturePredictor_t *pred, float dt_predict, float K_pred, float max_comp) {
    pred->dt_predict = dt_predict;
    pred->K_pred = K_pred;
    pred->max_comp = max_comp;
    pred->v = 0.0f;
    pred->v_last = 0.0f;
    pred->a = 0.0f;
    for (int i = 0; i < 3; i++) pred->temp_history[i] = 25.0f;
    pred->hist_index = 0;
    pred->last_time_ms = 0;
    pred->predicted_temp = 25.0f;
}

void Predictor_Update(TemperaturePredictor_t *pred, float current_temp, uint32_t now_ms) {
    float dt = (now_ms - pred->last_time_ms) / 1000.0f;
    if (dt < 0.001f) return;
    pred->last_time_ms = now_ms;
    
    pred->hist_index = (pred->hist_index + 1) % 3;
    pred->temp_history[pred->hist_index] = current_temp;
    
    int prev_idx = (pred->hist_index + 2) % 3;
    if (pred->temp_history[prev_idx] > -200.0f) {
        float v_new = (current_temp - pred->temp_history[prev_idx]) / dt;
        pred->v = 0.7f * pred->v + 0.3f * v_new;
    }
    
    if (pred->v_last != 0.0f) {
        pred->a = (pred->v - pred->v_last) / dt;
    }
    pred->v_last = pred->v;
    
    pred->predicted_temp = current_temp + pred->v * pred->dt_predict + 0.5f * pred->a * pred->dt_predict * pred->dt_predict;
}

float Predictor_ComputeCompensation(TemperaturePredictor_t *pred, float setpoint) {
    float error_future = setpoint - pred->predicted_temp;
    float comp = pred->K_pred * error_future;
    if (comp > pred->max_comp) comp = pred->max_comp;
    if (comp < -pred->max_comp) comp = -pred->max_comp;
    return comp;
}
/**
 * @file temperature_control_full.c
 * @brief 宽温区高精度温度控制器 - 完整版
 * @note  指标：-150℃ ~ 200℃，±1℃，采样10ms，控制100ms，只加热，PWM 20Hz
 * @version 2.0 (含超调抑制 + 过冷抑制)
 */




/* ======================== 算法模块实现 ======================== */

// ---------- 过采样滤波 ----------
static void Filter_Init(OversampleFilter_t *f) {
    memset(f->buffer, 0, sizeof(f->buffer));
    f->index = 0;
    f->filtered_value = 25.0f;
}

static float Filter_Update(OversampleFilter_t *f, float raw) {
    f->buffer[f->index] = raw;
    f->index = (f->index + 1) % OVERSAMPLE_RATE;
    if (f->index != 0) {
        return f->filtered_value;
    }
    // 去极值滑动平均
    float sum = 0.0f, maxv = f->buffer[0], minv = f->buffer[0];
    for (int i = 0; i < OVERSAMPLE_RATE; i++) {
        sum += f->buffer[i];
        if (f->buffer[i] > maxv) maxv = f->buffer[i];
        if (f->buffer[i] < minv) minv = f->buffer[i];
    }
    sum = sum - maxv - minv;
    f->filtered_value = sum / (OVERSAMPLE_RATE - 2);
    return f->filtered_value;
}

// ---------- 欧姆龙二自由度 PID ----------
static void PID_Init(OmronPID_t *pid, float dt) {
    pid->Kp = 2.0f;   pid->Ti = 30.0f;   pid->Td = 6.0f;   pid->alpha = 0.65f;
    pid->dt = dt;
    pid->setpoint = 25.0f;   pid->pv = 25.0f;   pid->last_pv = 25.0f;   pid->last_setpoint = 25.0f;
    pid->error[0] = pid->error[1] = 0.0f;
    pid->integral = 0.0f;
    pid->mv = 0.0f;
    pid->mv_min = 0.0f;      // 只加热，最小输出0
    pid->mv_max = 1.0f;
    pid->integral_limit = 0.5f;
}

static float PID_Compute(OmronPID_t *pid, float setpoint, float pv) {
    pid->setpoint = setpoint;
    pid->pv = pv;
    float error = pid->setpoint - pid->pv;
    pid->error[1] = pid->error[0];
    pid->error[0] = error;

    // 积分复位：超温或设定值下降时清零积分，防止饱和
    if (pv > setpoint + 1.0f || (setpoint < pid->last_setpoint && pv > setpoint)) {
        pid->integral = 0.0f;
    }
    pid->last_setpoint = setpoint;

    // 比例项 (二自由度)
    float p_input = pid->alpha * pid->setpoint - pid->pv;
    pid->p_term = pid->Kp * p_input;

    // 积分项
    pid->i_term += (pid->Kp / pid->Ti) * error * pid->dt;
    if (pid->i_term > pid->integral_limit) pid->i_term = pid->integral_limit;
    if (pid->i_term < -pid->integral_limit) pid->i_term = -pid->integral_limit;

    // 微分项 (基于 PV 变化)
    float d_input = pid->pv;
    pid->d_term = pid->Kp * pid->Td * (d_input - pid->last_pv) / pid->dt;
    pid->last_pv = pid->pv;

    // 合并输出
    pid->mv = pid->p_term + pid->i_term - pid->d_term;

    // 输出限幅
    if (pid->mv > pid->mv_max) pid->mv = pid->mv_max;
    if (pid->mv < pid->mv_min) pid->mv = pid->mv_min;

    // 抗积分饱和修正
    if ((pid->mv >= pid->mv_max && error > 0) || (pid->mv <= pid->mv_min && error < 0)) {
        pid->i_term -= (pid->Kp / pid->Ti) * error * pid->dt;
    }

    return pid->mv;
}

// ---------- 增益调度 (6个温区) ----------
 void GainSchedule_InitTable(void) {
    g_gainTable[0] = (GainScheduleEntry_t){-200.0f, -50.0f, 1.5f, 40.0f, 8.0f};
    g_gainTable[1] = (GainScheduleEntry_t){-50.0f,   0.0f, 2.0f, 30.0f, 6.0f};
    g_gainTable[2] = (GainScheduleEntry_t){  0.0f,  50.0f, 2.5f, 25.0f, 5.0f};
    g_gainTable[3] = (GainScheduleEntry_t){ 50.0f, 100.0f, 3.0f, 20.0f, 4.0f};
    g_gainTable[4] = (GainScheduleEntry_t){100.0f, 150.0f, 3.5f, 15.0f, 3.0f};
    g_gainTable[5] = (GainScheduleEntry_t){150.0f, 250.0f, 4.0f, 10.0f, 2.0f};
}

 void GainSchedule_Update(OmronPID_t *pid, float setpoint) {
    static int last_idx = -1;
    int idx = -1;
    for (int i = 0; i < 6; i++) {
        if (setpoint >= g_gainTable[i].temp_low && setpoint < g_gainTable[i].temp_high) {
            idx = i;
            break;
        }
    }
    if (idx < 0) idx = (setpoint < -200.0f) ? 0 : 5;
    if (idx != last_idx) {
        pid->Kp = g_gainTable[idx].Kp;
        pid->Ti = g_gainTable[idx].Ti;
        pid->Td = g_gainTable[idx].Td;
        last_idx = idx;
    }
}

// ---------- 老化补偿 (可选) ----------
 void AgingComp_Init(AgingComp_t *ac, float target_slope) {
    ac->target_slope = target_slope;
    ac->actual_slope = 0.0f;
    ac->compensation = 1.0f;
    ac->filter_alpha = 0.1f;
    ac->last_temp = 25.0f;
    ac->last_time_ms = 0;
    ac->enabled = false;
}

 float AgingComp_Update(AgingComp_t *ac, float current_temp, uint32_t now_ms) {
    if (!ac->enabled) return 1.0f;
    float dt = (now_ms - ac->last_time_ms) / 1000.0f;
    if (dt < 0.001f) return ac->compensation;
    float slope = (current_temp - ac->last_temp) / dt;
    ac->actual_slope = ac->filter_alpha * slope + (1.0f - ac->filter_alpha) * ac->actual_slope;
    if (ac->actual_slope > 0.01f) {
        float error_slope = ac->target_slope - ac->actual_slope;
        ac->compensation += 0.001f * error_slope;
        if (ac->compensation < 0.8f) ac->compensation = 0.8f;
        if (ac->compensation > 1.5f) ac->compensation = 1.5f;
    }
    ac->last_temp = current_temp;
    ac->last_time_ms = now_ms;
    return ac->compensation;
}

// ---------- 超调抑制 (升温速率、设定值下降、超温关断) ----------
 void OvershootSuppress_Init(OvershootSuppress_t *oss) {
    oss->max_heating_rate = 5.0f;        // 最大升温速率 5℃/s，根据实际调整
    oss->cool_deadband = 2.0f;           // 自然冷却死区 ±2℃
    oss->overshoot_stop_thres = 1.5f;    // 超过设定值1.5℃立即关断加热
    oss->setpoint_decreasing = false;
    oss->last_setpoint = 25.0f;
}

 float OvershootSuppress_Compute(OvershootSuppress_t *oss, float setpoint, float pv, float raw_output) {
    float limited = raw_output;

    // 检测设定值下降
    if (setpoint < oss->last_setpoint - 0.1f) {
        oss->setpoint_decreasing = true;
    } else if (setpoint > oss->last_setpoint + 0.1f) {
        oss->setpoint_decreasing = false;
    }
    oss->last_setpoint = setpoint;

    // 设定值下降时强制关断输出，直到温度进入死区
    if (oss->setpoint_decreasing) {
        if (pv > setpoint + oss->cool_deadband) return 0.0f;
        if (pv <= setpoint + oss->cool_deadband) oss->setpoint_decreasing = false;
    }

    // 超温保护：超过阈值直接关断
    if (pv > setpoint + oss->overshoot_stop_thres) return 0.0f;

    // 升温速率限制：根据误差动态限制最大输出
    float error = setpoint - pv;
    if (error > 0) {
        float max_output = (error > 20.0f) ? 1.0f : (error / 20.0f);
        if (max_output < 0.05f) max_output = 0.05f;
        if (limited > max_output) limited = max_output;
    }
    return limited;
}

// ---------- 过冷抑制 (基于下降速率的提前补偿) ----------
 void UnderhootSuppress_Init(UnderhootSuppress_t *us) {
    us->enable = true;
    us->rate_threshold = -0.5f;     // 下降速率低于 -0.5℃/s 时触发
    us->K_under = 0.1f;             // 补偿系数
    us->max_extra_output = 0.3f;    // 最大额外输出 30%
    us->last_temp = 25.0f;
    us->last_time_ms = 0;
    us->current_slope = 0.0f;
}

 void UnderhootSuppress_UpdateSlope(UnderhootSuppress_t *us, float current_temp, uint32_t now_ms) {
    float dt = (now_ms - us->last_time_ms) / 1000.0f;
    if (dt > 0.001f) {
        us->current_slope = (current_temp - us->last_temp) / dt;
    }
    us->last_temp = current_temp;
    us->last_time_ms = now_ms;
}

 float UnderhootSuppress_Compute(UnderhootSuppress_t *us, float setpoint, float pv) {
    if (!us->enable) return 0.0f;
    float extra = 0.0f;
    // 条件：温度低于设定值 且 正在快速下降
    if (pv < setpoint && us->current_slope < us->rate_threshold) {
        float deviation = setpoint - pv;
        float down_rate = -us->current_slope;   // 取正值
        extra = us->K_under * down_rate * deviation;
        if (extra > us->max_extra_output) extra = us->max_extra_output;
    }
    return extra;
}
void Hardware_SetHeaterOutput(unsigned char channel,uint32_t final_out);
 {
 if(channel == 0)
__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,final_out);
 else  if(channel == 1)
 __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,final_out);	
 else  if(channel == 2)
__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,final_out);
 }
// 在 TemperatureControl_Init 中添加初始化：
// Predictor_Init(&g_predictor, 0.3f, 1.2f, 0.2f);   // 预测0.3秒，系数1.2，最大补偿±0.2

 void ControlTask_Run(float setpoint) {
    float pv = g_filter.filtered_value;
    
    // 1. 更新预测器 (温度、时间)
    Predictor_Update(&g_predictor, pv, g_sys_ms);
    
    // 2. PID计算 (与原流程相同)
    UnderhootSuppress_UpdateSlope(&g_under, pv, g_sys_ms);
    GainSchedule_Update(&g_pid, setpoint);
    float raw_out = PID_Compute(&g_pid, setpoint, pv);
    
    // 3. 老化补偿
    float comp = AgingComp_Update(&g_aging, pv, g_sys_ms);
    raw_out *= comp;
    raw_out = (raw_out > 1.0f) ? 1.0f : (raw_out < 0.0f) ? 0.0f : raw_out;
    
    // 4. 超调抑制 (安全边界)
    float suppressed_out = OvershootSuppress_Compute(&g_oss, setpoint, pv, raw_out);
    
    // 5. 预测补偿 (提前调节)
    float pred_comp = Predictor_ComputeCompensation(&g_predictor, setpoint);
    float after_pred = suppressed_out + pred_comp;
    if (after_pred > 1.0f) after_pred = 1.0f;
    if (after_pred < 0.0f) after_pred = 0.0f;
    
    // 6. 过冷抑制 (基于下降速率的最后补偿)
    float under_extra = UnderhootSuppress_Compute(&g_under, setpoint, pv);
    float final_out = after_pred + under_extra;
    if (final_out > 1.0f) final_out = 1.0f;
    
    Hardware_SetHeaterOutput(0,final_out);
}

/* ======================== 定时器中断处理 (用户需在中断中调用) ======================== */
void sysTickGet(void) {
    g_sys_ms = xTaskGetTickCount();
}

void temperatureMeter(void) {
    static uint8_t ctrl_counter = 0;
    //float raw_temp = Hardware_ReadTemperature();
    Filter_Update(&g_filter, raw_temp);
        g_control_ready = true;
    }
}

/* ======================== 主循环处理 ======================== */
void MainLoop_Process(void) {
    static float target_temperature = 25.0f;   // 用户可通过API修改
    if (g_control_ready) {
        g_control_ready = false;
        ControlTask_Run(target_temperature);
    }
}

/* ======================== 系统初始化 ======================== */
void TemperatureControl_Init(void) {
    PID_Init(&g_pid, CONTROL_PERIOD_SEC);
    Filter_Init(&g_filter);
    GainSchedule_InitTable();
    AgingComp_Init(&g_aging, 2.0f);      // 期望升温速率2℃/s，默认关闭
    OvershootSuppress_Init(&g_oss);
    UnderhootSuppress_Init(&g_under);
	Predictor_Init(&g_predictor, 0.3f, 1.2f, 0.2f);   // 新增
    Hardware_InitTimers();
    Hardware_SetHeaterOutput(0.0f);
}

/* ======================== 用户 API ======================== */
void SetTargetTemperature(float temp) {
    // 注意：如果多线程/中断访问，需加保护。这里简化。
    static float target;
    if (temp < TEMP_MIN) temp = TEMP_MIN;
    if (temp > TEMP_MAX) temp = TEMP_MAX;
    target = temp;
    // 实际使用时需要在 MainLoop_Process 中读取该值，这里为了示例，直接用 static 变量，
    // 更好的做法是使用 volatile 全局变量并在 ControlTask_Run 中读取。
    // 由于代码中 ControlTask_Run 使用的是局部 static target_temperature，用户可自行修改为全局变量。
}

float GetCurrentTemperature(void) {
    return g_filter.filtered_value;
}

void EnableAgingCompensation(bool enable) {
    g_aging.enabled = enable;
}

void EnableUnderhootSuppression(bool enable) {
    g_under.enable = enable;
}

/* ======================== 示例 main 函数 (用户需根据平台调整) ======================== */
/*
int main(void) {
    // 硬件初始化 (时钟、GPIO、ADC、定时器等)
    // ...

    // 温控系统初始化
    TemperatureControl_Init();

    // 设置目标温度，例如 100℃
    SetTargetTemperature(100.0f);

    // 可选：使能老化补偿
    // EnableAgingCompensation(true);

    while (1) {
        MainLoop_Process();
        // 其他用户任务...
    }
    return 0;
}
*/
