/*
*********************************************************************************************************
*
*	ģ������ : ������ģ��
*	�ļ����� : bsp_beep.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2012-2013, �������ҽ����е�з����޹�˾ ShenYang HengDe Medical Instruments Co.��Ltd.
*
*********************************************************************************************************
*/

#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H

#include "main.h"
#define MUSIC_NUM1  3												// ��СֵΪ2
#define MUSIC_NUM2  3												// ��СֵΪ2
#define MUSIC_NUM3  5												// ��СֵΪ2
#define MUSIC_NUM4  7												// ��СֵΪ2
#define MUSIC_NUM5  5												// ��СֵΪ2
#define MUSIC_NUM6  5												// ��СֵΪ2

typedef struct _BEEP_T
{
	uint8_t ucEnalbe;
	uint8_t ucState;
	uint16_t usBeepTime;
	uint16_t usStopTime;
	uint16_t usCycle;
	uint16_t usCount;
	uint16_t usCycleCount;
	uint32_t uiFreq;
}BEEP_T;

/* ���ⲿ���õĺ������� */
void BEEP_InitHard(void);
void BEEP_Start(uint32_t _uiFreq, uint16_t _usBeepTime, uint16_t _usStopTime, uint16_t _usCycle);
void BEEP_Stop(void);
void BEEP_KeyTone(void);
void BEEP_Pro(void);
void Speak_value(void);
void beep_pro(unsigned char sound_num,unsigned char mode);

#endif

/***************************** �������ҽ����е�з����޹�˾ ShenYang HengDe Medical Instruments Co.��Ltd. (END OF FILE) *********************************/
