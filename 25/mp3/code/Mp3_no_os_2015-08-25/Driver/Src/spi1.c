
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : spi1.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : spi1 program body
* Description    : --
**********************************************************************/

/* Includes  == 文件包含 --------------------------------------------*/
#include "spi1.h"
#include "vs1053b.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//
//定义SPI1的GPIO引脚
#define	SPI1_SCK_GPIO					GPIOA
#define	SPI1_SCK_PIN               GPIO_Pin_5
#define	SPI1_SCK_CLK               RCC_APB2Periph_GPIOA

#define	SPI1_MISO_GPIO             GPIOA
#define	SPI1_MISO_PIN              GPIO_Pin_6
#define	SPI1_MISO_CLK              RCC_APB2Periph_GPIOA

#define	SPI1_MOSI_GPIO             GPIOA
#define	SPI1_MOSI_PIN              GPIO_Pin_7
#define	SPI1_MOSI_CLK              RCC_APB2Periph_GPIOA



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/
volatile	u8	SPI1_BusyFlag;
u8	SPI1_RxBuff[32];


	
/* Private functions  ==  私有函数定义 ------------------------------*/

/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	SPI1_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB2PeriphClockCmd(SPI1_SCK_CLK| SPI1_MISO_CLK | SPI1_MOSI_CLK , ENABLE);	

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	
	GPIO_InitStructure.GPIO_Pin   = SPI1_SCK_PIN;
	GPIO_Init(SPI1_SCK_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = SPI1_MISO_PIN;
	GPIO_Init(SPI1_MISO_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = SPI1_MOSI_PIN;
	GPIO_Init(SPI1_MOSI_GPIO, &GPIO_InitStructure);	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	SPI1_SPI_Config(void)
{
	SPI_InitTypeDef   SPI_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  

	// SPI1 Config  
	SPI_InitStructure.SPI_Direction 				= SPI_Direction_2Lines_FullDuplex; //SPI_Direction_1Line_Tx; //SPI_Direction_2Lines_FullDuplex; 
	SPI_InitStructure.SPI_Mode 					= SPI_Mode_Master; //SPI_Mode_Slave; //SPI_Mode_Master; 
	SPI_InitStructure.SPI_DataSize				= SPI_DataSize_8b; 
	SPI_InitStructure.SPI_CPOL 					= SPI_CPOL_Low; //SPI_CPOL_High; //SPI_CPOL_Low; 
	SPI_InitStructure.SPI_CPHA 					= SPI_CPHA_1Edge; 
	SPI_InitStructure.SPI_NSS 						= SPI_NSS_Soft; //SPI_NSS_Soft; //SPI_NSS_Hard
	SPI_InitStructure.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_32; 
	SPI_InitStructure.SPI_FirstBit 				= SPI_FirstBit_MSB; 
	SPI_InitStructure.SPI_CRCPolynomial 		= 7; 
	SPI_Init(SPI1, &SPI_InitStructure); 

	//如果SPI工作于主模式，NSS引脚作为输入，设置了SPI_NSS_Soft，则要调用SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set)函数
	//设置了SPI_NSS_Hard，则要保持NSS引脚为高电平。NSS引脚作为输出时，设置了SPI_NSS_Soft和SPI_NSS_Hard无所谓
	SPI_SSOutputCmd(SPI1, DISABLE);
	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void SPI1_DMA_TX_Config(u8* buff, u8 count) 
{
	DMA_InitTypeDef  DMA_InitStructure;

	RCC_AHBPeriphClockCmd(SPI1_DMA_TX_CLK, ENABLE);             

	DMA_DeInit(SPI1_DMA_TX_CHANNEL);
	DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)(&(SPI1->DR));	
	DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)(buff);
	DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize          = count;
	DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode                = DMA_Mode_Normal;//DMA_Mode_Circular; //DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority            = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M                 = DMA_M2M_Disable;

	DMA_Init(SPI1_DMA_TX_CHANNEL, &DMA_InitStructure);	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	SPI1_DMA_RX_Config(u8* Buffer, u8 NumData) 
{
	DMA_InitTypeDef  DMA_InitStructure;

	RCC_AHBPeriphClockCmd(SPI1_DMA_RX_CLK, ENABLE);             

	DMA_DeInit(SPI1_DMA_RX_CHANNEL);
	DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)(&(SPI1->DR));	
	DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)(Buffer);
	DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize          = NumData;
	DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode                = DMA_Mode_Normal;//DMA_Mode_Circular; //DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority            = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M                 = DMA_M2M_Disable;

	DMA_Init(SPI1_DMA_RX_CHANNEL, &DMA_InitStructure);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	SPI1_DMA_Rx_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* Configure and enable I2C DMA RX Channel interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = SPI1_DMA_RX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
  NVIC_Init(&NVIC_InitStructure);   
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	SPI1_HighSpeed(void)
{
  SPI_InitTypeDef  SPI_InitStructure;

  SPI_Cmd(SPI1, DISABLE);
  /* SPI1 configuration */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;				  	
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;			  	
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;				  		
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; 
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				  
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);
  
  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);  	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	SPI1_LowSpeed(void)
{
  SPI_InitTypeDef  SPI_InitStructure;

  SPI_Cmd(SPI1, DISABLE);
  /* SPI1 configuration */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;				  	
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;			  	
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;				  		
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; 
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				  
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);
  
  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);  
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	SPI1_Enable(void)
{
	SPI1_BusyFlag = 0;
	
	SPI1_GPIO_Config();
	SPI1_SPI_Config();
	
	SPI1_DMA_TX_Config(0, 0);
	SPI1_DMA_RX_Config(0, 0);
	SPI1_DMA_Rx_NVIC_Config();
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
//	DMA_Cmd(SPI1_DMA_TX_CHANNEL, ENABLE);
//	DMA_Cmd(SPI1_DMA_RX_CHANNEL, ENABLE);

//	DMA_ITConfig(SPI1_DMA_RX_CHANNEL, DMA_IT_TC, ENABLE);	
	
	//SPI1 enable  
	SPI_Cmd(SPI1, ENABLE); 	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	SPI1_WriteByte(u8 dat)
{
	//Wait until the transmit buffer is empty 
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET){;} 
	//Send the byte 
	SPI_I2S_SendData(SPI1, dat); 	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
u8		SPI1_ReadByte(void)
{
	u8 dat;
   //Wait until a data is received 
   while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET){;} 
   //Get the received data 
   dat = SPI_I2S_ReceiveData(SPI1);
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
u8		SPI1_ReadWriteByte(u8 dat)
{
	u8 temp;
	//Wait until the transmit buffer is empty 
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET){;} 
	//Send the byte 
	SPI_I2S_SendData(SPI1, dat); 
	//Wait until a data is received 
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET){;} 
	//Get the received data 
	temp = SPI_I2S_ReceiveData(SPI1);
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
void	SPI1_ReadWriteMore(u8 *txbuff, u8 *rxbuff, u8 count)
{
	u8 i;
	//Wait until the transmit buffer is empty 
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET){;} 
	SPI_I2S_SendData(SPI1, txbuff[0]);
	
	for(i=0; i<(count-1); i++)
	{
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET){;}
		SPI_I2S_SendData(SPI1, txbuff[1+i]);
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET){;}
		rxbuff [i] = SPI_I2S_ReceiveData(SPI1);		
	}
	
	//Wait until a data is received 	
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET){;}	
	rxbuff [count-1] = SPI_I2S_ReceiveData(SPI1);	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
