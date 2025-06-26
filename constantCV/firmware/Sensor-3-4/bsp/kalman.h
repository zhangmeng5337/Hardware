/*
 * @Description: 鍗″皵鏇兼护娉唬鐮? * @Author: TOTHTOT
 * @Date: 2023-03-28 10:14:16
 * @LastEditTime: 2023-03-28 10:33:27
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Work\MCU\stm32\read_cs1237_STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\KALMAN\kalman.h
 */
#ifndef __KALMAN_H__
#define __KALMAN_H__

#include "main.h"
//#define	ADC_CAPTURE			11		//采集数量
#define	FILTER_CAPTURE			10		//采集数量
#define WINDOW_SIZE        10
#define SLID_SIZE        3

typedef struct
{
    float Last_P; // 涓婃浼扮畻鍗忔柟宸?涓嶅彲浠ヤ负0 ! ! ! ! !
    float Now_P;  // 褰撳墠浼扮畻鍗忔柟宸?    
    float out;    // 鍗″皵鏇兼护娉㈠櫒杈撳嚭
    float Kg;     // 鍗″皵鏇煎鐩?    
    float Q;      // 杩囩▼鍣０鍗忔柟宸?
    float R;      // 瑙傛祴鍣０鍗忔柟宸?
    float medi;
    } kalman;







/* 澶栭儴璋冪敤鍑芥暟 */
void kalman_init(kalman *kfp);
float kalman_filter(kalman *kfp, float input);
kalman *kalman_kfp_get(void);
float SilderFilter(float _value);
float  GetMedianNum(float bArray);
float *swap_buff(float dat);
float medium_aver(float dat);

#endif /* __KALMAN_H__ */
