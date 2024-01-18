
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : setting.h
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : Header for setting.c module
* Description    : --
**********************************************************************/

/* Define to prevent recursive inclusion ----------------------------*/
#ifndef __SETTING_H
#define __SETTING_H

 

/* Includes  == 文件包含 --------------------------------------------*/
#include "stm32f10x.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//
typedef struct
{
	u8	Music_PathBuff[100];
	u8	Music_PathLen;
	u8	Music_SectorNum;
	
	u8 Music_Choose;
	u8 Music_Mode;
	u8 Music_Effice;
	u8 Music_Voice;
	
	u8	Sys_BackLightness;		//背光亮度
	u8	Sys_BackLightTime;		//背光时间
	
}SETTING_DataTypeDef;

#define	SETTING_PLAYSTYLE_CLESE				0x00
#define	SETTING_PLAYSTYLE_STRONG			0x01
#define	SETTING_PLAYSTYLE_FASHION			0x02
#define	SETTING_PLAYSTYLE_JAZZ				0x03
#define	SETTING_PLAYSTYLE_COUNTRY			0x04
#define	SETTING_PLAYSTYLE_CUSTOMIZE		0x05

#define	SETTING_PLAYMODE_LOOPALL			0x06
#define	SETTING_PLAYMODE_LOOPONE			0x07
#define	SETTING_PLAYMODE_ORDER				0x08

#define	SETTING_BACKLINGT_ON					0x09
#define	SETTING_BACKLINGT_OFF				0x0a



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/



/* Private function prototypes  ==  私有函数声明 --------------------*/
void	SETTING_Default(SETTING_DataTypeDef *setdata);
void	SETTING_WriteFlash(SETTING_DataTypeDef *setdata);
void	SETTING_ReadFlash(SETTING_DataTypeDef *setdata);



/* Define to prevent recursive inclusion ----------------------------*/
#endif /* __SETTING_H */




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
