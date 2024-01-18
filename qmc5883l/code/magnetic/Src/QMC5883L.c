#include "main.h"
#include "stm32f0xx_hal.h"
#include "QMC5883L.h"


/* ˽�к궨�� -----------------------------------------------------------------*/
#define MAT_N  10
#define QMC5883_ADDR               0x1a 
#define fabs(x)                    (x < 0 ? -x : x)
/* ˽�б��� -------------------------------------------------------------------*/
int16_t MagnetRawAd[3];
int16_t X_Buff[CALIBTATE_DATA_NUM],Y_Buff[CALIBTATE_DATA_NUM],
Z_Buff[CALIBTATE_DATA_NUM],CalibrateCnt;
  GPIO_InitTypeDef GPIO_InitStruct;
void SDA_OUT()
{
//  GPIO_InitStruct.Pin = SDA_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
}
void SDA_IN()
{
//  GPIO_InitStruct.Pin = SDA_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
}
void SDA_HIGH()
{
  HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);
}
void SDA_LOW()
{
  HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET);
}
void SCL_HIGH()
{
  HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET);
}
void SCL_LOW()
{
  HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET);
}
unsigned char SDA_READ()
{	
	u8 tmp;
	tmp=HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin);
	//GPIO_ReadInputDataBit(ADXL345_I2C_PORT, ADXL345_SDA);
	
	return tmp;
}
/**************************************
��ʱ5΢��(STC90C52RC@12M)
��ͬ�Ĺ�������,��Ҫ�����˺�����ע��ʱ�ӹ���ʱ��Ҫ�޸�
������1T��MCUʱ,���������ʱ����
**************************************/
void Delay_5us()
{
	int i;
 	for(i=0;i<6;i++);
}

/**************************************
��ʱ5����(STC90C52RC@12M)
��ͬ�Ĺ�������,��Ҫ�����˺���
������1T��MCUʱ,���������ʱ����
**************************************/
void Delay5ms()
{
	unsigned int n = 10;
	while (n--);
}
/**************************************
��ʼ�ź�
**************************************/
void QMC5883_Start()
{
    SDA_OUT();
    SDA_HIGH();                    //����������
    SCL_HIGH();                    //����ʱ����
    Delay_5us();                 //��ʱ
    SDA_LOW();                    //�����½���
    Delay_5us();                 //��ʱ
    SCL_LOW();                    //����ʱ����
}

/**************************************
ֹͣ�ź�
**************************************/
void QMC5883_Stop()
{
    SDA_LOW();                    //����������
    SCL_HIGH();                    //����ʱ����
    Delay_5us();                 //��ʱ
    SDA_HIGH();                    //����������
    Delay_5us();                 //��ʱ
}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void QMC5883_SendACK(unsigned char ack)
{
SDA_OUT();
	if(ack==1)
		SDA_HIGH(); 
	else
		SDA_LOW(); 
    //SDA = ack;                  //дӦ���ź�
    SCL_HIGH();                    //����ʱ����
    Delay_5us();                 //��ʱ
    SCL_LOW();                    //����ʱ����
    Delay_5us();                 //��ʱ
}

/**************************************
����Ӧ���ź�
**************************************/
unsigned char QMC5883_RecvACK()
{
  unsigned char CY;
    SCL_HIGH();                    //����ʱ����
    Delay_5us();                //���
    SDA_IN();
    CY = SDA_READ();                   //��λ��־ ��Ӧ���ź�
    SCL_LOW();                    //����ʱ����
    Delay_5us();                //��ʱ
    return CY;
}

/**************************************
��IIC���߷���һ���ֽ�����
**************************************/
void QMC5883_SendByte(u8 dat)
{
    u8 i;
 SDA_OUT();
    for (i=0; i<8; i++)         //8λ������
    {
     //   dat <<= 1;              //�Ƴ����ݵ����λ
		if(dat&0x80)
			SDA_HIGH(); 
		else
			SDA_LOW();
		dat <<= 1;  
       // SDA = CY;               //�����ݿ�
        SCL_HIGH();                //����ʱ����
        Delay_5us();             //��ʱ
        SCL_LOW();                //����ʱ����
        Delay_5us();             //��ʱ

    }
    QMC5883_RecvACK();
}

/**************************************
��IIC���߽���һ���ֽ�����
**************************************/
u8 QMC5883_RecvByte()
{
    u8 i;
    u8 dat = 0;
 SDA_OUT();
    SDA_HIGH();                    //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)         //8λ������
    {   SDA_IN();
        dat <<= 1;
        SCL_HIGH();                //����ʱ����
        Delay_5us();             //��ʱ
        
	if(SDA_READ())
			dat |=0x01;             //������ 
		else
			dat =dat;             //������            
       // dat |= SDA;             //������               
        SCL_LOW();                //����ʱ����
        Delay_5us();             //��ʱ
    }
    return dat;
}

