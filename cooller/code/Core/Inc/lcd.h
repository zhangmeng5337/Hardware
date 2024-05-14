/*! 
 *  \file hmi_driver.h
 *  \brief 串口屏驱动文件
 *  \version 1.0
 *  \date 2012-2017
 *  \copyright hunda tech
 */

#ifndef _LCD_H_
#define _LCD_H_

#define WAVE_WIDTH 		230
#define WAVE_HEIGHT 	240

#define TIME_100MS 3000

#define BATTERY_ID   	 23
#define TEMPERATURE_ID   24
#define WAVE_ID   		 19
#define COOLLER_ID   	 3
#define HEATER_ID   	 2

#define UP_T_ID   	 	4
#define DOW_T_ID   	 	5
#define WAR_T_ID   	 	6
#define REC_T_ID   	 	7
#define POW_T_ID   	 	8

#define USB_LOG_ID   	 	18

void lcd_proc(void);
void set_label_proc(unsigned char num);
void lcd_init(void);


#endif

