/*! 
 *  \file hmi_driver.h
 *  \brief 串口屏驱动文件
 *  \version 1.0
 *  \date 2012-2017
 *  \copyright hunda tech
 */

#ifndef _LCD_H_
#define _LCD_H_

#define WAVE_WIDTH 		300
#define WAVE_HEIGHT 	180

#define TIME_100MS 500

#define BATTERY_ID   	 23
#define TEMPERATURE_ID   27
#define WAVE_ID   		 19
#define COOLLER_ID   	 3
#define HEATER_ID   	 2

#define UP_T_ID   	 	4
#define DOW_T_ID   	 	5
#define WAR_T_ID   	 	6
#define REC_T_ID   	 	7
#define POW_T_ID   	 	8
#define USB_ID  		9
#define MODE_CHANGE_ID  36
#define USB_LOG_ID   	 	29
#define RETURN_ID   	 	37
#define RETURN_ID2   	 	38


#define MODE_X_POS   	 	372
#define MODE_Y_POS   	 	24


void lcd_proc(void);
void set_label_proc(unsigned char num);
void lcd_init(void);
void UpdateUI(void);


#endif

