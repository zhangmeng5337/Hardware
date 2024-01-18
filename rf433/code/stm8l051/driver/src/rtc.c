#include "stm8l15x.h"
#include "rtc.h"
void RTC_Config(uint16_t time) 
{
  RTC_DeInit(); //��ʼ��Ĭ��״̬ 
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //����RTCʱ�� 
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_2); //ѡ��RTCʱ��ԴLSI 38K��2=19K
  while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET);
  
  RTC_WakeUpCmd(DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //����RTCʱ��
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
  RTC_ITConfig(RTC_IT_WUT, ENABLE); //�����ж�
  RTC_SetWakeUpCounter(time); //����RTC Weakup��������ֵ
  RTC_WakeUpCmd(ENABLE);
}
void RTC_WAKEUP_ENABLE()
{
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  RTC_WakeUpCmd(ENABLE);
  
}
void RTC_WAKEUP_DISABLE()
{
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  RTC_WakeUpCmd(DISABLE);
  
}