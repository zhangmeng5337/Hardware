#include "2dpid.h"
#include "temperature_controller.h"
extern  temperatureStru temperatureU[3];
extern unsigned char divT ;
/* ======================== 全局变量 ======================== */
 OmronPID_t g_pid[3]; 
 OversampleFilter_t g_filter;
 GainScheduleEntry_t g_gainTable[6];
 predictor_stru predictor[6];
  predictor_stru predictorusr[3];
 AgingComp_t g_aging[3];
 OvershootSuppress_t g_oss[3];
 UnderhootSuppress_t g_under[3];
 //volatile bool g_control_ready = false;
 volatile uint32_t g_sys_ms[3] = 0;
 TemperaturePredictor_t g_predictor[3];   // 全局预测器
 
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
    pid->Kp = 2.0f;   pid->Ti = 30.0f;   pid->Td = 6.0f;   pid->alpha = 1.0f;
    pid->dt = dt;
    pid->setpoint = 25.0f;   pid->pv = 25.0f;   pid->last_pv = 25.0f;   pid->last_setpoint = 25.0f;
    pid->error[0] = pid->error[1] = 0.0f;
    pid->integral = 0.0f;
    pid->mv = 0.0f;
    pid->mv_min = 0.0f;      // 只加热，最小输出0
    pid->mv_max = 1.0f;
    pid->integral_limit = 0.8f;
}

