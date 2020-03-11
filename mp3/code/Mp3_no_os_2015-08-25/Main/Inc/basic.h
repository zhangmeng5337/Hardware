
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : basic.h
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : Header for basic.c module
* Description    : --
**********************************************************************/

/* Define to prevent recursive inclusion ----------------------------*/
#ifndef __BASIC_H
#define __BASIC_H

 

/* Includes  == 文件包含 --------------------------------------------*/
#include "stm32f10x.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//
#define	BASIC_YES	((u8)(0x00))
#define	BASIC_NO		((u8)(0x01))

#define	BASIC_RESET	((u8)(0x00))
#define	BASIC_SET	((u8)(0x01))

#define	BASIC_TURE	((u8)(0x00))
#define	BASIC_FAULE	((u8)(0x01))

#define	BASIC_OK		((u8)(0x00))
#define	BASIC_ERROR	((u8)(0x01))



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/



/* Private function prototypes  ==  私有函数声明 --------------------*/
void BASIC_Memcpy(u8 *des, u8 *src, u32 len);



/* Define to prevent recursive inclusion ----------------------------*/
#endif /* __BASIC_H */




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
