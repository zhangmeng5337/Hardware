/***************************************************************
       
    程 序 名：USB Mass Storage例程
    公    司：
    作    者：liu_xf
	版 本 号：V1.00
    建立日期：2009-5-18
    修改日期： 无  
    编译环境： RealView MDK-ARM  Version: 3.40
    硬件平台： STM32F103ZET6 + K9F1G08U0A + CH375 + LCM12864
    功能描述： 
    
               STM32+K9F1G08大页NAND+SD卡实现U盘

	注：仅供学习交流之用，请不要用作商业用途

***************************************************************/


#include "main.h"
#include "sdcard.h"
#include "fsmc_nand.h"  
#include <stdio.h>


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE         512    //数据缓存区大小为512个字节
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static vu32 TimingDelay = 0; 
u8 TxBuffer[BUFFER_SIZE], RxBuffer[BUFFER_SIZE];  //发送和接收数据缓冲区
vu32 WriteReadStatus = 0;
u32 j = 0;
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern void usart1(void);
extern void Usart1_SendData(u8 *DatBuf, u32 len);
void Fill_Buffer(u8 *pBuffer, u16 BufferLenght, u32 Offset);
void DelayXms(unsigned int x);
/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
  u8  i;
 // u8  ID_Code[5]={0,0,0,0,0};
  System_Init();
  usart1();       //串口初始化
   printf("  初始化完成，系统开始运行..... \r\n");
  /* Configure the systick */    
  SysTick_Config();
  /* Initialize the LCD */
  STM3210E_LCD_Init();
  /* Clear the LCD */ 
  LCD_Clear(White);
  LCD_SetBackColor(Blue);
  /* Set the LCD Text Color */
  LCD_SetTextColor(White); 

  LCD_DisplayStringLine(Line0, "   EM_STM32E Demo   ");
  LCD_DisplayStringLine(Line1, " Mass_Storage_Test  ");
  LCD_DisplayStringLine(Line2, " welcome to Embest  ");
  LCD_DisplayStringLine(Line3, " www.embedinfo.com  ");
  LCD_SetBackColor(White);                        // Set the Back Color */
  LCD_SetTextColor(Blue);                         // Set the Text Color */

   //读FLASH ID --------------------------
/*
  FlashReadId(ID_Code);

  //flash测试--------------------------------------------------
  //NAND_Format();  //格式化磁盘
 // LCD_DisplayStringLine(Line5, " NAND_Format......  ");
  Fill_Buffer(TxBuffer, BUFFER_SIZE , 0x00);  //给TxBuffer填充值
 
//   FlashWriteOneSector(0, TxBuffer, BUFFER_SIZE);
   LCD_DisplayStringLine(Line6, " NAND_Write......  ");
  FlashReadOneSector(0, RxBuffer, BUFFER_SIZE);
  LCD_DisplayStringLine(Line7, " NAND_Read......  ");
  //串口发送数
   Usart1_SendData(RxBuffer,BUFFER_SIZE);	 //发送数据

  // Verify the written data 
    for(j = 0; j < BUFFER_SIZE; j++)
    {
      if(TxBuffer[j] != RxBuffer[j])
      {     
        WriteReadStatus++;
      } 
    }

    if (WriteReadStatus == 0)
    {
    
      GPIO_SetBits(GPIOF, GPIO_Pin_6);
	  LCD_DisplayStringLine(Line8, "nand flash test ok");

    }
    else
    { 
      
      GPIO_SetBits(GPIOF, GPIO_Pin_7); 
	  LCD_DisplayStringLine(Line8, "nand flash error");   
    }

  //--------------------------------------------------------
 */
  while (bDeviceState != CONFIGURED);	  //等待USB枚举成功
  LCD_DisplayStringLine(Line5, "   Mass_Storage OK  ");
 // LCD_DisplayStringLine(Line6, "Check Storage On PC ");

  USB_Configured_LED();
  DelayXms(2000);    //等待SD卡准备
 /*
  //sd 测试------------------------------------------------
  WriteReadStatus=0;
  SD_WriteBlock(5120, (u32 *)TxBuffer, BUFFER_SIZE);
  LCD_DisplayStringLine(Line6, " SD_Write......  ");
  DelayXms(2000);  
  SD_ReadBlock(5120, (u32 *)RxBuffer, BUFFER_SIZE);
  LCD_DisplayStringLine(Line7, " SD_Read......  ");
   //串口发送数
 //  Usart1_SendData(RxBuffer,BUFFER_SIZE);	 //发送数据
  // Verify the written data 
    for(j = 0; j < BUFFER_SIZE; j++)
    {
      if(TxBuffer[j] != RxBuffer[j])
      {     
        WriteReadStatus++;
      } 
    }

    if (WriteReadStatus == 0)
    {
    
      GPIO_SetBits(GPIOF, GPIO_Pin_6);
	  LCD_DisplayStringLine(Line8, "SD   Card  test  ok");

    }
    else
    { 
      
      GPIO_SetBits(GPIOF, GPIO_Pin_7); 
	  LCD_DisplayStringLine(Line8, "SD Card test error");   
    }
*/
  //--------------------------------------------------------
/* 
  LCD_DisplayStringLine(Line6, " SD_Read......  ");
  for (i=1;i<256;i++)
  {
    SD_ReadBlock(i*512, (u32 *)RxBuffer, BUFFER_SIZE);
   // LCD_DisplayStringLine(Line7, " SD_Read......  ");
   // DelayXms(1000);
    //串口发送数
    Usart1_SendData(RxBuffer,BUFFER_SIZE);	 //发送数据
   }
   LCD_DisplayStringLine(Line7, " Send OK......  ");
   LCD_DisplayStringLine(Line8, "                   ");
 */  
  while (1)
  {}
}


/*******************************************************************************
* Function name : Fill_Buffer
* Description   : Fill the buffer
* Input         : - pBuffer: pointer on the Buffer to fill
*                 - BufferSize: size of the buffer to fill
*                 - Offset: first value to fill on the Buffer
* Output param  : None
*******************************************************************************/
void Fill_Buffer(u8 *pBuffer, u16 BufferLenght, u32 Offset)
{
  u16 IndexTmp = 0;

  /* Put in global buffer same values */
  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }
}


/*****************   延时x毫秒函数 ***********/
void DelayXms(unsigned int x)               
{
 unsigned int i,j;
 for(i=0;i<x;i++)
 for(j=0;j<5150;j++);
}

/*******************************************************************************
* Function Name  : SysTick_Config
* Description    : Configure a SysTick Base time to 10 ms.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
 void SysTick_Config(void)
{
  /* Configure HCLK clock as SysTick clock source */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	//设置SysTick时钟为AHB时钟
 
  /* SysTick interrupt each 100 Hz with HCLK equal to 72MHz */
  SysTick_SetReload(720000);   //设置重装值

  /* Enable the SysTick Interrupt */
  SysTick_ITConfig(ENABLE);
}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length (time base 10 ms).
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(u32 nCount)
{
  TimingDelay = nCount;

  /* Enable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Enable);
  
  while(TimingDelay != 0)
  {
  }

  /* Disable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Disable);

  /* Clear the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Clear);
}

/*******************************************************************************
* Function Name  : Decrement_TimingDelay
* Description    : Decrements the TimingDelay variable.
* Input          : None
* Output         : TimingDelay
* Return         : None
*******************************************************************************/
void Decrement_TimingDelay(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
