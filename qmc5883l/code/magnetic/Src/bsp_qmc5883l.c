/*
*********************************************************************************************************
*
*	模块名称 : 三轴磁力计QMC5883L驱动模块
*	文件名称 : bsp_QMC5883L.c
*	版    本 : V1.0
*	说    明 : 实现QMC5883L的读写操作。
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-04-27 Charles  正式发布
*
*	Copyright (C), 2016, QST
*
*********************************************************************************************************
*/

/*
	应用说明:访问QMC5883L前，请先调用一次 bsp_InitI2C()函数配置好I2C相关的GPIO.
*/
#include "stdio.h"
#include "main.h"
#include "stm32f0xx_hal.h"
#include "bsp_qmc5883l.h"
#include "bsp_i2c_gpio.h"
QMC5883L_T g_tMag;		/* 定义一个全局变量，保存实时数据 */

QMC5883L_T GetQMC5883LData()
{
	return g_tMag;
}
/*
*********************************************************************************************************
*	函 数 名: bsp_InitQMC5883L
*	功能说明: 初始化QMC5883L
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
//unsigned char id;
void bsp_InitQMC5883L(void)
{
	/* 设置Mode寄存器 */
		QMC5883L_WriteByte(0x0B, 0x01);	
		QMC5883L_WriteByte(0x20, 0x40);	
		QMC5883L_WriteByte(0x21, 0x01);	
		QMC5883L_WriteByte(0x09, 0x1D);	
 //  id = QMC5883L_ReadByte(0x0d);
}

/*
*********************************************************************************************************
*	函 数 名: QMC5883L_WriteByte
*	功能说明: 向 QMC5883L 寄存器写入一个数据
*	形    参: _ucRegAddr : 寄存器地址
*			  _ucRegData : 寄存器数据
*	返 回 值: 无
*********************************************************************************************************
*/
void QMC5883L_WriteByte(uint8_t _ucRegAddr, uint8_t _ucRegData)
{
    i2c_Start(1);							/* 总线开始信号 */

    i2c_SendByte(1 , QMC5883L_SLAVE_ADDRESS);	/* 发送设备地址+写信号 */
	i2c_WaitAck(1);

    i2c_SendByte(1 , _ucRegAddr);				/* 内部寄存器地址 */
	i2c_WaitAck(1);

    i2c_SendByte(1 ,_ucRegData);				/* 内部寄存器数据 */
	i2c_WaitAck(1);

    i2c_Stop(1);                   			/* 总线停止信号 */
}

/*
*********************************************************************************************************
*	函 数 名: QMC5883L_ReadByte
*	功能说明: 读取 MPU-6050 寄存器的数据
*	形    参: _ucRegAddr : 寄存器地址
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t QMC5883L_ReadByte(uint8_t _ucRegAddr)
{
	uint8_t ucData;

	i2c_Start(1);                  			/* 总线开始信号 */
	i2c_SendByte(1 , QMC5883L_SLAVE_ADDRESS);	/* 发送设备地址+写信号 */
	i2c_WaitAck(1 );
	i2c_SendByte(1, _ucRegAddr);     			/* 发送存储单元地址 */
	i2c_WaitAck(1);

	i2c_Start(1);                  			/* 总线开始信号 */

	i2c_SendByte(1 , QMC5883L_SLAVE_ADDRESS+1); 	/* 发送设备地址+读信号 */
	i2c_WaitAck(1);

	ucData = i2c_ReadByte(1);       			/* 读出寄存器数据 */
	i2c_NAck(1);
	i2c_Stop(1);                  			/* 总线停止信号 */
	return ucData;
}


/*
*********************************************************************************************************
*	函 数 名: QMC5883L_ReadData
*	功能说明: 读取 QMC5883 数据寄存器， 结果保存在全局变量 g_tMag.  主程序可以定时调用该程序刷新数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
signed short* QMC5883L_ReadData(void)
{
	uint8_t ucReadBuf[7];
	uint8_t i;
  signed short * p;
#if 1 /* 连续读 */
	i2c_Start(1);                  										/* 总线开始信号 */
	i2c_SendByte(1 , QMC5883L_SLAVE_ADDRESS);					/* 发送设备地址+写信号 */
	i2c_WaitAck(1);
	i2c_SendByte(1 , DATA_OUT_X);     								/* 发送存储单元地址  */
	i2c_WaitAck(1);

	i2c_Start(1);                  										/* 总线开始信号 */

	i2c_SendByte(1 , QMC5883L_SLAVE_ADDRESS + 1); 		/* 发送设备地址+读信号 */
	i2c_WaitAck(1);		

	for (i = 0; i < 6; i++)
	{
		ucReadBuf[i] = i2c_ReadByte(1);       					/* 读出寄存器数据 */
		i2c_Ack(1);
	}

	/* 读最后一个字节，时给 NAck */
	ucReadBuf[6] = i2c_ReadByte(1);
	i2c_NAck(1);

	i2c_Stop(1);                  										/* 总线停止信号 */

#else	/* 单字节读 */
	for (i = 0 ; i < 7; i++)
	{
		ucReadBuf[i] = QMC5883L_ReadByte(DATA_OUT_X + i);
	}
#endif

	/* 将读出的数据保存到全局结构体变量 */
	g_tMag.X = (int16_t)((ucReadBuf[1] << 8) + ucReadBuf[0]);
	g_tMag.Y = (int16_t)((ucReadBuf[3] << 8) + ucReadBuf[2]);
	g_tMag.Z = (int16_t)((ucReadBuf[5] << 8) + ucReadBuf[4]);
	
	g_tMag.Status = ucReadBuf[6];
	g_tMag.mangbuffer[0] = g_tMag.X ;
	g_tMag.mangbuffer[1] = g_tMag.Y ;
	g_tMag.mangbuffer[2] = g_tMag.Z ;

      printf("  Manetic X:  %d",g_tMag.X);
      printf("  Manetic Y:  %d",g_tMag.Y);
      printf("  Manetic Z:  %d\n",g_tMag.Z);
	return g_tMag.mangbuffer;	
}

/***************************** QST Technology *********************************/

