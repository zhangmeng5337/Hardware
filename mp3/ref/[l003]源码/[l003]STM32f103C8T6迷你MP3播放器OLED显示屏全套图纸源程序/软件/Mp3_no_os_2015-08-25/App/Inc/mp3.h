
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : mp3.h
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : Header for mp3.c module
* Description    : --
**********************************************************************/

/* Define to prevent recursive inclusion ----------------------------*/
#ifndef __MP3_H
#define __MP3_H

 

/* Includes  == �ļ����� --------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x.h"
#include "microsec.h"
#include "stm3210e_eval_sdio_sd.h"
#include "sdcard.h"
#include "led.h"
#include "key.h"
#include "oled.h"
#include "vs1053b.h"
#include "power.h"
#include "fatfs.h"
#include "basic.h"
#include "playsong.h"
#include "display.h"
#include "mp3.h"
#include "setting.h"
#include "adc.h"
#include "timer.h"

#include <stdio.h>
#include <string.h>



/* Private typedef    ==  ˽������ ----------------------------------*/
/* Private define     ==  ˽�ж��� ----------------------------------*/

//-----------------------------------------//



/* Private macro      ==  ˽�к� ------------------------------------*/
/* Private variables  ==  ˽�б��� ----------------------------------*/
extern FATFS_FileListInfoTypeDef		filelis;
extern SETTING_DataTypeDef				Setting;



/* Private function prototypes  ==  ˽�к������� --------------------*/
void MP3_Main(void);
void MP3_Init(void);
void MP3_Menu(void);
void MP3_ScanFile(void);
void MP3_PlaySong(void);
void MP3_Setting(void);

void MP3_PowerOn(void);
void MP3_PowerOffWithoutSD(void);
void MP3_PowerOffWithSD(void);
void MP3_LowBatteryWithoutSD(void);
void MP3_LowBatteryWithSD(void);

void MP3_CheckSD(void);



/* Define to prevent recursive inclusion ----------------------------*/
#endif /* __MP3_H */




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
