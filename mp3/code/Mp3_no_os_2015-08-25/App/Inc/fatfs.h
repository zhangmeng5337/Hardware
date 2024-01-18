
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

 

/* Includes  == �ļ����� --------------------------------------------*/
#include "stm32f10x.h"
#include "ff.h"
#include "diskio.h"



/* Private typedef    ==  ˽������ ----------------------------------*/
/* Private define     ==  ˽�ж��� ----------------------------------*/

#define FATFS_MAX_FILE_NUM          200    //һ��Ŀ¼������ļ����ļ�����
#define FATFS_MAX_LAYER_NUM         10     //�ļ�Ŀ¼���Ƕ�ײ���
#define FATFS_MAX_FILE_PATH_NAME    250    //һ���ļ�·������೤��, �� /mp3/song/chiansong/�Ǳ�.MP3
#define FATFS_MAX_FILE_NAME_TOTAL   2048   //һ��Ŀ¼�����ļ������ļ��������ܳ���

//-----------------------------------------//
typedef struct
{
	//-------------------------------------//
	//��ǰĿ¼�µ��ļ������Ϣ
	u8		FileQuantity;			//��ǰĿ¼�µ����ļ���
	u8		FileNameBuff[FATFS_MAX_FILE_NAME_TOTAL]; 	//��ǰĿ¼��ȫ���ļ�����Ż��棬ÿ���ļ�����'\0'����
	u32		FileNameLen;         	//��ǰĿ¼ȫ���ļ������ܳ���	
	//-------------------------------------//
	//��ǰĿ¼��Ϣ
	u8		FilePathName[FATFS_MAX_FILE_PATH_NAME];   	//��ǰ·����
	u8		FilePathLen;         	//��ǰ·��������
	u8		FileCurNum;     		//��ǰ��ѡ�е��ļ����	
	//-------------------------------------//
	//��ǰĿ¼�µ��ļ�������Ϣ
	u8		FileLen[FATFS_MAX_FILE_NUM];     		//��ʾ��N���ļ����ĳ���
	u8		FileType[FATFS_MAX_FILE_NUM];    		//��ʾ��N���ļ�������
	u32		FilePostion[FATFS_MAX_FILE_NUM];    	//��ʾ��N���ļ��������FileNameBuff��λ��
	u8		FileSongPos[FATFS_MAX_FILE_NUM];    	//��ǰĿ¼�µĿɲ��Ÿ���������ܿɲ��Ÿ�����λ��
	//-------------------------------------//
	//Ŀ¼Ƕ�ײ���Ϣ
	u8		FileLayerTol;        	//�ļ�·������
	u8		FileLayerNum[FATFS_MAX_LAYER_NUM];   	//�ļ�·��ÿһ��ı�ѡ�е��ļ����
	//-------------------------------------//
	//��ǰĿ¼�ɲ��Ÿ�����Ϣ
	u8		FileCurSongNum;			//��ǰ���ֵı��
	u8		FileSongAll;			//��ǰĿ¼�µĿɲ��Ÿ�������
	u8		FileSongNum[FATFS_MAX_FILE_NUM];		//��ǰĿ¼�µĿɲ��Ÿ�����������ļ�Ŀ¼��λ��
}FATFS_FileListInfoTypeDef;

#define	FATFS_FILEISDIR			0x01
#define	FATFS_FILEISMP3			0x02
#define	FATFS_FILEISWMA			0x03
#define	FATFS_FILEISWAV			0x04
#define	FATFS_FILEISFLAC		0x05
#define	FATFS_FILEISOTHER		0x06



/* Private macro      ==  ˽�к� ------------------------------------*/
/* Private variables  ==  ˽�б��� ----------------------------------*/



/* Private function prototypes  ==  ˽�к������� --------------------*/
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
