
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : vs1053b.h
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : Header for vs1053b.c module
* Description    : --
**********************************************************************/

/* Define to prevent recursive inclusion ----------------------------*/
#ifndef __VS1053B_H
#define __VS1053B_H

 

/* Includes  == 文件包含 --------------------------------------------*/
#include "stm32f10x.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//
#define VS1053B_WRITE_COMMAND 		0x02
#define VS1053B_READ_COMMAND 			0x03		 		 

#define VS1053B_MODE        			0x00   
#define VS1053B_STATUS      			0x01   
#define VS1053B_BASS        			0x02   
#define VS1053B_CLOCKF      			0x03   
#define VS1053B_DECODE_TIME 			0x04   
#define VS1053B_AUDATA      			0x05   
#define VS1053B_WRAM        			0x06   
#define VS1053B_WRAMADDR    			0x07   
#define VS1053B_HDAT0       			0x08   
#define VS1053B_HDAT1       			0x09   
#define VS1053B_AIADDR      			0x0a   
#define VS1053B_VOL         			0x0b   
#define VS1053B_AICTRL0     			0x0c   
#define VS1053B_AICTRL1     			0x0d   
#define VS1053B_AICTRL2     			0x0e   
#define VS1053B_AICTRL3     			0x0f  



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/



/* Private function prototypes  ==  私有函数声明 --------------------*/
void	VS1053B_GPIO_Config(void);

void	VS1053B_CS_High(void);
void	VS1053B_CS_Low(void);
void	VS1053B_DCS_High(void);
void	VS1053B_DCS_Low(void);
void	VS1053B_RESET_High(void);
void	VS1053B_RESET_Low(void);
u8		VS1053B_DREQ_State(void);

u8		VS1053B_ReadWriteByte(u8 dat);
void	VS1053B_WriteReg(u8 reg, u16 dat);
u16		VS1053B_ReadReg(u8 reg);
void	VS1053B_WriteMemory(u16 addr, u16 dat);
u16		VS1053B_ReadMemory(u16 addr);

void	VS1053B_Config(void);
void	VS1053B_SofeReset(void);
void	VS1053B_HardReset(void);
void	VS1053B_SineTest(void);
void	MP3_Play1(u8 *path);



/* Define to prevent recursive inclusion ----------------------------*/
#endif /* __VS1053B_H */




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
