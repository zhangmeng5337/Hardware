/*
 * @Description: 卡尔曼滤波代�? * @Author: TOTHTOT
 * @Date: 2023-03-28 10:14:16
 * @LastEditTime: 2023-03-28 10:33:27
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Work\MCU\stm32\read_cs1237_STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\KALMAN\kalman.h
 */
#ifndef __KALMAN_H__
#define __KALMAN_H__

#include "main.h"
#define	ADC_CAPTURE			25		//�ɼ�����

typedef struct
{
    float Last_P; // 上次估算协方�?不可以为0 ! ! ! ! !
    float Now_P;  // 当前估算协方�?    
    float out;    // 卡尔曼滤波器输出
    float Kg;     // 卡尔曼增�?    
    float Q;      // 过程噪声协方�?
    float R;      // 观测噪声协方�?
    } kalman;



/* 外部调用函数 */
void kalman_init(kalman *kfp);
float kalman_filter(kalman *kfp, float input);
kalman *kalman_kfp_get(void);
float SilderFilter(float _value);
float  GetMedianNum(float bArray);

#endif /* __KALMAN_H__ */
