
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : spi2.h
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : Header for spi2.c module
* Description    : --
**********************************************************************/

/* Define to prevent recursive inclusion ----------------------------*/
#ifndef __SPI2_H
#define __SPI2_H

 

/* Includes  == 文件包含 --------------------------------------------*/
#include "stm32f10x.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/



/* Private function prototypes  ==  私有函数声明 --------------------*/
void	SPI2_GPIO_Config(void);
void	SPI2_SPI_Config(void);
void	SPI2_DMA_Tx_Config(u8* Buffer, u8 NumData);
void	SPI2_DMA_Rx_Config(u8* Buffer, u8 NumData);
void	SPI2_NVIC_Config(void);
void	SPI2_DMA_Tx_NVIC_Config(void);
void	SPI2_DMA_Rx_NVIC_Config(void);
void	SPI2_Enable(void);

void	SPI2_WriteByte(u8 dat);
u8		SPI2_ReadByte(void);
u8		SPI2_ReadWriteByte(u8 dat);
void	SPI2_ReadWriteMore(u8 *txbuff, u8 *rxbuff, u8 count);



/* Define to prevent recursive inclusion ----------------------------*/
#endif /* __SPI2_H */




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
