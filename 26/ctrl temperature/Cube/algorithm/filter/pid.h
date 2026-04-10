#ifndef PID1_H_
#define PID1_H_
#include "main.h"
#define MAX_RATE  80

typedef struct {
    float Kp_rate;      // 速率比例系数
    float Ki_rate;      // 速率积分系数
    float Kd_rate;      // 速率微分系数
    float integral;
    float prev_error;
    float out_min, out_max;
    float integral_limit;
	float integral_limitL;
	float out;
} RatePID_t;
typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float integral;
    float prev_error;
    float out_min;   // 输出下限（对应最小加热脉宽，比如0ms）
    float out_max;   // 输出上限（对应最大允许加热脉宽，即50ms）
    float integral_limit; // 积分限幅值
	  float integral_limitL; // 积分限幅值
	  float out;
	  float pterm;
			
} PID_TypeDef;
// 控制状态机
typedef enum {
    RATE_STATE_FULL,        // 全速加热
    RATE_STATE_RATE_CTRL,   // 速率控制
    RATE_STATE_HOLD         // 稳态保持
} RateControlState;
void RateControl_Loop_10ms(unsigned char channel);
void RateControl_Init(void);
#endif

