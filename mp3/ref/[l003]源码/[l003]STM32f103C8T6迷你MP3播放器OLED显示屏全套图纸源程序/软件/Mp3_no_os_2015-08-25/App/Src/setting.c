
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : setting.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : setting program body
* Description    : --
**********************************************************************/

/* Includes  == 文件包含 --------------------------------------------*/
#include "setting.h"
#include "fatfs.h"
#include "ff.h"
#include "diskio.h"
#include <stdio.h>
#include <string.h>



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/
SETTING_DataTypeDef	SETTING_Data;

	
	
/* Private functions  ==  私有函数定义 ------------------------------*/

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

	f_mount(0, &fs);                                  	//挂载一个文件系统结构
	res = f_open(&fil, "system/setting.ini", FA_CREATE_ALWAYS | FA_WRITE);  //打开字库文件
	if (res) FATFS_Die(res);		                       	//如果打开字库不成功，返回错误号
	
	res = f_lseek(&fil, sizeof(SETTING_DataTypeDef));
	if (res) FATFS_Die(res);
	res = f_lseek(&fil, 0);                      	//找到要显示汉字在字库文件里的偏移地址并定位
	if (res) FATFS_Die(res);
	
	res = f_write(&fil, setdata, sizeof(SETTING_DataTypeDef), &bw);   		//读取size*size大小的汉字点阵数据到GBK[]数组中
	if (res) FATFS_Die(res);
	
	res = f_close(&fil);		                          	//关闭文件系统
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

	f_mount(0, &fs);                                  	//挂载一个文件系统结构
	res = f_open(&fil, "system/setting.ini", FA_OPEN_EXISTING | FA_READ);  //打开字库文件
	if (res) FATFS_Die(res);		                       	//如果打开字库不成功，返回错误号
	
	res = f_lseek(&fil, 0);                      	//找到要显示汉字在字库文件里的偏移地址并定位
	if (res) FATFS_Die(res);
	
	res = f_read(&fil, setdata, sizeof(SETTING_DataTypeDef), &br);   		//读取size*size大小的汉字点阵数据到GBK[]数组中
	if (res) FATFS_Die(res);
	
	res = f_close(&fil);		                          	//关闭文件系统
	if (res) FATFS_Die(res);									  
	
	res = f_mount(0, NULL);
	if (res) FATFS_Die(res);	
}




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