static float PID_Compute(OmronPID_t *pid, float setpoint, float pv) {
    pid->setpoint = setpoint;
    pid->pv = pv;
    float error = pid->setpoint - pid->pv;
    pid->error[1] = pid->error[0];
    pid->error[0] = error;
  // 积分复位：超温或设定值下降时清零积分，防止饱和
    if (pv > setpoint+0.5 ) {
        pid->i_term = 0.0f;
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
    g_gainTable[0] = (GainScheduleEntry_t){-200.0f, -50.0f, 0.02f, 40.0f, 8.0f};
    g_gainTable[1] = (GainScheduleEntry_t){-50.0f,   0.0f, 0.02f, 60.0f, 6.0f};
    g_gainTable[2] = (GainScheduleEntry_t){  0.0f,  50.0f, 0.02f, 80.0f, 5.0f};
    g_gainTable[3] = (GainScheduleEntry_t){ 50.0f, 100.0f, 0.02f, 20.0f, 7.0f};
    g_gainTable[4] = (GainScheduleEntry_t){100.0f, 150.0f, 0.05f, 6.0f, 1.0f};
    g_gainTable[5] = (GainScheduleEntry_t){150.0f, 250.0f, 0.05f, 20.0f, 2.0f};
}
// ---------- 增益调度 (6个温区) ----------
 void predictSchedule_InitTable(void) {
    predictor[0] = (predictor_stru){-200.0f, -50.0f,	0.4,	1.5,	0.8,	20};
    predictor[1] = (predictor_stru){-50.0,   0.0f,		0.4,	1.5,	1,		40};
    predictor[2] = (predictor_stru){0.0,     50.0f,		0.4,	1.5,	0.8,	41};
    predictor[3] = (predictor_stru){50.0,   100.0f,		0.6,	1.5,	0.8,	42};
    predictor[4] = (predictor_stru){100.0,  150.0f,		0.4,	2.5,	1,		80};
    predictor[5] = (predictor_stru){150.0,  250.0f,		0.4,	1.2,	1,		100};
}
  void preditc_Update(predictor_stru *predictorU, float setpoint) {
    static int last_idx = -1;
    int idx = -1;
    for (int i = 0; i < 6; i++) {
        if (setpoint >= predictor[i].tempLow && setpoint < predictor[i].tempHigh) {
            idx = i;
            break;
        }
    }
		    float dtPredict;
    float kPred;
	  float maxComp;
	  float factor;
    if (idx < 0) idx = (setpoint < -200.0f) ? 0 : 5;
    if (idx != last_idx) {
			predictorU->dtPredict = predictor[idx].dtPredict;
			predictorU->kPred = predictor[idx].kPred;
			predictorU->maxComp = predictor[idx].maxComp;
			predictorU->factor = predictor[idx].factor;
			predictorU->tempLow =  predictor[idx].tempLow;
			predictorU->tempHigh =  predictor[idx].tempHigh;
        last_idx = idx;
    }
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
    oss->cool_deadband = 1.0f;           // 自然冷却死区 ±2℃
    oss->overshoot_stop_thres = 1.5f;    // 超过设定值1.5℃立即关断加热
    oss->setpoint_decreasing = false;
    oss->last_setpoint = 25.0f;
	 oss->factor = 1.0f;
}

 float OvershootSuppress_Compute(OvershootSuppress_t *oss, float setpoint, float pv, float raw_output) {
     oss->limited = raw_output;

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
     oss->error = setpoint - pv;
    if (oss->error > 0) {
         oss->max_output = (oss->error > 20.0f) ? 1.0f : (oss->error* oss->factor/ 20.0f);
        if (oss->max_output < 0.05f) oss->max_output = 0.05f;
        if (oss->limited > oss->max_output) oss->limited = oss->max_output;
    }
    return oss->limited;
}

// ---------- 过冷抑制 (基于下降速率的提前补偿) ----------
 void UnderhootSuppress_Init(UnderhootSuppress_t *us) {
    us->enable = true;
    us->rate_threshold = -0.1f;     // 下降速率低于 -0.5℃/s 时触发
    us->K_under = 0.1f;             // 补偿系数
    us->max_extra_output = 0.6f;    // 最大额外输出 30%
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

// 在 TemperatureControl_Init 中添加初始化：
// Predictor_Init(&g_predictor, 0.3f, 1.2f, 0.2f);   // 预测0.3秒，系数1.2，最大补偿±0.2
float buf[3];
extern  float slopekft[3];
extern float pwmMax;
void ControlTask_Run(unsigned char channel ,float setpoint) {
    float pv = slopekft[channel];//temperatureU[channel].temperatureFlt;
    
    // 1. 更新预测器 (温度、时间)
	 preditc_Update(&predictorusr[0], setpoint);
	 g_predictor[channel].dt_predict = predictorusr[0].dtPredict;
	 g_predictor[channel].K_pred = predictorusr[0].kPred;
	 g_predictor[channel].max_comp = predictorusr[0].maxComp;	 
	 g_oss[channel].factor = predictorusr[0].factor;
    Predictor_Update(&g_predictor[channel], pv, g_sys_ms[channel]);
    
    // 2. PID计算 (与原流程相同)
    UnderhootSuppress_UpdateSlope(&g_under[channel], pv, g_sys_ms[channel]);
    GainSchedule_Update(&g_pid[channel], setpoint);
     g_pid[channel].raw_out = PID_Compute(&g_pid[channel], setpoint, pv);
    
//    // 3. 老化补偿
//    float comp = AgingComp_Update(&g_aging[channel], pv, g_sys_ms);
//    raw_out *= comp;
//    raw_out = (raw_out > 1.0f) ? 1.0f : (raw_out < 0.0f) ? 0.0f : raw_out;
    

    // 5. 预测补偿 (提前调节)
     g_pid[channel].pred_comp = Predictor_ComputeCompensation(&g_predictor[channel], setpoint);
     g_pid[channel].after_pred =  g_pid[channel].raw_out +  g_pid[channel].pred_comp;
    if (g_pid[channel].after_pred > 1.0f) g_pid[channel].after_pred = 1.0f;
    if (g_pid[channel].after_pred < 0.0f) g_pid[channel].after_pred = 0.0f;
    

		
    // 6. 过冷抑制 (基于下降速率的最后补偿)
     g_pid[channel].under_extra = UnderhootSuppress_Compute(&g_under[channel], setpoint, pv);
     g_pid[channel].after_under = g_pid[channel].after_pred +g_pid[channel]. under_extra;
    if (g_pid[channel].after_under > 1.0f) g_pid[channel].after_under = 1.0f;   

// 4. 超调抑制 (安全边界)
if(channel == 2)
{
	unsigned char result;
	result = 1;
}
     g_pid[channel].suppressed_out = OvershootSuppress_Compute(&g_oss[channel], setpoint, pv,  g_pid[channel].after_under);
    		 g_pid[channel].final_out =  g_pid[channel].suppressed_out;
    if(g_pid[channel].suppressed_out!=0)
			buf[channel] = g_pid[channel].final_out;
		float pwmHighTime = pwmMax / 50/PERIOD;
		
		if(g_pid[channel].final_out*pwmHighTime < 10)
			g_pid[channel].final_out = 0;
    Hardware_SetHeaterOutput(channel,g_pid[channel].final_out);
}

/* ======================== 定时器中断处理 (用户需在中断中调用) ======================== */
void sysTickGet(unsigned char channel) {
    g_sys_ms[channel] = xTaskGetTickCount();
}



/* ======================== 主循环处理 ======================== */
void MainLoop_Process(void) {
  
}

/* ======================== 系统初始化 ======================== */
void TemperatureControl_Init(void) {
  PID_Init(&g_pid[0], temperatureU[0].periodMeter*temperatureU[0].periodTask);
	PID_Init(&g_pid[1], temperatureU[1].periodMeter*temperatureU[1].periodTask);
	PID_Init(&g_pid[2], temperatureU[2].periodMeter*temperatureU[2].periodTask);
    //Filter_Init(&g_filter);
    GainSchedule_InitTable();
	  predictSchedule_InitTable();
    AgingComp_Init(&g_aging[0], 2.0f);      // 期望升温速率2℃/s，默认关闭
    AgingComp_Init(&g_aging[1], 2.0f);      // 期望升温速率2℃/s，默认关闭
    AgingComp_Init(&g_aging[2], 2.0f);      // 期望升温速率2℃/s，默认关闭
    
	g_oss[0].factor = 5;
	g_oss[1].factor = 5;
	g_oss[1].factor = 5;
    OvershootSuppress_Init(&g_oss[0]);
	OvershootSuppress_Init(&g_oss[1]);
	OvershootSuppress_Init(&g_oss[2]);
    UnderhootSuppress_Init(&g_under[0]);
	UnderhootSuppress_Init(&g_under[1]);
	UnderhootSuppress_Init(&g_under[2]);
	Predictor_Init(&g_predictor[0], 0.5f, 1.2f, 1.0f);   // 新增
	Predictor_Init(&g_predictor[1], 0.5f, 1.2f, 1.0f);	 // 新增
	Predictor_Init(&g_predictor[2], 0.8f, 1.8f, 1.0f);	 // 新增
//0.8 1.8 1  
	//pid 5 2 1//0.01 5 7
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
void updatePIDperiod()
{

	g_pid[0].dt = temperatureU[0].periodMeter*temperatureU[0].periodTask;
	g_pid[1].dt = temperatureU[1].periodMeter*temperatureU[1].periodTask;	
	g_pid[2].dt = temperatureU[2].periodMeter*temperatureU[2].periodTask;	
}

//float GetCurrentTemperature(void) {
//    return g_filter.filtered_value;
//}
//
//void EnableAgingCompensation(bool enable) {
//    g_aging.enabled = enable;
//}
//
//void EnableUnderhootSuppression(bool enable) {
//    g_under.enable = enable;
//}

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
