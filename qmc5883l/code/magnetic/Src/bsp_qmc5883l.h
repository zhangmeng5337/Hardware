/*
*********************************************************************************************************
*
*	模块名称 : 三轴磁力计QMC5883L驱动模块
*	文件名称 : bsp_QMC5883L.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2016-04-29 Charles  QST
*
*	Copyright (C), 2016, QST
*
*********************************************************************************************************
*/

#ifndef _BSP_QMC5883L_H
#define _BSP_QMC5883L_H
#include "stm32f0xx_hal.h"
#define QMC5883L_SLAVE_ADDRESS    0x1a		/* I2C从机地址 */

//****************************************
// 定义QMC5883L内部地址
//****************************************
#define DATA_OUT_X		0x00

typedef struct
{
	int16_t X;
	int16_t Y;
	int16_t Z;

	int16_t X_Min;
	int16_t Y_Min;
	int16_t Z_Min;

	int16_t X_Max;
	int16_t Y_Max;
	int16_t Z_Max;	

	uint8_t	Status;
	
	uint8_t CfgRegA;
	uint8_t CfgRegB;
	uint8_t CfgRegC;
	uint8_t ModeReg;
	int16_t mangbuffer[3];		
	uint8_t IDReg[3+1];	
}QMC5883L_T;

extern QMC5883L_T g_tMag;

void bsp_InitQMC5883L(void);

signed short* QMC5883L_ReadData(void);
void QMC5883L_WriteByte(uint8_t _ucRegAddr, uint8_t _ucRegData);
uint8_t QMC5883L_ReadByte(uint8_t _ucRegAddr);

#endif

/***************************** QST Technology (END OF FILE) *********************************/

