#include "temperature_controller.h"
#include "math.h"
extern TIM_HandleTypeDef htim4;
temperatureStru temperatureU[3];


// 温区边界
static const float ZONE_BOUNDARIES[] = {-150.0f, -100.0f, -50.0f, 0.0f, 50.0f, 100.0f, 150.0f, 200.0f};

static int GetZoneIndex(float temp)
{
    if (temp <= -100.0f) return 0;
    if (temp <= -50.0f) return 1;
    if (temp <= 0.0f) return 2;
    if (temp <= 50.0f) return 3;
    if (temp <= 100.0f) return 4;
    if (temp <= 150.0f) return 5;
    return 6;
}

static void UpdateAdaptiveParams(HybridController_t* hc, float current_temp)
{
    if (!hc->adaptive_enabled) return;
    
    int zone = GetZoneIndex(current_temp);
    if (zone == hc->current_zone) return;
    
    hc->current_zone = zone;
    ZoneParam_t* zp = &hc->zones[zone];
    
    // 更新速率PID参数
    hc->rate_pid.Kp = hc->base_kp_rate * zp->kp_factor;
    hc->rate_pid.Ki = hc->base_ki_rate * zp->ki_factor;
    hc->rate_pid.Kd = hc->base_kd_rate * zp->kd_factor;
    
    // 更新温度PID参数
    hc->temp_pid.Kp = hc->base_kp_temp * zp->kp_factor;
    hc->temp_pid.Ki = hc->base_ki_temp * zp->ki_factor;
    hc->temp_pid.Kd = hc->base_kd_temp * zp->kd_factor;
    
    // 更新目标速率
    hc->target_rate = hc->base_target_rate * zp->rate_factor;
    PID_SetSetpoint(&hc->rate_pid, hc->target_rate);
    
    // 更新阈值
    hc->full_power_threshold = hc->base_full_power_threshold * zp->full_power_factor;
    hc->switch_threshold = hc->base_switch_threshold * zp->switch_factor;
}

void Hybrid_ResetStateMachine(HybridController_t* hc)
{
    hc->last_mode = hc->mode;
    
    float temp_error = hc->target_temp - hc->filtered_temp;
    float abs_error = fabsf(temp_error);
    
    if (abs_error <= hc->switch_threshold) {
        hc->mode = CTRL_MODE_TEMP;
        PID_ResetIntegral(&hc->temp_pid, hc->last_output);
        hc->temp_pid.prev_pv = hc->filtered_temp;
    } else if (abs_error <= hc->full_power_threshold) {
        hc->mode = CTRL_MODE_RATE;
        hc->first_rate_entry = true;
        hc->rate_mode_entry_count = 0;
        PID_Reset(&hc->rate_pid);
    } else {
        hc->mode = CTRL_MODE_FULL;
    }
    
    hc->peak_temp = hc->filtered_temp;
    hc->overshoot_detected = false;
    hc->switch_ready = false;
}

