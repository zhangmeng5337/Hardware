#include "rtc.h"
 RTC_STRU rtc_usr;

 RTC_DateTypeDef sdatestructure;
 RTC_TimeTypeDef stimestructure;
 RTC_STRU *getRtcStatus(void)
 {
	return &rtc_usr;
 }

void SetWakeUp(unsigned char *p)
{
   RTC_HandleTypeDef hrtc;
   unsigned int WakeUpCounter;
   WakeUpCounter = ((u16)(p[0]))<<8+p[1];
   WakeUpCounter = WakeUpCounter*60;   
   HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, WakeUpCounter, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);

}

void  setRtc(unsigned char* p,unsigned char item)
{
	/* USER CODE BEGIN RTC_Init 0 */
	
	  /* USER CODE END RTC_Init 0 */
	if(item <=3)
	{
		RTC_HandleTypeDef hrtc;
		RTC_TimeTypeDef sTime = {0};
        RTC_DateTypeDef sDate = {0};
		sDate.Year = p[0];
		sDate.Year = p[1];
		sDate.Year = p[2];
		sTime.Hours = p[3];
		sTime.Minutes = p[4];
		sTime.Seconds = p[5];

		HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);


	}
	 

}
unsigned char *getRTC()
{
    unsigned char *p;
	RTC_HandleTypeDef hrtc;	
	HAL_RTC_GetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sdatestructure, RTC_FORMAT_BIN); 
	*p++ = sdatestructure.Year;
	*p++ = sdatestructure.Month;
	*p++ = sdatestructure.Date;
	*p++ = stimestructure.Hours;
	*p++ = stimestructure.Minutes;
	*p++ = stimestructure.Seconds;
	return p;

}



