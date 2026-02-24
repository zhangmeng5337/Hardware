
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : fatfs.h
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : Header for fatfs.c module
* Description    : --
**********************************************************************/

/* Define to prevent recursive inclusion ----------------------------*/
#ifndef __FATFS_H
#define __FATFS_H

 

/* Includes  == 文件包含 --------------------------------------------*/
#include "stm32f10x.h"
#include "ff.h"
#include "diskio.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

#define FATFS_MAX_FILE_NUM          200    //一个目录下最多文件及文件夹数
#define FATFS_MAX_LAYER_NUM         10     //文件目录最多嵌套层数
#define FATFS_MAX_FILE_PATH_NAME    250    //一个文件路径的最多长度, 如 /mp3/song/chiansong/吻别.MP3
#define FATFS_MAX_FILE_NAME_TOTAL   2048   //一个目录所有文件名及文件夹名的总长度

//-----------------------------------------//
typedef struct
{
	//-------------------------------------//
	//当前目录下的文件大概信息
	u8		FileQuantity;			//当前目录下的总文件数
	u8		FileNameBuff[FATFS_MAX_FILE_NAME_TOTAL]; 	//当前目录的全部文件名存放缓存，每个文件名以'\0'结束
	u32		FileNameLen;         	//当前目录全部文件名的总长度	
	//-------------------------------------//
	//当前目录信息
	u8		FilePathName[FATFS_MAX_FILE_PATH_NAME];   	//当前路径名
	u8		FilePathLen;         	//当前路径名长度
	u8		FileCurNum;     		//当前被选中的文件编号	
	//-------------------------------------//
	//当前目录下的文件具体信息
	u8		FileLen[FATFS_MAX_FILE_NUM];     		//表示第N个文件名的长度
	u8		FileType[FATFS_MAX_FILE_NUM];    		//表示第N个文件的类型
	u32		FilePostion[FATFS_MAX_FILE_NUM];    	//表示第N个文件名存放在FileNameBuff的位置
	u8		FileSongPos[FATFS_MAX_FILE_NUM];    	//当前目录下的可播放歌曲相对于总可播放歌曲的位置
	//-------------------------------------//
	//目录嵌套层信息
	u8		FileLayerTol;        	//文件路径层数
	u8		FileLayerNum[FATFS_MAX_LAYER_NUM];   	//文件路径每一层的被选中的文件编号
	//-------------------------------------//
	//当前目录可播放歌曲信息
	u8		FileCurSongNum;			//当前音乐的编号
	u8		FileSongAll;			//当前目录下的可播放歌曲总数
	u8		FileSongNum[FATFS_MAX_FILE_NUM];		//当前目录下的可播放歌曲相对于总文件目录的位置
}FATFS_FileListInfoTypeDef;

#define	FATFS_FILEISDIR			0x01
#define	FATFS_FILEISMP3			0x02
#define	FATFS_FILEISWMA			0x03
#define	FATFS_FILEISWAV			0x04
#define	FATFS_FILEISFLAC		0x05
#define	FATFS_FILEISOTHER		0x06



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/



/* Private function prototypes  ==  私有函数声明 --------------------*/
void FATFS_Die(FRESULT rc);
void FATFS_GetChinaDocData(u8 *buff, u8 size, u8 *china);
void FATFS_FileSacn(u8 *path, FATFS_FileListInfoTypeDef *filelist);
void FATFS_DisplayFileName(FATFS_FileListInfoTypeDef *list);
void FATFS_DisplayFileNameBefore(FATFS_FileListInfoTypeDef *list);
void FATFS_DisplayFileNameNext(FATFS_FileListInfoTypeDef *list);
void FATFS_DisplayFileNameBack(FATFS_FileListInfoTypeDef *list);
void FATFS_DisplayFileNameMove(FATFS_FileListInfoTypeDef *list);



/* Define to prevent recursive inclusion ----------------------------*/
#endif /* __FATFS_H */




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
