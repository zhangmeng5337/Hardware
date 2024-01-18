
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : vs1053b.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : vs1053b program body
* Description    : --
**********************************************************************/

/* Includes  == 文件包含 --------------------------------------------*/
#include "vs1053b.h"
#include "microsec.h"
#include "spi1.h"
#include "ff.h"
#include "diskio.h"
#include <string.h>
#include "playsong.h"
#include "vs1053b_flac.h"
#include "display.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//
#define	D_VS1053B_CS_GPIO			GPIOA	
#define	D_VS1053B_CS_PIN			GPIO_Pin_4	
#define	D_VS1053B_CS_CLK			RCC_APB2Periph_GPIOA

#define	D_VS1053B_DREQ_GPIO		GPIOC
#define	D_VS1053B_DREQ_PIN		GPIO_Pin_5	
#define	D_VS1053B_DREQ_CLK		RCC_APB2Periph_GPIOC

#define	D_VS1053B_DCS_GPIO		GPIOB
#define	D_VS1053B_DCS_PIN			GPIO_Pin_0	
#define	D_VS1053B_DCS_CLK			RCC_APB2Periph_GPIOB

#define	D_VS1053B_RESET_GPIO		GPIOC	
#define	D_VS1053B_RESET_PIN		GPIO_Pin_4	
#define	D_VS1053B_RESET_CLK		RCC_APB2Periph_GPIOC



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/

	
	
/* Private functions  ==  私有函数定义 ------------------------------*/

