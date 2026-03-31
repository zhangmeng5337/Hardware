/******************************************************************************
 * 文件: temp_control_heater_only.c
 * 描述: 单加热器温控系统（无主动制冷），适应 -150℃~200℃ 温区
 *       实现大温差全速加热 + 分段自适应PID + 卡尔曼滤波 + 前馈
 * 注意: 降温只能依靠被动散热，稳定时间取决于散热条件，可能不满足30s要求
 * 版本: 3.0 (单加热器版)
 ******************************************************************************/

#include <stdint.h>
#include <math.h>
#include "temperature_controller.h"
#include "main.h"
/* ======================= 硬件抽象层（需用户实现） ========================= */
extern TIM_HandleTypeDef htim4;
 temperatureStru temperatureU[3];
 KalmanFilter_t kf;
KalmanConfig_t cfg;
 KalmanFilter_t slopekf;
/* ============================= 宏定义 ===================================== */
#define CONTROL_PERIOD_MS   20      // 控制周期20ms (50Hz)
#define SAMPLE_PERIOD_S     0.02f   // 采样周期(s)

// 大温差阈值（℃），误差绝对值大于此值则全速加热
#define LARGE_ERROR_THRESH  20.0f


TempControl_t g_tc[3];
/* ========================= PID控制器实现 =================================== */

// 初始化PID (仅加热，输出范围0~100)
void PID_Init(PID_t *pid, float Kp, float Ki, float Kd, float out_min, float out_max) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->integral = 0.0f;
    pid->prev_err = 0.0f;
    pid->out_min = out_min;
    pid->out_max = out_max;
}

// PID计算 (增量式，仅用于误差>0时)
float PID_Compute(PID_t *pid, float error, float Ts) {
    float P = pid->Kp * error;
	pid->pterm = P;
    // 积分累加，并加入积分限幅
    pid->integral += pid->Ki * error ;
    if (pid->integral > pid->out_max) pid->integral = pid->out_max;
    if (pid->integral < pid->out_min) pid->integral = pid->out_min;
    
    float D = pid->Kd * (error - pid->prev_err) ;
    float output = P + pid->integral + D;
    pid->dterm = D;
    pid->prev_err = error;
    
    // 输出限幅
    if (output > pid->out_max) output = pid->out_max;
    if (output < pid->out_min) output = pid->out_min;
	pid->output = output;
    return output;
}

/* ========================= 温控系统核心 ==================================== */

// 根据目标温度更新温区及PID参数 (仅加热)
static void UpdateZoneParameters(TempControl_t *tc) {
    float sp = tc->setpoint;
    if (sp < -50.0f) {
        tc->zone = 0;   // 低温区
        PID_Init(&tc->pid_heat, 2.5f, 0.1f, 0.8f, 5.0f, 60.0f);
        tc->heat_ff_gain = 0.5f;
    } else if (sp < 50.0f) {
        tc->zone = 1;   // 中温区
        PID_Init(&tc->pid_heat, 2.0f, 0.15f, 0.5f, 5.0f, 80.0f);
        tc->heat_ff_gain = 1.2f;
    } else {
        tc->zone = 2;   // 高温区
        PID_Init(&tc->pid_heat, 1.5f, 0.2f, 0.3f, 5.0f, 100.0f);
        tc->heat_ff_gain = 1.8f;
    }
}

// 初始化温控系统
void TempControl_Init(TempControl_t *tc, float init_temp, float init_setpoint) {
    tc->setpoint = init_setpoint;
    tc->output = 0.0f;
    tc->enable = 1;
    tc->large_error_mode = 0;
    
    // 初始化卡尔曼滤波器
    //Kalman_Init(&tc->kalman, init_temp,0.1 ,0.1,0.1,0.05);
   cfg.Q = 0.05;
    cfg.R = 5;
    cfg.init_x = 20;
    cfg.init_P = 0;
      Kalman_Init(&tc->kalman, &cfg);    
			Kalman_Init(&slopekf, &cfg); 	
    // 根据目标温度设置初始参数
    UpdateZoneParameters(tc);
}

// 设置目标温度
void TempControl_SetSetpoint(TempControl_t *tc, float setpoint) {
    tc->setpoint = setpoint;
    UpdateZoneParameters(tc);
    tc->large_error_mode = 0;   // 重置大温差模式
}

