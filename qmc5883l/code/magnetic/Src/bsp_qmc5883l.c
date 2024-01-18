/*
*********************************************************************************************************
*
*	ģ������ : ���������QMC5883L����ģ��
*	�ļ����� : bsp_QMC5883L.c
*	��    �� : V1.0
*	˵    �� : ʵ��QMC5883L�Ķ�д������
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-04-27 Charles  ��ʽ����
*
*	Copyright (C), 2016, QST
*
*********************************************************************************************************
*/

/*
	Ӧ��˵��:����QMC5883Lǰ�����ȵ���һ�� bsp_InitI2C()�������ú�I2C��ص�GPIO.
*/
#include "stdio.h"
#include "main.h"
#include "stm32f0xx_hal.h"
#include "bsp_qmc5883l.h"
#include "bsp_i2c_gpio.h"
QMC5883L_T g_tMag;		/* ����һ��ȫ�ֱ���������ʵʱ���� */

QMC5883L_T GetQMC5883LData()
{
	return g_tMag;
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitQMC5883L
*	����˵��: ��ʼ��QMC5883L
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
//unsigned char id;
void bsp_InitQMC5883L(void)
{
	/* ����Mode�Ĵ��� */
		QMC5883L_WriteByte(0x0B, 0x01);	
		QMC5883L_WriteByte(0x20, 0x40);	
		QMC5883L_WriteByte(0x21, 0x01);	
		QMC5883L_WriteByte(0x09, 0x1D);	
 //  id = QMC5883L_ReadByte(0x0d);
}

/*
*********************************************************************************************************
*	�� �� ��: QMC5883L_WriteByte
*	����˵��: �� QMC5883L �Ĵ���д��һ������
*	��    ��: _ucRegAddr : �Ĵ�����ַ
*			  _ucRegData : �Ĵ�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void QMC5883L_WriteByte(uint8_t _ucRegAddr, uint8_t _ucRegData)
{
    i2c_Start(1);							/* ���߿�ʼ�ź� */

    i2c_SendByte(1 , QMC5883L_SLAVE_ADDRESS);	/* �����豸��ַ+д�ź� */
	i2c_WaitAck(1);

    i2c_SendByte(1 , _ucRegAddr);				/* �ڲ��Ĵ�����ַ */
	i2c_WaitAck(1);

    i2c_SendByte(1 ,_ucRegData);				/* �ڲ��Ĵ������� */
	i2c_WaitAck(1);

    i2c_Stop(1);                   			/* ����ֹͣ�ź� */
}

/*
*********************************************************************************************************
*	�� �� ��: QMC5883L_ReadByte
*	����˵��: ��ȡ MPU-6050 �Ĵ���������
*	��    ��: _ucRegAddr : �Ĵ�����ַ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t QMC5883L_ReadByte(uint8_t _ucRegAddr)
{
	uint8_t ucData;

	i2c_Start(1);                  			/* ���߿�ʼ�ź� */
	i2c_SendByte(1 , QMC5883L_SLAVE_ADDRESS);	/* �����豸��ַ+д�ź� */
	i2c_WaitAck(1 );
	i2c_SendByte(1, _ucRegAddr);     			/* ���ʹ洢��Ԫ��ַ */
	i2c_WaitAck(1);

	i2c_Start(1);                  			/* ���߿�ʼ�ź� */

	i2c_SendByte(1 , QMC5883L_SLAVE_ADDRESS+1); 	/* �����豸��ַ+���ź� */
	i2c_WaitAck(1);

	ucData = i2c_ReadByte(1);       			/* �����Ĵ������� */
	i2c_NAck(1);
	i2c_Stop(1);                  			/* ����ֹͣ�ź� */
	return ucData;
}


/*
*********************************************************************************************************
*	�� �� ��: QMC5883L_ReadData
*	����˵��: ��ȡ QMC5883 ���ݼĴ����� ���������ȫ�ֱ��� g_tMag.  ��������Զ�ʱ���øó���ˢ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
signed short* QMC5883L_ReadData(void)
{
	uint8_t ucReadBuf[7];
	uint8_t i;
  signed short * p;
#if 1 /* ������ */
	i2c_Start(1);                  										/* ���߿�ʼ�ź� */
	i2c_SendByte(1 , QMC5883L_SLAVE_ADDRESS);					/* �����豸��ַ+д�ź� */
	i2c_WaitAck(1);
	i2c_SendByte(1 , DATA_OUT_X);     								/* ���ʹ洢��Ԫ��ַ  */
	i2c_WaitAck(1);

	i2c_Start(1);                  										/* ���߿�ʼ�ź� */

	i2c_SendByte(1 , QMC5883L_SLAVE_ADDRESS + 1); 		/* �����豸��ַ+���ź� */
	i2c_WaitAck(1);		

	for (i = 0; i < 6; i++)
	{
		ucReadBuf[i] = i2c_ReadByte(1);       					/* �����Ĵ������� */
		i2c_Ack(1);
	}

	/* �����һ���ֽڣ�ʱ�� NAck */
	ucReadBuf[6] = i2c_ReadByte(1);
	i2c_NAck(1);

	i2c_Stop(1);                  										/* ����ֹͣ�ź� */

#else	/* ���ֽڶ� */
	for (i = 0 ; i < 7; i++)
	{
		ucReadBuf[i] = QMC5883L_ReadByte(DATA_OUT_X + i);
	}
#endif

	/* �����������ݱ��浽ȫ�ֽṹ����� */
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

