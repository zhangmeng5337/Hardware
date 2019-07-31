/*
*********************************************************************************************************
*
*	模块名称 : I2C总线驱动模块
*	文件名称 : bsp_i2c_gpio.c
*	版    本 : V1.0
*	说    明 : 用gpio模拟i2c总线, 适用于STM32F4系列CPU。该模块不包括应用层命令帧，仅包括I2C总线基本操作函数。
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-04-027 Charls  正式发布
*
*	Copyright (C), 2016, QST
*
*********************************************************************************************************
*/

/*
	应用说明：
	在访问I2C设备前，请先调用 i2c_CheckDevice() 检测I2C设备是否正常，该函数会配置GPIO
*/

#include "main.h"
#include "stm32f0xx_hal.h"
#include "bsp_i2c_gpio.h"
  GPIO_InitTypeDef GPIO_InitStruct;
/*
	QST 九轴系统 i2c总线GPIO:
	传感器I2C总线：
 		|--PB8/I2C1_SCL
 		|--PB9/I2C1_SDA
		
	音频I2C总线：
		|--PB10/I2C2_SCL
		|--PB11/I2C2_SDA
		
	电源管理I2C总线
		|--PA8/I2C3_SCL
		|--PC9/I2C3_SDA
*/

/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */




/* 定义读写SCL和SDA的宏 */
#define I2C1_SCL_1()   HAL_GPIO_WritePin(SDA_GPIO_Port, SCL_Pin, GPIO_PIN_SET)				/* SCL = 1 */
#define I2C1_SCL_0()   HAL_GPIO_WritePin(SDA_GPIO_Port, SCL_Pin, GPIO_PIN_RESET)				/* SCL = 0 */

#define I2C1_SDA_1()   HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);				/* SDA = 1 */
#define I2C1_SDA_0()   HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET);				/* SDA = 0 */

#define I2C1_SDA_READ()  HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin)	/* 读SDA口线状态 */
#define I2C1_SCL_READ()  HAL_GPIO_ReadPin(SCL_GPIO_Port, SCL_Pin)		/* 读SCL口线状态 */


//#define I2C1_SCL_IN() 					{GPIOB->MODER&=~(3<<(8*2));GPIOB->MODER|=0<<8*2;}   /* PB8设置成输入 */
//#define I2C1_SCL_OUT() 					{GPIOB->MODER&=~(3<<(8*2));GPIOB->MODER|=1<<8*2;}		/* PB8设置成输出 */

void I2C1_SCL_IN() 					
	{  GPIO_InitStruct.Pin = SCL_Pin;
																		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
																		GPIO_InitStruct.Pull = GPIO_NOPULL;
																			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
															      HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);}   /* PB9设置成输入 */
void I2C1_SCL_OUT() 				
	{  GPIO_InitStruct.Pin = SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);}		/* PB9设置成输出 */
void I2C1_SDA_IN() 					
	{  GPIO_InitStruct.Pin = SDA_Pin;
																		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
																		GPIO_InitStruct.Pull = GPIO_NOPULL;
																			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
															      HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);}   /* PB9设置成输入 */
void I2C1_SDA_OUT() 				
	{  GPIO_InitStruct.Pin = SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);}		/* PB9设置成输出 */
	
void i2c_Stop(uint8_t _TermNumber);