// 获取当前估计温度（滤波后）
extern unsigned char divT;
float slope[3],slopekft[3],last[3];
float pwmTotal = 580;
float errThr = 0;
// 主控步进函数（应在每个控制周期调用）
void TempControl_Step(TempControl_t *tc,unsigned char channel) {
    if (!tc->enable) return;
     
    // 1. 读取传感器原始值
    //float T_raw = temperatureU[channel].temperatureOri;
	float u_norm=0;
   //  Kalman_Step(&tc->kalman, T_raw, float z, g_tc.pid_heat.periodMeter, 0.5, 2)；
    // 2. 卡尔曼滤波 (系统模型参数需实际辨识)
   // float u_norm = tc->output / 100.0f;   // 归一化输出[0,1]
   // Kalman_Step(&tc->kalman, u_norm, T_raw,g_tc.pid_heat.periodMeter, 0.5f, 2.0f);
	
  // temperatureU[channel].temperatureFlt = Kalman_Update(&tc->kalman, T_raw, g_tc[channel].pid_heat.periodMeter);
    // 3. 误差计算
    float error = tc->setpoint - temperatureU[channel].temperatureFlt;
    float abs_error = fabsf(error);
	

    // 4. 大温差全速加热逻辑
    //    升温: 误差>阈值且目标高于当前 -> 全速加热
    //    降温: 误差< -阈值 -> 只能关闭加热，等待自然降温
    if (abs_error > LARGE_ERROR_THRESH) {
        tc->large_error_mode = 1;
        if (error > 0) {
            // 需要大幅升温，全速加热
            tc->output = 100.0f;
        } else {
            // 需要大幅降温，但无制冷，只能停止加热，依靠被动散热
            tc->output = 0.0f;
        }
    } else {
        // 退出大温差模式
        if (tc->large_error_mode) {
            // 重置PID积分项，避免跳变
            tc->pid_heat.integral = 0.0f;
            tc->large_error_mode = 0;
        }
        
        // ---- 正常PID + 前馈控制 (仅加热方向) ----
        float ff = 0.0f;
        float pid_out = 0.0f;

        if (error > errThr) {
					 {
            // 需要加热
            ff = tc->heat_ff_gain * error;
            if (ff > 100.0f) ff = 100.0f;
            
            pid_out = PID_Compute(&tc->pid_heat, error, g_tc[0].pid_heat.periodMeter*divT);
            // 合并输出，但限制在0~100
            float output = ff + pid_out;
						 tc->fterm = ff;
            if (output > 100.0f) output = 100.0f;
            if (output < 5.0f) output = 5.0f;
            tc->output = output;
					    tc->totaloutput = output;					 
					 }
					// else
						//  tc->output = 0.0f;

        } else {
					
            // 需要降温或保温：关闭加热，自然冷却
            tc->output = 0.0f;
            // 注意：积分项在误差为负时不应累加，所以PID_Compute不会被调用
        }
    }
    
    // 5. 驱动执行器
		if(channel ==0)
    __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,tc->output*pwmTotal);
		else if(channel ==1)
    __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,tc->output*pwmTotal);		
 		else if(channel ==2)
    __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,tc->output*pwmTotal);  
}

/* ========================= 示例主程序 ===================================== */

void kalmanProc(unsigned char channel)
{
temperatureU[channel].temperatureFlt = Kalman_Update(&g_tc[channel].kalman, temperatureU[channel].temperatureOri, g_tc[0].pid_heat.periodMeter);
slope[channel] = (temperatureU[channel].temperatureFlt - last[channel]) /g_tc[0].pid_heat.periodMeter;
	slopekft[channel] = Kalman_Update(&slopekf, slope[channel], g_tc[0].pid_heat.periodMeter);
	last[channel] = temperatureU[channel].temperatureFlt;
}
void controllerInit(void)
{
	temperatureU[0].periodMeter = 0.002;
	temperatureU[1].periodMeter = 0.1;	
	temperatureU[2].periodMeter = 0.1;
	g_tc[0].pid_heat.periodMeter = temperatureU[0].periodMeter;
	g_tc[1].pid_heat.periodMeter = temperatureU[1].periodMeter;
	g_tc[2].pid_heat.periodMeter = temperatureU[2].periodMeter;
	// 温控系统初始化：初始温度假设为25℃，初始目标温度25℃
	TempControl_Init(&g_tc[0], 25.0f, -200.0f);
	TempControl_Init(&g_tc[1], 25.0f, -200.0f);
	TempControl_Init(&g_tc[2], 25.0f, -200.0f);
	// 设置目标温度（例如：从室温加热到200℃）
	TempControl_SetSetpoint(&g_tc[0], -200.0f);
	TempControl_SetSetpoint(&g_tc[1], -200.0f);
	TempControl_SetSetpoint(&g_tc[2], -200.0f);
	
}
void controller(unsigned char channel) {

	//g_tc[channel].setpoint = temperatureU[channel].temperatureTarget;
	if(g_tc[channel].setpoint != temperatureU[channel].temperatureTarget)
	{
		g_tc[channel].setpoint = temperatureU[channel].temperatureTarget;
	  UpdateZoneParameters(&g_tc[channel]);
	}
	
 TempControl_Step(&g_tc[channel],channel);
    

}
