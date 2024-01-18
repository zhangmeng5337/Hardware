
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : fatfs.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : fatfs program body
* Description    : --
**********************************************************************/

/* Includes  == �ļ����� --------------------------------------------*/
#include "fatfs.h"
#include "ff.h"
#include "diskio.h"
#include <stdio.h>
#include <string.h>

#include "display.h"
#include "microsec.h"



/* Private typedef    ==  ˽������ ----------------------------------*/
/* Private define     ==  ˽�ж��� ----------------------------------*/

//-----------------------------------------//
#define   FATFS_DISPLAYTIME		200



/* Private macro      ==  ˽�к� ------------------------------------*/
/* Private variables  ==  ˽�б��� ----------------------------------*/

	
	
/* Private functions  ==  ˽�к������� ------------------------------*/

/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void FATFS_Die(FRESULT res)
{
	while(1);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void FATFS_GetChinaDocData(u8 *buff, u8 size, u8 *china)
{ 
	FRESULT res;                                      	// FatFs function common result code
	FATFS fs;                                        	// Work area (file system object) for logical drive
	FIL fil;                                         	// file objects
	UINT br;			                                  	// File R count
	u8 region, location;
	u32 index;
	
	region   = (*china)-0x81;                       	//�õ�����  gb2312 
	location = (*(china+1))-0x41;                   	//�õ�λ�� 
	index = ((0xbe*region)+location) * ((size*size)/8);//�ֿ������ڵ�ַ����Ϊÿ��������0xbe�����֣�
																		//ÿ����������Ϊsize*size����8�������Ϊһ
																		//�������� 
	f_mount(0, &fs);                                  	//����һ���ļ�ϵͳ�ṹ
	if(size == 16)
		res = f_open(&fil, "system/songti16.DZK", FA_OPEN_EXISTING | FA_READ);  //���ֿ��ļ�
	else if(size == 32)
		res = f_open(&fil, "system/songti16.DZK", FA_OPEN_EXISTING | FA_READ);  //���ֿ��ļ�
	else
	{;}
	if (res) FATFS_Die(res);		                       	//������ֿⲻ�ɹ������ش����
	
	res = f_lseek(&fil, index);                      	//�ҵ�Ҫ��ʾ�������ֿ��ļ����ƫ�Ƶ�ַ����λ
	if (res) FATFS_Die(res);
	
	res = f_read(&fil, buff, size*size/8, &br);   		//��ȡsize*size��С�ĺ��ֵ������ݵ�GBK[]������
	if (res) FATFS_Die(res);
	
	res = f_close(&fil);		                          	//�ر��ļ�ϵͳ
	if (res) FATFS_Die(res);									  
	
	res = f_mount(0, NULL);
	if (res) FATFS_Die(res);	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void FATFS_FileSacn(u8 *path, FATFS_FileListInfoTypeDef *filelist)
{
	FRESULT res;                                      	// FatFs function common result code
	FATFS fs;                                        	// Work area (file system object) for logical drive
	FIL fil;                                         	// file objects
	UINT br;			                                  	// File R count    
	FILINFO fno;
	DIR dir;
	
	u8 i;
	u8 char_num;
	u8 f_name[10];
	u8 f_name_cnt;
	u8 f_tmp;

	char *fn;   /* This function is assuming non-Unicode cfg. */
	#if _USE_LFN
		static char lfn[_MAX_LFN + 1];
		fno.lfname = lfn;
		fno.lfsize = sizeof lfn;
	#endif

	filelist->FileQuantity		= 0;
	filelist->FileCurNum 		= 0;
	filelist->FileNameLen		= 0;
	filelist->FileSongAll      = 0;
	
	f_mount(0, &fs);		                                 //Register volume work area (never fails)
	res = f_opendir(&dir, (const TCHAR*)path);                             //Open the directory 
	if (res == FR_OK) 
	{
		for (;;) 
		{
			res = f_readdir(&dir, &fno);                     //Read a directory item 
			if (res != FR_OK || fno.fname[0] == 0) break;    //Break on error or end of dir 
			if (fno.fname[0] == '.') continue;               //Ignore dot entry 
			
			#if _USE_LFN
				fn = *fno.lfname ? fno.lfname : fno.fname;
			#else
				fn = fno.fname;
			#endif

			char_num=0;
			while((*(fn+char_num))!='\0')				 //��Ŀ¼�����浽�ļ��б�����f_list��
			{
				filelist->FileNameBuff[filelist->FileNameLen] = *(fn+char_num);
				char_num=char_num+1;
				filelist->FileNameLen = filelist->FileNameLen + 1;							 
			}
			filelist->FileNameBuff[filelist->FileNameLen] = '\0';
			filelist->FileNameLen = filelist->FileNameLen + 1;
			char_num = char_num+1;
								
			//����ǰĿ¼�µ�ȫ���ļ������浽�ļ��б�����f_list�У���ౣ��40��������˵
			//һ��Ŀ¼�µ��ļ�(�����ļ���)���ܳ���40��������40���ļ������ᱻ����
			if (fno.fattrib & AM_DIR) 
			{                                                //It is a directory
				filelist->FileType[filelist->FileQuantity] = FATFS_FILEISDIR;		
			} 
			else 
			{                                                //It is a file. 
				f_tmp = 0;
				f_name_cnt = 0;
				//�������ļ���β
				while((*(fn+f_name_cnt))!='\0')				 
				{
					f_name_cnt++;
				}
				//���ļ���.����ĺ�׺�浽f_name��
				f_name[0] = *(fn+f_name_cnt-3);
				f_name[1] = *(fn+f_name_cnt-2);
				f_name[2] = *(fn+f_name_cnt-1);

			   if(f_name[0] == 'm' && f_name[1] =='p' && f_name[2] == '3')
			   {
			      filelist->FileType[filelist->FileQuantity] = FATFS_FILEISMP3;
					filelist->FileSongPos[filelist->FileQuantity] = filelist->FileSongAll;
					filelist->FileSongNum[filelist->FileSongAll] = filelist->FileQuantity;
					filelist->FileSongAll = filelist->FileSongAll + 1;
			   }
			   else if(f_name[0] == 'M' && f_name[1] =='P' && f_name[2] == '3')
			   {
					filelist->FileType[filelist->FileQuantity] = FATFS_FILEISMP3;
					filelist->FileSongPos[filelist->FileQuantity] = filelist->FileSongAll;
					filelist->FileSongNum[filelist->FileSongAll] = filelist->FileQuantity;					
					filelist->FileSongAll = filelist->FileSongAll + 1;
			   }
			   else if(f_name[0] == 'w' && f_name[1] =='m' && f_name[2] == 'a')
			   {
					filelist->FileType[filelist->FileQuantity] = FATFS_FILEISWMA;
					filelist->FileSongPos[filelist->FileQuantity] = filelist->FileSongAll;
					filelist->FileSongNum[filelist->FileSongAll] = filelist->FileQuantity;					
					filelist->FileSongAll = filelist->FileSongAll + 1;
			   }
			   else if(f_name[0] == 'W' && f_name[1] =='M' && f_name[2] == 'A')
			   {
					filelist->FileType[filelist->FileQuantity] = FATFS_FILEISWMA;
					filelist->FileSongPos[filelist->FileQuantity] = filelist->FileSongAll;
					filelist->FileSongNum[filelist->FileSongAll] = filelist->FileQuantity;					
					filelist->FileSongAll = filelist->FileSongAll + 1;
			   }
			   else if(f_name[0] == 'w' && f_name[1] =='a' && f_name[2] == 'v')
			   {
					filelist->FileType[filelist->FileQuantity] = FATFS_FILEISWAV;
					filelist->FileSongPos[filelist->FileQuantity] = filelist->FileSongAll;
					filelist->FileSongNum[filelist->FileSongAll] = filelist->FileQuantity;					
					filelist->FileSongAll = filelist->FileSongAll + 1;
			   }
			   else if(f_name[0] == 'W' && f_name[1] =='A' && f_name[2] == 'V')
			   {
					filelist->FileType[filelist->FileQuantity] = FATFS_FILEISWAV;
					filelist->FileSongPos[filelist->FileQuantity] = filelist->FileSongAll;
					filelist->FileSongNum[filelist->FileSongAll] = filelist->FileQuantity;					
					filelist->FileSongAll = filelist->FileSongAll + 1;
			   }
			   else if(f_name[0] == 'f' && f_name[1] =='l' && f_name[2] == 'a' && f_name[3] == 'c')
			   {
					filelist->FileType[filelist->FileQuantity] = FATFS_FILEISFLAC;
					filelist->FileSongPos[filelist->FileQuantity] = filelist->FileSongAll;
					filelist->FileSongNum[filelist->FileSongAll] = filelist->FileQuantity;					
					filelist->FileSongAll = filelist->FileSongAll + 1;
			   }
			   else if(f_name[0] == 'F' && f_name[1] =='L' && f_name[2] == 'A' && f_name[3] == 'C')
			   {
					filelist->FileType[filelist->FileQuantity] = FATFS_FILEISFLAC;
					filelist->FileSongPos[filelist->FileQuantity] = filelist->FileSongAll;
					filelist->FileSongNum[filelist->FileSongAll] = filelist->FileQuantity;					
					filelist->FileSongAll = filelist->FileSongAll + 1;
			   }
			   else
			   {
					filelist->FileType[filelist->FileQuantity] = FATFS_FILEISOTHER;
			   }			  
			}

			filelist->FilePostion[filelist->FileQuantity] = filelist->FileNameLen - char_num;
			filelist->FileLen[filelist->FileQuantity] = char_num;
			filelist->FileQuantity = filelist->FileQuantity + 1;				
		}
	}
	f_mount(0, NULL);	
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void FATFS_DisplayFileName(FATFS_FileListInfoTypeDef *list)
{
	Display_ClearFileNameZoom();
	
	if(list->FileQuantity == 0)
	{
	}
	else if(list->FileQuantity == 1)
	{
		Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
		Display_TurnColor(2);
		Display_TurnColor(3);
	}
	else if(list->FileQuantity == 2)
	{
		Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
		Display_String(0, 4, list->FileNameBuff + list->FilePostion[1]);		
		if(list->FileCurNum == 0)
		{
			Display_TurnColor(2);
			Display_TurnColor(3);			
		}
		else
		{
			Display_TurnColor(4);
			Display_TurnColor(5);			
		}	
	}
	else
	{
		if(list->FileCurNum == 0)
		{
			Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
			Display_String(0, 4, list->FileNameBuff + list->FilePostion[1]);
			Display_String(0, 6, list->FileNameBuff + list->FilePostion[2]);
			Display_TurnColor(2);
			Display_TurnColor(3);				
		}
      else if(list->FileCurNum == (list->FileQuantity - 1))
		{
			Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileQuantity - 3]);
			Display_String(0, 4, list->FileNameBuff + list->FilePostion[list->FileQuantity - 2]);
			Display_String(0, 6, list->FileNameBuff + list->FilePostion[list->FileQuantity - 1]);				
			Display_TurnColor(6);
			Display_TurnColor(7);			
		}
      else
		{
			Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileCurNum - 1]);
			Display_String(0, 4, list->FileNameBuff + list->FilePostion[list->FileCurNum]);
			Display_String(0, 6, list->FileNameBuff + list->FilePostion[list->FileCurNum + 1]);			
			Display_TurnColor(4);
			Display_TurnColor(5);			
		}
	}
	Display_UpdateFileNameZoom();
	//Display_Num(list.FileQuantity, list.FileCurNum + 1);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void FATFS_DisplayFileNameBefore(FATFS_FileListInfoTypeDef *list)
{
	Display_ClearFileNameZoom();
		
	if(list->FileQuantity == 0)
	{
	}
	else if(list->FileQuantity == 1)
	{
		Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
		Display_TurnColor(2);
		Display_TurnColor(3);		
	}
	else if(list->FileQuantity == 2)
	{
		Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
		Display_String(0, 4, list->FileNameBuff + list->FilePostion[1]);		
		if(list->FileCurNum == 0)
		{
			Display_TurnColor(4);
			Display_TurnColor(5);
			list->FileCurNum = 1;
		}
		else
		{
			Display_TurnColor(2);
			Display_TurnColor(3);
			list->FileCurNum = 0;
		}
		MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
		Display_UpdateFileNameZoom();
	}
	else if(list->FileQuantity == 3)
	{
		Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
		Display_String(0, 4, list->FileNameBuff + list->FilePostion[1]);
		Display_String(0, 6, list->FileNameBuff + list->FilePostion[2]);		
		if(list->FileCurNum == 0)
		{
			Display_TurnColor(6);
			Display_TurnColor(7);
			list->FileCurNum = 2;
		}
		else if(list->FileCurNum == 1)
		{
			Display_TurnColor(2);
			Display_TurnColor(3);
			list->FileCurNum = 0;
		}
		else if(list->FileCurNum == 2)
		{
			Display_TurnColor(4);
			Display_TurnColor(5);
			list->FileCurNum = 1;
		}
		else
		{;}
		MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
		Display_UpdateFileNameZoom();		
	}
	else
	{
		if(list->FileCurNum == 0)
		{
			Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileQuantity - 3]);
			Display_String(0, 4, list->FileNameBuff + list->FilePostion[list->FileQuantity - 2]);
			Display_String(0, 6, list->FileNameBuff + list->FilePostion[list->FileQuantity - 1]);				
			Display_TurnColor(6);
			Display_TurnColor(7);
			list->FileCurNum = list->FileQuantity - 1;
			MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
			Display_UpdateFileNameZoom();	
		}
		else if(list->FileCurNum == 1)
		{
			Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
			Display_String(0, 4, list->FileNameBuff + list->FilePostion[1]);
			Display_String(0, 6, list->FileNameBuff + list->FilePostion[2]);				
			Display_TurnColor(2);
			Display_TurnColor(3);
			list->FileCurNum = 0;
			MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
			Display_UpdateFileNameZoom();				
		}
		else if(list->FileCurNum == (list->FileQuantity - 1))
		{
			Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileQuantity - 3]);
			Display_String(0, 4, list->FileNameBuff + list->FilePostion[list->FileQuantity - 2]);
			Display_String(0, 6, list->FileNameBuff + list->FilePostion[list->FileQuantity - 1]);
			Display_TurnColor(4);
			Display_TurnColor(5);
			list->FileCurNum = list->FileQuantity - 2;
			MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
			Display_UpdateFileNameZoom();	
		}
		else
		{
			Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileCurNum - 1]);
			Display_String(0, 4, list->FileNameBuff + list->FilePostion[list->FileCurNum - 0]);
			Display_String(0, 6, list->FileNameBuff + list->FilePostion[list->FileCurNum + 1]);				
			Display_TurnColor(2);
			Display_TurnColor(3);
			MICROSEC_Delay_ms(FATFS_DISPLAYTIME / 2);
			Display_UpdateFileNameZoom();
			Display_ClearFileNameZoom();
			Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileCurNum - 2]);
			Display_String(0, 4, list->FileNameBuff + list->FilePostion[list->FileCurNum - 1]);
			Display_String(0, 6, list->FileNameBuff + list->FilePostion[list->FileCurNum]);				
			Display_TurnColor(4);
			Display_TurnColor(5);
			list->FileCurNum = list->FileCurNum - 1;
			MICROSEC_Delay_ms(FATFS_DISPLAYTIME / 2);
			Display_UpdateFileNameZoom();			
		}
	}
	//Display_Num(list->FileQuantity, list->FileCurNum + 1);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void FATFS_DisplayFileNameNext(FATFS_FileListInfoTypeDef *list)
{
	Display_ClearFileNameZoom();
		
	if(list->FileQuantity == 0)
	{
	}
	else if(list->FileQuantity == 1)
	{
		Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
		Display_TurnColor(2);
		Display_TurnColor(3);		
	}
	else if(list->FileQuantity == 2)
	{
		Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
		Display_String(0, 4, list->FileNameBuff + list->FilePostion[1]);		
		if(list->FileCurNum == 0)
		{
			Display_TurnColor(4);
			Display_TurnColor(5);
			list->FileCurNum = 1;
		}
		else
		{
			Display_TurnColor(2);
			Display_TurnColor(3);
			list->FileCurNum = 0;
		}
		MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
		Display_UpdateFileNameZoom();
	}
	else if(list->FileQuantity == 3)
	{
		Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
		Display_String(0, 4, list->FileNameBuff + list->FilePostion[1]);
		Display_String(0, 6, list->FileNameBuff + list->FilePostion[2]);		
		if(list->FileCurNum == 0)
		{
			Display_TurnColor(4);
			Display_TurnColor(5);
			list->FileCurNum = 1;
		}
		else if(list->FileCurNum == 1)
		{
			Display_TurnColor(6);
			Display_TurnColor(7);
			list->FileCurNum = 2;
		}
		else if(list->FileCurNum == 2)
		{
			Display_TurnColor(2);
			Display_TurnColor(3);
			list->FileCurNum = 0;
		}
		else
		{;}
		MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
		Display_UpdateFileNameZoom();		
	}
	else
	{
		if(list->FileCurNum == 0)
		{
			Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
			Display_String(0, 4, list->FileNameBuff + list->FilePostion[1]);
			Display_String(0, 6, list->FileNameBuff + list->FilePostion[2]);
			Display_TurnColor(4);
			Display_TurnColor(5);
			list->FileCurNum = 1;
			MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
			Display_UpdateFileNameZoom();	
		}
		else if(list->FileCurNum == (list->FileQuantity - 2))
		{
			Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileQuantity - 3]);
			Display_String(0, 4, list->FileNameBuff + list->FilePostion[list->FileQuantity - 2]);
			Display_String(0, 6, list->FileNameBuff + list->FilePostion[list->FileQuantity - 1]);				
			Display_TurnColor(6);
			Display_TurnColor(7);
			list->FileCurNum = list->FileQuantity - 1;
			MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
			Display_UpdateFileNameZoom();						
		}
		else if(list->FileCurNum == (list->FileQuantity - 1))
		{
			Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
			Display_String(0, 4, list->FileNameBuff + list->FilePostion[1]);
			Display_String(0, 6, list->FileNameBuff + list->FilePostion[2]);
			Display_TurnColor(2);
			Display_TurnColor(3);
			list->FileCurNum = 0;
			MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
			Display_UpdateFileNameZoom();					
		}
		else
		{
			Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileCurNum - 1]);
			Display_String(0, 4, list->FileNameBuff + list->FilePostion[list->FileCurNum - 0]);
			Display_String(0, 6, list->FileNameBuff + list->FilePostion[list->FileCurNum + 1]);				
			Display_TurnColor(6);
			Display_TurnColor(7);
			MICROSEC_Delay_ms(FATFS_DISPLAYTIME / 2);
			Display_UpdateFileNameZoom();
			Display_ClearFileNameZoom();
			Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileCurNum ]);
			Display_String(0, 4, list->FileNameBuff + list->FilePostion[list->FileCurNum + 1]);
			Display_String(0, 6, list->FileNameBuff + list->FilePostion[list->FileCurNum + 2]);				
			Display_TurnColor(4);
			Display_TurnColor(5);
			list->FileCurNum = list->FileCurNum + 1;
			MICROSEC_Delay_ms(FATFS_DISPLAYTIME / 2);
			Display_UpdateFileNameZoom();			
		}
	}
	//Display_Num(list->FileQuantity, list->FileCurNum + 1);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void FATFS_DisplayFileNameBack(FATFS_FileListInfoTypeDef *list)
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
	FATFS_DisplayFileName(list);
	//Display_Num(list->FileQuantity, list->FileCurNum + 1);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void FATFS_DisplayFileNameMove(FATFS_FileListInfoTypeDef *list)
{
	*(list->FilePathName + list->FilePathLen) = '/';
	list->FilePathLen = list->FilePathLen + 1;
	memcpy(list->FilePathName + list->FilePathLen, list->FileNameBuff + list->FilePostion[list->FileCurNum], list->FileLen[list->FileCurNum]);
	list->FilePathLen = list->FilePathLen + list->FileLen[list->FileCurNum] - 1;
	
	list->FileLayerNum[list->FileLayerTol] = list->FileCurNum;
	list->FileLayerTol = list->FileLayerTol + 1;
	
	FATFS_FileSacn(list->FilePathName, list);
	FATFS_DisplayFileName(list);
	//Display_Num(list->FileQuantity, list->FileCurNum + 1);
}




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