/*
*********************************************************************************************************
*	函 数 名: bsp_InitI2C
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitI2C(void)
{


//GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

 

  /*Configure GPIO pin : SCL_Pin */
  GPIO_InitStruct.Pin = SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SCL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SDA_Pin */
  GPIO_InitStruct.Pin = SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);



	
	
	

	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	i2c_Stop(1);
	i2c_Stop(2);
	i2c_Stop(3);
	
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*　
		CPU主频168MHz时，在内部Flash运行, MDK工程不优化。用台式示波器观测波形。
		循环次数为5时，SCL频率 = 1.78MHz (读耗时: 92ms, 读写正常，但是用示波器探头碰上就读写失败。时序接近临界)
		循环次数为10时，SCL频率 = 1.1MHz (读耗时: 138ms, 读速度: 118724B/s)
		循环次数为30时，SCL频率 = 440KHz， SCL高电平时间1.0us，SCL低电平时间1.2us

		上拉电阻选择2.2K欧时，SCL上升沿时间约0.5us，如果选4.7K欧，则上升沿约1us

		实际应用选择400KHz左右的速率即可
	*/
	for (i = 0; i < 30; i++);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参:  TermNumber:I2C总线号
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Start(uint8_t _TermNumber)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	switch(_TermNumber)
	{
		case 1:
			I2C1_SDA_OUT();
			I2C1_SDA_1();
			I2C1_SCL_1();
			i2c_Delay();
			I2C1_SDA_0();
			i2c_Delay();
			I2C1_SCL_0();
			i2c_Delay();
			break;
		
		default:
			printf("No Such Terminal,Please Check Again,Start Err\r\n");
			break;	
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参:  TermNumber:I2C总线号
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Stop(uint8_t _TermNumber)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	switch(_TermNumber)
	{
		case 1:
			I2C1_SDA_OUT();
			I2C1_SDA_0();
			I2C1_SCL_1();
			i2c_Delay();
			I2C1_SDA_1();
			break;
		default:
			printf("No Such Terminal,Please Check Again,Stop Err\r\n");
			break;
			
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参:  TermNumber:I2C总线号 _ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _TermNumber , uint8_t _ucByte)
{
	uint8_t i;
	switch( _TermNumber )
	{
		case 1:
			I2C1_SDA_OUT();
		/* 先发送字节的高位bit7 */
			for (i = 0; i < 8; i++)
			{
				if (_ucByte & 0x80)
				{
					I2C1_SDA_1();
				}
				else
				{
				I2C1_SDA_0();
				}
				i2c_Delay();
				I2C1_SCL_1();
				i2c_Delay();
				I2C1_SCL_0();
				if (i == 7)
				{
					I2C1_SDA_1(); // 释放总线
				}
				_ucByte <<= 1;	/* 左移一个bit */
				i2c_Delay();
			}
		break;
		
		
		default:
			printf("No Such Terminal,Please Check Again\r\n");
			break;
			
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参:  TermNumber:I2C总线号
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(uint8_t _TermNumber)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	switch(_TermNumber)
	{
		case 1:
			I2C1_SDA_IN();
			for (i = 0; i < 8; i++)
			{
				value <<= 1;
				I2C1_SCL_1();
				i2c_Delay();
				if (I2C1_SDA_READ())
				{
					value++;
				}
				I2C1_SCL_0();
				i2c_Delay();
			}
			break;
		
		default:
			printf("No Such Terminal,Please Check Again,ReadByte Err\r\n");
			break;		
	}
	return value;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参:  TermNumber:I2C总线号
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(uint8_t _TermNumber)
{
	uint8_t re;
	
	switch(_TermNumber)
	{
		case 1:
			I2C1_SDA_1();	/* CPU释放SDA总线 */
			i2c_Delay();
			I2C1_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
			i2c_Delay();
			I2C1_SDA_IN();
			if (I2C1_SDA_READ())	/* CPU读取SDA口线状态 */
			{
				re = 1;
			}
			else
			{
				re = 0;
			}
			I2C1_SCL_0();
			i2c_Delay();
			break;
		
		default:
			printf("No Such Terminal,Please Check Again,WaitACK Err\r\n");
			break;
	}
	return re;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参:  TermNumber:I2C总线号
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Ack(uint8_t _TermNumber)
{
	
	switch(_TermNumber)
	{
		case 1:
			I2C1_SDA_OUT();
			I2C1_SDA_0();	/* CPU驱动SDA = 0 */
			i2c_Delay();
			I2C1_SCL_1();	/* CPU产生1个时钟 */
			i2c_Delay();
			I2C1_SCL_0();
			i2c_Delay();
			I2C1_SDA_1();	/* CPU释放SDA总线 */
		break;
		default:
			printf("No Such Terminal,Please Check Again,ACK Err\r\n");
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参:  TermNumber:I2C总线号
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_NAck(uint8_t _TermNumber)
{
	switch(_TermNumber)
	{
		case 1:
			I2C1_SDA_OUT();
			I2C1_SDA_1();	/* CPU驱动SDA = 1 */
			i2c_Delay();
			I2C1_SCL_1();	/* CPU产生1个时钟 */
			i2c_Delay();
			I2C1_SCL_0();
			i2c_Delay();
		break;
		default:
			printf("No Such Terminal,Please Check Again,NACK Err\r\n");
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参: TermNumber:I2C总线号 _Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
uint8_t i2c_CheckDevice(uint8_t _TermNumber , uint8_t _Address)
{
	uint8_t ucAck;
	
	switch(_TermNumber)
	{
		case 1:
			I2C1_SDA_IN();
			I2C1_SCL_IN();
			if (I2C1_SDA_READ() && I2C1_SCL_READ())
			{
				I2C1_SDA_OUT();
				I2C1_SCL_OUT();
				i2c_Start(1);		/* 发送启动信号 */

				/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
				i2c_SendByte(1 , _Address | I2C_WR);
				ucAck = i2c_WaitAck(1);	/* 检测设备的ACK应答 */

				i2c_Stop(1);			/* 发送停止信号 */

				return ucAck;
			}
			I2C1_SDA_OUT();
			I2C1_SCL_OUT();
			break;
		
		default:
			printf("No Such Terminal,Please Check Again,CheckDevice Err\r\n");
			break;
		}
	return 1;	/* I2C总线异常 */
}
