
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : mp3.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : mp3 program body
* Description    : --
**********************************************************************/

/* Includes  == 文件包含 --------------------------------------------*/
#include "mp3.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/
FATFS_FileListInfoTypeDef		filelis;
SETTING_DataTypeDef				Setting;


	
/* Private functions  ==  私有函数定义 ------------------------------*/

/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void MP3_Main(void)
{
	MP3_Init();
	MP3_Menu();
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void MP3_Init(void)
{
	MICROSEC_Init();
	POWER_Config();
	ADC_Enable();
	LED_Config();
	KEY_Config();
	OLED_Config();
	VS1053B_Config();
	
	MP3_PowerOn();
	ADC_CheckBattery();
	SDCARD_Config();
	MP3_CheckSD();
	TIMER_Enble();
	
	MUSIC_Init();
	OLED_DisplayBuffClearAll();
	OLED_DisplayUpdate();
}


	
/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void MP3_Menu(void)
{
	u8 flag;
	u8 i, j;
	u8 update;
	D_KEY_StatusTypeDef key;

	//显示一横
	OLED_SetColumnAddress(0, 127);
	OLED_SetPageAddress(0, 1);	
	for(i=0; i<2; i++)
	{
		for(j=0; j<128; j++)
		{
			if(i == 0)
			{
				OLED_WriteData(0x00);
			}
			else
			{
				OLED_WriteData(0x80);
			}
		}
	}
			
	flag = 0;
	update = 1;
	while(1)
	{
		key = KEY_Scan(20000);
		if(key.Active == D_KEY_ACTIVE_YES)
		{
			switch(key.Type)
			{
				case D_KEY_IS_VINC:

					break;
							
				case D_KEY_IS_VDEC:

					break;

				case D_KEY_IS_POWER:
					if((key.State == D_KEY_UP) && (key.LongPushCnt < 20))
					{
					}
					else if((key.State == D_KEY_LONGDOWN) && (key.LongPushCnt > 100))
					{
						MP3_PowerOffWithSD();
					}
					else
					{;}					
					break;

				case D_KEY_IS_BACK:

					break;

				case D_KEY_IS_NEXT:
					if((key.State == D_KEY_UP) && (key.LongPushCnt < 20))
					{
						if(flag == 0)
						{
							flag = 1;
						}
						else
						{
							flag = 0;
						}
						update = 1;
					}
					else if((key.State == D_KEY_LONGDOWN) && ((key.LongPushCnt % 20) == 0))
					{
						if(flag == 0)
						{
							flag = 1;
						}
						else
						{
							flag = 0;
						}
						update = 1;					
					}
					else
					{;}
					break;
							
				case D_KEY_IS_BEFORE:
					if((key.State == D_KEY_UP) && (key.LongPushCnt < 20)) 
					{
						if(flag == 0)
						{
							flag = 1;
						}
						else
						{
							flag = 0;
						}
						update = 1;
					}
					else if((key.State == D_KEY_LONGDOWN) && ((key.LongPushCnt % 20) == 0))
					{
						if(flag == 0)
						{
							flag = 1;
						}
						else
						{
							flag = 0;
						}
						update = 1;					
					}
					else
					{;}
					break;

				case D_KEY_IS_STOP:
					if((key.State == D_KEY_UP) && (key.LongPushCnt < 20)) 
					{
						if(flag == 0)
						{
						}
						else
						{
							MP3_ScanFile();
							OLED_DisplayBuffClearAll();
							OLED_DisplayUpdate();
							//显示一横
							OLED_SetColumnAddress(0, 127);
							OLED_SetPageAddress(0, 1);	
							for(i=0; i<2; i++)
							{
								for(j=0; j<128; j++)
								{
									if(i == 0)
									{
										OLED_WriteData(0x00);
									}
									else
									{
										OLED_WriteData(0x80);
									}
								}
							}							
						}
						update = 1;
					}	
					break;

				case D_KEY_IS_LOCK:

					break;
				
				default: 
					break;			
			}
		}
		if(update == 1)
		{
			update = 0;
			//显示文件数和电池电量
			Display_Num(0, 0);			
			Display_Icon(flag);
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
void MP3_ScanFile(void)
{
	D_KEY_StatusTypeDef key;
	filelis.FileLayerTol = 0;
	filelis.FilePathName[0] = '\0';
	filelis.FilePathLen = 0;
	FATFS_FileSacn((u8 *)"/", &filelis);
	FATFS_DisplayFileName(&filelis);
	Display_Num(filelis.FileQuantity, filelis.FileCurNum + 1);
	
	while(1)
	{
		key = KEY_Scan(20000);
		if(key.Active == D_KEY_ACTIVE_YES)
		{
			switch(key.Type)
			{
				case D_KEY_IS_VINC:

					break;
							
				case D_KEY_IS_VDEC:

					break;

				case D_KEY_IS_POWER:
					if((key.State == D_KEY_UP) && (key.LongPushCnt < 20))
					{
					}
					else if((key.State == D_KEY_LONGDOWN) && (key.LongPushCnt > 100))
					{
						MP3_PowerOffWithSD();
					}
					else
					{;}	
					break;

				case D_KEY_IS_BACK:
					if((key.State == D_KEY_UP) && (key.LongPushCnt < 20))
					{
						if(filelis.FilePathLen == 0)
						{
							return;
						}
						FATFS_DisplayFileNameBack(&filelis);
						Display_Num(filelis.FileQuantity, filelis.FileCurNum + 1);
					}
					break;

				case D_KEY_IS_NEXT:
					if((key.State == D_KEY_UP) && (key.LongPushCnt < 20))
					{
						FATFS_DisplayFileNameNext(&filelis);
						Display_Num(filelis.FileQuantity, filelis.FileCurNum + 1);
					}
					else if((key.State == D_KEY_LONGDOWN) && (key.LongPushCnt > 20) && (((key.LongPushCnt - 20) % 10) == 0))
					{
						FATFS_DisplayFileNameNext(&filelis);
						Display_Num(filelis.FileQuantity, filelis.FileCurNum + 1);
					}
					else
					{;}
					break;
							
				case D_KEY_IS_BEFORE:
					if((key.State == D_KEY_UP) && (key.LongPushCnt < 20))
					{
						FATFS_DisplayFileNameBefore(&filelis);
						Display_Num(filelis.FileQuantity, filelis.FileCurNum + 1);
					}
					else if((key.State == D_KEY_LONGDOWN) && (key.LongPushCnt > 20) && (((key.LongPushCnt - 20) % 10) == 0))
					{
						FATFS_DisplayFileNameBefore(&filelis);
						Display_Num(filelis.FileQuantity, filelis.FileCurNum + 1);
					}
					else
					{;}
					break;

				case D_KEY_IS_STOP:
					if((key.State == D_KEY_UP) && (key.LongPushCnt < 20))
					{
						if(filelis.FileQuantity > 0)
						{	
							if(filelis.FileType[filelis.FileCurNum] == FATFS_FILEISDIR)
							{
								FATFS_DisplayFileNameMove(&filelis);
								Display_Num(filelis.FileQuantity, filelis.FileCurNum + 1);
							}
							else if(filelis.FileType[filelis.FileCurNum] == FATFS_FILEISOTHER)
							{

							}
							else
							{
								MUSIC_PlaySong(&filelis);
								FATFS_DisplayFileNameBack(&filelis);
								Display_Num(filelis.FileQuantity, filelis.FileCurNum + 1);										
							}	
						}
					}
					break;

				case D_KEY_IS_LOCK:

					break;
				
				default: 
					break;			
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
void MP3_PlaySong(void)
{
	
}


	
/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void MP3_Setting(void)
{
	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void MP3_PowerOn(void)
{
	MICROSEC_Delay_ms(500);
	Display_PowerOn();
	POWER_On();
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
void MP3_PowerOffWithoutSD(void)
{
	TIMER_Disble();
	Display_PowerOff();
	MICROSEC_Delay_ms(1000);
	POWER_Off();	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void MP3_PowerOffWithSD(void)
{
	TIMER_Disble();
	Display_PowerOff();
	SETTING_WriteFlash(&Setting);
	MICROSEC_Delay_ms(1000);
	POWER_Off();	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void MP3_LowBatteryWithoutSD(void)
{
	TIMER_Disble();
	Display_LowBattery();
	MICROSEC_Delay_ms(2000);
	POWER_Off();	

}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void MP3_LowBatteryWithSD(void)
{
	TIMER_Disble();
	Display_LowBattery();
	SETTING_WriteFlash(&Setting);
	MICROSEC_Delay_ms(2000);
	POWER_Off();	

}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void MP3_CheckSD(void)
{
	FIL fil;
	FATFS fs;
	FRESULT res;
	
	f_mount(0, &fs); 
	res = f_open(&fil, "system/songti16.DZK", FA_OPEN_EXISTING | FA_READ);
	if(res != FR_OK)
	{
		Display_ErrorWithoutSD((u8 *)"No_System_File");
	}
	res = f_close(&fil);
	
	res = f_open(&fil, "system/setting.ini", FA_OPEN_EXISTING | FA_READ);
	if(res != FR_OK)
	{
		Display_ErrorWithoutSD((u8 *)"No_System_File");
	}
	res = f_close(&fil);
	
	res = f_mount(0, NULL);
}


/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
