
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : oled.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : oled program body
* Description    : --
**********************************************************************/

/* Includes  == 文件包含 --------------------------------------------*/
#include "oled.h"
#include "spi2.h"
#include "microsec.h"
//#include "fixedsys_16x8.h"
#include "songti_32x16.h"
#include "basic.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//
#define	D_OLED_CS_GPIO			GPIOB	
#define	D_OLED_CS_PIN			GPIO_Pin_12	
#define	D_OLED_CS_CLK			RCC_APB2Periph_GPIOB

#define	D_OLED_RST_GPIO		GPIOC	
#define	D_OLED_RST_PIN			GPIO_Pin_6	
#define	D_OLED_RST_CLK			RCC_APB2Periph_GPIOC

#define	D_OLED_DC_GPIO			GPIOC	
#define	D_OLED_DC_PIN			GPIO_Pin_7	
#define	D_OLED_DC_CLK			RCC_APB2Periph_GPIOC

 /*--定义汉字字体的大小--*/
#define   FSize_L          32      //32*32大字体
#define   FSize_M          24	   //24*24中字体
#define   FSize_S          16	   //16*16小字体

#define   OLED_DISPLAYTIME		200



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/
u8		OLED_DisplayBuff[8][128]={0};

	
	
/* Private functions  ==  私有函数定义 ------------------------------*/

