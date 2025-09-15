#ifndef CONFIG_H_
#define CONFIG_H_
#include "main.h"


// 电机物理参数：
#define POLE_PAIRS 5 // 极对数

// 电路参数：
#define R_SHUNT 0.01           // 电流采样电阻，欧姆
#define OP_GAIN 21             // 运放放大倍数
#define MAX_CURRENT 12          // 最大q轴电流，安培A
#define ADC_REFERENCE_VOLT 3.3 // 电流采样adc参考电压，伏
#define ADC_BITS 12            // ADC精度，bit

// 单片机配置参数：
#define motor_pwm_freq 20000      // 驱动桥pwm频率，Hz
#define motor_speed_calc_freq 930 // 电机速度计算频率，Hz
#define motor_pwm_deadTIime  200
// 软件参数：
#define position_cycle 6 * 3.14159265358979 // 电机多圈周期，等于正半周期+负半周期


#endif