void Hybrid_Init(HybridController_t* hc, HybridConfig_t* cfg)
{
    // 初始化卡尔曼滤波器
    Kalman_Init(&hc->kalman_filter, &cfg->kalman_cfg);
    
    // 初始化斜率计算器
    Slope_Init(&hc->slope_calc, &cfg->slope_cfg);
    
    // 设置归一化范围
    hc->temp_error_range = cfg->temp_error_range;
    hc->rate_error_range = cfg->rate_error_range;
    
    // 配置速率PID（启用归一化）
    PIDConfig_t rate_cfg = cfg->rate_pid_cfg;
    rate_cfg.enable_normalization = true;
    rate_cfg.error_range = cfg->rate_error_range;      // 速率误差范围
    rate_cfg.derivative_range = cfg->rate_error_range; // 微分使用相同范围
    PID_Init(&hc->rate_pid, &rate_cfg);
    
    // 配置温度PID（启用归一化）
    PIDConfig_t temp_cfg = cfg->temp_pid_cfg;
    temp_cfg.enable_normalization = true;
    temp_cfg.error_range = cfg->temp_error_range;      // 温度误差范围
    temp_cfg.derivative_range = cfg->rate_error_range; // 微分使用速率范围
    PID_Init(&hc->temp_pid, &temp_cfg);
    
    // 目标参数
    hc->target_temp = cfg->target_temp;
    hc->target_rate = cfg->target_rate;
    hc->switch_threshold = cfg->switch_threshold;
    hc->full_power_threshold = cfg->full_power_threshold;
    
    // 基准参数
    hc->base_target_rate = cfg->base_target_rate;
    hc->base_switch_threshold = cfg->base_switch_threshold;
    hc->base_full_power_threshold = cfg->base_full_power_threshold;
    
    // PID基准参数
    hc->base_kp_rate = cfg->base_kp_rate;
    hc->base_ki_rate = cfg->base_ki_rate;
    hc->base_kd_rate = cfg->base_kd_rate;
    hc->base_kp_temp = cfg->base_kp_temp;
    hc->base_ki_temp = cfg->base_ki_temp;
    hc->base_kd_temp = cfg->base_kd_temp;
    
    // 温区自适应
    hc->adaptive_enabled = cfg->adaptive_enabled;
    for (int i = 0; i < 7; i++) {
        hc->zones[i] = cfg->zones[i];
    }
    hc->current_zone = GetZoneIndex(cfg->kalman_cfg.init_x);
    
    // 输出限制
    hc->output_rate_limit = cfg->output_rate_limit;
    hc->last_output = 0.0f;
    
    // 切换条件
    hc->require_rate_zero = cfg->require_rate_zero;
    hc->rate_zero_thresh = cfg->rate_zero_thresh;
    hc->early_switch_factor = cfg->early_switch_factor;
    
    // 状态变量
    hc->filtered_temp = cfg->kalman_cfg.init_x;
    hc->current_rate = 0.0f;
    hc->temp_error = 0.0f;
    hc->switch_ready = false;
    hc->first_rate_entry = true;
    hc->rate_mode_entry_count = 0;
    hc->peak_temp = cfg->kalman_cfg.init_x;
    hc->overshoot_detected = false;
    hc->last_target_temp = cfg->target_temp;
    hc->target_changed = false;
    
    // 低温保护
    hc->low_temp_protection = cfg->low_temp_protection;
    hc->min_heating_power = cfg->min_heating_power;
    
    // 首次应用自适应参数
    UpdateAdaptiveParams(hc, hc->filtered_temp);
    
    // 重置状态机
    Hybrid_ResetStateMachine(hc);
}

/**
 * 更新控制器
 */