void I2C_WriteByte(uint8_t Val,uint8_t  Reg, uint8_t addr)
{
	QMC5883_Start();                  //��ʼ�ź�
	QMC5883_SendByte(addr);   //�����豸��ַ+д�ź�
	QMC5883_SendByte(Reg);    //�ڲ��Ĵ�����ַ����ο�����pdf22ҳ 
	QMC5883_SendByte(Val);       //�ڲ��Ĵ������ݣ���ο�����pdf22ҳ 
	QMC5883_Stop();
}
 void  I2C_ReadByte(uint8_t *RegVal,uint8_t recv_cnt,uint8_t Reg,uint8_t addr)
{
	unsigned char i;
        QMC5883_Start();                          //��ʼ�ź�
	QMC5883_SendByte(addr);           //�����豸��ַ+д�ź�
	QMC5883_SendByte(Reg);                   //���ʹ洢��Ԫ��ַ����0��ʼ	
	QMC5883_Start();                          //��ʼ�ź�
	QMC5883_SendByte(addr+1);         //�����豸��ַ+���ź�
	for(i=0;i<recv_cnt;i++)
	{
		RegVal[i]=QMC5883_RecvByte();              //�����Ĵ�������

		if(i==(recv_cnt-1))
			QMC5883_SendACK(0x01);
		else
			QMC5883_SendACK(0x00);
	}

	
	QMC5883_Stop();                           //ֹͣ�ź�
}
/* ˽�к��� -------------------------------------------------------------------*/
/******************************************************************************
* @�������ƣ�void MPU6050_Init(void)
* @����������MPU6050��������ʼ��
* @���������None
* @���������None
*******************************************************************************/
uint8_t QMC5883_ReadReg(uint8_t Reg)
{
    u8 RegVal = 0;
    I2C_ReadByte(&RegVal,1,Reg,QMC5883_ADDR);
    return RegVal;
}
/******************************************************************************
* @�������ƣ�void MPU6050_Init(void)
* @����������MPU6050��������ʼ��
* @���������None
* @���������None
*******************************************************************************/
void QMC5883_WriteReg(uint8_t Val,uint8_t Reg)
{
    I2C_WriteByte(Val,Reg, QMC5883_ADDR);
}
/******************************************************************************
* @�������ƣ�void QMC5883_Init(void)
* @�����������������
* @���������None
* @���������None
*******************************************************************************/
uint8_t Temp;
uint8_t QMC5883_InitConfig(void)
{
	
	
	QMC5883_WriteReg(0x01, 0x0B);
	QMC5883_WriteReg(0x40, 0x20);
	QMC5883_WriteReg(0x01, 0x21);
	QMC5883_WriteReg(0x0D, 0x09);/****OSR=512,RNG=+/-2G,ODR=200Hz,MODE= continuous*******/
	//while(!TIME_CheckTimeMs(TIME_DELAY,2));///DELAY 2ms
	HAL_Delay(10);
	Temp = QMC5883_ReadReg(0x09);
	 while(Temp != 0x1D) return 1;
	
    return 0;
}
/******************************************************************************
* @�������ƣ�void QMC5883_GetData(float *Magnet)
* @������������ȡ�ش�����
* @����������ش�buff
* @���������None
*******************************************************************************/
void QMC5883_GetData(float *Magnet)
{
    uint8_t Buff[6],i;
    uint8_t Temp;
    
    Temp = QMC5883_ReadReg(0x06);
//    if (Temp&0x06)
//    {
//    	//LOG("output data OVL or ERROR!")
//    	return;
//    }
//    else if (Temp&0x00)
//    {
//    	//LOG("output data not ready!")
//    	return;
//    }
    
    I2C_ReadByte(Buff,6,0x00,QMC5883_ADDR);
	MagnetRawAd[0] = ((int16_t)Buff[1] << 8) | Buff[0];
	MagnetRawAd[1] = ((int16_t)Buff[3] << 8) | Buff[2];
	MagnetRawAd[2] = ((int16_t)Buff[5] << 8) | Buff[4];

    for(i=0;i<3;i++)
    {
        Magnet[i] = (float)MagnetRawAd[i] / 120.f;//��λuT
		
    }
}

/* �ļ����� --------------------------------------------------------------------*/

