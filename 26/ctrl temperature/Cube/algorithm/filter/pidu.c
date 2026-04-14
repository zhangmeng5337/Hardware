#include "pid.h"
#include "filter.h"
// 全局变量
static RateControlState rate_state[3] = {RATE_STATE_FULL};
static RatePID_t rate_pid[3];
static float target_rate = 0.0f;
static float actual_rate = 0.0f;
static float last_temp = 0.0f;
static uint32_t last_time_ms = 0;
PID_TypeDef pid_steadyBuf[3] =
{
    {0},
    {0},
    {0}
};
float current_tempBuf[3] = {200};
float target_temp[3] = {-200};
float current_temp;
float current_tempBufFt[3] = {200};


float RatePID_Update(RatePID_t *pid, float target_rate, float actual_rate, float dt)
{
    float error = target_rate - actual_rate;
    float p_out = pid->Kp_rate * error;

    // 积分分离：仅在误差较小时积分
    if (fabs(error) < 20.0f)
    {
        pid->integral += pid->Ki_rate * error * dt;
        if (pid->integral > pid->integral_limit)
            pid->integral = pid->integral_limit;
        if (pid->integral < -pid->integral_limit)
            pid->integral = -pid->integral_limit;
    }

    float d_out = pid->Kd_rate * (error - pid->prev_error) / dt;
    pid->prev_error = error;
    float output = p_out + pid->integral + d_out;
    if (pid->out > pid->out_max)
    {
        output = pid->out_max;
        pid->out	 = pid->out_max;
        if (error > 0)
        {

        }
        else
        {
            pid->integral += pid->Ki_rate * error;
        }
    }
    else if (output < pid->out_min)
    {
        output = pid->out_min;
        pid->out	 = pid->out_min;
        if (error < 0)
            ;
        else
        {
            pid->integral += pid->Ki_rate * error;
        }
    }
    else
    {
        if (fabs(error) < 20.0f)
        {
            pid->integral += pid->Ki_rate * error * dt;
            if (pid->integral > pid->integral_limit)
                pid->integral = pid->integral_limit;
            if (pid->integral < -pid->integral_limit)
                pid->integral = -pid->integral_limit;
        }
        else
            pid->integral  = 0;
    }



    // 积分限幅
    if (pid->integral > pid->integral_limit)
        pid->integral = pid->integral_limit;
    if (pid->integral < pid->integral_limitL)
        pid->integral = pid->integral_limitL;

    pid->out = output;

    return output;
}
float PID_Update_Position(PID_TypeDef *pid, float setpoint, float measurement)
{
    float error = setpoint - measurement;
    pid->pterm = pid->Kp * error;


    float d_out = pid->Kd * (error - pid->prev_error);
    pid->prev_error = error;

    float output = pid->pterm + pid->integral + d_out;
    pid->out = output;


    if (pid->out > pid->out_max)
    {
        output = pid->out_max;
        pid->out	 = pid->out_max;
        if (error > 0)
        {

        }
        else
        {
            pid->integral += pid->Ki * error;
        }
    }
    else if (output < pid->out_min)
    {
        output = pid->out_min;
        pid->out	 = pid->out_min;
        if (error < 0)
            ;
        else
        {
            pid->integral += pid->Ki * error;
        }
    }
    else
    {
        if (fabs(error) <= 2.0f)
            pid->integral += pid->Ki * error;
        else
            pid->integral  = 0;
    }



    // 积分限幅
    if (pid->integral > pid->integral_limit)
        pid->integral = pid->integral_limit;
    if (pid->integral < pid->integral_limitL)
        pid->integral = pid->integral_limitL;



    return output;
}



