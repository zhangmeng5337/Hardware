
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : key.h
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : Header for key.c module
* Description    : --
**********************************************************************/

/* Define to prevent recursive inclusion ----------------------------*/
#ifndef __KEY_H
#define __KEY_H

 

/* Includes  == 文件包含 --------------------------------------------*/
#include "stm32f10x.h"
#include "microsec.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//
#define	D_KEY_NULL			0x00
#define	D_KEY_IS_VINC		0x01
#define	D_KEY_IS_VDEC		0x02
#define	D_KEY_IS_POWER		0x03
#define	D_KEY_IS_BACK		0x04
#define	D_KEY_IS_NEXT		0x05
#define	D_KEY_IS_BEFORE	0x06
#define	D_KEY_IS_STOP		0x07
#define	D_KEY_IS_LOCK		0x08

#define	D_KEY_NUM			0x08

#define	D_KEY_DOWN			0x09
#define	D_KEY_LONGDOWN		0x0a
#define	D_KEY_UP			0x0b

#define	D_KEY_ACTIVE_YES	0xc
#define	D_KEY_ACTIVE_NO	0xd


typedef struct
{
   u8		Type;
	u8		State;
	u8		Active;	
	u32	LongPushCnt;
}D_KEY_StatusTypeDef;



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/



/* Private function prototypes  ==  私有函数声明 --------------------*/
void KEY_GPIO_Config(void);
void KEY_Config(void);
u8 KEY_GetStatus(u8 keynum);
void KEY_Clear(D_KEY_StatusTypeDef key);
D_KEY_StatusTypeDef KEY_Scan(s64 scan_time);



/* Define to prevent recursive inclusion ----------------------------*/
#endif /* __KEY_H */




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