float Hybrid_Update(HybridController_t* hc, float pv_raw, float dt)
{
    if (dt <= 0.0f) {
        return hc->last_output;
    }
    
    // 1. 卡尔曼滤波
    hc->filtered_temp = Kalman_Update(&hc->kalman_filter, pv_raw, dt);
    
    // 2. 更新自适应参数
    UpdateAdaptiveParams(hc, hc->filtered_temp);
    
    // 3. 计算升温速率
    hc->current_rate = Slope_Update(&hc->slope_calc, hc->filtered_temp);
    
    // 4. 计算温度误差
    hc->temp_error = hc->target_temp - hc->filtered_temp;
    float abs_error = fabsf(hc->temp_error);
    
    // 5. 过冲预防检测
    if (hc->filtered_temp > hc->peak_temp) {
        hc->peak_temp = hc->filtered_temp;
    }
    
    if (!hc->overshoot_detected && hc->filtered_temp > hc->target_temp) {
        hc->overshoot_detected = true;
        if (hc->mode != CTRL_MODE_TEMP) {
            hc->mode = CTRL_MODE_TEMP;
            PID_ResetIntegral(&hc->temp_pid, hc->last_output);
            hc->temp_pid.prev_pv = hc->filtered_temp;
        }
    }
    
    // 6. 低温保护
    float min_power = 0.0f;
    if (hc->low_temp_protection && hc->filtered_temp < -50.0f) {
        float temp_ratio = (hc->filtered_temp + 150.0f) / 100.0f;
        if (temp_ratio < 0.0f) temp_ratio = 0.0f;
        if (temp_ratio > 1.0f) temp_ratio = 1.0f;
        min_power = hc->min_heating_power * (1.0f - temp_ratio * 0.5f);
        if (min_power > 0.2f) min_power = 0.2f;
    }
    
    // 7. 状态机切换逻辑
    switch (hc->mode) {
        case CTRL_MODE_FULL:
            // 全速加热：当温差小于阈值时切换到速率控制
            if (abs_error <= hc->full_power_threshold) {
                hc->mode = CTRL_MODE_RATE;
                hc->first_rate_entry = true;
                hc->rate_mode_entry_count = 0;
                PID_Reset(&hc->rate_pid);
            }
            break;
            
        case CTRL_MODE_RATE:
            hc->rate_mode_entry_count++;
            
            {
                float dynamic_threshold = hc->switch_threshold;
                if (hc->current_rate > hc->target_rate) {
                    dynamic_threshold = hc->switch_threshold * hc->early_switch_factor;
                }
                
                bool temp_near = (abs_error <= dynamic_threshold);
                bool rate_near_zero = (fabsf(hc->current_rate) <= hc->rate_zero_thresh);
                bool enough_time = (hc->rate_mode_entry_count >= 20);
                
                hc->switch_ready = temp_near && (rate_near_zero || enough_time);
                
                if (hc->switch_ready) {
                    hc->mode = CTRL_MODE_TEMP;
                    float current_output = hc->rate_pid.last_output;
                    PID_ResetIntegral(&hc->temp_pid, current_output);
                    hc->temp_pid.prev_pv = hc->filtered_temp;
                }
            }
            break;
            
        case CTRL_MODE_TEMP:
            // 温度PID模式：检查是否需要重新加热
            // 如果目标温度升高且温差超过阈值，重新进入全速加热模式
            if (hc->target_changed) {
                hc->target_changed = false;
                if (hc->target_temp > hc->filtered_temp + hc->full_power_threshold) {
                    hc->mode = CTRL_MODE_FULL;
                } else if (hc->target_temp > hc->filtered_temp) {
                    hc->mode = CTRL_MODE_RATE;
                    hc->first_rate_entry = true;
                    hc->rate_mode_entry_count = 0;
                    PID_Reset(&hc->rate_pid);
                }
            }
            break;
    }
    
    // 8. 根据模式计算输出
    float raw_output;
    switch (hc->mode) {
        case CTRL_MODE_FULL:
            raw_output = 1.0f;
            break;
            
        case CTRL_MODE_RATE:
            raw_output = PID_Update(&hc->rate_pid, hc->current_rate, dt);
            
            if (hc->first_rate_entry) {
                if (raw_output > hc->last_output + 0.15f) {
                    raw_output = hc->last_output + 0.15f;
                }
                hc->first_rate_entry = false;
            }
            
            if (raw_output > 1.0f) raw_output = 1.0f;
            if (raw_output < min_power) raw_output = min_power;
            break;
            
        case CTRL_MODE_TEMP:
            raw_output = PID_Update(&hc->temp_pid, hc->filtered_temp, dt);
            
            // 过冲预防
            if (hc->filtered_temp > hc->target_temp * 0.85f && hc->target_temp > 0) {
                float progress = (hc->filtered_temp - hc->target_temp * 0.85f) / (hc->target_temp * 0.15f);
                if (progress > 1.0f) progress = 1.0f;
                raw_output *= (1.0f - progress * 0.5f);
            }
            
            if (raw_output > 1.0f) raw_output = 1.0f;
            if (raw_output < 0.0f) raw_output = 0.0f;
            break;
            
        default:
            raw_output = 0.0f;
            break;
    }
    
    // 9. 输出变化率限制
    float output = raw_output;
    if (hc->output_rate_limit > 0.0f && dt > 0.0f) {
        float max_change = hc->output_rate_limit * dt;
        float change = raw_output - hc->last_output;
        
        if (change > max_change) change = max_change;
        if (change < -max_change) change = -max_change;
        
        output = hc->last_output + change;
        
        if (output > 1.0f) output = 1.0f;
        if (output < min_power) output = min_power;
    }
    
    hc->last_output = output;
    return output;
}

/**
 * 设置目标温度（增强版，正确处理状态机切换）
 */
