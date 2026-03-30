#include "filter.h"
#include "kalman.h"
// temp_sensor.c

#include "FreeRTOS.h"  // 用于获取时间
#include <math.h>
// 自适应卡尔曼滤波器实例
 temperatureStru temperatureU[3];

 AdaptiveKalman_t akf[3];
 uint32_t last_time_ms = 0;
 float temperature_rate = 0.0f;
// 初始化卡尔曼滤波器
void Kalman_Init(KalmanFilter_t *kf, float initial_temp, float q, float r)
{
    kf->x = initial_temp;      // 初始温度估计
    kf->p = 1.0f;              // 初始误差（不确定度较大）
    kf->q = q;                 // 过程噪声
    kf->r = r;                 // 测量噪声
    kf->k = 0.0f;
    kf->last_dt = 0.01f;       // 默认10ms
}

// 初始化自适应卡尔曼滤波器
void AdaptiveKalman_Init(AdaptiveKalman_t *akf, float initial_temp)
{
    // 初始参数设置
    akf->q_min = 0.001f;     // 稳态过程噪声（很小）
    akf->q_max = 0.1f;       // 动态过程噪声（较大）
    akf->r_min = 0.05f;      // 稳态测量噪声（较小）
    akf->r_max = 0.5f;       // 动态测量噪声（较大）
    akf->rate_threshold = 10.0f;  // 变化率阈值（℃/s）
    
    // 初始化基础卡尔曼滤波器
    Kalman_Init(&akf->kf, initial_temp, akf->q_min, akf->r_min);
    
    // 初始化状态检测变量
    akf->last_temp = initial_temp;
    akf->last_time = 0;
    akf->initialized = 0;
}

// 计算温度变化率（用于自适应判断）
static float Calculate_Rate(AdaptiveKalman_t *akf, float current_temp, float dt)
{
    if (!akf->initialized || dt < 0.001f) {
        akf->last_temp = current_temp;
        akf->initialized = 1;
        return 0.0f;
    }
    
    float rate = (current_temp - akf->last_temp) / dt;
    akf->last_temp = current_temp;
    
    // 限幅，防止异常
    if (rate > 100.0f) rate = 100.0f;
    if (rate < -100.0f) rate = -100.0f;
    
    return rate;
}



// 获取温度变化率（使用卡尔曼滤波后的温度计算，更平滑）
float Get_Temp_Rate_Kalman(AdaptiveKalman_t *akf, float current_temp, float dt)
{
    static float last_filtered_temp = 0.0f;
    static uint8_t init = 0;
    
    float filtered_temp = akf->kf.x;
    
    if (!init) {
        last_filtered_temp = filtered_temp;
        init = 1;
        return 0.0f;
    }
    
    if (dt < 0.001f) return 0.0f;
    
    float rate = (filtered_temp - last_filtered_temp) / dt;
    last_filtered_temp = filtered_temp;
    
    // 一阶滤波平滑速率
    static float filtered_rate = 0.0f;
    filtered_rate = 0.3f * rate + 0.7f * filtered_rate;
    
    return filtered_rate;
}



// 读取原始温度（需根据实际硬件实现）
static float Read_Raw_Temperature(void)
{
    // 示例：从ADS1220读取温度
    // 实际实现需替换为您的ADC读取代码
    
    // 模拟：返回一个带噪声的温度值（实际使用时删除）
    static float sim_temp = 25.0f;
    static int counter = 0;
    counter++;
    // 添加模拟噪声 ±0.3℃
    float noise = (float)(counter % 100 - 50) / 100.0f * 0.6f;
    sim_temp += 0.01f;  // 缓慢上升
    return sim_temp + noise;
    
    /* 实际代码示例：
    uint32_t adc_value = ADS1220_Read();
    float temp = (float)adc_value * 0.001f;  // 根据传感器转换
    return temp;
    */
}

// 获取系统时间（毫秒）
 uint32_t Get_System_Time_Ms(void)
{
        return xTaskGetTickCount() * portTICK_PERIOD_MS;

}

// 初始化温度传感器
void TempSensor_Init(void)
{
//    // 读取初始温度（多次平均）
//    float init_temp = 0.0f;
//    for (int i = 0; i < 10; i++) {
//        init_temp += Read_Raw_Temperature();
//    }
//    init_temp /= 10.0f;
    
    // 初始化自适应卡尔曼滤波器
    AdaptiveKalman_Init(&akf[0], current_tempBuf[0]);
    AdaptiveKalman_Init(&akf[1], current_tempBuf[1]);
	AdaptiveKalman_Init(&akf[2], current_tempBuf[2]);
    last_time_ms = Get_System_Time_Ms();
}

// 获取滤波后的温度（主函数调用）
float Get_Filtered_Temperature(float dt,unsigned char channel)
{

    // 自适应卡尔曼滤波
     temperatureU[channel].temperatureFlt = AdaptiveKalman_Update(&akf[channel], temperatureU[channel].temperatureOri, dt);

}

// 获取温度变化率（使用卡尔曼滤波结果）
float Get_Temperature_Rate(unsigned char channel)
{
    static float last_temp = 0.0f;
    static uint32_t last_time = 0;
    static uint8_t init = 0;
    
    uint32_t now = Get_System_Time_Ms();
    float current_temp = akf[channel].kf.x;  // 使用滤波后的温度
    
    if (!init) {
        last_temp = current_temp;
        last_time = now;
        init = 1;
        return 0.0f;
    }
    
    float dt = (now - last_time) / 1000.0f;
    if (dt < 0.001f) return 0.0f;
    
    float rate = (current_temp - last_temp) / dt;
    last_temp = current_temp;
    last_time = now;
    
    // 一阶滤波平滑速率
    static float filtered_rate = 0.0f;
    filtered_rate = 0.2f * rate + 0.8f * filtered_rate;
    
    return filtered_rate;
}

// 重置滤波器（设定值变化时调用）
void TempSensor_Reset(float *initial_temp)
{
    AdaptiveKalman_Init(&akf[0], initial_temp[0]);
    AdaptiveKalman_Init(&akf[0], initial_temp[1]);
    AdaptiveKalman_Init(&akf[0], initial_temp[2]);	
    last_time_ms = Get_System_Time_Ms();
}





