
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : vs1053b.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : vs1053b program body
* Description    : --
**********************************************************************/

/* Includes  == �ļ����� --------------------------------------------*/
#include "vs1053b.h"
#include "microsec.h"
#include "spi1.h"
#include "ff.h"
#include "diskio.h"
#include <string.h>
#include "playsong.h"
#include "vs1053b_flac.h"
#include "display.h"



/* Private typedef    ==  ˽������ ----------------------------------*/
/* Private define     ==  ˽�ж��� ----------------------------------*/

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



/* Private macro      ==  ˽�к� ------------------------------------*/
/* Private variables  ==  ˽�б��� ----------------------------------*/

	
	
/* Private functions  ==  ˽�к������� ------------------------------*/

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

   //ʹ��VS1053B���GPIO��ʱ��
	RCC_APB2PeriphClockCmd(D_VS1053B_CS_CLK | D_VS1053B_DREQ_CLK | D_VS1053B_DCS_CLK | D_VS1053B_RESET_CLK, ENABLE);

	//����VS1053B���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
		
	GPIO_InitStructure.GPIO_Pin = D_VS1053B_CS_PIN;
	GPIO_Init(D_VS1053B_CS_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = D_VS1053B_DCS_PIN;
	GPIO_Init(D_VS1053B_DCS_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = D_VS1053B_RESET_PIN;
	GPIO_Init(D_VS1053B_RESET_GPIO, &GPIO_InitStructure); 
	
	//����VS1053B����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
   
	GPIO_InitStructure.GPIO_Pin = D_VS1053B_DREQ_PIN;
	GPIO_Init(D_VS1053B_DREQ_GPIO, &GPIO_InitStructure);
	// ��DREQ����Ϊ1����ʼ��Ϊ��������
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
   VS1053B_ReadWriteByte(VS1053B_WRITE_COMMAND); 	//����д����
   VS1053B_ReadWriteByte(reg);           		//����8λ��ַ
   VS1053B_ReadWriteByte((u8)(dat >> 8)); 	//����16λ���ݵĸ�8λ
   VS1053B_ReadWriteByte((u8)dat);     		//����16λ���ݵĵ�8λ
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
   VS1053B_ReadWriteByte(VS1053B_READ_COMMAND);  	//����д����
   VS1053B_ReadWriteByte(reg);             	//����8λ��ַ
   high = VS1053B_ReadWriteByte(0x00);      	//����16λ���ݵĸ�8λ
   low  = VS1053B_ReadWriteByte(0x00);      	//����16λ���ݵĵ�8λ
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

	VS1053B_WriteReg(VS1053B_CLOCKF, 0xf800);	// ����vs1003��ʱ��,(3+2)��Ƶ
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
	VS1053B_RESET_Low();             //xReset = 0   ��λvs1003
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
	VS1053B_WriteReg(VS1053B_MODE,0x0820);//����vs1003�Ĳ���ģʽ
	while(VS1053B_DREQ_State() == 0){;}

	VS1053B_DCS_Low();       //     //xDCS = 0��ѡ��vs1003�����ݽӿ�
	//��vs1003�������Ҳ������0x53 0xef 0x6e n 0x00 0x00 0x00 0x00
	//����n = 0x24, �趨vs1003�����������Ҳ���Ƶ��ֵ��������㷽����vs1003��datasheet
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
	VS1053B_DCS_High();    //����ִ�е������Ӧ���ܴӸ���һƵ�ʵ�����

	//�˳����Ҳ���				 
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
	//�ٴν������Ҳ��Բ�����nֵΪ0x44���������Ҳ���Ƶ������Ϊ�����ֵ
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

	//�˳����Ҳ���
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
	f_mount(0, &fs);			                          /* �����ļ�ϵͳ��0�� */
	res = f_open(&fil, (const TCHAR *)path, FA_OPEN_EXISTING | FA_READ); /* ��ֻ����ʽ�� */					
//   res = f_lseek(&fil, 1024*3000);

	VS1053B_DCS_Low();       /* ѡ��VS1003�����ݽӿ�	*/	
	/* ------------------- һ����ʼ --------------------*/
	while(1)
	{			
		res = f_read(&fil, buffer, 512, &br);
		if (res == FR_OK)
		{				
			count = 0;								/* 512�ֽ������¼��� */
			while (count < 512)		  /* SD����ȡһ��sector��һ��sectorΪ512�ֽ� */	
			{ 						
				if(VS1053B_DREQ_State() != 0)	      /* �ȴ�DREQΪ�ߣ������������� */
				{
					for (i=0; i<32; i++) /* VS1003��FIFOֻ��32���ֽڵĻ��� */
					{										
						VS1053B_ReadWriteByte(buffer[count]);										
						count++;
					}
				}		    	      	
			}
		}
		if (res || br == 0) break;   /* ������ߵ����ļ�β */		   
	}
	/* ����VS1003��Ҫ����һ���������跢��2048��0��ȷ����һ�׵��������� */
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
	VS1053B_DCS_High();   /* �ر�VS1003���ݶ˿� */ 
	f_close(&fil);	 /* �رմ򿪵��ļ� */  		                       
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
	f_mount(0, &fs);			                          /* �����ļ�ϵͳ��0�� */
	res = f_open(&fil, (const TCHAR *)path, FA_OPEN_EXISTING | FA_READ); /* ��ֻ����ʽ�� */					
						
	VS1053B_DCS_Low();       /* ѡ��VS1003�����ݽӿ�	*/
	/* ------------------- һ����ʼ --------------------*/
	while(1)
	{					
		res = f_read(&fil, spitxbuffer, 1024, &br);
		if (res == FR_OK)
		{				
			count = 0;								/* 512�ֽ������¼��� */
			while (count < 1024)		  /* SD����ȡһ��sector��һ��sectorΪ512�ֽ� */	
			{ 						
				if(VS1053B_DREQ_State() != 0)	      /* �ȴ�DREQΪ�ߣ������������� */
				{
//					for (i=0; i<32; i++) /* VS1003��FIFOֻ��32���ֽڵĻ��� */
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
		if (res || br == 0) break;   /* ������ߵ����ļ�β */		   
	}
	
	/* ����VS1003��Ҫ����һ���������跢��2048��0��ȷ����һ�׵��������� */
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
	VS1053B_DCS_High();   /* �ر�VS1003���ݶ˿� */ 
	f_close(&fil);	 /* �رմ򿪵��ļ� */  		                       
} 



/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