void Hybrid_SetTargetTemp(HybridController_t* hc, float target_temp)
{
    // 限幅
    if (target_temp < -150.0f) target_temp = -150.0f;
    if (target_temp > 200.0f) target_temp = 200.0f;
    
    // 检测目标是否改变
    if (fabsf(target_temp - hc->target_temp) > 0.01f) {
        hc->last_target_temp = hc->target_temp;
        hc->target_temp = target_temp;
        hc->target_changed = true;
        
        // 更新温度PID的目标值
        PID_SetSetpoint(&hc->temp_pid, target_temp);
        
        // 根据新目标温度与当前温度的差值决定状态机切换
        float temp_error = target_temp - hc->filtered_temp;
        float abs_error = fabsf(temp_error);
        
        printf("Target changed: %.1f℃ -> %.1f℃, Error: %.1f℃\n", 
               hc->last_target_temp, target_temp, temp_error);
        
        // 决定新的控制模式
        if (temp_error > 0) {
            // 需要升温
            if (abs_error >= hc->full_power_threshold) {
                // 温差大，全速加热
                hc->mode = CTRL_MODE_FULL;
                printf("Switching to FULL mode (error: %.1f℃)\n", abs_error);
            } else if (abs_error >= hc->switch_threshold) {
                // 温差适中，速率控制
                hc->mode = CTRL_MODE_RATE;
                hc->first_rate_entry = true;
                hc->rate_mode_entry_count = 0;
                PID_Reset(&hc->rate_pid);
                printf("Switching to RATE mode (error: %.1f℃)\n", abs_error);
            } else {
                // 温差小，PID控制
                hc->mode = CTRL_MODE_TEMP;
                // 重置积分项，避免突变
                PID_ResetIntegral(&hc->temp_pid, hc->last_output);
                hc->temp_pid.prev_pv = hc->filtered_temp;
                printf("Switching to TEMP mode (error: %.1f℃)\n", abs_error);
            }
        } else if (temp_error < 0) {
            // 需要降温（仅加热系统，只能自然冷却）
            // 关闭加热，让系统自然降温
            hc->mode = CTRL_MODE_TEMP;
            PID_ResetIntegral(&hc->temp_pid, 0.0f);
            printf("Cooling down, target: %.1f℃, current: %.1f℃\n", target_temp, hc->filtered_temp);
        }
        
        // 重置过冲检测
        hc->peak_temp = hc->filtered_temp;
        hc->overshoot_detected = false;
        hc->switch_ready = false;
    }
}

void Hybrid_SetTargetRate(HybridController_t* hc, float target_rate)
{
    hc->target_rate = target_rate;
    PID_SetSetpoint(&hc->rate_pid, target_rate);
}

ControlMode_t Hybrid_GetMode(HybridController_t* hc)
{
    return hc->mode;
}

const char* Hybrid_GetModeName(ControlMode_t mode)
{
    switch (mode) {
        case CTRL_MODE_FULL: return "FULL";
        case CTRL_MODE_RATE: return "RATE";
        case CTRL_MODE_TEMP: return "TEMP";
        default: return "UNKNOWN";
    }
}

float Hybrid_GetCurrentRate(HybridController_t* hc)
{
    return hc->current_rate;
}

float Hybrid_GetFilteredTemp(HybridController_t* hc)
{
    return hc->filtered_temp;
}

float Hybrid_GetTempError(HybridController_t* hc)
{
    return hc->temp_error;
}

void Hybrid_SetAdaptiveEnabled(HybridController_t* hc, bool enabled)
{
    hc->adaptive_enabled = enabled;
}

int Hybrid_GetCurrentZone(HybridController_t* hc)
{
    return hc->current_zone;
}





