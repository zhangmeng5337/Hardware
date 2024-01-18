/*
*********************************************************************************************************
*
*	ģ������ : I2C��������ģ��
*	�ļ����� : bsp_i2c_gpio.c
*	��    �� : V1.0
*	˵    �� : ��gpioģ��i2c����, ������STM32F4ϵ��CPU����ģ�鲻����Ӧ�ò�����֡��������I2C���߻�������������
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-04-027 Charls  ��ʽ����
*
*	Copyright (C), 2016, QST
*
*********************************************************************************************************
*/

/*
	Ӧ��˵����
	�ڷ���I2C�豸ǰ�����ȵ��� i2c_CheckDevice() ���I2C�豸�Ƿ��������ú���������GPIO
*/

#include "main.h"
#include "stm32f0xx_hal.h"
#include "bsp_i2c_gpio.h"
  GPIO_InitTypeDef GPIO_InitStruct;
/*
	QST ����ϵͳ i2c����GPIO:
	������I2C���ߣ�
 		|--PB8/I2C1_SCL
 		|--PB9/I2C1_SDA
		
	��ƵI2C���ߣ�
		|--PB10/I2C2_SCL
		|--PB11/I2C2_SDA
		
	��Դ����I2C����
		|--PA8/I2C3_SCL
		|--PC9/I2C3_SDA
*/

/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */




/* �����дSCL��SDA�ĺ� */
#define I2C1_SCL_1()   HAL_GPIO_WritePin(SDA_GPIO_Port, SCL_Pin, GPIO_PIN_SET)				/* SCL = 1 */
#define I2C1_SCL_0()   HAL_GPIO_WritePin(SDA_GPIO_Port, SCL_Pin, GPIO_PIN_RESET)				/* SCL = 0 */

#define I2C1_SDA_1()   HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);				/* SDA = 1 */
#define I2C1_SDA_0()   HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET);				/* SDA = 0 */

#define I2C1_SDA_READ()  HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin)	/* ��SDA����״̬ */
#define I2C1_SCL_READ()  HAL_GPIO_ReadPin(SCL_GPIO_Port, SCL_Pin)		/* ��SCL����״̬ */


//#define I2C1_SCL_IN() 					{GPIOB->MODER&=~(3<<(8*2));GPIOB->MODER|=0<<8*2;}   /* PB8���ó����� */
//#define I2C1_SCL_OUT() 					{GPIOB->MODER&=~(3<<(8*2));GPIOB->MODER|=1<<8*2;}		/* PB8���ó���� */

void I2C1_SCL_IN() 					
	{  GPIO_InitStruct.Pin = SCL_Pin;
																		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
																		GPIO_InitStruct.Pull = GPIO_NOPULL;
																			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
															      HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);}   /* PB9���ó����� */
void I2C1_SCL_OUT() 				
	{  GPIO_InitStruct.Pin = SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);}		/* PB9���ó���� */
void I2C1_SDA_IN() 					
	{  GPIO_InitStruct.Pin = SDA_Pin;
																		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
																		GPIO_InitStruct.Pull = GPIO_NOPULL;
																			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
															      HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);}   /* PB9���ó����� */
void I2C1_SDA_OUT() 				
	{  GPIO_InitStruct.Pin = SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);}		/* PB9���ó���� */
	
void i2c_Stop(uint8_t _TermNumber);

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitI2C
*	����˵��: ����I2C���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
*	��    ��:  ��
*	�� �� ֵ: ��
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



	
	
	

	/* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
	i2c_Stop(1);
	i2c_Stop(2);
	i2c_Stop(3);
	
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Delay
*	����˵��: I2C����λ�ӳ٣����400KHz
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*��
		CPU��Ƶ168MHzʱ�����ڲ�Flash����, MDK���̲��Ż�����̨ʽʾ�����۲Ⲩ�Ρ�
		ѭ������Ϊ5ʱ��SCLƵ�� = 1.78MHz (����ʱ: 92ms, ��д������������ʾ����̽ͷ���ϾͶ�дʧ�ܡ�ʱ��ӽ��ٽ�)
		ѭ������Ϊ10ʱ��SCLƵ�� = 1.1MHz (����ʱ: 138ms, ���ٶ�: 118724B/s)
		ѭ������Ϊ30ʱ��SCLƵ�� = 440KHz�� SCL�ߵ�ƽʱ��1.0us��SCL�͵�ƽʱ��1.2us

		��������ѡ��2.2Kŷʱ��SCL������ʱ��Լ0.5us�����ѡ4.7Kŷ����������Լ1us

		ʵ��Ӧ��ѡ��400KHz���ҵ����ʼ���
	*/
	for (i = 0; i < 30; i++);
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C���������ź�
*	��    ��:  TermNumber:I2C���ߺ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Start(uint8_t _TermNumber)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
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
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    ��:  TermNumber:I2C���ߺ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Stop(uint8_t _TermNumber)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
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
*	�� �� ��: i2c_SendByte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    ��:  TermNumber:I2C���ߺ� _ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _TermNumber , uint8_t _ucByte)
{
	uint8_t i;
	switch( _TermNumber )
	{
		case 1:
			I2C1_SDA_OUT();
		/* �ȷ����ֽڵĸ�λbit7 */
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
					I2C1_SDA_1(); // �ͷ�����
				}
				_ucByte <<= 1;	/* ����һ��bit */
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
*	�� �� ��: i2c_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    ��:  TermNumber:I2C���ߺ�
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(uint8_t _TermNumber)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
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
*	�� �� ��: i2c_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    ��:  TermNumber:I2C���ߺ�
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(uint8_t _TermNumber)
{
	uint8_t re;
	
	switch(_TermNumber)
	{
		case 1:
			I2C1_SDA_1();	/* CPU�ͷ�SDA���� */
			i2c_Delay();
			I2C1_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
			i2c_Delay();
			I2C1_SDA_IN();
			if (I2C1_SDA_READ())	/* CPU��ȡSDA����״̬ */
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
*	�� �� ��: i2c_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    ��:  TermNumber:I2C���ߺ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Ack(uint8_t _TermNumber)
{
	
	switch(_TermNumber)
	{
		case 1:
			I2C1_SDA_OUT();
			I2C1_SDA_0();	/* CPU����SDA = 0 */
			i2c_Delay();
			I2C1_SCL_1();	/* CPU����1��ʱ�� */
			i2c_Delay();
			I2C1_SCL_0();
			i2c_Delay();
			I2C1_SDA_1();	/* CPU�ͷ�SDA���� */
		break;
		default:
			printf("No Such Terminal,Please Check Again,ACK Err\r\n");
			break;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    ��:  TermNumber:I2C���ߺ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_NAck(uint8_t _TermNumber)
{
	switch(_TermNumber)
	{
		case 1:
			I2C1_SDA_OUT();
			I2C1_SDA_1();	/* CPU����SDA = 1 */
			i2c_Delay();
			I2C1_SCL_1();	/* CPU����1��ʱ�� */
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
*	�� �� ��: i2c_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    ��: TermNumber:I2C���ߺ� _Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
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
				i2c_Start(1);		/* ���������ź� */

				/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
				i2c_SendByte(1 , _Address | I2C_WR);
				ucAck = i2c_WaitAck(1);	/* ����豸��ACKӦ�� */

				i2c_Stop(1);			/* ����ֹͣ�ź� */

				return ucAck;
			}
			I2C1_SDA_OUT();
			I2C1_SCL_OUT();
			break;
		
		default:
			printf("No Such Terminal,Please Check Again,CheckDevice Err\r\n");
			break;
		}
	return 1;	/* I2C�����쳣 */
}