u8		SPI1_GetBusyFlag(void)
{
	return SPI1_BusyFlag;
}	



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	SPI1_SetBusyFlag(void)
{
	SPI1_BusyFlag = 1;
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	SPI1_SendDataDelegate(u8 *buff, u8 count)
{	
	SPI1_SetBusyFlag();
	SPI_Cmd(SPI1, DISABLE);	
	DMA_Cmd(SPI1_DMA_TX_CHANNEL, DISABLE);
	DMA_Cmd(SPI1_DMA_RX_CHANNEL, DISABLE);
	DMA_ITConfig(SPI1_DMA_RX_CHANNEL, DMA_IT_TC, DISABLE);

	SPI1_DMA_Rx_NVIC_Config();
			
	SPI1_DMA_TX_Config(buff, count);
	SPI1_DMA_RX_Config(SPI1_RxBuff, count);

	DMA_ITConfig(SPI1_DMA_RX_CHANNEL, DMA_IT_TC, ENABLE);	
	DMA_Cmd(SPI1_DMA_TX_CHANNEL, ENABLE);
	DMA_Cmd(SPI1_DMA_RX_CHANNEL, ENABLE);
	SPI_Cmd(SPI1, ENABLE);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	SPI1_DMA_RX_IRQHandler(void)
{
	/* Check if the DMA transfer is complete */
	if(DMA_GetITStatus(SPI1_DMA_RX_FLAG) != RESET)
	{         
		//SPI1_StopDMA();
		DMA_ClearITPendingBit(SPI1_DMA_RX_FLAG);
		DMA_Cmd(SPI1_DMA_TX_CHANNEL, DISABLE);
		DMA_Cmd(SPI1_DMA_RX_CHANNEL, DISABLE);
		DMA_ITConfig(SPI1_DMA_RX_CHANNEL, DMA_IT_TC, DISABLE);
		SPI1_BusyFlag = 0;		
	}
}




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