HybridConfig_t cfg[3] =
{
	 // 目标参数
       { .target_temp = 200.0f,
        .target_rate = 1.5f,
        .switch_threshold = 8.0f,
        .full_power_threshold = 30.0f,
        
        // 归一化范围（关键！）
        .temp_error_range = 100.0f,      // 温度误差归一化到[-1,1]对应±100℃
        .rate_error_range = 5.0f,        // 速率误差归一化到[-1,1]对应±5℃/s
        
        // 温区自适应
        .adaptive_enabled = true,
        
        // PID基准参数（无量纲，基于归一化误差）
        // 这些参数与温度/速率的具体数值无关，只与归一化后的范围有关
        .base_kp_rate = 0.8f,            // 速率控制比例增益
        .base_ki_rate = 0.2f,            // 速率控制积分增益
        .base_kd_rate = 0.05f,           // 速率控制微分增益
        .base_kp_temp = 0.5f,            // 温度控制比例增益
        .base_ki_temp = 0.03f,           // 温度控制积分增益
        .base_kd_temp = 0.6f,            // 温度控制微分增益
        
        // 基准目标参数
        .base_target_rate = 1.5f,
        .base_switch_threshold = 8.0f,
        .base_full_power_threshold = 30.0f,
        
        // 卡尔曼滤波器配置
        .kalman_cfg = {
            .Q = 0.5f,
            .R = 1.0f,
            .init_x = -150.0f,
            .init_P = 20.0f
        },
        
        // 速率PID配置
        .rate_pid_cfg = {
            .Kp = 0.8f,
            .Ki = 0.2f,
            .Kd = 0.05f,
            .setpoint = 1.5f,
            .out_min = 0.0f,
            .out_max = 1.0f,
            .integral_sep_thresh = 100.0f,
            .integral_limit_ratio = 0.7f,
            .derivative_on_pv = 0,
            .anti_windup = true
        },
        
        // 温度PID配置
        .temp_pid_cfg = {
            .Kp = 0.5f,
            .Ki = 0.03f,
            .Kd = 0.6f,
            .setpoint = 200.0f,
            .out_min = 0.0f,
            .out_max = 1.0f,
            .integral_sep_thresh = 8.0f,
            .integral_limit_ratio = 0.5f,
            .derivative_on_pv = 1,
            .anti_windup = true
        },
        
        // 斜率计算配置
        .slope_cfg = {
            .dt = 0.1f,
            .deadband = 0.1f,
            .enable_slope_filter = true,
            .slope_kalman_cfg = {
                .Q = 0.3f,
                .R = 0.5f,
                .init_x = 0.0f,
                .init_P = 2.0f
            }
        },
        
        // 输出限制
        .output_rate_limit = 0.1f,
        
        // 切换条件
        .require_rate_zero = false,
        .rate_zero_thresh = 0.2f,
        .early_switch_factor = 0.7f,
        
        // 低温保护
        .low_temp_protection = true,
        .min_heating_power = 0.15f
	 	},
	// 目标参数
		   { .target_temp = 200.0f,
			.target_rate = 1.5f,
			.switch_threshold = 8.0f,
			.full_power_threshold = 30.0f,
			
			// 归一化范围（关键！）
			.temp_error_range = 100.0f, 	 // 温度误差归一化到[-1,1]对应±100℃
			.rate_error_range = 5.0f,		 // 速率误差归一化到[-1,1]对应±5℃/s
			
			// 温区自适应
			.adaptive_enabled = true,
			
			// PID基准参数（无量纲，基于归一化误差）
			// 这些参数与温度/速率的具体数值无关，只与归一化后的范围有关
			.base_kp_rate = 0.8f,			 // 速率控制比例增益
			.base_ki_rate = 0.2f,			 // 速率控制积分增益
			.base_kd_rate = 0.05f,			 // 速率控制微分增益
			.base_kp_temp = 0.5f,			 // 温度控制比例增益
			.base_ki_temp = 0.03f,			 // 温度控制积分增益
			.base_kd_temp = 0.6f,			 // 温度控制微分增益
			
			// 基准目标参数
			.base_target_rate = 1.5f,
			.base_switch_threshold = 8.0f,
			.base_full_power_threshold = 30.0f,
			
			// 卡尔曼滤波器配置
			.kalman_cfg = {
				.Q = 0.5f,
				.R = 1.0f,
				.init_x = -150.0f,
				.init_P = 20.0f
			},
			
			// 速率PID配置
			.rate_pid_cfg = {
				.Kp = 0.8f,
				.Ki = 0.2f,
				.Kd = 0.05f,
				.setpoint = 1.5f,
				.out_min = 0.0f,
				.out_max = 1.0f,
				.integral_sep_thresh = 100.0f,
				.integral_limit_ratio = 0.7f,
				.derivative_on_pv = 0,
				.anti_windup = true
			},
			
			// 温度PID配置
			.temp_pid_cfg = {
				.Kp = 0.5f,
				.Ki = 0.03f,
				.Kd = 0.6f,
				.setpoint = 200.0f,
				.out_min = 0.0f,
				.out_max = 1.0f,
				.integral_sep_thresh = 8.0f,
				.integral_limit_ratio = 0.5f,
				.derivative_on_pv = 1,
				.anti_windup = true
			},
			
			// 斜率计算配置
			.slope_cfg = {
				.dt = 0.1f,
				.deadband = 0.1f,
				.enable_slope_filter = true,
				.slope_kalman_cfg = {
					.Q = 0.3f,
					.R = 0.5f,
					.init_x = 0.0f,
					.init_P = 2.0f
				}
			},
			
			// 输出限制
			.output_rate_limit = 0.1f,
			
			// 切换条件
			.require_rate_zero = false,
			.rate_zero_thresh = 0.2f,
			.early_switch_factor = 0.7f,
			
			// 低温保护
			.low_temp_protection = true,
			.min_heating_power = 0.15f
			},
	// 目标参数
		   { .target_temp = 200.0f,
			.target_rate = 1.5f,
			.switch_threshold = 8.0f,
			.full_power_threshold = 30.0f,
			
			// 归一化范围（关键！）
			.temp_error_range = 100.0f, 	 // 温度误差归一化到[-1,1]对应±100℃
			.rate_error_range = 5.0f,		 // 速率误差归一化到[-1,1]对应±5℃/s
			
			// 温区自适应
			.adaptive_enabled = true,
			
			// PID基准参数（无量纲，基于归一化误差）
			// 这些参数与温度/速率的具体数值无关，只与归一化后的范围有关
			.base_kp_rate = 0.8f,			 // 速率控制比例增益
			.base_ki_rate = 0.2f,			 // 速率控制积分增益
			.base_kd_rate = 0.05f,			 // 速率控制微分增益
			.base_kp_temp = 0.5f,			 // 温度控制比例增益
			.base_ki_temp = 0.03f,			 // 温度控制积分增益
			.base_kd_temp = 0.6f,			 // 温度控制微分增益
			
			// 基准目标参数
			.base_target_rate = 1.5f,
			.base_switch_threshold = 8.0f,
			.base_full_power_threshold = 30.0f,
			
			// 卡尔曼滤波器配置
			.kalman_cfg = {
				.Q = 0.5f,
				.R = 1.0f,
				.init_x = -150.0f,
				.init_P = 20.0f
			},
			
			// 速率PID配置
			.rate_pid_cfg = {
				.Kp = 0.8f,
				.Ki = 0.2f,
				.Kd = 0.05f,
				.setpoint = 1.5f,
				.out_min = 0.0f,
				.out_max = 1.0f,
				.integral_sep_thresh = 100.0f,
				.integral_limit_ratio = 0.7f,
				.derivative_on_pv = 0,
				.anti_windup = true
			},
			
			// 温度PID配置
			.temp_pid_cfg = {
				.Kp = 0.5f,
				.Ki = 0.03f,
				.Kd = 0.6f,
				.setpoint = 200.0f,
				.out_min = 0.0f,
				.out_max = 1.0f,
				.integral_sep_thresh = 8.0f,
				.integral_limit_ratio = 0.5f,
				.derivative_on_pv = 1,
				.anti_windup = true
			},
			
			// 斜率计算配置
			.slope_cfg = {
				.dt = 0.1f,
				.deadband = 0.1f,
				.enable_slope_filter = true,
				.slope_kalman_cfg = {
					.Q = 0.3f,
					.R = 0.5f,
					.init_x = 0.0f,
					.init_P = 2.0f
				}
			},
			
			// 输出限制
			.output_rate_limit = 0.1f,
			
			// 切换条件
			.require_rate_zero = false,
			.rate_zero_thresh = 0.2f,
			.early_switch_factor = 0.7f,
			
			// 低温保护
			.low_temp_protection = true,
			.min_heating_power = 0.15f
			}

};
HybridController_t hc[3];
void SetHeaterPower(unsigned char channel, uint32_t power)
{ 
	if(channel == 0)
	{
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,power);
	}
	else if(channel == 1)
	{
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,power);

	}
	else if(channel == 2)
	{
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,power);

	}	
}