/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

   //使能OLED相关GPIO口时钟
	RCC_APB2PeriphClockCmd(D_OLED_CS_CLK | D_OLED_RST_CLK | D_OLED_DC_CLK, ENABLE);

	//配置OLED的输出引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
		
	GPIO_InitStructure.GPIO_Pin = D_OLED_CS_PIN;
	GPIO_Init(D_OLED_CS_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = D_OLED_RST_PIN;
	GPIO_Init(D_OLED_RST_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = D_OLED_DC_PIN;
	GPIO_Init(D_OLED_DC_GPIO, &GPIO_InitStructure); 
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_CS_High(void)
{
	GPIO_SetBits(D_OLED_CS_GPIO, D_OLED_CS_PIN);
}


	
/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_CS_Low(void)
{
	GPIO_ResetBits(D_OLED_CS_GPIO, D_OLED_CS_PIN);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_RST_High(void)
{
	GPIO_SetBits(D_OLED_RST_GPIO, D_OLED_RST_PIN);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_RST_Low(void)
{
	GPIO_ResetBits(D_OLED_RST_GPIO, D_OLED_RST_PIN);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DC_High(void)
{
	GPIO_SetBits(D_OLED_DC_GPIO, D_OLED_DC_PIN);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DC_Low(void)
{
	GPIO_ResetBits(D_OLED_DC_GPIO, D_OLED_DC_PIN);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_WriteData(u8 dat)
{
	OLED_DC_High();
//	OLED_CS_Low();
	SPI2_WriteByte(dat);
//	OLED_CS_High();
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_WriteCommand(u8 cmd)
{
	OLED_DC_Low();
//	OLED_CS_Low();
	SPI2_WriteByte(cmd);
//	OLED_CS_High();	
}



/*******************************************************************************************
* Function Name  : void LED_SetPos(u8 ucIdxX, u8 ucIdxY)
* Description    : 向OLED写命令函数
* Input          : ucCmd ：要写入的8位命令
* Output         : 无
* Return         : 无
*******************************************************************************************/
void OLED_SetStartColumn(u8 ucData)
{
    OLED_WriteCommand(0x00+ucData % 16);   // Set Lower Column Start Address for Page Addressing Mode
                                   // Default => 0x00
    OLED_WriteCommand(0x10+ucData / 16);   // Set Higher Column Start Address for Page Addressing Mode
                                   // Default => 0x10
}

void OLED_SetAddressingMode(u8 ucData)
{
    OLED_WriteCommand(0x20);        // Set Memory Addressing Mode
    OLED_WriteCommand(ucData);      // Default => 0x02
                            // 0x00 => Horizontal Addressing Mode
                            // 0x01 => Vertical Addressing Mode
                            // 0x02 => Page Addressing Mode
}

void OLED_SetColumnAddress(u8 a, u8 b)
{
    OLED_WriteCommand(0x21);        // Set Column Address
    OLED_WriteCommand(a);           // Default => 0x00 (Column Start Address)
    OLED_WriteCommand(b);           // Default => 0x7F (Column End Address)
}

void OLED_SetPageAddress(u8 a, u8 b)
{
    OLED_WriteCommand(0x22);        // Set Page Address
    OLED_WriteCommand(a);           // Default => 0x00 (Page Start Address)
    OLED_WriteCommand(b);           // Default => 0x07 (Page End Address)
}

void OLED_SetStartLine(u8 ucData)
{
    OLED_WriteCommand(0x40|ucData); // Set Display Start Line
                            // Default => 0x40 (0x00)
}

void OLED_SetContrastControl(u8 ucData)
{
    OLED_WriteCommand(0x81);        // Set Contrast Control
    OLED_WriteCommand(ucData);      // Default => 0x7F
}

void OLED_SetChargePump(u8 ucData)
{
    OLED_WriteCommand(0x8D);        // Set Charge Pump
    OLED_WriteCommand(0x10|ucData); // Default => 0x10
                            // 0x10 (0x00) => Disable Charge Pump
                            // 0x14 (0x04) => Enable Charge Pump
}

void OLED_SetSegmentRemap(u8 ucData)
{
    OLED_WriteCommand(0xA0|ucData); // Set Segment Re-Map
                            // Default => 0xA0
                            // 0xA0 (0x00) => Column Address 0 Mapped to SEG0
                            // 0xA1 (0x01) => Column Address 0 Mapped to SEG127
}

void OLED_SetEntireDisplay(u8 ucData)
{
    OLED_WriteCommand(0xA4|ucData); // Set Entire Display On / Off
                            // Default => 0xA4
                            // 0xA4 (0x00) => Normal Display
                            // 0xA5 (0x01) => Entire Display On
}

void OLED_SetInverseDisplay(u8 ucData)
{
    OLED_WriteCommand(0xA6|ucData); // Set Inverse Display On/Off
                            // Default => 0xA6
                            // 0xA6 (0x00) => Normal Display
                            // 0xA7 (0x01) => Inverse Display On
}

void OLED_SetMultiplexRatio(u8 ucData)
{
    OLED_WriteCommand(0xA8);        // Set Multiplex Ratio
    OLED_WriteCommand(ucData);      // Default => 0x3F (1/64 Duty)
}

void OLED_SetDisplayOnOff(u8 ucData)
{
    OLED_WriteCommand(0xAE|ucData); // Set Display On/Off
                            // Default => 0xAE
                            // 0xAE (0x00) => Display Off
                            // 0xAF (0x01) => Display On
}

void OLED_SetStartPage(u8 ucData)
{
    OLED_WriteCommand(0xB0|ucData); // Set Page Start Address for Page Addressing Mode
                            // Default => 0xB0 (0x00)
}

void OLED_SetCommonRemap(u8 ucData)
{
    OLED_WriteCommand(0xC0|ucData); // Set COM Output Scan Direction
                            // Default => 0xC0
                            // 0xC0 (0x00) => Scan from COM0 to 63
                            // 0xC8 (0x08) => Scan from COM63 to 0
}

void OLED_SetDisplayOffset(u8 ucData)
{
    OLED_WriteCommand(0xD3);        // Set Display Offset
    OLED_WriteCommand(ucData);      // Default => 0x00
}

void OLED_SetDisplayClock(u8 ucData)
{
    OLED_WriteCommand(0xD5);        // Set Display Clock Divide Ratio / Oscillator Frequency
    OLED_WriteCommand(ucData);      // Default => 0x80
                            // D[3:0] => Display Clock Divider
                            // D[7:4] => Oscillator Frequency
}

void OLED_SetPrechargePeriod(u8 ucData)
{
    OLED_WriteCommand(0xD9);        // Set Pre-Charge Period
    OLED_WriteCommand(ucData);      // Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
                            // D[3:0] => Phase 1 Period in 1~15 Display Clocks
                            // D[7:4] => Phase 2 Period in 1~15 Display Clocks
}

void OLED_SetCommonConfig(u8 ucData)
{
    OLED_WriteCommand(0xDA);        // Set COM Pins Hardware Configuration
    OLED_WriteCommand(0x02|ucData); // Default => 0x12 (0x10)
                            // Alternative COM Pin Configuration
                            // Disable COM Left/Right Re-Map
}

void OLED_SetVCOMH(u8 ucData)
{
    OLED_WriteCommand(0xDB);        // Set VCOMH Deselect Level
    OLED_WriteCommand(ucData);      // Default => 0x20 (0.77*VCC)
}

void OLED_SetNop(void)
{
    OLED_WriteCommand(0xE3);        // Command for No Operation
}



/*******************************************************************************************
* Function Name  : void LED_Fill(u8 ucData)
* Description    : 向OLED填充数据
* Input          : ucData ：要写入的8位数据
* Output         : 无
* Return         : 无
*******************************************************************************************/
void OLED_Fill(u8 ucData)
{
   u8 i,j;

   MICROSEC_Delay_us(150);
   OLED_SetAddressingMode(0x00);   // Set Page Addressing Mode (0x00/0x01/0x02)
   MICROSEC_Delay_us(150);
   OLED_SetColumnAddress(0, 127);
   MICROSEC_Delay_us(150);
   OLED_SetPageAddress(0, 7);
   MICROSEC_Delay_us(150);

   for(i=0; i<8; i++)
   {
      for(j=0; j<128; j++)
         OLED_WriteData(ucData);
   }
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_Config(void)
{
	OLED_GPIO_Config();
	SPI2_Enable();
	OLED_CS_Low();

	OLED_RST_Low();
	MICROSEC_Delay_us(1000);
	OLED_RST_High();

	OLED_SetDisplayOnOff(0x00);     // Display Off (0x00/0x01)
	OLED_SetDisplayClock(0x80);     // Set Clock as 100 Frames/Sec
	OLED_SetMultiplexRatio(0x3F);   // 1/64 Duty (0x0F~0x3F)
	OLED_SetDisplayOffset(0x00);    // Shift Mapping RAM Counter (0x00~0x3F)
	OLED_SetStartLine(0x00);        // Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_SetChargePump(0x04);       // Enable Embedded DC/DC Converter (0x00/0x04)
	OLED_SetSegmentRemap(0x01);     // Set SEG/Column Mapping     0x00左右反置 0x01正常
	OLED_SetCommonRemap(0x08);      // Set COM/Row Scan Direction 0x00上下反置 0x08正常
	
	OLED_SetCommonConfig(0x10);     // Set Sequential Configuration (0x00/0x10)
	//此处设置亮度
	OLED_SetContrastControl(0x00);;//(0xCF);  // Set SEG Output Current
	OLED_SetPrechargePeriod(0xF1);  // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_SetVCOMH(0x40);            // Set VCOM Deselect Level
	OLED_SetEntireDisplay(0x00);    // Disable Entire Display On (0x00/0x01)
	OLED_SetInverseDisplay(0x00);   // Disable Inverse Display On (0x00/0x01)  
	OLED_SetDisplayOnOff(0x01);     // Display On (0x00/0x01)
	OLED_Fill(0x00);            // 初始清屏
	OLED_SetAddressingMode(0x00);   // Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_SetColumnAddress(0, 127);
	OLED_SetPageAddress(0, 7);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DisplayAsciiChar(u8 x, u8 y, u8 size, u8 ascii)
{
   u8 i, j;
	u8 xlen, ylen;
	u8 *point;
	
	xlen = size / 2;
	ylen = size / 8;

	if(size == 16)
		;//point = (u8 *)Ascii_16x8+(ascii-0x20)*16;
 	if(size == 32)
		point = (u8 *)ascii32x16+(ascii-0x20)*64;
	
   for(i=0; i<xlen; i++)
	{
		for(j=0; j<ylen; j++)
		{
			OLED_DisplayBuff[y+j][x+i] = point[i*ylen+j];
		}
	}
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DisplayChinaChar(u8 x, u8 y, u8 size, u8 *hzchar)
{
   u8 i, j;
	u8 dat, temp;
	u8 xlen, ylen;
	u8 buff[128];
	
	xlen = size;
	ylen = size / 8;
   FATFS_GetChinaDocData(buff, 16, hzchar);
	
   for(i=0; i<xlen; i++)
	{
		for(j=0; j<ylen; j++)
		{
			temp = buff[i*ylen+j];
			dat = ((temp<<7)&0x80) +
			((temp<<5)&0x40) +
			((temp<<3)&0x20) +
			((temp<<1)&0x10) +
			((temp>>7)&0x01) +
			((temp>>5)&0x02) +
			((temp>>3)&0x04) +
			((temp>>1)&0x08);			
			OLED_DisplayBuff[y+j][x+i] = dat;
		}
	}
}



 /**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DisplayString(u8 x, u8 y, u8 size, u8 *buff)
{
	u8 *point;										
	u8 tx, ty;							

	point = buff;											 
	tx=x; ty=y;									  

	while((*point)!='\0')									  
	{
		if(ty > (8 - size/8))						
			break;
		
		if((*point) < 0x80)
		{
			if(tx > (128-size/2))
				break;
			OLED_DisplayAsciiChar(tx, ty, size, *point);
			tx = tx + size/2;
			point = point + 1;			
		}
		else
		{
			if(tx > (128-size))
				break;
			OLED_DisplayChinaChar(tx, ty, size, point);
			tx = tx + size;
			point = point + 2;			
		}
	}    
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DisplayFileNameBefore(FATFS_FileListInfoTypeDef *list)
{
	OLED_DisplayBuffClearFile();
		
	if(list->FileQuantity == 0)
	{
	}
	else if(list->FileQuantity == 1)
	{
		OLED_DisplayString(0, 2, 16, list->FileNameBuff + list->FilePostion[0]);
		OLED_DisplayTurnColor(2);
		OLED_DisplayTurnColor(3);		
	}
	else if(list->FileQuantity == 2)
	{
		OLED_DisplayString(0, 2, 16, list->FileNameBuff + list->FilePostion[0]);
		OLED_DisplayString(0, 4, 16, list->FileNameBuff + list->FilePostion[1]);		
		if(list->FileCurNum == 0)
		{
			OLED_DisplayTurnColor(4);
			OLED_DisplayTurnColor(5);
			list->FileCurNum = 1;
		}
		else
		{
			OLED_DisplayTurnColor(2);
			OLED_DisplayTurnColor(3);
			list->FileCurNum = 0;
		}
		MICROSEC_Delay_ms(OLED_DISPLAYTIME);
		OLED_DisplayUpdate();
	}
	else if(list->FileQuantity == 3)
	{
		OLED_DisplayString(0, 2, 16, list->FileNameBuff + list->FilePostion[0]);
		OLED_DisplayString(0, 4, 16, list->FileNameBuff + list->FilePostion[1]);
		OLED_DisplayString(0, 6, 16, list->FileNameBuff + list->FilePostion[2]);		
		if(list->FileCurNum == 0)
		{
			OLED_DisplayTurnColor(6);
			OLED_DisplayTurnColor(7);
			list->FileCurNum = 2;
		}
		else if(list->FileCurNum == 1)
		{
			OLED_DisplayTurnColor(2);
			OLED_DisplayTurnColor(3);
			list->FileCurNum = 0;
		}
		else if(list->FileCurNum == 2)
		{
			OLED_DisplayTurnColor(4);
			OLED_DisplayTurnColor(5);
			list->FileCurNum = 1;
		}
		else
		{;}
		MICROSEC_Delay_ms(OLED_DISPLAYTIME);
		OLED_DisplayUpdate();		
	}
	else
	{
		if(list->FileCurNum == 0)
		{
			OLED_DisplayString(0, 2, 16, list->FileNameBuff + list->FilePostion[list->FileQuantity - 3]);
			OLED_DisplayString(0, 4, 16, list->FileNameBuff + list->FilePostion[list->FileQuantity - 2]);
			OLED_DisplayString(0, 6, 16, list->FileNameBuff + list->FilePostion[list->FileQuantity - 1]);				
			OLED_DisplayTurnColor(6);
			OLED_DisplayTurnColor(7);
			list->FileCurNum = list->FileQuantity - 1;
			MICROSEC_Delay_ms(OLED_DISPLAYTIME);
			OLED_DisplayUpdate();	
		}
		else if(list->FileCurNum == 1)
		{
			OLED_DisplayString(0, 2, 16, list->FileNameBuff + list->FilePostion[0]);
			OLED_DisplayString(0, 4, 16, list->FileNameBuff + list->FilePostion[1]);
			OLED_DisplayString(0, 6, 16, list->FileNameBuff + list->FilePostion[2]);				
			OLED_DisplayTurnColor(2);
			OLED_DisplayTurnColor(3);
			list->FileCurNum = 0;
			MICROSEC_Delay_ms(OLED_DISPLAYTIME);
			OLED_DisplayUpdate();				
		}
		else if(list->FileCurNum == (list->FileQuantity - 1))
		{
			OLED_DisplayString(0, 2, 16, list->FileNameBuff + list->FilePostion[list->FileQuantity - 3]);
			OLED_DisplayString(0, 4, 16, list->FileNameBuff + list->FilePostion[list->FileQuantity - 2]);
			OLED_DisplayString(0, 6, 16, list->FileNameBuff + list->FilePostion[list->FileQuantity - 1]);
			OLED_DisplayTurnColor(4);
			OLED_DisplayTurnColor(5);
			list->FileCurNum = list->FileQuantity - 2;
			MICROSEC_Delay_ms(OLED_DISPLAYTIME);
			OLED_DisplayUpdate();	
		}
		else
		{
			OLED_DisplayString(0, 2, 16, list->FileNameBuff + list->FilePostion[list->FileCurNum - 1]);
			OLED_DisplayString(0, 4, 16, list->FileNameBuff + list->FilePostion[list->FileCurNum - 0]);
			OLED_DisplayString(0, 6, 16, list->FileNameBuff + list->FilePostion[list->FileCurNum + 1]);				
			OLED_DisplayTurnColor(2);
			OLED_DisplayTurnColor(3);
			MICROSEC_Delay_ms(OLED_DISPLAYTIME / 2);
			OLED_DisplayUpdate();
			OLED_DisplayBuffClearAll();
			OLED_DisplayString(0, 2, 16, list->FileNameBuff + list->FilePostion[list->FileCurNum - 2]);
			OLED_DisplayString(0, 4, 16, list->FileNameBuff + list->FilePostion[list->FileCurNum - 1]);
			OLED_DisplayString(0, 6, 16, list->FileNameBuff + list->FilePostion[list->FileCurNum]);				
			OLED_DisplayTurnColor(4);
			OLED_DisplayTurnColor(5);
			list->FileCurNum = list->FileCurNum - 1;
			MICROSEC_Delay_ms(OLED_DISPLAYTIME / 2);
			OLED_DisplayUpdate();			
		}
	}	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DisplayFileNameNext(FATFS_FileListInfoTypeDef *list)
{
	OLED_DisplayBuffClearFile();
		
	if(list->FileQuantity == 0)
	{
	}
	else if(list->FileQuantity == 1)
	{
		OLED_DisplayString(0, 2, 16, list->FileNameBuff + list->FilePostion[0]);
		OLED_DisplayTurnColor(2);
		OLED_DisplayTurnColor(3);		
	}
	else if(list->FileQuantity == 2)
	{
		OLED_DisplayString(0, 2, 16, list->FileNameBuff + list->FilePostion[0]);
		OLED_DisplayString(0, 4, 16, list->FileNameBuff + list->FilePostion[1]);		
		if(list->FileCurNum == 0)
		{
			OLED_DisplayTurnColor(4);
			OLED_DisplayTurnColor(5);
			list->FileCurNum = 1;
		}
		else
		{
			OLED_DisplayTurnColor(2);
			OLED_DisplayTurnColor(3);
			list->FileCurNum = 0;
		}
		MICROSEC_Delay_ms(OLED_DISPLAYTIME);
		OLED_DisplayUpdate();
	}
	else if(list->FileQuantity == 3)
	{
		OLED_DisplayString(0, 2, 16, list->FileNameBuff + list->FilePostion[0]);
		OLED_DisplayString(0, 4, 16, list->FileNameBuff + list->FilePostion[1]);
		OLED_DisplayString(0, 6, 16, list->FileNameBuff + list->FilePostion[2]);		
		if(list->FileCurNum == 0)
		{
			OLED_DisplayTurnColor(4);
			OLED_DisplayTurnColor(5);
			list->FileCurNum = 1;
		}
		else if(list->FileCurNum == 1)
		{
			OLED_DisplayTurnColor(6);
			OLED_DisplayTurnColor(7);
			list->FileCurNum = 2;
		}
		else if(list->FileCurNum == 2)
		{
			OLED_DisplayTurnColor(2);
			OLED_DisplayTurnColor(3);
			list->FileCurNum = 0;
		}
		else
		{;}
		MICROSEC_Delay_ms(OLED_DISPLAYTIME);
		OLED_DisplayUpdate();		
	}
	else
	{
		if(list->FileCurNum == 0)
		{
			OLED_DisplayString(0, 2, 16, list->FileNameBuff + list->FilePostion[0]);
			OLED_DisplayString(0, 4, 16, list->FileNameBuff + list->FilePostion[1]);
			OLED_DisplayString(0, 6, 16, list->FileNameBuff + list->FilePostion[2]);
			OLED_DisplayTurnColor(4);
			OLED_DisplayTurnColor(5);
			list->FileCurNum = 1;
			MICROSEC_Delay_ms(OLED_DISPLAYTIME);
			OLED_DisplayUpdate();	
		}
		else if(list->FileCurNum == (list->FileQuantity - 2))
		{
			OLED_DisplayString(0, 2, 16, list->FileNameBuff + list->FilePostion[list->FileQuantity - 3]);
			OLED_DisplayString(0, 4, 16, list->FileNameBuff + list->FilePostion[list->FileQuantity - 2]);
			OLED_DisplayString(0, 6, 16, list->FileNameBuff + list->FilePostion[list->FileQuantity - 1]);				
			OLED_DisplayTurnColor(6);
			OLED_DisplayTurnColor(7);
			list->FileCurNum = list->FileQuantity - 1;
			MICROSEC_Delay_ms(OLED_DISPLAYTIME);
			OLED_DisplayUpdate();						
		}
		else if(list->FileCurNum == (list->FileQuantity - 1))
		{
			OLED_DisplayString(0, 2, 16, list->FileNameBuff + list->FilePostion[0]);
			OLED_DisplayString(0, 4, 16, list->FileNameBuff + list->FilePostion[1]);
			OLED_DisplayString(0, 6, 16, list->FileNameBuff + list->FilePostion[2]);
			OLED_DisplayTurnColor(2);
			OLED_DisplayTurnColor(3);
			list->FileCurNum = 0;
			MICROSEC_Delay_ms(OLED_DISPLAYTIME);
			OLED_DisplayUpdate();					
		}
		else
		{
			OLED_DisplayString(0, 2, 16, list->FileNameBuff + list->FilePostion[list->FileCurNum - 1]);
			OLED_DisplayString(0, 4, 16, list->FileNameBuff + list->FilePostion[list->FileCurNum - 0]);
			OLED_DisplayString(0, 6, 16, list->FileNameBuff + list->FilePostion[list->FileCurNum + 1]);				
			OLED_DisplayTurnColor(6);
			OLED_DisplayTurnColor(7);
			MICROSEC_Delay_ms(OLED_DISPLAYTIME / 2);
			OLED_DisplayUpdate();
			OLED_DisplayBuffClearAll();
			OLED_DisplayString(0, 2, 16, list->FileNameBuff + list->FilePostion[list->FileCurNum ]);
			OLED_DisplayString(0, 4, 16, list->FileNameBuff + list->FilePostion[list->FileCurNum + 1]);
			OLED_DisplayString(0, 6, 16, list->FileNameBuff + list->FilePostion[list->FileCurNum + 2]);				
			OLED_DisplayTurnColor(4);
			OLED_DisplayTurnColor(5);
			list->FileCurNum = list->FileCurNum + 1;
			MICROSEC_Delay_ms(OLED_DISPLAYTIME / 2);
			OLED_DisplayUpdate();			
		}
	}	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DisplayFileNameMove(FATFS_FileListInfoTypeDef *list)
{
	*(list->FilePathName + list->FilePathLen) = '/';
	list->FilePathLen = list->FilePathLen + 1;
	BASIC_Memcpy(list->FilePathName + list->FilePathLen, list->FileNameBuff + list->FilePostion[list->FileCurNum], list->FileLen[list->FileCurNum]);
	list->FilePathLen = list->FilePathLen + list->FileLen[list->FileCurNum] - 1;
	
	list->FileLayerNum[list->FileLayerTol] = list->FileCurNum;
	list->FileLayerTol = list->FileLayerTol + 1;
	
	FATFS_FileSacn(list->FilePathName, list);
	OLED_DisplayFileName(*list);
}
	
	
	
/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DisplayFileNameBack(FATFS_FileListInfoTypeDef *list)
{
	u8 i;
	if(list->FilePathLen > 1)
	{
		for(i=list->FilePathLen; i>0; i--)
		{
			if(list->FilePathName[i] == '/')
			{
				list->FilePathName[i] = '\0';			
				break;
			}
			else
			{
				list->FilePathName[i] = '\0';
				list->FilePathLen = list->FilePathLen - 1;
			}
		}
	}
	FATFS_FileSacn(list->FilePathName, list);
	list->FileLayerTol = list->FileLayerTol - 1;
	list->FileCurNum = list->FileLayerNum[list->FileLayerTol];
	OLED_DisplayFileName(*list);	
}
	
	
	
/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DisplayFileName(FATFS_FileListInfoTypeDef list)
{
	OLED_DisplayBuffClearAll();
	
	if(list.FileQuantity == 0)
	{
	}
	else if(list.FileQuantity == 1)
	{
		OLED_DisplayString(0, 2, 16, list.FileNameBuff + list.FilePostion[0]);
		OLED_DisplayTurnColor(2);
		OLED_DisplayTurnColor(3);
	}
	else if(list.FileQuantity == 2)
	{
		OLED_DisplayString(0, 2, 16, list.FileNameBuff + list.FilePostion[0]);
		OLED_DisplayString(0, 4, 16, list.FileNameBuff + list.FilePostion[1]);		
		if(list.FileCurNum == 0)
		{
			OLED_DisplayTurnColor(2);
			OLED_DisplayTurnColor(3);			
		}
		else
		{
			OLED_DisplayTurnColor(4);
			OLED_DisplayTurnColor(5);			
		}	
	}
	else
	{
		if(list.FileCurNum == 0)
		{
			OLED_DisplayString(0, 2, 16, list.FileNameBuff + list.FilePostion[0]);
			OLED_DisplayString(0, 4, 16, list.FileNameBuff + list.FilePostion[1]);
			OLED_DisplayString(0, 6, 16, list.FileNameBuff + list.FilePostion[2]);
			OLED_DisplayTurnColor(2);
			OLED_DisplayTurnColor(3);				
		}
      else if(list.FileCurNum == (list.FileQuantity - 1))
		{
			OLED_DisplayString(0, 2, 16, list.FileNameBuff + list.FilePostion[list.FileQuantity - 3]);
			OLED_DisplayString(0, 4, 16, list.FileNameBuff + list.FilePostion[list.FileQuantity - 2]);
			OLED_DisplayString(0, 6, 16, list.FileNameBuff + list.FilePostion[list.FileQuantity - 1]);				
			OLED_DisplayTurnColor(6);
			OLED_DisplayTurnColor(7);			
		}
      else
		{
			OLED_DisplayString(0, 2, 16, list.FileNameBuff + list.FilePostion[list.FileCurNum - 1]);
			OLED_DisplayString(0, 4, 16, list.FileNameBuff + list.FilePostion[list.FileCurNum]);
			OLED_DisplayString(0, 6, 16, list.FileNameBuff + list.FilePostion[list.FileCurNum + 1]);			
			OLED_DisplayTurnColor(4);
			OLED_DisplayTurnColor(5);			
		}
	}
	OLED_DisplayUpdate();
}	



 /**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DisplayFileNum(FATFS_FileListInfoTypeDef *list)
{ 
	u8 bai, shi, ge;
	u8 buff[3];
	
	OLED_DisplayBuffClearPage(0);
	OLED_DisplayBuffClearPage(1);
	bai = (list->FileQuantity % 1000) / 100;
	shi = (list->FileQuantity % 100) / 10;
	ge = (list->FileQuantity % 10) / 1;
	buff[0]  = bai + '0';
	buff[1]  = shi + '0';
	buff[2]  = ge + '0';
	OLED_DisplayString(32, 0, 16, buff);

	bai = ((list->FileCurNum + 1) % 1000) / 100;
	shi = ((list->FileCurNum + 1) % 100) / 10;
	ge = ((list->FileCurNum + 1) % 10) / 1;
	buff[0]  = bai + '0';
	buff[1]  = shi + '0';
	buff[2]  = ge + '0';
	OLED_DisplayString(0, 0, 16, buff);

	OLED_DisplayString(24, 0, 16, "/");
   OLED_DisplayUpdate();
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DisplayRoll(u8 page, u8 size, u8 *buff)
{
	
}	
	


/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DisplayTurnColor(u8 page)
{
	u8 i;
	
	if(page < 8)
	{	
		for(i=0; i<128; i++)
		{
			OLED_DisplayBuff[page][i] = ~OLED_DisplayBuff[page][i];
		}
	}
}	



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DisplayUpdate(void)
{
	u8 i, j;

	OLED_SetColumnAddress(0, 127);
	OLED_SetPageAddress(0, 7);	
	
	for(i=0; i<8; i++)
	{
		for(j=0; j<128; j++)
		{
			OLED_WriteData(OLED_DisplayBuff[i][j]);
		}
	}
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DisplayOn(void)
{
	OLED_SetDisplayOnOff(0x01);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DisplayOff(void)
{
	OLED_SetDisplayOnOff(0x00);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DisplayBuffClearAll(void)
{
	u8 i, j;
	
	for(i=0; i<8; i++)
	{
		for(j=0; j<128; j++)
		{
			OLED_DisplayBuff[i][j] = 0x00;
		}
	}
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DisplayBuffClearPage(u8 page)
{
	u8 i;	
	for(i=0; i<128; i++)
	{
		OLED_DisplayBuff[page][i] = 0x00;
	}
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DisplayBuffClearFile(void)
{
	u8 i, j;
	
	for(i=2; i<8; i++)
	{
		for(j=0; j<128; j++)
		{
			OLED_DisplayBuff[i][j] = 0x00;
		}
	}
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	OLED_DisplayBuffFill(void)
{
	u8 i, j;
	
	for(i=0; i<8; i++)
	{
		for(j=0; j<128; j++)
		{
			OLED_DisplayBuff[i][j] = 0xff;
		}
	}
}




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
