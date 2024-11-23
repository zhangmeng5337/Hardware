/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "rtc.h"
#include "time.h"
int weekday_cal(void);

/* USER CODE BEGIN 0 */
rtc_stru rtc_usr;
RTC_TimeTypeDef Now_Time;//?¡§¨°?¨º¡À???¨¢11¨¬?
RTC_DateTypeDef Now_Date;//?¡§¨°?¨¨??¨²?¨¢11¨¬?
RTC_TimeTypeDef Now_Time_BIN;//?¡§¨°?¨º¡À???¨¢11¨¬?
RTC_DateTypeDef Now_Date_BIN;//?¡§¨°?¨¨??¨²?¨¢11¨¬?

uint8_t Data[5];
//extern sgp30_data_t sgp30_data;
RTC_AlarmTypeDef sAlarm = {0};

/* USER CODE END 0 */

extern RTC_HandleTypeDef hrtc;
RTC_TimeTypeDef *getRtcTime(void)
{
    return &Now_Time_BIN;
}
RTC_DateTypeDef *getRtcDate(void)
{
    return &Now_Date_BIN;
}
rtc_stru *rtc_value_set(void)
{
    return &rtc_usr;
}
/************************utc to rtc****************************



**************************************************************/
 struct tm *gm_date;

 void utcTortc(uint64_t utc_t)
{
 //??unix timestamp ¡Áa?a¡À?¦Ì?¨º¡À??
unsigned int tmp;
 time_t seconds=utc_t/1000;//unix timestamp
 gm_date = localtime(&seconds);
tmp = gm_date->tm_year-100;
if(tmp==rtc_usr.Year)
{
	//rtc_usr.Year = tmp;
	//rtc_usr.Month = gm_date->tm_mon+1;
	//rtc_usr.Date = gm_date->tm_mday;	
	rtc_usr.Hours = gm_date->tm_hour+8;
		//rtc_usr.WeekDay = gm_date->tm_wday;
if(rtc_usr.Hours>=24)
			rtc_usr.Hours = rtc_usr.Hours - 24;
	rtc_usr.Minutes = gm_date->tm_min;
	rtc_usr.Seconds = gm_date->tm_sec;
	rtc_usr.update = 1;

}




// printf("?¨º:%d \r\n",gm_date->tm_year);
// printf("??:%d \r\n",gm_date->tm_mon);
// printf("¨¨?:%d \r\n",gm_date->tm_mday);
// printf("¨º¡À:%d \r\n",gm_date->tm_hour);
// printf("¡¤?:%d \r\n",gm_date->tm_min);
// printf("??:%d \r\n",gm_date->tm_sec);

 //??¡À?¦Ì?¨º¡À??¡Áa?aunix timestamp 
// gm_date->tm_year=118;//2018?¨º,+1900?¨ª¨º????¨²¦Ì??¨º
// gm_date->tm_mon=3;//4??
// gm_date->tm_mday=11;
// gm_date->tm_hour=10;
// gm_date->tm_min=8;
// gm_date->tm_sec=30;
// seconds=mktime(gm_date);
 //printf("unix timestamp:x \r\n",seconds);
 //unix timestamp:5acdde9e 
 //¡À¡À??¨º¡À??¡êo 2018/4/11 18:8:30
}













