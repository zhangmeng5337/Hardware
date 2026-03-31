/******************************************************************************
 * 文件: feedforward_control.c
 * 描述: 纯前馈 + 轻量反馈校正 温控系统
 *       适用于大滞后、强非线性温控对象
 ******************************************************************************/

#include <stdint.h>
#include <math.h>

/* ======================= 硬件抽象层 ===================================== */
extern float read_temperature_sensor(void);
extern void set_heater_pwm(float percent);
extern void get_timer_interval_ms(uint32_t *ms);

/* ============================= 宏定义 ===================================== */
#define CONTROL_PERIOD_MS   50      // 控制周期50ms（可适当放宽）
#define SAMPLE_PERIOD_S     0.05f

// 前馈参数
#define FEEDFORWARD_UPDATE_PERIOD  1000  // 前馈模型更新周期(ms)
#define MIN_OUTPUT          0.0f
#define MAX_OUTPUT          100.0f

// 反馈校正参数（轻量级）
#define CORRECTION_GAIN     0.3f    // 校正增益（0~1，越小越平滑）
#define CORRECTION_DEADZONE 0.5f    // 校正死区(℃)，误差小于此值不校正

/* ============================= 数据结构 =================================== */

// 前馈模型（分段线性或多项式）
typedef struct {
    // 分段点温度(℃)
    float temp_points[5];
    // 对应稳态功率(%)
    float power_points[5];
    // 分段数量
    uint8_t num_points;
} FeedforwardModel_t;

// 简易一阶滤波器（用于平滑）
typedef struct {
    float value;
    float alpha;    // 滤波系数(0~1)，越小越平滑
} LowPassFilter_t;

// 温控系统
typedef struct {
    FeedforwardModel_t model;   // 前馈模型
    LowPassFilter_t    filter;  // 输出平滑滤波器
    
    float setpoint;             // 目标温度(℃)
    float output;               // 输出功率(%)
    float last_error;           // 上次误差（用于趋势判断）
    
    uint32_t last_update_ms;    // 上次模型更新时间
    uint8_t enable;
} TempControl_t;

/* ========================= 工具函数 ======================================= */

// 初始化低通滤波器
void Filter_Init(LowPassFilter_t *f, float init_val, float alpha) {
    f->value = init_val;
    f->alpha = alpha;
}

// 低通滤波
float Filter_Update(LowPassFilter_t *f, float input) {
    f->value = f->alpha * input + (1.0f - f->alpha) * f->value;
    return f->value;
}

// 线性插值
float LinearInterp(float x, float x1, float y1, float x2, float y2) {
    if (x2 == x1) return y1;
    return y1 + (y2 - y1) * (x - x1) / (x2 - x1);
}

/* ========================= 前馈模型建立 =================================== */

// 初始化前馈模型（需根据实际系统标定）
void FeedforwardModel_Init(FeedforwardModel_t *m) {
    // 典型分段点：温度 -> 维持该温度所需的功率百分比
    // 这些值需要通过实验标定获得
    m->num_points = 5;
    
    // 温度点(℃)
    m->temp_points[0] = -150.0f;
    m->temp_points[1] = -50.0f;
    m->temp_points[2] = 25.0f;
    m->temp_points[3] = 100.0f;
    m->temp_points[4] = 200.0f;
    
    // 对应稳态功率(%) - 初始估算值，实际需校准
    // 低温区需要大功率维持（因为散热快），高温区也需较大功率
    m->power_points[0] = 60.0f;   // -150℃ 维持功率
    m->power_points[1] = 30.0f;   // -50℃
    m->power_points[2] = 0.0f;    // 25℃ 室温（与环境平衡）
    m->power_points[3] = 35.0f;   // 100℃
    m->power_points[4] = 70.0f;   // 200℃
}

// 根据目标温度查询前馈功率（非线性插值）
float FeedforwardModel_Lookup(FeedforwardModel_t *m, float temp) {
    // 边界处理
    if (temp <= m->temp_points[0]) return m->power_points[0];
    if (temp >= m->temp_points[m->num_points - 1]) 
        return m->power_points[m->num_points - 1];
    
    // 查找所在区间
    for (int i = 0; i < m->num_points - 1; i++) {
        if (temp >= m->temp_points[i] && temp <= m->temp_points[i+1]) {
            return LinearInterp(temp, 
                                m->temp_points[i], m->power_points[i],
                                m->temp_points[i+1], m->power_points[i+1]);
        }
    }
    return 0.0f;
}

// 动态更新模型（在线学习）- 可选功能
void FeedforwardModel_Update(FeedforwardModel_t *m, float temp, float steady_power) {
    // 找到最近的分段点进行微调
    // 使用指数加权平均更新
    for (int i = 0; i < m->num_points; i++) {
        if (fabsf(temp - m->temp_points[i]) < 20.0f) {
            // 加权更新，学习率0.1
            m->power_points[i] = 0.9f * m->power_points[i] + 0.1f * steady_power;
            break;
        }
    }
}

/* ========================= 温控系统核心 =================================== */