void controllerInit(void)
{
	 // 温区自适应参数
	 ZoneParam_t zones[7] = {
		 // Zone1: -150℃ ~ -100℃
		 {.temp_low = -150.0f, .temp_high = -100.0f,
		  .kp_factor = 0.8f, .ki_factor = 0.7f, .kd_factor = 0.6f,
		  .rate_factor = 0.5f, .full_power_factor = 1.5f, .switch_factor = 1.2f},
		 
		 // Zone2: -100℃ ~ -50℃
		 {.temp_low = -100.0f, .temp_high = -50.0f,
		  .kp_factor = 0.9f, .ki_factor = 0.8f, .kd_factor = 0.7f,
		  .rate_factor = 0.7f, .full_power_factor = 1.3f, .switch_factor = 1.1f},
		 
		 // Zone3: -50℃ ~ 0℃
		 {.temp_low = -50.0f, .temp_high = 0.0f,
		  .kp_factor = 1.0f, .ki_factor = 0.9f, .kd_factor = 0.9f,
		  .rate_factor = 0.9f, .full_power_factor = 1.1f, .switch_factor = 1.0f},
		 
		 // Zone4: 0℃ ~ 50℃ (基准温区)
		 {.temp_low = 0.0f, .temp_high = 50.0f,
		  .kp_factor = 1.0f, .ki_factor = 1.0f, .kd_factor = 1.0f,
		  .rate_factor = 1.0f, .full_power_factor = 1.0f, .switch_factor = 1.0f},
		 
		 // Zone5: 50℃ ~ 100℃
		 {.temp_low = 50.0f, .temp_high = 100.0f,
		  .kp_factor = 1.1f, .ki_factor = 1.2f, .kd_factor = 1.2f,
		  .rate_factor = 1.2f, .full_power_factor = 0.9f, .switch_factor = 0.9f},
		 
		 // Zone6: 100℃ ~ 150℃
		 {.temp_low = 100.0f, .temp_high = 150.0f,
		  .kp_factor = 1.2f, .ki_factor = 1.3f, .kd_factor = 1.3f,
		  .rate_factor = 1.3f, .full_power_factor = 0.8f, .switch_factor = 0.8f},
		 
		 // Zone7: 150℃ ~ 200℃
		 {.temp_low = 150.0f, .temp_high = 200.0f,
		  .kp_factor = 1.3f, .ki_factor = 1.4f, .kd_factor = 1.4f,
		  .rate_factor = 1.4f, .full_power_factor = 0.7f, .switch_factor = 0.7f}
	 };


	
	// 复制温区参数
	    for(unsigned char j = 0;j<3;j++)
		for (int i = 0; i < 7; i++) {
			cfg[j].zones[i] = zones[i];
		}

    Hybrid_Init(&hc[0], &cfg[0]);
    Hybrid_Init(&hc[1], &cfg[1]);
    Hybrid_Init(&hc[2], &cfg[2]);
    


    for (unsigned char i = 0; i < 3; i++)
    {
        temperatureU[i].periodMeter = PERIOD_SAMPLE;
        temperatureU[i].temperatureTarget = -200;
        temperatureU[i].pwmFraction = 20000;
    }

}

// ==================== 主程序 ====================
void controller(unsigned channel)
{

    // 3. 控制循环
    float temp_raw, power;
    float dt = temperatureU[0].periodMeter;  // 100ms控制周期

    // 更新控制器
    power = Hybrid_Update(&hc[channel], temperatureU[channel].temperatureOri, dt);

    // 输出功率
    if(temperatureU[channel].temperatureOri > 200)
    SetHeaterPower(channel, 0);
	else
	SetHeaterPower(channel, power);	


}


