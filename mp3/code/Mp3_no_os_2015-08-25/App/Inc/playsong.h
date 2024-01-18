
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

 

/* Includes  == �ļ����� --------------------------------------------*/
#include "stm32f10x.h"
#include "fatfs.h"



/* Private typedef    ==  ˽������ ----------------------------------*/
/* Private define     ==  ˽�ж��� ----------------------------------*/

//-----------------------------------------//
//һ�������ļ���������������ɣ�һ��������ҳ���
//һ������Ϊ1024�ֽڣ�һ��ҳΪ32�ֽڣ�һ����������32ҳ���������һ���������⣩
//����Ϊ��SD����������ļ����ݵ���С��λ
//ҳΪSTM32��VS1053B�������ݵ���С��λ
typedef struct
{
	u8		Music_PathBuff[255];      		//���ڲ��ŵ�����Ŀ¼·����
	u8		Music_PathLen;            		//���ڲ��ŵ�����Ŀ¼·�������ȣ����ֽ�Ϊ��λ
	u8		Music_SectorBuff[1024]; 		//һ���������������ݻ���
	u32	Music_SectorAllNum;       		//�ܵ���������������
	u32	Music_SectorCurNum;       		//��ǰ�����������������
	u8 	Music_PageLastNum;        		//���һ����������������ҳ��Ŀ
	u8		Music_PageCurNum;        		//��ǰ���������ݵ�ҳ���
	u8		Music_Speed;						//��־��ǰ�����Ƿ���Կ��
	u8		Music_PlayState;           	//��־��ǰ�����Ƿ񲥷�״̬
	u32	Music_PlayTime;					//���ֲ���ʱ�䣬����Ϊ��λ
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



/* Private macro      ==  ˽�к� ------------------------------------*/
/* Private variables  ==  ˽�б��� ----------------------------------*/
extern	MUSIC_InfoTypeDef		MUSIC_Info;



/* Private function prototypes  ==  ˽�к������� --------------------*/
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