/* RTC init function */
void set_rtc_time()
{

    /* USER CODE BEGIN RTC_Init 0 */


    if(rtc_usr.update == 1)
    {
        rtc_stru urtc_bcd;
        int cal_result,tmp1;
        weekday_cal();
        rtc_usr.Year = Now_Date_BIN.Year;
        cal_result = rtc_usr.Year;
        cal_result = cal_result/10*16;
        cal_result = cal_result+ rtc_usr.Year%10;
        urtc_bcd.Year = cal_result;
rtc_usr.Month = Now_Date_BIN.Month;
        cal_result = rtc_usr.Month;
        cal_result = cal_result/10*16;
        cal_result = cal_result+ rtc_usr.Month%10;
        urtc_bcd.Month = cal_result;
rtc_usr.Date = Now_Date_BIN.Date;
        cal_result = rtc_usr.Date;
        cal_result = cal_result/10*16;
        cal_result = cal_result+ rtc_usr.Date%10;
        urtc_bcd.Date = cal_result;

        cal_result = rtc_usr.Hours;
        cal_result = cal_result/10*16;
        cal_result = cal_result+ rtc_usr.Hours%10;
        urtc_bcd.Hours = cal_result;

        cal_result = rtc_usr.Minutes;
        cal_result = cal_result/10*16;
        cal_result = cal_result+ rtc_usr.Minutes%10;
        urtc_bcd.Minutes = cal_result;
rtc_usr.WeekDay = Now_Date_BIN.WeekDay;
        cal_result = rtc_usr.Seconds;
        cal_result = cal_result/10*16;
        cal_result = cal_result+ rtc_usr.Seconds%10;
        urtc_bcd.Seconds = cal_result;
        urtc_bcd.WeekDay = rtc_usr.WeekDay;

        /* USER CODE END RTC_Init 0 */

        RTC_TimeTypeDef sTime = {0};
        RTC_DateTypeDef sDate = {0};

        sTime.Hours = urtc_bcd.Hours;
        sTime.Minutes = urtc_bcd.Minutes;
        sTime.Seconds = urtc_bcd.Seconds;
        sTime.SubSeconds = 0;
        sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
        sTime.StoreOperation = RTC_STOREOPERATION_RESET;

        sDate.WeekDay = urtc_bcd.WeekDay;
        sDate.Month = urtc_bcd.Month;
        sDate.Date = urtc_bcd.Date;
        sDate.Year = urtc_bcd.Year;
        // if(HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR0)!=0x5A5A)			 /* ?¨¬2¨¦¨º?¡¤?D¡ä¨¨?1y¨°?¡ä?RTC¡ê?¡À¡ê?¡è¦Ì?¦Ì?2??a¨º¡ìRTC¨º¡À?¨® */
        {
            /* USER CODE END Check_RTC_BKUP */

            /** Initialize RTC and set the Time and Date
            */

            if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
            {
                Error_Handler();
            }

            if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
            {
                Error_Handler();
            }

            /* USER CODE BEGIN RTC_Init 2 */
            HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0x5A5A);  //?a¨¤??¨ª¨º????a????¡ä??¡Â¦Ì?¡À¨º??¨¦¨¨?a??2?¦Ì??????¦Ì¡ê???¡ä?¦Ì?¦Ì?o¨®?¨ª2??¨¢??¨¨?¦Ì??a¨¤?¨¤¡ä
        }
//        else
//        {
//            RTC_AlarmConfig(); 						  /* RTC¦Ì??D??????3?¨º??¡¥¨°??¡ã???¡¥ */
//        }

        rtc_usr.update = 0;

    }

}



/* USER CODE BEGIN 1 */


void GET_Time(void)//??¨¨?¦Ì¡À?¡ã¨º¡À??
{
    HAL_RTC_GetTime(&hrtc,&Now_Time,RTC_FORMAT_BCD);//RTC_FORMAT_BIN
    HAL_RTC_GetDate(&hrtc,&Now_Date,RTC_FORMAT_BCD);
    Now_Time_BIN.Hours = Now_Time.Hours/16*10;
    Now_Time_BIN.Hours = Now_Time_BIN.Hours + Now_Time.Hours%16;

    Now_Time_BIN.Minutes = Now_Time.Minutes/16*10;
    Now_Time_BIN.Minutes = Now_Time_BIN.Minutes + Now_Time.Minutes%16;

    Now_Time_BIN.Seconds = Now_Time.Seconds/16*10;
    Now_Time_BIN.Seconds = Now_Time_BIN.Seconds + Now_Time.Seconds%16;


    Now_Date_BIN.WeekDay = Now_Date.WeekDay/16*10;
    Now_Date_BIN.WeekDay = Now_Date_BIN.WeekDay + Now_Date.WeekDay%16;

    Now_Date_BIN.Date = Now_Date.Date/16*10;
    Now_Date_BIN.Date = Now_Date_BIN.Date + Now_Date.Date%16;



    Now_Date_BIN.Month = Now_Date.Month/16*10;
    Now_Date_BIN.Month = Now_Date_BIN.Month + Now_Date.Month%16;

    Now_Date_BIN.Year = Now_Date.Year/16*10;
    Now_Date_BIN.Year = Now_Date_BIN.Year + Now_Date.Year%16;


}

