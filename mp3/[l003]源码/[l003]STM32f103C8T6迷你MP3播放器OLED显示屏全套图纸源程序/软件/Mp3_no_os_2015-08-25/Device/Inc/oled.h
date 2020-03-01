
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : oled.h
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : Header for oled.c module
* Description    : --
**********************************************************************/

/* Define to prevent recursive inclusion ----------------------------*/
#ifndef __OLED_H
#define __OLED_H

 

/* Includes  == 文件包含 --------------------------------------------*/
#include "stm32f10x.h"
#include "fatfs.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/



/* Private function prototypes  ==  私有函数声明 --------------------*/
void	OLED_GPIO_Config(void);
void	OLED_Config(void);

void	OLED_CS_High(void);
void	OLED_CS_Low(void);
void	OLED_RST_High(void);
void	OLED_RST_Low(void);
void	OLED_DC_High(void);
void	OLED_DC_Low(void);

void	OLED_WriteData(u8 dat);
void	OLED_WriteCommand(u8 cmd);

void OLED_SetStartColumn(u8 ucData);
void OLED_SetAddressingMode(u8 ucData);
void OLED_SetColumnAddress(u8 a, u8 b);
void OLED_SetPageAddress(u8 a, u8 b);
void OLED_SetStartLine(u8 ucData);
void OLED_SetContrastControl(u8 ucData);
void OLED_SetChargePump(u8 ucData);
void OLED_SetSegmentRemap(u8 ucData);
void OLED_SetEntireDisplay(u8 ucData);
void OLED_SetInverseDisplay(u8 ucData);
void OLED_SetMultiplexRatio(u8 ucData);
void OLED_SetDisplayOnOff(u8 ucData);
void OLED_SetStartPage(u8 ucData);
void OLED_SetCommonRemap(u8 ucData);
void OLED_SetDisplayOffset(u8 ucData);
void OLED_SetDisplayClock(u8 ucData);
void OLED_SetPrechargePeriod(u8 ucData);
void OLED_SetCommonConfig(u8 ucData);
void OLED_SetVCOMH(u8 ucData);
void OLED_SetNop(void);

void	OLED_Config(void);
void	OLED_DisplayAsciiChar(u8 x, u8 y, u8 size, u8 ascii);
void	OLED_DisplayChinaChar(u8 x, u8 y, u8 size, u8 *hzchar);
void	OLED_DisplayString(u8 x, u8 y, u8 size, u8 *buff);
void	OLED_DisplayFileNameBefore(FATFS_FileListInfoTypeDef *list);
void	OLED_DisplayFileNameNext(FATFS_FileListInfoTypeDef *list);
void	OLED_DisplayFileNameMove(FATFS_FileListInfoTypeDef *list);
void	OLED_DisplayFileNameBack(FATFS_FileListInfoTypeDef *list);
void	OLED_DisplayFileName(FATFS_FileListInfoTypeDef list);
void	OLED_DisplayFileNum(FATFS_FileListInfoTypeDef *list);

void	OLED_DisplayRoll(u8 page, u8 size, u8 *buff);
void	OLED_DisplayTurnColor(u8 page);
void	OLED_DisplayUpdate(void);
void	OLED_DisplayOn(void);
void	OLED_DisplayOff(void);
void	OLED_DisplayBuffClearAll(void);
void	OLED_DisplayBuffClearPage(u8 page);
void	OLED_DisplayBuffClearFile(void);
void	OLED_DisplayBuffFill(void);



/* Define to prevent recursive inclusion ----------------------------*/
#endif /* __OLED_H */




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
