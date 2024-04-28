/*
*********************************************************************************************************
*
*	ģ������ : ����STM32F4�ڲ�TIM���PWM�źţ�˳��ʵ��
*	�ļ����� : bsp_tim_pwm.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2012-2013, �������ҽ����е�з����޹�˾ ShenYang HengDe Medical Instruments Co.��Ltd.
*
*********************************************************************************************************
*/

#ifndef __BSP_TIM_PWM_H
#define __BSP_TIM_PWM_H

#include "main.h"

void bsp_SetTIMOutPWM( uint8_t _ucChannel,uint32_t _ulFreq, uint32_t _ulDutyCycle);
void bsp_SetTIMOutPWM_N( uint8_t _ucChannel,uint32_t _ulFreq, uint32_t _ulDutyCycle);
//void bsp_SetTIMforInt(TIM_TypeDef* TIMx, uint32_t _ulFreq, uint8_t _PreemptionPriority, uint8_t _SubPriority);

#endif

/***************************** �������ҽ����е�з����޹�˾ ShenYang HengDe Medical Instruments Co.��Ltd. (END OF FILE) *********************************/
