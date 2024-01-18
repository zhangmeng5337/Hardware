
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : setting.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : setting program body
* Description    : --
**********************************************************************/

/* Includes  == �ļ����� --------------------------------------------*/
#include "setting.h"
#include "fatfs.h"
#include "ff.h"
#include "diskio.h"
#include <stdio.h>
#include <string.h>



/* Private typedef    ==  ˽������ ----------------------------------*/
/* Private define     ==  ˽�ж��� ----------------------------------*/

//-----------------------------------------//



/* Private macro      ==  ˽�к� ------------------------------------*/
/* Private variables  ==  ˽�б��� ----------------------------------*/
SETTING_DataTypeDef	SETTING_Data;

	
	
/* Private functions  ==  ˽�к������� ------------------------------*/

/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void SETTING_DeInit(SETTING_DataTypeDef *setdata)
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
void SETTING_WriteFlash(SETTING_DataTypeDef *setdata)
{
	FRESULT res;                                      	// FatFs function common result code
	FATFS fs;                                        	// Work area (file system object) for logical drive
	FIL fil;                                         	// file objects
	UINT bw;			                                  	// File R count

	f_mount(0, &fs);                                  	//����һ���ļ�ϵͳ�ṹ
	res = f_open(&fil, "system/setting.ini", FA_CREATE_ALWAYS | FA_WRITE);  //���ֿ��ļ�
	if (res) FATFS_Die(res);		                       	//������ֿⲻ�ɹ������ش����
	
	res = f_lseek(&fil, sizeof(SETTING_DataTypeDef));
	if (res) FATFS_Die(res);
	res = f_lseek(&fil, 0);                      	//�ҵ�Ҫ��ʾ�������ֿ��ļ����ƫ�Ƶ�ַ����λ
	if (res) FATFS_Die(res);
	
	res = f_write(&fil, setdata, sizeof(SETTING_DataTypeDef), &bw);   		//��ȡsize*size��С�ĺ��ֵ������ݵ�GBK[]������
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
* Remark         : None
**********************************************************************/
void SETTING_ReadFlash(SETTING_DataTypeDef *setdata)
{
	FRESULT res;                                      	// FatFs function common result code
	FATFS fs;                                        	// Work area (file system object) for logical drive
	FIL fil;                                         	// file objects
	UINT br;			                                  	// File R count

	f_mount(0, &fs);                                  	//����һ���ļ�ϵͳ�ṹ
	res = f_open(&fil, "system/setting.ini", FA_OPEN_EXISTING | FA_READ);  //���ֿ��ļ�
	if (res) FATFS_Die(res);		                       	//������ֿⲻ�ɹ������ش����
	
	res = f_lseek(&fil, 0);                      	//�ҵ�Ҫ��ʾ�������ֿ��ļ����ƫ�Ƶ�ַ����λ
	if (res) FATFS_Die(res);
	
	res = f_read(&fil, setdata, sizeof(SETTING_DataTypeDef), &br);   		//��ȡsize*size��С�ĺ��ֵ������ݵ�GBK[]������
	if (res) FATFS_Die(res);
	
	res = f_close(&fil);		                          	//�ر��ļ�ϵͳ
	if (res) FATFS_Die(res);									  
	
	res = f_mount(0, NULL);
	if (res) FATFS_Die(res);	
}




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
