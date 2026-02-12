
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : display.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : display program body
* Description    : --
**********************************************************************/

/* Includes  == 文件包含 --------------------------------------------*/
#include "display.h"
#include "mp3.h"
#include "oled.h"
#include "icon.h"
#include "fixedsys_16x8.h"
#include "key.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/
u8		Display_Buff[8][128]={0};

	
	
/* Private functions  ==  私有函数定义 ------------------------------*/

/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_Icon(u8 flag)
{
	u16 i;
	u8 *icon1, *icon2;
	
	if(flag == 0)
	{
		icon1 = (u8 *)ICON_Setting_Select;
		icon2 = (u8 *)ICON_Floder_Notselect;	
	}
	else if(flag == 1)
	{
		icon1 = (u8 *)ICON_Setting_Notselect;
		icon2 = (u8 *)ICON_Floder_Select;	
	}
	else
	{
		return;
	}

	OLED_SetColumnAddress(8, 8+48-1);
	OLED_SetPageAddress(2, 7);	
	for(i=0; i<6*48; i++)
	{
		OLED_WriteData(icon1[i]);
	}
	OLED_SetColumnAddress(72, 72+48-1);
	OLED_SetPageAddress(2, 7);	
	for(i=0; i<6*48; i++)
	{
		OLED_WriteData(icon2[i]);
	}			
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_Power(u8 num)
{
	u8 *power;
	u8 i, j;
	
	switch(num)
	{					 
		case 0:      
            power = (u8 *)ICON_Power_00;
				break;

		case 10:
            power = (u8 *)ICON_Power_10;
				break;

		case 20:      
            power = (u8 *)ICON_Power_20;
				break;

		case 30:
            power = (u8 *)ICON_Power_30;
				break;

		case 40:      
            power = (u8 *)ICON_Power_40;
				break;

		case 50:
            power = (u8 *)ICON_Power_50;
				break;

		case 60:      
            power = (u8 *)ICON_Power_60;
				break;

		case 70:
            power = (u8 *)ICON_Power_70;
				break;

		case 80:
            power = (u8 *)ICON_Power_80;
				break;

		case 90:      
            power = (u8 *)ICON_Power_90;
				break;

		case 100:
            power = (u8 *)ICON_Power_100;
				break;
		
		default:
				return;
	}

	OLED_SetColumnAddress(96, 127);
	OLED_SetPageAddress(0, 1);	
	for(i=0; i<2; i++)
	{
		for(j=0; j<32; j++)
		{
			if(i == 0)
			{
				OLED_WriteData(power[i*32+j]);
			}
			else
			{
				OLED_WriteData(power[i*32+j] | 0x80);
			}
		}
	}	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_Num(u16 total, u16 current)
{
	u8 bai, shi, ge;
	u8 buff[3];
	u8 i, j;
	
	bai = (current % 1000) / 100;
	shi = (current % 100) / 10;
	ge = (current % 10) / 1;
	buff[0]  = bai + '0';
	buff[1]  = shi + '0';
	buff[2]  = ge + '0';
	Display_String(0, 0, buff);

	bai = (total % 1000) / 100;
	shi = (total % 100) / 10;
	ge = (total % 10) / 1;
	buff[0]  = bai + '0';
	buff[1]  = shi + '0';
	buff[2]  = ge + '0';
	Display_String(32, 0, buff);
	
	Display_String(24, 0, (u8 *)"/");
	
	OLED_SetColumnAddress(0, 55);
	OLED_SetPageAddress(0, 1);	
	for(i=0; i<2; i++)
	{
		for(j=0; j<56; j++)
		{
			if(i == 0)
			{
				OLED_WriteData(Display_Buff[i][j]);
			}
			else
			{
				OLED_WriteData(Display_Buff[i][j] | 0x80);
			}
		}
	}
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_AsciiChar(u8 x, u8 y, u8 ascii)
{
   u8 i, j;
	u8 *point;
	
	point =  (u8 *)Ascii_16x8 + (ascii - 0x20) * 16;
	
   for(i=0; i<8; i++)
	{
		for(j=0; j<2; j++)
		{
			Display_Buff[y+j][x+i] = point[i*2+j];
		}
	}
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_ChinaChar(u8 x, u8 y, u8 *hzchar)
{
	u8 i, j;
	u8 dat, temp;
	u8 xlen, ylen;
	u8 buff[128];
	
	xlen = 16;
	ylen = 16 / 8;
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
			Display_Buff[y+j][x+i] = dat;
		}
	}
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_String(u8 x, u8 y, u8 *buff)
{
	u8 *point;										
	u8 tx, ty;							

	point = buff;											 
	tx=x; ty=y;									  

	while((*point)!='\0')									  
	{
		if(ty > 6)						
			break;
		
		if((*point) < 0x80)
		{
			if(tx > 120)
				break;
			Display_AsciiChar(tx, ty, *point);
			tx = tx + 8;
			point = point + 1;			
		}
		else
		{
			if(tx > 112)
				break;
			Display_ChinaChar(tx, ty, point);
			tx = tx + 16;
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
* Remark         : None
**********************************************************************/
void Display_TurnColor(u8 page)
{
	u8 i;
	
	if(page < 8)
	{	
		for(i=0; i<128; i++)
		{
			Display_Buff[page][i] = ~Display_Buff[page][i];
		}
	}
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_ClearFileNameZoom(void)
{
	u8 i, j;
	
	for(i=2; i<8; i++)
	{
		for(j=0; j<128; j++)
		{
			Display_Buff[i][j] = 0x00;
		}
	}	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_UpdateFileNameZoom(void)
{
	u8 i, j;

	OLED_SetColumnAddress(0, 127);
	OLED_SetPageAddress(2, 7);		
	for(i=2; i<8; i++)
	{
		for(j=0; j<128; j++)
		{
			OLED_WriteData(Display_Buff[i][j]);
		}
	}	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_ClearSongNameZoom(void)
{
	u8 i, j;
	
	for(i=2; i<4; i++)
	{
		for(j=0; j<128; j++)
		{
			Display_Buff[i][j] = 0x00;
		}
	}	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_UpdateSongNameZoom(void)
{
	u8 i, j;

	OLED_SetColumnAddress(0, 127);
	OLED_SetPageAddress(2, 3);		
	for(i=2; i<4; i++)
	{
		for(j=0; j<128; j++)
		{
			OLED_WriteData(Display_Buff[i][j]);
		}
	}	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_ClearAllZoom(void)
{
	u8 i, j;
	
	for(i=0; i<8; i++)
	{
		for(j=0; j<128; j++)
		{
			Display_Buff[i][j] = 0x00;
		}
	}	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_UpdateAllZoom(void)
{
	u8 i, j;

	OLED_SetColumnAddress(0, 127);
	OLED_SetPageAddress(0, 7);		
	for(i=0; i<8; i++)
	{
		for(j=0; j<128; j++)
		{
			OLED_WriteData(Display_Buff[i][j]);
		}
	}	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_PlayTime(u8 tim)
{
	float scale, timtotal;
	u8 timtemp;
	u8 i, j;
	
	scale = 1.12f;
	timtotal = scale * (float)tim;
	timtemp = (u8)timtotal;
	for(j=0; j<112; j++)
	{
		if(j<timtemp)
		{
			Display_Buff[4][j] = 0xc0;
		}
		else
		{
			Display_Buff[4][j] = 0x40;
		}
	}	
	for(j=0; j<112; j++)
	{
		if(j<timtemp)
		{
			Display_Buff[5][j] = 0x83;
		}
		else
		{
			Display_Buff[5][j] = 0x82;
		}
	}	
	
	OLED_SetColumnAddress(0, 111);
	OLED_SetPageAddress(4, 5);		
	for(i=4; i<6; i++)
	{
		for(j=0; j<112; j++)
		{
			OLED_WriteData(Display_Buff[i][j]);
		}
	}	
}	
	


/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_Line(void)
{
	u8 i, j;
	for(j=0; j<128; j++)
	{
		Display_Buff[4][j] = 0x00;
	}	
	for(j=0; j<128; j++)
	{
		Display_Buff[5][j] = 0x80;
	}	
	
	OLED_SetColumnAddress(0, 127);
	OLED_SetPageAddress(4, 5);		
	for(i=4; i<6; i++)
	{
		for(j=0; j<128; j++)
		{
			OLED_WriteData(Display_Buff[i][j]);
		}
	}	
}	



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_Stop(u8 index)
{
	u8 i;
    u8 *buff;
	
	switch(index)
	{
		case 1:
            buff = (u8 *)ICON_Play;
			break;
					
		case 2:
            buff = (u8 *)ICON_Stop;
			break;
		
		default:
			break;
	}
			
	OLED_SetColumnAddress(112, 127);
	OLED_SetPageAddress(4, 5);		
	for(i=0; i<2*16; i++)
	{
		OLED_WriteData(buff[i]);
	}	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_Choose(u8 index)
{
	u8 i, j;

	switch(index)
	{					
		case 1:
			Display_String(0, 6, (u8 *)"模式");
			break;

		case 2:
            Display_String(0, 6, (u8 *)"音效");
			break;
		
		case 3:
            Display_String(0, 6, (u8 *)"音量");
			break;
		
		default:
			break;
	}
	
	Display_Buff[6][31] = 0xff;
	Display_Buff[7][31] = 0xff;
		
	OLED_SetColumnAddress(0, 31);
	OLED_SetPageAddress(6, 7);		
	for(i=6; i<8; i++)
	{
		for(j=0; j<32; j++)
		{
			OLED_WriteData(Display_Buff[i][j]);
		}
	}
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_Mode(u8 index)
{
	u8 i;
    u8 *buff;
	
	switch(index)
	{
		case 1:
            buff = (u8 *)ICON_AllCyc;
			break;
					
		case 2:
            buff = (u8 *)ICON_OneCyc;
			break;

		case 3:
            buff = (u8 *)ICON_AllOrder;
			break;

		case 4:
            buff = (u8 *)ICON_OneOrder;
			break;
		
		default:
			break;
	}
			
	OLED_SetColumnAddress(32, 63);
	OLED_SetPageAddress(6, 7);		
	for(i=0; i<2*32; i++)
	{
		OLED_WriteData(buff[i]);
	}	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_Effect(u8 index)
{
	u8 i, j;

	switch(index)
	{
		case 1:
            Display_String(64, 6, (u8 *)"低音");
			break;
					
		case 2:
            Display_String(64, 6, (u8 *)"正常");
			break;

		case 3:
            Display_String(64, 6, (u8 *)"高音");
			break;

		case 4:
            Display_String(64, 6, (u8 *)"增强");
			break;

		case 5:
            Display_String(64, 6, (u8 *)"环绕");
			break;
		
		default:
			break;
	}
	
	Display_Buff[6][95] = 0xff;
	Display_Buff[7][95] = 0xff;
		
	OLED_SetColumnAddress(64, 95);
	OLED_SetPageAddress(6, 7);		
	for(i=6; i<8; i++)
	{
		for(j=64; j<96; j++)
		{
			OLED_WriteData(Display_Buff[i][j]);
		}
	}	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_Voice(u8 num)
{
	u8 i, j;
    u8 shi, ge;
	u8 buff[2];
	
    shi = (num % 100) /10;
	ge = num % 10;
	buff[0] = shi + '0';
	buff[1] = ge + '0';
	
	Display_String(96, 6, (u8 *)"V:");
	Display_String(112, 6, buff);
	
	OLED_SetColumnAddress(96, 127);
	OLED_SetPageAddress(6, 7);		
	for(i=6; i<8; i++)
	{
		for(j=96; j<128; j++)
		{
			OLED_WriteData(Display_Buff[i][j]);
		}
	}		
}
	
	

/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_ErrorWithSD(u8 *buff)
{	
	D_KEY_StatusTypeDef key;
	
	TIMER_Disble();
	Display_ClearAllZoom();
	Display_String(0, 2, buff);
	Display_String(0, 4, (u8 *)"Press PowerOff!");
	Display_UpdateAllZoom();
	while(1)
	{
		key = KEY_Scan(20000);
		if(key.Active == D_KEY_ACTIVE_YES)
		{
			if((key.Type == D_KEY_IS_POWER) && (key.State == D_KEY_LONGDOWN) && (key.LongPushCnt > 100))
			{
				MP3_PowerOffWithSD();
			}																									
		}
	}
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_ErrorWithoutSD(u8 *buff)
{	
	D_KEY_StatusTypeDef key;
	
	TIMER_Disble();
	Display_ClearAllZoom();
	Display_String(0, 2, buff);
	Display_String(0, 4, (u8 *)"Press PowerOff!");
	Display_UpdateAllZoom();
	while(1)
	{
		key = KEY_Scan(20000);
		if(key.Active == D_KEY_ACTIVE_YES)
		{
			if((key.Type == D_KEY_IS_POWER) && (key.State == D_KEY_LONGDOWN) && (key.LongPushCnt > 100))
			{
				MP3_PowerOffWithoutSD();
			}																									
		}
	}
}
/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_PowerOn(void)
{
	Display_ClearAllZoom();
	Display_String(0, 2, (u8 *)"POWER ON...");
	Display_UpdateAllZoom();
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_PowerOff(void)
{
	Display_ClearAllZoom();
	Display_String(0, 2, (u8 *)"POWER OFF...");
	Display_UpdateAllZoom();
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void Display_LowBattery(void)
{
	Display_ClearAllZoom();
	Display_String(0, 2, (u8 *)"Low Battery...");
	Display_String(0, 4, (u8 *)"POWER OFF...");
	Display_UpdateAllZoom();
}




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
