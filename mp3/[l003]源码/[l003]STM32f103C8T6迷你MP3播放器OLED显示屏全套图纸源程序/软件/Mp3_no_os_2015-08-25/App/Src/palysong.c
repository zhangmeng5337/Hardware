
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : playsong.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : playsong program body
* Description    : --
**********************************************************************/

/* Includes  == 文件包含 --------------------------------------------*/
#include "playsong.h"
#include "vs1053b.h"
#include "spi1.h"
#include "ff.h"
#include "diskio.h"
#include <string.h>
#include "vs1053b_flac.h"
#include "fatfs.h"
#include "microsec.h"
#include "key.h"
#include "oled.h"
#include "basic.h"
#include "display.h"
#include "mp3.h"
#include "setting.h"
#include "power.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//



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
void MUSIC_Init(void)
{
    SETTING_ReadFlash(&Setting);	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void MUSIC_ChooseSwitch(u8 flag)
{
	switch(flag)
	{
		case 0:
            Setting.Music_Choose = Setting.Music_Choose;
			break;
					
		case 1:
			if(Setting.Music_Choose == 3)
			{
				Setting.Music_Choose = 1;
			}
			else
			{
				Setting.Music_Choose = Setting.Music_Choose + 1;
			}
			break;

		default: 
			break;
	}

	Display_Choose(Setting.Music_Choose);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void MUSIC_ModeSwitch(u8 flag)
{
	switch(flag)
	{
		case 0:
            Setting.Music_Mode = Setting.Music_Mode;
			break;
					
		case 1:
            if(Setting.Music_Mode == 4)
			{
				Setting.Music_Mode = 1;
			}
			else
			{
				Setting.Music_Mode = Setting.Music_Mode + 1;
			}
			break;

		case 2:
            if(Setting.Music_Mode == 1)
			{
				Setting.Music_Mode = 4;
			}
			else
			{
				Setting.Music_Mode = Setting.Music_Mode - 1;
			}
			break;

		default: 
			break;
	}

	Display_Mode(Setting.Music_Mode);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void MUSIC_EfficeSwitch(u8 flag)
{
	u16 temp, bass;
	
	switch(flag)
	{
		case 0:
            Setting.Music_Effice = Setting.Music_Effice;
			break;
					
		case 1:
            if(Setting.Music_Effice == 5)
			{
				Setting.Music_Effice = 1;
			}
			else
			{
				Setting.Music_Effice = Setting.Music_Effice + 1;
			}
			break;

		case 2:
            if(Setting.Music_Effice == 1)
			{
				Setting.Music_Effice = 5;
			}
			else
			{
				Setting.Music_Effice = Setting.Music_Effice - 1;
			}
			break;

		default: 
			break;
	}

    temp = VS1053B_ReadReg(VS1053B_MODE);
	while(VS1053B_DREQ_State() == 0){;}
	switch(Setting.Music_Effice)
	{
		case 1:
            temp &= 0xff6f;
			bass = 0xf5bb;
			break;
					
		case 2:
            temp &= 0xff6f;
			bass = 0xff00;
			break;

		case 3:
            temp &= 0xff6f;
			bass = 0x5800;
			break;

		case 4:
            temp &= 0xff6f;
			bass = 0x58bb;
			break;

		case 5:
            temp &= 0xff7f;
			temp |= 0x0010;
			bass = 0xf5bb;
			break;
		
		default:
			break;
	}
	
	VS1053B_WriteReg(VS1053B_MODE, temp);
	while(VS1053B_DREQ_State() == 0){;}
		
    VS1053B_WriteReg(VS1053B_BASS, bass);
	while(VS1053B_DREQ_State() == 0){;}
	Display_Effect(Setting.Music_Effice);	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void MUSIC_VoiceSwitch(u8 flag)
{
	u8 choose;

	switch(flag)
	{
		case 0:
			Setting.Music_Voice = Setting.Music_Voice;
			break;
					
		case 1:
			if(Setting.Music_Voice == 30)
			{
				//Setting.Music_Voice = Setting.Music_Voice;
			}
			else
			{
				Setting.Music_Voice = Setting.Music_Voice + 1;
			}
			break;

		case 2:
			if(Setting.Music_Voice == 0)
			{
				//Setting.Music_Voice = Setting.Music_Voice;
			}
			else
			{
				Setting.Music_Voice = Setting.Music_Voice - 1;
			}
			break;

		default:
			break;
	}

	VS1053B_WriteReg(VS1053B_VOL, Setting.Music_Voice*2 + (Setting.Music_Voice*2 << 8));
	while(VS1053B_DREQ_State() == 0){;}
	Display_Voice(30-Setting.Music_Voice);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void MUSIC_PlaySong(FATFS_FileListInfoTypeDef *list)
{ 
	u8 i;
	u8 keyreturn;
	u8 endflag;
	D_KEY_StatusTypeDef key;
	list->FileCurSongNum = list->FileSongPos[list->FileCurNum];

	list->FileLayerNum[list->FileLayerTol] = list->FileSongNum[list->FileCurSongNum];
	list->FileLayerTol = list->FileLayerTol + 1;
	
	Display_Line();
	Display_Stop(1);
	MUSIC_VoiceSwitch(0);	
	MUSIC_ChooseSwitch(0);
	MUSIC_ModeSwitch(0);
	MUSIC_EfficeSwitch(0);
	
	endflag = 0;
	while(1)
	{
		if((list->FileType[list->FileSongNum[list->FileCurSongNum]] != FATFS_FILEISDIR) && (list->FileType[list->FileSongNum[list->FileCurSongNum]] != FATFS_FILEISOTHER))
		{
			if(endflag == 0)
			{
				MUSIC_DisplaySongName(list);
				
				*(list->FilePathName + list->FilePathLen) = '/';
				list->FilePathLen = list->FilePathLen + 1;
				memcpy(list->FilePathName + list->FilePathLen, list->FileNameBuff + list->FilePostion[list->FileSongNum[list->FileCurSongNum]], list->FileLen[list->FileSongNum[list->FileCurSongNum]]);
				list->FilePathLen = list->FilePathLen + list->FileLen[list->FileSongNum[list->FileCurSongNum]] - 1;
						
				keyreturn = MUSIC_Play(list->FilePathName, list->FileType[list->FileSongNum[list->FileCurSongNum]]);
				switch(keyreturn)
				{
					case  MUSIC_KEYRETURN_ISNEXT:
								//将当前文件路径后退到当前文件夹目录
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
								
								list->FileCurSongNum = list->FileCurSongNum + 1;
								if(list->FileCurSongNum == list->FileSongAll)
								{
									list->FileCurSongNum = 0;
								}							
								break;
								
					case  MUSIC_KEYRETURN_ISBEFORE:
							    //将当前文件路径后退到当前文件夹目录
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
								if(list->FileCurSongNum == 0)
								{
									list->FileCurSongNum = list->FileSongAll;
								}
								list->FileCurSongNum = list->FileCurSongNum - 1;
								break;

					case  MUSIC_KEYRETURN_ISBACK:
								return;
								break;

					case  MUSIC_KEYRETURN_ISEND:
								//将当前文件路径后退到当前文件夹目录
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
								switch(Setting.Music_Mode)
								{
									case 1:     //全部循环
										list->FileCurSongNum = list->FileCurSongNum + 1;
										if(list->FileCurSongNum == list->FileSongAll)
										{
											list->FileCurSongNum = 0;
										}									
										break;
									
									case 2:     //单曲循环
										list->FileCurSongNum = list->FileCurSongNum;
										break;
									
									case 3:     //全部顺序
										list->FileCurSongNum = list->FileCurSongNum + 1;
										if(list->FileCurSongNum == list->FileSongAll)
										{
											list->FileCurSongNum = 0;
										}
										endflag = 1;
										break;
									
									case 4:     //单曲顺序
										list->FileCurSongNum = list->FileCurSongNum;
										endflag = 1;
										break;
									
									default:
										break;
								}
								break;
					
					default: 
								break;
				}
			}
			else
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
                            return;
							break;

						case D_KEY_IS_NEXT:
							//将当前文件路径后退到当前文件夹目录
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
							
							list->FileCurSongNum = list->FileCurSongNum + 1;
							if(list->FileCurSongNum == list->FileSongAll)
							{
								list->FileCurSongNum = 0;
							}
							break;
									
						case D_KEY_IS_BEFORE:
							//将当前文件路径后退到当前文件夹目录
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
							if(list->FileCurSongNum == 0)
							{
								list->FileCurSongNum = list->FileSongAll;
							}
							list->FileCurSongNum = list->FileCurSongNum - 1;
							break;

						case D_KEY_IS_STOP:
								
							break;

						case D_KEY_IS_LOCK:

							break;
						
						default: 
							break;			
					}
				}				
			}
		}
		else
		{
			list->FileCurSongNum = list->FileCurSongNum + 1;
			if(list->FileCurSongNum == list->FileSongAll)
			{
				list->FileCurSongNum = 0;
			}	
		}
//		OLED_DisplayFileNameBack(list);
//		break;
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
void MUSIC_DisplaySongName(FATFS_FileListInfoTypeDef *list)
{ 
    Display_ClearSongNameZoom();
	Display_Num(list->FileSongAll, list->FileSongPos[list->FileSongNum[list->FileCurSongNum]] + 1);
	Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileSongNum[list->FileCurSongNum]]);
	Display_UpdateSongNameZoom();
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
u8 MUSIC_Play(u8 *path, u8 songtype)
{
	FRESULT res;      	// FatFs function common result code
	FATFS fs;          	// Work area (file system object) for logical drive
	FIL fil;           	// file objects
	UINT br;			    	// File R count 
	
	D_KEY_StatusTypeDef key;
	u16 j;
	u8 buffer[1024];          // file copy buffer
	u16 voice;
	u16 count;
	s64 timesong, timetemp;
	u8 flag;
	u8 playflag;
	u32 filesize;
	u32 playsize;
	float scale, timtotla;
	u8 timtemp, timbefore;
	u8 moveflag;
	u16 fast;
	
	SPI1_LowSpeed();
	//如果是flac音频文件，加载flac插件
	if(songtype == FATFS_FILEISFLAC)
	{
		VS1053B_WriteReg(VS1053B_CLOCKF, 0xf800);  // 设置vs1003的时钟,(5+2)倍频
		while(VS1053B_DREQ_State() == 0){;}
		VS1053B_FLAC_WritePlugin();
	}
	else
	{
		VS1053B_WriteReg(VS1053B_CLOCKF, 0xb800);  // 设置vs1003的时钟,(3+2)倍频
		while(VS1053B_DREQ_State() == 0){;}
		VS1053B_FLAC_WriteNull();
	}
	
	SPI1_HighSpeed(); 
	f_mount(0, &fs);			                          /* 挂载文件系统到0区 */
	res = f_open(&fil, (const TCHAR *)path, FA_OPEN_EXISTING | FA_READ); /* 以只读方式打开 */					
    if(res)	
	{
		Display_ErrorWithSD((u8 *)"Play_Error_SD");
	}
	
	filesize = f_size(&fil) / 1024;
	timesong = 0;
	timetemp = 0;
	timbefore = 0;
	flag = 0;
	playflag = 0;
	scale = 100.0f / (float)filesize;
	playsize = 0;
	Display_Stop(1);
	Display_PlayTime(0);
	VS1053B_DCS_Low();       /* 选择VS1003的数据接口	*/	
	/* ------------------- 一曲开始 --------------------*/
	while(1)
	{			
		f_lseek(&fil, playsize * 1024);
		res = f_read(&fil, buffer, 1024, &br);
		
		moveflag = 0;
		
		timtotla = scale * (float)playsize;
		timtemp = (u8)timtotla;
		if(timtemp != timbefore)
		{
			Display_PlayTime(timtemp+1);
			timbefore = timtemp;
		}
		if (res == FR_OK)
		{				
			count = 0;								/* 512字节完重新计数 */
			while (count < br)		  /* SD卡读取一个sector，一个sector为512字节 */	
			{ 						
				if((SPI1_GetBusyFlag() == 0x00) && (VS1053B_DREQ_State() != 0) && playflag == 0)
				{	
					//SPI发送32字节数据需要91us左右，时间很短无需用DMA模式
					//SPI1_SendDataDelegate(buffer+count, 32);
					for(j=0; j<32; j++)
					VS1053B_ReadWriteByte(buffer[count + j]);
					count = count + 32;
					flag = 1;
					LED_STA_Trun();
				}
				timetemp = MICROSEC_GetTime();
				if((timetemp - timesong) > 500000)
				{
					if(flag == 0)
					{
						Display_ErrorWithSD((u8 *)"Play_Error_VS");
					}
					timesong = timetemp;
				}
				key = KEY_Scan(20000);
				if(key.Active == D_KEY_ACTIVE_YES)
				{
					switch(key.Type)
					{
						case D_KEY_IS_VINC:
							if((key.State == D_KEY_UP) && (key.LongPushCnt < 20))
							{
								switch(Setting.Music_Choose)
								{
									case 1:
										MUSIC_ModeSwitch(1);
										break;
									
									case 2:
										SPI1_LowSpeed();
										while((SPI1_GetBusyFlag() != 0x00) || (VS1053B_DREQ_State() == 0)) {;}
										MUSIC_EfficeSwitch(1);
										SPI1_HighSpeed();
										VS1053B_DCS_Low();
										break;

									case 3:
										SPI1_LowSpeed();
										while((SPI1_GetBusyFlag() != 0x00) || (VS1053B_DREQ_State() == 0)) {;}
										MUSIC_VoiceSwitch(1);
										SPI1_HighSpeed();
										VS1053B_DCS_Low();
										break;
									
									default: 
										break;					
								}
							}
							else if((key.State == D_KEY_LONGDOWN) && (key.LongPushCnt > 20) && (((key.LongPushCnt - 20) % 5) == 0))
							{
								if(Setting.Music_Choose == 3)
								{
									SPI1_LowSpeed();
									while((SPI1_GetBusyFlag() != 0x00) || (VS1053B_DREQ_State() == 0)) {;}
									MUSIC_VoiceSwitch(1);
									SPI1_HighSpeed();
									VS1053B_DCS_Low();									
								}
							}
							else
							{;}
							break;
									
						case D_KEY_IS_VDEC:
							if((key.State == D_KEY_UP) && (key.LongPushCnt < 20))
							{
								switch(Setting.Music_Choose)
								{
									case 1:
										MUSIC_ModeSwitch(2);
										break;
									
									case 2:
										SPI1_LowSpeed();
										while((SPI1_GetBusyFlag() != 0x00) || (VS1053B_DREQ_State() == 0)) {;}
										MUSIC_EfficeSwitch(2);
										SPI1_HighSpeed();
										VS1053B_DCS_Low();
										break;

									case 3:
										SPI1_LowSpeed();
										while((SPI1_GetBusyFlag() != 0x00) || (VS1053B_DREQ_State() == 0)) {;}
										MUSIC_VoiceSwitch(2);
										SPI1_HighSpeed();
										VS1053B_DCS_Low();
										break;
									
									default: 
										break;					
								}
							}
							else if((key.State == D_KEY_LONGDOWN) && (key.LongPushCnt > 20) && (((key.LongPushCnt - 20) % 5) == 0))
							{
								if(Setting.Music_Choose == 3)
								{
									SPI1_LowSpeed();
									while((SPI1_GetBusyFlag() != 0x00) || (VS1053B_DREQ_State() == 0)) {;}
									MUSIC_VoiceSwitch(2);
									SPI1_HighSpeed();
									VS1053B_DCS_Low();									
								}
							}
							else
							{;}
							break;

						case D_KEY_IS_POWER:
							if((key.State == D_KEY_UP) && (key.LongPushCnt < 20))
							{
								MUSIC_ChooseSwitch(1);
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
								while((SPI1_GetBusyFlag() != 0x00) || (VS1053B_DREQ_State() == 0)) {;}
								SPI1_LowSpeed();
								MUSIC_CancelPlay();
								VS1053B_DCS_High();		// 关闭VS1003数据端口
								f_close(&fil);			// 关闭打开的文件																
								return MUSIC_KEYRETURN_ISBACK;
							}
							else if((key.State == D_KEY_LONGDOWN) && (key.LongPushCnt > 20) && (((key.LongPushCnt - 20) % 5) == 0))
							{
							}
							else
							{;}
							break;
                            
						case D_KEY_IS_NEXT:
							if((key.State == D_KEY_UP) && (key.LongPushCnt < 20))
							{
								while((SPI1_GetBusyFlag() != 0x00) || (VS1053B_DREQ_State() == 0)) {;}
								SPI1_LowSpeed();
								MUSIC_CancelPlay();
								VS1053B_DCS_High();		// 关闭VS1003数据端口
								f_close(&fil);			// 关闭打开的文件																
								return MUSIC_KEYRETURN_ISNEXT;
							}
							else if((key.State == D_KEY_LONGDOWN) && (key.LongPushCnt > 20) && (((key.LongPushCnt - 20) % 5) == 0))
							{
								fast = 0;
								while(1)
								{
									key = KEY_Scan(50000);
									if(key.Active == D_KEY_ACTIVE_YES)
									{
										if((key.State == D_KEY_LONGDOWN) && (key.LongPushCnt > 20) && (((key.LongPushCnt - 20) % 5) == 0))
										{
											playsize += fast * 3;
										}
										else if(key.State == D_KEY_UP)
										{
											moveflag = 1;
						
											while((SPI1_GetBusyFlag() != 0x00) || (VS1053B_DREQ_State() == 0)) {;}
											SPI1_LowSpeed();
											MUSIC_FastForward();
											SPI1_HighSpeed();
											VS1053B_DCS_Low();
											break;
										}
										else
										{;}
										fast += 1;
									}
									timtotla = scale * (float)playsize;
									timtemp = (u8)timtotla;
									if(timtemp != timbefore)
									{
										Display_PlayTime(timtemp+1);
										timbefore = timtemp;
									}									
								}
							}
							else
							{;}
							break;

						case D_KEY_IS_BEFORE:
							if((key.State == D_KEY_UP) && (key.LongPushCnt < 20))
							{
								while((SPI1_GetBusyFlag() != 0x00) || (VS1053B_DREQ_State() == 0)) {;}
								SPI1_LowSpeed();
								MUSIC_CancelPlay();
								VS1053B_DCS_High();		// 关闭VS1003数据端口
								f_close(&fil);			// 关闭打开的文件																
								return MUSIC_KEYRETURN_ISBEFORE;
							}
							else if((key.State == D_KEY_LONGDOWN) && (key.LongPushCnt > 20) && (((key.LongPushCnt - 20) % 5) == 0))
							{
								fast = 0;
								while(1)
								{
									key = KEY_Scan(50000);
									if(key.Active == D_KEY_ACTIVE_YES)
									{
										if((key.State == D_KEY_LONGDOWN) && (key.LongPushCnt > 20) && (((key.LongPushCnt - 20) % 5) == 0))
										{
											playsize -= fast * 3;
										}
										else if(key.State == D_KEY_UP)
										{
											moveflag = 1;
						
											while((SPI1_GetBusyFlag() != 0x00) || (VS1053B_DREQ_State() == 0)) {;}
											SPI1_LowSpeed();
											MUSIC_FastForward();
											SPI1_HighSpeed();
											VS1053B_DCS_Low();
											break;
										}
										else
										{;}
										fast += 1;
									}
									timtotla = scale * (float)playsize;
									timtemp = (u8)timtotla;
									if(timtemp != timbefore)
									{
										Display_PlayTime(timtemp+1);
										timbefore = timtemp;
									}									
								}								
							}
							else
							{;}
							break;

						case D_KEY_IS_STOP:
							if((key.State == D_KEY_UP) && (key.LongPushCnt < 20))
							{
								if(playflag == 0)
								{
									Display_Stop(2);
									playflag = 1;
								}
								else
								{
									Display_Stop(1);
									playflag = 0;									
								}
							}
							else if((key.State == D_KEY_LONGDOWN) && (key.LongPushCnt > 20) && (((key.LongPushCnt - 20) % 15) == 0))
							{
							}
							else
							{;}
							break;

						case D_KEY_IS_LOCK:

							break;
						
						default: 
							break;			
					}
				}
				else
				{
					
				}
				if(moveflag == 1)
				{
					break;
				}
			}
			playsize += 1;
		}
		if (res || br == 0) break;   /* 出错或者到了文件尾 */		   
	}
	
	while((SPI1_GetBusyFlag() != 0x00) || (VS1053B_DREQ_State() == 0)) {;}
	SPI1_LowSpeed();
	MUSIC_CancelPlay();
	VS1053B_DCS_High();		// 关闭VS1003数据端口
	f_close(&fil);			// 关闭打开的文件
	return MUSIC_KEYRETURN_ISEND;
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void MUSIC_FastForward(void)
{
	u16 endfillbyte;
	u16 i;
	
	endfillbyte = VS1053B_ReadMemory(0x1e06);
	VS1053B_DCS_Low();		                  //xDCS = 1
	VS1053B_CS_High();		                  //xCS = 0
	for(i=0; i<4096; i++)
	{
		VS1053B_ReadWriteByte(endfillbyte & 0x00ff);
	}
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
void MUSIC_CancelPlay(void)
{
	u16 endfillbyte;
	u16 datatemp;
	u16 i;
	
	//发送取消解码命令
	VS1053B_WriteReg(VS1053B_MODE, 0x0808);     
	while(VS1053B_DREQ_State() == 0){;}
	
	//继续发送32字节数据
	VS1053B_DCS_Low();		                  //xDCS = 1
	VS1053B_CS_High();		                  //xCS = 0
	for(i=0; i<32; i++)
	{
		VS1053B_ReadWriteByte(0);
	}
	VS1053B_DCS_High();		                  //xDCS = 1
	while(VS1053B_DREQ_State() == 0){;}
	
	
	datatemp = VS1053B_ReadReg(VS1053B_MODE);     
	while(VS1053B_DREQ_State() == 0){;}
	if((datatemp & 0x0080) != 0)
	{
		VS1053B_DCS_Low();		                  //xDCS = 1
		VS1053B_CS_High();		                  //xCS = 0
		for(i=0; i<32; i++)
		{
			VS1053B_ReadWriteByte(0);
		}
		VS1053B_DCS_High();		                  //xDCS = 1
		while(VS1053B_DREQ_State() == 0){;}
			
		datatemp = VS1053B_ReadReg(VS1053B_MODE);     
		while(VS1053B_DREQ_State() == 0){;}
		if((datatemp & 0x0080) != 0)
		{
			VS1053B_DCS_Low();		                  //xDCS = 1
			VS1053B_CS_High();		                  //xCS = 0
			for(i=0; i<2048; i++)
			{
				VS1053B_ReadWriteByte(0);
			}
			VS1053B_DCS_High();		                  //xDCS = 1
			while(VS1053B_DREQ_State() == 0){;}
			VS1053B_SofeReset();
		}			
	}
	endfillbyte = VS1053B_ReadMemory(0x1e06);
	VS1053B_DCS_Low();		                  //xDCS = 1
	VS1053B_CS_High();		                  //xCS = 0
	for(i=0; i<2052; i++)
	{
		VS1053B_ReadWriteByte(endfillbyte & 0x00ff);
	}
	VS1053B_DCS_High();		                  //xDCS = 1
	while(VS1053B_DREQ_State() == 0){;}	
	datatemp = VS1053B_ReadReg(VS1053B_HDAT0);     
	while(VS1053B_DREQ_State() == 0){;}
	if(datatemp != 0)
	{
		//Display_Error(7);
	}
	datatemp = VS1053B_ReadReg(VS1053B_HDAT1);     
	while(VS1053B_DREQ_State() == 0){;}
	if(datatemp != 0)
	{
		//Display_Error(7);
	}
}




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
