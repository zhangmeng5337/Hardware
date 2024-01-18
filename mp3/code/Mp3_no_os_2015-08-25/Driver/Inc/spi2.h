
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

 

/* Includes  == �ļ����� --------------------------------------------*/
#include "stm32f10x.h"



/* Private typedef    ==  ˽������ ----------------------------------*/
/* Private define     ==  ˽�ж��� ----------------------------------*/

//-----------------------------------------//



/* Private macro      ==  ˽�к� ------------------------------------*/
/* Private variables  ==  ˽�б��� ----------------------------------*/



/* Private function prototypes  ==  ˽�к������� --------------------*/
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
