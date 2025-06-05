/*
 * @Description: �������˲����m * @Author: TOTHTOT
 * @Date: 2023-03-28 10:14:16
 * @LastEditTime: 2023-03-28 10:33:27
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Work\MCU\stm32\read_cs1237_STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\KALMAN\kalman.h
 */
#ifndef __KALMAN_H__
#define __KALMAN_H__

#include "main.h"

typedef struct
{
    float Last_P; // �ϴι���Э����������Ϊ0 ! ! ! ! !
    float Now_P;  // ��ǰ����Э����    
    float out;    // �������˲������
    float Kg;     // ���������a    
    float Q;      // ��������Э����
    float R;      // �۲�����Э����
    } kalman;



/* �ⲿ���ú��� */
void kalman_init(kalman *kfp);
float kalman_filter(kalman *kfp, float input);
kalman *kalman_kfp_get(void);

#endif /* __KALMAN_H__ */
