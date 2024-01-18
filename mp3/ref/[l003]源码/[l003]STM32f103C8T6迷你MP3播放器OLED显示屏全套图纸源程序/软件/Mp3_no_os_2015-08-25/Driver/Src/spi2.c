
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : spi2.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : spi2 program body
* Description    : --
**********************************************************************/

/* Includes  == 文件包含 --------------------------------------------*/
#include "spi2.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//
//定义SPI2的GPIO引脚
#define	SPI2_SCK_GPIO					GPIOB
#define	SPI2_SCK_PIN               GPIO_Pin_13
#define	SPI2_SCK_CLK               RCC_APB2Periph_GPIOB

#define	SPI2_MISO_GPIO             GPIOB
#define	SPI2_MISO_PIN              GPIO_Pin_14
#define	SPI2_MISO_CLK              RCC_APB2Periph_GPIOB

#define	SPI2_MOSI_GPIO             GPIOB
#define	SPI2_MOSI_PIN              GPIO_Pin_15
#define	SPI2_MOSI_CLK              RCC_APB2Periph_GPIOB

#define	SPI2_DMA_TX_CLK			   RCC_AHBPeriph_DMA1
#define	SPI2_DMA_TX_CHANNEL     	DMA1_Channel5
#define	SPI2_DMA_TX_FLAG      		DMA1_FLAG_TC5
#define	SPI2_DMA_TX_IRQn				DMA1_Channel5_IRQn
#define	SPI2_DMA_TX_IRQHandler		DMA1_Channel5_IRQHandler

#define	SPI2_DMA_RX_CLK			   RCC_AHBPeriph_DMA1
#define	SPI2_DMA_RX_CHANNEL        DMA1_Channel4
#define	SPI2_DMA_RX_FLAG	      	DMA1_FLAG_TC4
#define	SPI2_DMA_RX_IRQn				DMA1_Channel4_IRQn
#define	SPI2_DMA_RX_IRQHandler		DMA1_Channel4_IRQHandler

#define	SPI2TXBUFLEN        			128
#define	SPI2RXBUFLEN        			128



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/

	
	
/* Private functions  ==  私有函数定义 ------------------------------*/

/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	SPI2_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB2PeriphClockCmd(SPI2_SCK_CLK| SPI2_MISO_CLK | SPI2_MOSI_CLK , ENABLE);	

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	
	GPIO_InitStructure.GPIO_Pin   = SPI2_SCK_PIN;
	GPIO_Init(SPI2_SCK_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = SPI2_MISO_PIN;
	GPIO_Init(SPI2_MISO_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = SPI2_MOSI_PIN;
	GPIO_Init(SPI2_MOSI_GPIO, &GPIO_InitStructure);		
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	SPI2_SPI_Config(void)
{
	SPI_InitTypeDef   SPI_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  
 
	// SPI1 Config  
	SPI_InitStructure.SPI_Direction 				= SPI_Direction_2Lines_FullDuplex; //SPI_Direction_1Line_Tx; //SPI_Direction_2Lines_FullDuplex; 
	SPI_InitStructure.SPI_Mode 					= SPI_Mode_Master; //SPI_Mode_Slave; //SPI_Mode_Master; 
	SPI_InitStructure.SPI_DataSize				= SPI_DataSize_8b; 
	SPI_InitStructure.SPI_CPOL 					= SPI_CPOL_Low; //SPI_CPOL_High; //SPI_CPOL_Low; 
	SPI_InitStructure.SPI_CPHA 					= SPI_CPHA_1Edge; 
	SPI_InitStructure.SPI_NSS 						= SPI_NSS_Soft; //SPI_NSS_Soft; //SPI_NSS_Hard
	SPI_InitStructure.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_2; 
	SPI_InitStructure.SPI_FirstBit 				= SPI_FirstBit_MSB; 
	SPI_InitStructure.SPI_CRCPolynomial 		= 7; 
	SPI_Init(SPI2, &SPI_InitStructure); 

	//如果SPI工作于主模式，NSS引脚作为输入，设置了SPI_NSS_Soft，则要调用SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set)函数
	//设置了SPI_NSS_Hard，则要保持NSS引脚为高电平。NSS引脚作为输出时，设置了SPI_NSS_Soft和SPI_NSS_Hard无所谓
	SPI_SSOutputCmd(SPI2, DISABLE);
	SPI_NSSInternalSoftwareConfig(SPI2, SPI_NSSInternalSoft_Set);	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	SPI2_Enable(void)
{
	SPI2_GPIO_Config();
	SPI2_SPI_Config();
	//SPI2 enable  
	SPI_Cmd(SPI2, ENABLE); 	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	SPI2_WriteByte(u8 dat)
{
	//Wait until the transmit buffer is empty 
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET){;} 
	//Send the byte 
	SPI_I2S_SendData(SPI2, dat); 	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
u8		SPI2_ReadByte(void)
{
	u8 dat;
   //Wait until a data is received 
   while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET){;} 
   //Get the received data 
   dat = SPI_I2S_ReceiveData(SPI2);
   //Return the shifted data 
   return dat;		
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
u8		SPI2_ReadWriteByte(u8 dat)
{
	u8 temp;
	//Wait until the transmit buffer is empty 
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET){;} 
	//Send the byte 
	SPI_I2S_SendData(SPI2, dat); 
	//Wait until a data is received 
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET){;} 
	//Get the received data 
	temp = SPI_I2S_ReceiveData(SPI2);
	//Return the shifted data 
	return temp; 	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	SPI2_ReadWriteMore(u8 *txbuff, u8 *rxbuff, u8 count)
{
	u8 i;
	//Wait until the transmit buffer is empty 
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET){;} 
	SPI_I2S_SendData(SPI2, txbuff[0]);
	
	for(i=0; i<(count-1); i++)
	{
		while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET){;}
		SPI_I2S_SendData(SPI2, txbuff[1+i]);
		while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET){;}
		rxbuff [i] = SPI_I2S_ReceiveData(SPI2);		
	}
	
	//Wait until a data is received 	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET){;}	
	rxbuff [count-1] = SPI_I2S_ReceiveData(SPI2);	
}	




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
