
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : spi1.h
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : Header for spi1.c module
* Description    : --
**********************************************************************/

/* Define to prevent recursive inclusion ----------------------------*/
#ifndef __SPI1_H
#define __SPI1_H

 

/* Includes  == 文件包含 --------------------------------------------*/
#include "stm32f10x.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//
#define	SPI1_DMA_TX_CLK			   RCC_AHBPeriph_DMA1
#define	SPI1_DMA_TX_CHANNEL        DMA1_Channel3
#define	SPI1_DMA_TX_FLAG      		DMA1_FLAG_TC3
#define	SPI1_DMA_TX_IRQn				DMA1_Channel3_IRQn
#define	SPI1_DMA_TX_IRQHandler		DMA1_Channel3_IRQHandler

#define	SPI1_DMA_RX_CLK			   RCC_AHBPeriph_DMA1
#define	SPI1_DMA_RX_CHANNEL        DMA1_Channel2
#define	SPI1_DMA_RX_FLAG	      	DMA1_FLAG_TC2
#define	SPI1_DMA_RX_IRQn				DMA1_Channel2_IRQn
#define	SPI1_DMA_RX_IRQHandler		DMA1_Channel2_IRQHandler

#define	SPI1TXBUFLEN        			80
#define	SPI1RXBUFLEN        			80



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/



/* Private function prototypes  ==  私有函数声明 --------------------*/
void	SPI1_GPIO_Config(void);
void	SPI1_SPI_Config(void);
void	SPI1_DMA_TX_Config(u8* buff, u8 count);
void	SPI1_DMA_RX_Config(u8* Buffer, u8 NumData) ;
void	SPI1_DMA_Rx_NVIC_Config(void);
void	SPI1_HighSpeed(void);
void	SPI1_LowSpeed(void);
void	SPI1_Enable(void);

void	SPI1_WriteByte(u8 dat);
u8		SPI1_ReadByte(void);
u8		SPI1_ReadWriteByte(u8 dat);
void	SPI1_ReadWriteMore(u8 *txbuff, u8 *rxbuff, u8 count);

u8		SPI1_GetBusyFlag(void);
void	SPI1_SetBusyFlag(void);
void	SPI1_SendDataDelegate(u8 *buff, u8 count);
void	SPI1_DMA_RX_IRQHandler(void);



/* Define to prevent recursive inclusion ----------------------------*/
#endif /* __SPI1_H */




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
