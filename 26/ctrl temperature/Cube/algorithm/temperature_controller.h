#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include "kalman.h"
// PID控制器 (仅用于加热方向)
typedef struct {
    float Kp;       // 比例系数
    float Ki;       // 积分系数
    float Kd;       // 微分系数
    float integral; // 积分累加和
    float prev_err; // 上一次误差
    float out_min;  // 输出下限 (0%)
    float out_max;  // 输出上限 (100%)
    float periodMeter;
	float output;
	float pterm;
	float dterm;
	float fterm;

} PID_t;

// 温控系统主结构
typedef struct {
   KalmanFilter_t kalman;        // 卡尔曼滤波器
    PID_t    pid_heat;      // 加热PID (输出0~100)
    
    float setpoint;         // 目标温度 (℃)
    float output;           // 最终输出功率百分比(0~100)
    
    float heat_ff_gain;     // 加热前馈增益 (%/℃)
    
    uint8_t zone;           // 当前温区 (0:低温区, 1:中温区, 2:高温区)
    uint8_t enable;         // 系统使能标志
    uint8_t large_error_mode; // 大温差模式标志 (1:全速加热, 0:正常PID)
	 float totaloutput;
	float fterm;
	
} TempControl_t;
typedef struct
{
float temperatureOri;
	float temperatureTarget;
float	temperatureFlt;

	float periodMeter;
	unsigned char periodTask;
	unsigned char div;
}temperatureStru;
void controller(unsigned char channel);
void kalmanProc(unsigned char channel);
void Hardware_SetHeaterOutput(unsigned char channel,float final_out);
#endif

