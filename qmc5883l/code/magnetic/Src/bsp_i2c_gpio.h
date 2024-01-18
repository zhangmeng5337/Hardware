/*
*********************************************************************************************************
*
*	ģ������ : I2C��������ģ��
*	�ļ����� : bsp_i2c_gpio.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ���
*
*	Copyright (C), 2012-2013, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */

void bsp_InitI2C(void);
void i2c_Start(uint8_t _TermNumber);
void i2c_Stop(uint8_t _TermNumber);
void i2c_SendByte(uint8_t _TermNumber , uint8_t _ucByte);
uint8_t i2c_ReadByte(uint8_t _TermNumber );
uint8_t i2c_WaitAck(uint8_t _TermNumber );
void i2c_Ack(uint8_t _TermNumber );
void i2c_NAck(uint8_t _TermNumber );
uint8_t i2c_CheckDevice(uint8_t _TermNumber  , uint8_t _Address);

#endif