// 初始化速率控制器
uint32_t pwmMax = 20000;
void RateControl_Init(void)
{
    rate_pid[0].Kp_rate = 0.5f;      // 速率比例系数（需整定）
    rate_pid[0].Ki_rate = 0.1f;      // 速率积分系数
    rate_pid[0].Kd_rate = 0.02f;     // 速率微分系数
    rate_pid[0].integral = 0.0f;
    rate_pid[0].prev_error = 0.0f;
    rate_pid[0].out_min = 0.0f;
    rate_pid[0].out_max = pwmMax;    // 最大脉宽50ms
    rate_pid[0].integral_limit = 30.0f;
		rate_pid[0].integral_limit = 2000.0f;
		rate_pid[0].integral_limitL = -2000;
    rate_state[0] = RATE_STATE_FULL;

    rate_pid[1].Kp_rate = 0.5f;      // 速率比例系数（需整定）
    rate_pid[1].Ki_rate = 0.1f;      // 速率积分系数
    rate_pid[1].Kd_rate = 0.02f;     // 速率微分系数
    rate_pid[1].integral = 0.0f;
    rate_pid[1].prev_error = 0.0f;
    rate_pid[1].out_min = 0.0f;
    rate_pid[1].out_max = pwmMax;     // 最大脉宽50ms
	  rate_pid[1].integral_limit = 2000.0f;
		rate_pid[1].integral_limitL = -2000;
    rate_state[1] = RATE_STATE_FULL;

    rate_pid[2].Kp_rate = 0.5f;      // 速率比例系数（需整定）
    rate_pid[2].Ki_rate = 0.1f;      // 速率积分系数
    rate_pid[2].Kd_rate = 0.02f;     // 速率微分系数
    rate_pid[2].integral = 0.0f;
    rate_pid[2].prev_error = 0.0f;
    rate_pid[2].out_min = 0.0f;
    rate_pid[2].out_max = pwmMax;     // 最大脉宽50ms
    rate_pid[2].integral_limit = 2000.0f;
		rate_pid[2].integral_limitL = -2000;
    rate_state[2] = RATE_STATE_FULL;
    last_temp = current_temp;
    last_time_ms = Get_System_Time_Ms();

    pid_steadyBuf[0].Kp = 1;
    pid_steadyBuf[0].Ki = 0.01;
    pid_steadyBuf[0].Kd = 0;
    pid_steadyBuf[0].integral_limit = 20000;
    pid_steadyBuf[0].integral_limitL = -2000;
    pid_steadyBuf[0].out_max = pwmMax;
    pid_steadyBuf[0].out_min = 0;
    pid_steadyBuf[0].prev_error = 0;

    pid_steadyBuf[1].Kp = 1;
    pid_steadyBuf[1].Ki = 0.01;
    pid_steadyBuf[1].Kd = 0;
    pid_steadyBuf[1].integral_limit = 20000;
    pid_steadyBuf[1].integral_limitL = -2000;
    pid_steadyBuf[1].out_max = pwmMax;
    pid_steadyBuf[1].out_min = 0;
    pid_steadyBuf[1].prev_error = 0;

    pid_steadyBuf[2].Kp = 1;
    pid_steadyBuf[2].Ki = 0.01;
    pid_steadyBuf[2].Kd = 0;
    pid_steadyBuf[2].integral_limit = 20000;
    pid_steadyBuf[2].integral_limitL = -2000;
    pid_steadyBuf[2].out_max = pwmMax;
    pid_steadyBuf[2].out_min = 0;
    pid_steadyBuf[2].prev_error = 0;
}
void Set_Heating_PulseWidth(unsigned char channel, uint16_t pulse_us)
{
    if (channel == 0)
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, pulse_us);
    else  if (channel == 1)
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, pulse_us);
    else if (channel == 2)
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, pulse_us);
    //  timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, pulse_us);
}
// 计算实际温升速率（℃/s）
float Calculate_Rate(float current_temp, uint32_t now_ms)
{
    float dt = (now_ms - last_time_ms) / 1000.0f;
    if (dt < 0.001f)
        return 0.0f;

    float rate = (current_temp - last_temp) / dt;
    last_temp = current_temp;
    last_time_ms = now_ms;

    // 一阶滤波平滑速率
    static float filtered_rate = 0.0f;
    filtered_rate = 0.3f * rate + 0.7f * filtered_rate;
    return filtered_rate;
}

// 获取目标速率（根据当前温度）
float Get_Target_Rate(float current_temp, float target_temp)
{
    float delta = target_temp - current_temp;
    // const float MAX_RATE = 80.0f;      // 最大允许升温速率（℃/s）
    const float RATE_START = 20.0f;    // 开始减速的温差（℃）
    const float RATE_END = 5.0f;       // 结束减速的温差（℃）

    if (delta > RATE_START)
    {
        return MAX_RATE;
    }
    else if (delta > RATE_END)
    {
        // 线性减速
        return MAX_RATE * (delta - RATE_END) / (RATE_START - RATE_END);
    }
    else
    {
        return 0.0f;
    }
}

// 主控制循环（每10ms调用）
void RateControl_Loop_10ms(unsigned char channel)
{
    static uint32_t last_time = 0;
    uint32_t now_time = Get_System_Time_Ms();
    float dt = (now_time - last_time) / 1000.0f;
    if (dt < 0.001f)
        return;
    last_time = now_time;

    // 1. 获取当前温度（已滤波）
    current_temp = Get_Filtered_Temperature(channel);
    current_tempBufFt[channel] = current_temp;
    // 2. 计算实际温升速率
    actual_rate = Calculate_Rate(current_temp, now_time);

    // 3. 状态机切换
    float delta = target_temp[channel] - current_temp;

    switch (rate_state[channel])
    {
        case RATE_STATE_FULL:
            if (delta <= 20.0f)
            {
                rate_state[channel] = RATE_STATE_RATE_CTRL;
                rate_pid[channel].integral = 0.0f;  // 重置积分
            }
            break;

        case RATE_STATE_RATE_CTRL:
            if (delta <= 2.0f && actual_rate < 5.0f)
            {
                rate_state[channel] = RATE_STATE_HOLD;
                // 切换到稳态PID
                pid_steadyBuf[channel].integral = rate_pid[channel].out;
            }
            break;

        case RATE_STATE_HOLD:
            if (delta > 5.0f)
            {
                rate_state[channel] = RATE_STATE_RATE_CTRL;
            }
			
            break;
    }

    // 4. 计算输出脉宽
    float pulse_ms[3] = {0.0f};

    switch (rate_state[channel])
    {
        case RATE_STATE_FULL:
            pulse_ms[channel] = 50.0f;  // 全速加热
            break;

        case RATE_STATE_RATE_CTRL:
            // 获取目标速率
            target_rate = Get_Target_Rate(current_temp, target_temp[channel]);
            // 速率PID控制
            pulse_ms[channel] = RatePID_Update(&rate_pid[channel], target_rate, actual_rate, dt);
            break;

        case RATE_STATE_HOLD:
            // 稳态保持：使用传统PID
            pulse_ms[channel] = PID_Update_Position(&pid_steadyBuf[channel], target_temp[channel], current_tempBuf[channel]);
            break;
    }

    // 5. 输出限幅和死区
    if (pulse_ms[channel] > 50.0f)
        pulse_ms[channel] = 50.0f;
    if (pulse_ms[channel] < 0.5f)
        pulse_ms[channel] = 0.0f;

    Set_Heating_PulseWidth((uint16_t)(pulse_ms[channel]), channel);
}


