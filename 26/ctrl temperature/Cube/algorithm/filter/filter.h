#ifndef FILTER_H_
#define FILTER_H_
#include "main.h"
#include "kalman.h"

float Get_Temp_Rate_Kalman(AdaptiveKalman_t *akf, float current_temp, float dt);
float Get_Filtered_Temperature(unsigned char channel);
 uint32_t Get_System_Time_Ms(void);
 void TempSensor_Init(void);
#endif

