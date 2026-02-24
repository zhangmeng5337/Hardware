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


/* RTC init function */
void set_rtc_time()
{

    /* USER CODE BEGIN RTC_Init 0 */


    if(rtc_usr.update == 1)
    {
        /* USER CODE END RTC_Init 0 */

        RTC_TimeTypeDef sTime = {0};
        RTC_DateTypeDef sDate = {0};

//        sTime.Hours = urtc_bcd.Hours;
//        sTime.Minutes = urtc_bcd.Minutes;
//        sTime.Seconds = urtc_bcd.Seconds;
//        sTime.SubSeconds = 0;
//        sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
//        sTime.StoreOperation = RTC_STOREOPERATION_RESET;
//
//        sDate.WeekDay = urtc_bcd.WeekDay;
//        sDate.Month = urtc_bcd.Month;
//        sDate.Date = urtc_bcd.Date;
//        sDate.Year = urtc_bcd.Year;
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


int weekday_cal()
{
    int year, total_days = 0, i;
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
    GET_Time();//
    set_rtc_time();
}

