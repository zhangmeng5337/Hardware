#include "pid.h"


// 初始化速率控制器
void RateControl_Init(void)
{
}


// 1. 获取当前温度（已滤波）
current_temp = Get_Filtered_Temperature(channel);

// 2. 计算实际温升速率
actual_rate = Calculate_Rate(current_temp, now_time);