/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void VS1053B_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

   //使能VS1053B相关GPIO口时钟
	RCC_APB2PeriphClockCmd(D_VS1053B_CS_CLK | D_VS1053B_DREQ_CLK | D_VS1053B_DCS_CLK | D_VS1053B_RESET_CLK, ENABLE);

	//配置VS1053B的输出引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
		
	GPIO_InitStructure.GPIO_Pin = D_VS1053B_CS_PIN;
	GPIO_Init(D_VS1053B_CS_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = D_VS1053B_DCS_PIN;
	GPIO_Init(D_VS1053B_DCS_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = D_VS1053B_RESET_PIN;
	GPIO_Init(D_VS1053B_RESET_GPIO, &GPIO_InitStructure); 
	
	//配置VS1053B的输入引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
   
	GPIO_InitStructure.GPIO_Pin = D_VS1053B_DREQ_PIN;
	GPIO_Init(D_VS1053B_DREQ_GPIO, &GPIO_InitStructure);
	// 将DREQ设置为1，初始化为输入引脚
	GPIO_SetBits(D_VS1053B_DREQ_GPIO, D_VS1053B_DREQ_PIN); 	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void VS1053B_CS_High(void)
{
	GPIO_SetBits(D_VS1053B_CS_GPIO, D_VS1053B_CS_PIN);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void VS1053B_CS_Low(void)
{
	GPIO_ResetBits(D_VS1053B_CS_GPIO, D_VS1053B_CS_PIN);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void VS1053B_DCS_High(void)
{
	GPIO_SetBits(D_VS1053B_DCS_GPIO, D_VS1053B_DCS_PIN);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void VS1053B_DCS_Low(void)
{
	GPIO_ResetBits(D_VS1053B_DCS_GPIO, D_VS1053B_DCS_PIN);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void VS1053B_RESET_High(void)
{
	GPIO_SetBits(D_VS1053B_RESET_GPIO, D_VS1053B_RESET_PIN);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void VS1053B_RESET_Low(void)
{
	GPIO_ResetBits(D_VS1053B_RESET_GPIO, D_VS1053B_RESET_PIN);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
u8 VS1053B_DREQ_State(void)
{
	u8 flag;
	flag = GPIO_ReadInputDataBit(D_VS1053B_DREQ_GPIO, D_VS1053B_DREQ_PIN);	   
	return flag;
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
u8 VS1053B_ReadWriteByte(u8 dat)
{
	return SPI1_ReadWriteByte(dat);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void VS1053B_WriteReg(u8 reg, u16 dat)
{
   VS1053B_DCS_High();		               	//xDCS = 1
   VS1053B_CS_Low();		               		//xCS = 0
   VS1053B_ReadWriteByte(VS1053B_WRITE_COMMAND); 	//发送写命令
   VS1053B_ReadWriteByte(reg);           		//发送8位地址
   VS1053B_ReadWriteByte((u8)(dat >> 8)); 	//发送16位数据的高8位
   VS1053B_ReadWriteByte((u8)dat);     		//发送16位数据的低8位
   VS1053B_CS_High();                  		//xCS = 1
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
u16	VS1053B_ReadReg(u8 reg)
{
   u16 dat;
   u8 high, low;

   VS1053B_DCS_High();		               	//xDCS = 1
   VS1053B_CS_Low();		               		//xCS = 0
   VS1053B_ReadWriteByte(VS1053B_READ_COMMAND);  	//发送写命令
   VS1053B_ReadWriteByte(reg);             	//发送8位地址
   high = VS1053B_ReadWriteByte(0x00);      	//发送16位数据的高8位
   low  = VS1053B_ReadWriteByte(0x00);      	//发送16位数据的低8位
   VS1053B_CS_High();                  		//xCS = 1

   dat  = (high<<8) + low;
   return dat;
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void VS1053B_WriteMemory(u16 addr, u16 dat)
{
	VS1053B_WriteReg(VS1053B_WRAMADDR, addr);     
	while(VS1053B_DREQ_State() == 0){;}

	VS1053B_WriteReg(VS1053B_WRAM, dat);     
	while(VS1053B_DREQ_State() == 0){;}	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
u16	VS1053B_ReadMemory(u16 addr)
{
	u16 temp;
	VS1053B_WriteReg(VS1053B_WRAMADDR, addr);     
	while(VS1053B_DREQ_State() == 0){;}

	temp = VS1053B_ReadReg(VS1053B_WRAM);     
	while(VS1053B_DREQ_State() == 0){;}	
	
	return temp;
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void VS1053B_Config(void)
{
	VS1053B_GPIO_Config();
	SPI1_Enable();
	VS1053B_HardReset();             
	VS1053B_SofeReset();					

	VS1053B_WriteReg(VS1053B_MODE, 0x0800);     
	while(VS1053B_DREQ_State() == 0){;}	

	VS1053B_WriteReg(VS1053B_CLOCKF, 0xf800);	// 设置vs1003的时钟,(3+2)倍频
	while(VS1053B_DREQ_State() == 0){;}	

	VS1053B_WriteReg(VS1053B_VOL, 0x1515);
	while(VS1053B_DREQ_State() == 0){;}

	VS1053B_WriteReg(VS1053B_BASS, 0x0066);     
	while(VS1053B_DREQ_State() == 0){;}	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void VS1053B_SofeReset(void)
{
	VS1053B_WriteReg(0, 0x0804);     
	while(VS1053B_DREQ_State() == 0){;}

	VS1053B_DCS_Low();		                  //xDCS = 1
	VS1053B_CS_High();		                  //xCS = 0
	VS1053B_ReadWriteByte(0);
	VS1053B_ReadWriteByte(0);
	VS1053B_ReadWriteByte(0);
	VS1053B_ReadWriteByte(0);
	VS1053B_DCS_High();		                  //xDCS = 1
	while(VS1053B_DREQ_State() == 0){;}
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void VS1053B_HardReset(void)
{
	VS1053B_RESET_Low();             //xReset = 0   复位vs1003
	MICROSEC_Delay_ms(100);
	VS1053B_ReadWriteByte(0xff);
	VS1053B_CS_High();              	//xCS = 1
	VS1053B_DCS_High();            	//xDCS = 1
	VS1053B_RESET_High();           	//xRESET = 1
	MICROSEC_Delay_ms(100); 
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void VS1053B_SineTest(void)
{
	VS1053B_WriteReg(VS1053B_MODE,0x0820);//进入vs1003的测试模式
	while(VS1053B_DREQ_State() == 0){;}

	VS1053B_DCS_Low();       //     //xDCS = 0，选择vs1003的数据接口
	//向vs1003发送正弦测试命令：0x53 0xef 0x6e n 0x00 0x00 0x00 0x00
	//其中n = 0x24, 设定vs1003所产生的正弦波的频率值，具体计算方法见vs1003的datasheet
	VS1053B_ReadWriteByte(0x53);      
	VS1053B_ReadWriteByte(0xef);      
	VS1053B_ReadWriteByte(0x6e);      
	VS1053B_ReadWriteByte(0x24);      
	VS1053B_ReadWriteByte(0x00);      
	VS1053B_ReadWriteByte(0x00);
	VS1053B_ReadWriteByte(0x00);
	VS1053B_ReadWriteByte(0x00);
	//	OSTimeDly(OS_TICKS_PER_SEC/2);
	MICROSEC_Delay_ms(100);
	VS1053B_DCS_High();    //程序执行到这里后应该能从个单一频率的声音

	//退出正弦测试				 
	VS1053B_DCS_Low();    
	VS1053B_ReadWriteByte(0x45);
	VS1053B_ReadWriteByte(0x78);
	VS1053B_ReadWriteByte(0x69);
	VS1053B_ReadWriteByte(0x74);
	VS1053B_ReadWriteByte(0x00);
	VS1053B_ReadWriteByte(0x00);
	VS1053B_ReadWriteByte(0x00);
	VS1053B_ReadWriteByte(0x00);
	//	OSTimeDly(OS_TICKS_PER_SEC/2);
	MICROSEC_Delay_ms(100);
	VS1053B_DCS_High();     
	//再次进入正弦测试并设置n值为0x44，即将正弦波的频率设置为另外的值
	VS1053B_DCS_Low();   
	VS1053B_ReadWriteByte(0x53);      
	VS1053B_ReadWriteByte(0xef);      
	VS1053B_ReadWriteByte(0x6e);      
	VS1053B_ReadWriteByte(0x44);      
	VS1053B_ReadWriteByte(0x00);      
	VS1053B_ReadWriteByte(0x00);
	VS1053B_ReadWriteByte(0x00);
	VS1053B_ReadWriteByte(0x00);
	//	OSTimeDly(OS_TICKS_PER_SEC/2);
	MICROSEC_Delay_ms(100);
	VS1053B_DCS_High();  

	//退出正弦测试
	VS1053B_DCS_Low();    
	VS1053B_ReadWriteByte(0x45);
	VS1053B_ReadWriteByte(0x78);
	VS1053B_ReadWriteByte(0x69);
	VS1053B_ReadWriteByte(0x74);
	VS1053B_ReadWriteByte(0x00);
	VS1053B_ReadWriteByte(0x00);
	VS1053B_ReadWriteByte(0x00);
	VS1053B_ReadWriteByte(0x00);
	//	OSTimeDly(OS_TICKS_PER_SEC/2);
	MICROSEC_Delay_ms(100);
	VS1053B_DCS_High(); 
}
	
	
/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void MP3_Play1(u8 *path)
{ 
	FRESULT res;                                      	// FatFs function common result code
	FATFS fs;                                        	// Work area (file system object) for logical drive
	FIL fil;                                         	// file objects
	UINT br;			                                  	// File R count  
	
	u16 i;
	u16 count;
	u8 buffer[512];          // file copy buffer
	
	VS1053B_FLAC_WritePlugin();
   SPI1_HighSpeed(); 
	f_mount(0, &fs);			                          /* 挂载文件系统到0区 */
	res = f_open(&fil, (const TCHAR *)path, FA_OPEN_EXISTING | FA_READ); /* 以只读方式打开 */					
//   res = f_lseek(&fil, 1024*3000);

	VS1053B_DCS_Low();       /* 选择VS1003的数据接口	*/	
	/* ------------------- 一曲开始 --------------------*/
	while(1)
	{			
		res = f_read(&fil, buffer, 512, &br);
		if (res == FR_OK)
		{				
			count = 0;								/* 512字节完重新计数 */
			while (count < 512)		  /* SD卡读取一个sector，一个sector为512字节 */	
			{ 						
				if(VS1053B_DREQ_State() != 0)	      /* 等待DREQ为高，请求数据输入 */
				{
					for (i=0; i<32; i++) /* VS1003的FIFO只有32个字节的缓冲 */
					{										
						VS1053B_ReadWriteByte(buffer[count]);										
						count++;
					}
				}		    	      	
			}
		}
		if (res || br == 0) break;   /* 出错或者到了文件尾 */		   
	}
	/* 根据VS1003的要求，在一曲结束后需发送2048个0来确保下一首的正常播放 */
	count = 0;
	while (count < 2048)	
	{	
		if (VS1053B_DREQ_State() != 0)
		{
			for (i=0; i<32; i++)
			{					   			
				VS1053B_ReadWriteByte(0x00);								
				count++;
			}
		}
	}
	VS1053B_DCS_High();   /* 关闭VS1003数据端口 */ 
	f_close(&fil);	 /* 关闭打开的文件 */  		                       
} 



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
u8 spitxbuffer[1024];          // file copy buffer
u8	spirxbuff[32];
void	MUSIC_Play1(u8 *path)
{ 
	FRESULT res;                                      	// FatFs function common result code
	FATFS fs;                                        	// Work area (file system object) for logical drive
	FIL fil;                                         	// file objects
	UINT br;			                                  	// File R count  
	
	u16 i;
	u16 count;
	u16 temp;
	
//	SPI1_HighSpeed();
	f_mount(0, &fs);			                          /* 挂载文件系统到0区 */
	res = f_open(&fil, (const TCHAR *)path, FA_OPEN_EXISTING | FA_READ); /* 以只读方式打开 */					
						
	VS1053B_DCS_Low();       /* 选择VS1003的数据接口	*/
	/* ------------------- 一曲开始 --------------------*/
	while(1)
	{					
		res = f_read(&fil, spitxbuffer, 1024, &br);
		if (res == FR_OK)
		{				
			count = 0;								/* 512字节完重新计数 */
			while (count < 1024)		  /* SD卡读取一个sector，一个sector为512字节 */	
			{ 						
				if(VS1053B_DREQ_State() != 0)	      /* 等待DREQ为高，请求数据输入 */
				{
//					for (i=0; i<32; i++) /* VS1003的FIFO只有32个字节的缓冲 */
//					{										
//						VS1053B_ReadWriteByte(buffer[count]);										
//						count++;
//					}
//					DMA_Cmd(SPI1_DMA_TX_CHANNEL, DISABLE);
//					SPI1_DMA_TX_Config(spitxbuffer + count, 32);
//					DMA_Cmd(SPI1_DMA_TX_CHANNEL, ENABLE);
//					count = count + 32;
//					while(DMA_GetFlagStatus(SPI1_DMA_TX_FLAG) == RESET) {;}
						
//					while(SPI1_GetBusyFlag() == 1) {;}
					
					SPI1_SetBusyFlag();
					SPI_Cmd(SPI1, DISABLE);	
					DMA_Cmd(SPI1_DMA_TX_CHANNEL, DISABLE);
					DMA_Cmd(SPI1_DMA_RX_CHANNEL, DISABLE);
					DMA_ITConfig(SPI1_DMA_RX_CHANNEL, DMA_IT_TC, DISABLE);
						
					SPI1_DMA_Rx_NVIC_Config();
									
					SPI1_DMA_TX_Config(spitxbuffer + count, 32);
					SPI1_DMA_RX_Config(spirxbuff, 32);
					
					DMA_ITConfig(SPI1_DMA_RX_CHANNEL, DMA_IT_TC, ENABLE);	
					DMA_Cmd(SPI1_DMA_TX_CHANNEL, ENABLE);
					DMA_Cmd(SPI1_DMA_RX_CHANNEL, ENABLE);
					SPI_Cmd(SPI1, ENABLE);	
					count = count + 32;
					while(SPI1_GetBusyFlag() == 1) {;}
//					while(VS1053B_DREQ_State() == 0){;}	
//					temp = VS1053B_ReadReg(0x04);
//					while(VS1053B_DREQ_State() == 0){;}
//					if(temp != 0x0000)
//					{
//						temp = 0;
//					}
				}		    	      	
			}
		}
		if (res || br == 0) break;   /* 出错或者到了文件尾 */		   
	}
	
	/* 根据VS1003的要求，在一曲结束后需发送2048个0来确保下一首的正常播放 */
	count = 0;
	while (count < 2048)	
	{	
		if (VS1053B_DREQ_State() != 0)
		{
			for (i=0; i<32; i++)
			{					   			
				VS1053B_ReadWriteByte(0x00);								
				count++;
			}
		}
	}
	VS1053B_DCS_High();   /* 关闭VS1003数据端口 */ 
	f_close(&fil);	 /* 关闭打开的文件 */  		                       
} 



/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
