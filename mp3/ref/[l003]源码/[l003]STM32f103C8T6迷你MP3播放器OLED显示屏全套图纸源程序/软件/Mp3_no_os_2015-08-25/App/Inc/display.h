
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : display.h
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : Header for display.c module
* Description    : --
**********************************************************************/

/* Define to prevent recursive inclusion ----------------------------*/
#ifndef __DISPLAY_H
#define __DISPLAY_H

 

/* Includes  == 文件包含 --------------------------------------------*/
#include "stm32f10x.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/



/* Private function prototypes  ==  私有函数声明 --------------------*/
void Display_Icon(u8 flag);
void Display_Power(u8 num);
void Display_Num(u16 total, u16 current);

void Display_AsciiChar(u8 x, u8 y, u8 ascii);
void Display_ChinaChar(u8 x, u8 y, u8 *hzchar);
void Display_String(u8 x, u8 y, u8 *buff);

void Display_TurnColor(u8 page);
void Display_ClearFileNameZoom(void);
void Display_UpdateFileNameZoom(void);
void Display_ClearSongNameZoom(void);
void Display_UpdateSongNameZoom(void);
void Display_ClearAllZoom(void);
void Display_UpdateAllZoom(void);
void Display_UpdateAllZoom(void);

void Display_PlayTime(u8 tim);
void Display_Line(void);
void Display_Stop(u8 index);
void Display_Choose(u8 index);
void Display_Mode(u8 index);
void Display_Effect(u8 index);
void Display_Voice(u8 num);

void Display_ErrorWithSD(u8 *buff);
void Display_ErrorWithoutSD(u8 *buff);
void Display_PowerOn(void);
void Display_PowerOff(void);
void Display_LowBattery(void);



/* Define to prevent recursive inclusion ----------------------------*/
#endif /* __DISPLAY_H */




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
