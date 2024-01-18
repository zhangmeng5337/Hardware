﻿#ifndef RTC_H_
#define RTC_H_
#include "main.h"
typedef struct{
 unsigned char RtcWakeUP;
 unsigned int wakeup_period;
 unsigned char calibration;
}RTC_STRU;

void  setRtc(unsigned char* p,unsigned char item);
void SetWakeUp(uint32_t p);
RTC_STRU *getRtcStatus(void);
void MX_RTC_Init2(void);
u8 RTC_Init(void);              		//RTC��ʼ��
void RtcIT_ctrl(unsigned char flag);
void  RTC_WAKEUP_Init(uint32_t p);
void RTC_Calibration(unsigned char *p);
RTC_TimeTypeDef *getRTCTime(void);
RTC_DateTypeDef *getRTCDATE(void);
u8 RTC_InitSetting(unsigned char *p);

#endif

