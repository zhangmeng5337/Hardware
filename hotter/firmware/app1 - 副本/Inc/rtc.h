#ifndef RTC_H_
#define RTC_H_
#include "main.h"
typedef struct
{
  unsigned char update;//rtc time synch 4g module
  uint8_t Hours;            /*!< Specifies the RTC Time Hour.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 12 if the RTC_HourFormat_12 is selected
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 23 if the RTC_HourFormat_24 is selected */
  uint8_t Minutes;          /*!< Specifies the RTC Time Minutes.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 59 */

  uint8_t Seconds;          /*!< Specifies the RTC Time Seconds.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 59 */
                       // This parameter can be a value of @ref RTC_AM_PM_Definitions */                                Saving Time, please use HAL_RTC_DST_xxx functions */
  uint8_t WeekDay;  /*!< Specifies the RTC Date WeekDay.
                         This parameter can be a value of @ref RTC_WeekDay_Definitions */

  uint8_t Month;    /*!< Specifies the RTC Date Month (in BCD format).
                         This parameter can be a value of @ref RTC_Month_Date_Definitions */

  uint8_t Date;     /*!< Specifies the RTC Date.
                         This parameter must be a number between Min_Data = 1 and Max_Data = 31 */

  uint8_t Year;     /*!< Specifies the RTC Date Year.
                         This parameter must be a number between Min_Data = 0 and Max_Data = 99 */
} rtc_stru;
//void GET_Time(void);//获取当前时间
void urtc_proc(void);
RTC_DateTypeDef *getRtcDate(void);
RTC_TimeTypeDef *getRtcTime(void);
rtc_stru *rtc_value_set(void);

#endif