//	uint32_t next_trigger_hours = Now_Time.Hours;
//  sAlarm.AlarmTime.Hours = next_trigger_hours;
/* RTC¦Ì????D?????????¡¥o¡¥¨ºy */
//void RTC_AlarmConfig(void)
//{
//
//    RTC_AlarmTypeDef sAlarm = {0};
//    HAL_RTC_GetTime(&hrtc, &Now_Time, RTC_FORMAT_BIN);
//    HAL_RTC_GetDate(&hrtc, &Now_Date, RTC_FORMAT_BIN);
//
//    // ??????¨°???¡ä£¤¡¤¡é¨º¡À??
//    uint32_t next_trigger_seconds = Now_Time.Seconds + 9;
//    if (next_trigger_seconds >= 60)
//    {
//        next_trigger_seconds -= 60;
//    }
//    sAlarm.AlarmTime.Hours = Now_Time.Hours;
//    sAlarm.AlarmTime.Minutes = Now_Time.Minutes;
//    sAlarm.AlarmTime.Seconds = next_trigger_seconds;
//    sAlarm.AlarmTime.SubSeconds = 0x0;
//    sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
//    sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
//    sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
//    sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_SS14_9;
//    sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
//    sAlarm.AlarmDateWeekDay = 0x0 ;
//    sAlarm.Alarm = RTC_ALARM_A;
//
//    if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
//    {
//        Error_Handler();
//    }
//
//}


/* RTC?D????¦Ì¡Â */
//void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
//{
//
//    printf("20s pass\r\n");
//// ?¨¹D???¨°??????¨®¡ä£¤¡¤¡é¨º¡À??
//    RTC_AlarmConfig();
//    //  GET_Time();
//    printf("%02d:%02d:%02d\n",Now_Time.Hours,Now_Time.Minutes,Now_Time.Seconds);
//    printf("%02d:%02d:%02d\n",Now_Date.Year,Now_Date.Month,Now_Date.Date);
//    printf("week%01d\n",Now_Date.WeekDay);
//    printf("Temperature:%02d\r\n",Data[2]);
//    printf("Humidity:%02d\r\n",Data[0]);
//    printf("CO2_Density:%03d\r\n",sgp30_data.co2);
//
//}
int weekday_cal()
{
    int year, month, day, total_days = 0, i;
    year = rtc_usr.Year+2000;
//    for (i = 1900; i < year; i++)
//    {
//        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
//            total_days += 366;
//        else
//            total_days += 365;
//    }

    for (i = 1; i < rtc_usr.Month; i++)
    {
        switch (i)
        {
        case 2:
            if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
                total_days += 29;
            else
                total_days += 28;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            total_days += 30;
            break;
        default:
            total_days += 31;
            break;
        }
    }
    total_days += rtc_usr.Date;
    total_days = year-1+((year-1)/4)-((year-1)/100)+((year-1)/400)+total_days;
    switch (total_days % 7)
    {
    case 1:
        rtc_usr.WeekDay = 1;
        break;
    case 2:
        rtc_usr.WeekDay = 2;

        break;
    case 3:
        rtc_usr.WeekDay = 3;

        break;
    case 4:
        rtc_usr.WeekDay = 4;

        break;
    case 5:
        rtc_usr.WeekDay = 5;

        break;
    case 6:
        rtc_usr.WeekDay = 6;

        break;
    case 0:
        rtc_usr.WeekDay = 7;


        break;
    }

    return 0;
}

/* USER CODE END 1 */
void urtc_proc(void)
{
    GET_Time();//??¨¨?¦Ì¡À?¡ã¨º¡À??
    set_rtc_time();

}

