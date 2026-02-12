
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : playsong.h
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : Header for playsong.c module
* Description    : --
**********************************************************************/

/* Define to prevent recursive inclusion ----------------------------*/
#ifndef __PLAYSONG_H
#define __PLAYSONG_H

 

/* Includes  == 文件包含 --------------------------------------------*/
#include "stm32f10x.h"
#include "fatfs.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//
//一个音乐文件的数据由扇区组成，一个扇区由页组成
//一个扇区为1024字节，一个页为32字节，一个扇区包含32页（除了最后一个扇区例外）
//扇区为从SD卡里读音乐文件数据的最小单位
//页为STM32往VS1053B发送数据的最小单位
typedef struct
{
	u8		Music_PathBuff[255];      		//正在播放的音乐目录路径名
	u8		Music_PathLen;            		//正在播放的音乐目录路径名长度，以字节为单位
	u8		Music_SectorBuff[1024]; 		//一个扇区的音乐数据缓存
	u32	Music_SectorAllNum;       		//总的音乐数据扇区数
	u32	Music_SectorCurNum;       		//当前的音乐数据扇区编号
	u8 	Music_PageLastNum;        		//最后一个音乐数据扇区的页数目
	u8		Music_PageCurNum;        		//当前的音乐数据的页编号
	u8		Music_Speed;						//标志当前音乐是否可以快进
	u8		Music_PlayState;           	//标志当前音乐是否播放状态
	u32	Music_PlayTime;					//音乐播放时间，以秒为单位
}MUSIC_InfoTypeDef;

#define	MUSIC_PLAYSTATE_ISNEW			0x01
#define	MUSIC_PLAYSTATE_ISPLAYING		0x02
#define	MUSIC_PLAYSTATE_ISSPEED			0x03
#define	MUSIC_PLAYSTATE_ISPAUSE			0x04
#define	MUSIC_PLAYSTATE_ISPAGEFINISH	0x05
#define	MUSIC_PLAYSTATE_ISSONGFINISH	0x06

#define	MUSIC_KEYRETURN_ISNEXT			0x07
#define	MUSIC_KEYRETURN_ISBEFORE		0x08
#define	MUSIC_KEYRETURN_ISBACK			0x09
#define	MUSIC_KEYRETURN_ISEND			0x0A



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/
extern	MUSIC_InfoTypeDef		MUSIC_Info;



/* Private function prototypes  ==  私有函数声明 --------------------*/
void MUSIC_Init(void);
void MUSIC_ChooseSwitch(u8 flag);
void MUSIC_ModeSwitch(u8 flag);
void MUSIC_EfficeSwitch(u8 flag);
void MUSIC_VoiceSwitch(u8 flag);

void MUSIC_PlaySong(FATFS_FileListInfoTypeDef *list);
void MUSIC_DisplaySongName(FATFS_FileListInfoTypeDef *list);
u8 MUSIC_Play(u8 *path, u8 songtype);
void MUSIC_SendEmptyData(void);
void MUSIC_FastForward(void);
void MUSIC_CancelPlay(void);



/* Define to prevent recursive inclusion ----------------------------*/
#endif /* __PLAYSONG_H */




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