// 初始化温控系统
void TempControl_Init(TempControl_t *tc, float init_temp, float init_setpoint) {
    FeedforwardModel_Init(&tc->model);
    Filter_Init(&tc->filter, 0.0f, 0.3f);  // 平滑系数0.3
    
    tc->setpoint = init_setpoint;
    tc->output = 0.0f;
    tc->last_error = 0.0f;
    tc->last_update_ms = 0;
    tc->enable = 1;
}

// 设置目标温度
void TempControl_SetSetpoint(TempControl_t *tc, float setpoint) {
    tc->setpoint = setpoint;
}

// 获取当前温度
float TempControl_GetTemp(TempControl_t *tc) {
    return read_temperature_sensor();  // 直接读取，无需复杂滤波
}

// 计算前馈输出（核心算法）
float TempControl_ComputeFeedforward(TempControl_t *tc, float current_temp) {
    float target = tc->setpoint;
    float current = current_temp;
    float error = target - current;
    
    // 1. 稳态前馈：目标温度所需的维持功率
    float steady_power = FeedforwardModel_Lookup(&tc->model, target);
    
    // 2. 动态前馈：温差驱动的额外功率
    //    使用非线性函数：温差大时快速逼近，温差小时平滑过渡
    float dynamic_power = 0.0f;
    
    if (error > 0) {
        // 需要加热
        if (error > 30.0f) {
            // 大温差：全速加热
            dynamic_power = 100.0f - steady_power;
        } else if (error > 5.0f) {
            // 中温差：线性比例
            dynamic_power = (error - 5.0f) / 25.0f * (100.0f - steady_power);
        } else if (error > 0.5f) {
            // 小温差：微调
            dynamic_power = (error / 5.0f) * 10.0f;
        } else {
            // 进入死区
            dynamic_power = 0.0f;
        }
    } else {
        // 需要降温：只能停止加热
        dynamic_power = 0.0f;
        // 如果超温较多，可以考虑主动散热（如果有风扇等）
        if (error < -5.0f) {
            // 关闭加热，依靠自然散热
            dynamic_power = -steady_power;  // 减少输出
        }
    }
    
    // 3. 变化率抑制（防止超调）
    //    根据温度变化趋势调整输出
    float dt = error - tc->last_error;
    tc->last_error = error;
    
    // 如果温度快速上升且接近目标，提前减少输出
    if (dt > 0.5f && error < 10.0f) {
        // 升温过快且接近目标，降低动态功率
        dynamic_power *= (1.0f - (dt / 5.0f));
        if (dynamic_power < 0) dynamic_power = 0;
    }
    
    // 4. 合并输出
    float output = steady_power + dynamic_power;
    
    // 限幅
    if (output > MAX_OUTPUT) output = MAX_OUTPUT;
    if (output < MIN_OUTPUT) output = MIN_OUTPUT;
    
    return output;
}

// 轻量级反馈校正（可选，可禁用）
float TempControl_ApplyCorrection(TempControl_t *tc, float output, float error) {
    // 只在接近目标时进行微小校正
    if (fabsf(error) < CORRECTION_DEADZONE) {
        return output;  // 在死区内不校正
    }
    
    // 简单的比例校正，增益很小
    float correction = error * CORRECTION_GAIN;
    
    // 限制最大校正量（不超过10%）
    if (correction > 10.0f) correction = 10.0f;
    if (correction < -10.0f) correction = -10.0f;
    
    float new_output = output + correction;
    if (new_output > MAX_OUTPUT) new_output = MAX_OUTPUT;
    if (new_output < MIN_OUTPUT) new_output = MIN_OUTPUT;
    
    return new_output;
}

// 主控步进
void TempControl_Step(TempControl_t *tc) {
    if (!tc->enable) return;
    
    // 1. 读取当前温度
    float current_temp = read_temperature_sensor();
    float error = tc->setpoint - current_temp;
    
    // 2. 计算前馈输出
    float ff_output = TempControl_ComputeFeedforward(tc, current_temp);
    
    // 3. 可选：应用轻量反馈校正
    float output = TempControl_ApplyCorrection(tc, ff_output, error);
    
    // 4. 平滑输出（避免突变）
    output = Filter_Update(&tc->filter, output);
    
    tc->output = output;
    
    // 5. 驱动执行器
    set_heater_pwm(tc->output);
}

// 在线模型校准（可选，长时间运行时使用）
void TempControl_Calibrate(TempControl_t *tc) {
    uint32_t now;
    get_timer_interval_ms(&now);
    
    // 每隔一段时间，在稳态时更新模型
    if (now - tc->last_update_ms >= FEEDFORWARD_UPDATE_PERIOD) {
        float current_temp = read_temperature_sensor();
        float error = tc->setpoint - current_temp;
        
        // 判断是否进入稳态（误差小且输出稳定）
        if (fabsf(error) < 1.0f) {
            // 记录当前温度与维持功率的关系
            FeedforwardModel_Update(&tc->model, current_temp, tc->output);
            tc->last_update_ms = now;
        }
    }
}

