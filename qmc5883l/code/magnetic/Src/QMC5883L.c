#include "main.h"
#include "stm32f0xx_hal.h"
#include "QMC5883L.h"


/* ÀΩ”–∫Í∂®“Â -----------------------------------------------------------------*/
#define MAT_N  10
#define QMC5883_ADDR               0x1a 
#define fabs(x)                    (x < 0 ? -x : x)
/* ÀΩ”–±‰¡ø -------------------------------------------------------------------*/
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
—” ±5Œ¢√Î(STC90C52RC@12M)
≤ªÕ¨µƒπ§◊˜ª∑æ≥,–Ë“™µ˜’˚¥À∫Ø ˝£¨◊¢“‚ ±÷”π˝øÏ ±–Ë“™–ﬁ∏ƒ
µ±∏ƒ”√1TµƒMCU ±,«Îµ˜’˚¥À—” ±∫Ø ˝
**************************************/
void Delay_5us()
{
	int i;
 	for(i=0;i<6;i++);
}

/**************************************
—” ±5∫¡√Î(STC90C52RC@12M)
≤ªÕ¨µƒπ§◊˜ª∑æ≥,–Ë“™µ˜’˚¥À∫Ø ˝
µ±∏ƒ”√1TµƒMCU ±,«Îµ˜’˚¥À—” ±∫Ø ˝
**************************************/
void Delay5ms()
{
	unsigned int n = 10;
	while (n--);
}
/**************************************
∆ º–≈∫≈
**************************************/
void QMC5883_Start()
{
    SDA_OUT();
    SDA_HIGH();                    //¿≠∏ﬂ ˝æ›œﬂ
    SCL_HIGH();                    //¿≠∏ﬂ ±÷”œﬂ
    Delay_5us();                 //—” ±
    SDA_LOW();                    //≤˙…˙œ¬Ωµ—ÿ
    Delay_5us();                 //—” ±
    SCL_LOW();                    //¿≠µÕ ±÷”œﬂ
}

/**************************************
Õ£÷π–≈∫≈
**************************************/
void QMC5883_Stop()
{
    SDA_LOW();                    //¿≠µÕ ˝æ›œﬂ
    SCL_HIGH();                    //¿≠∏ﬂ ±÷”œﬂ
    Delay_5us();                 //—” ±
    SDA_HIGH();                    //≤˙…˙…œ…˝—ÿ
    Delay_5us();                 //—” ±
}

/**************************************
∑¢ÀÕ”¶¥–≈∫≈
»Îø⁄≤Œ ˝:ack (0:ACK 1:NAK)
**************************************/
void QMC5883_SendACK(unsigned char ack)
{
SDA_OUT();
	if(ack==1)
		SDA_HIGH(); 
	else
		SDA_LOW(); 
    //SDA = ack;                  //–¥”¶¥–≈∫≈
    SCL_HIGH();                    //¿≠∏ﬂ ±÷”œﬂ
    Delay_5us();                 //—” ±
    SCL_LOW();                    //¿≠µÕ ±÷”œﬂ
    Delay_5us();                 //—” ±
}

/**************************************
Ω” ’”¶¥–≈∫≈
**************************************/
unsigned char QMC5883_RecvACK()
{
  unsigned char CY;
    SCL_HIGH();                    //¿≠∏ﬂ ±÷”œﬂ
    Delay_5us();                //—” 
    SDA_IN();
    CY = SDA_READ();                   //Ω¯Œª±Í÷æ ∂¡”¶¥–≈∫≈
    SCL_LOW();                    //¿≠µÕ ±÷”œﬂ
    Delay_5us();                //—” ±
    return CY;
}

/**************************************
œÚIIC◊‹œﬂ∑¢ÀÕ“ª∏ˆ◊÷Ω⁄ ˝æ›
**************************************/
void QMC5883_SendByte(u8 dat)
{
    u8 i;
 SDA_OUT();
    for (i=0; i<8; i++)         //8Œªº∆ ˝∆˜
    {
     //   dat <<= 1;              //“∆≥ˆ ˝æ›µƒ◊Ó∏ﬂŒª
		if(dat&0x80)
			SDA_HIGH(); 
		else
			SDA_LOW();
		dat <<= 1;  
       // SDA = CY;               //ÀÕ ˝æ›ø⁄
        SCL_HIGH();                //¿≠∏ﬂ ±÷”œﬂ
        Delay_5us();             //—” ±
        SCL_LOW();                //¿≠µÕ ±÷”œﬂ
        Delay_5us();             //—” ±

    }
    QMC5883_RecvACK();
}

/**************************************
¥”IIC◊‹œﬂΩ” ’“ª∏ˆ◊÷Ω⁄ ˝æ›
**************************************/
u8 QMC5883_RecvByte()
{
    u8 i;
    u8 dat = 0;
 SDA_OUT();
    SDA_HIGH();                    // πƒ‹ƒ⁄≤ø…œ¿≠,◊º±∏∂¡»° ˝æ›,
    for (i=0; i<8; i++)         //8Œªº∆ ˝∆˜
    {   SDA_IN();
        dat <<= 1;
        SCL_HIGH();                //¿≠∏ﬂ ±÷”œﬂ
        Delay_5us();             //—” ±
        
	if(SDA_READ())
			dat |=0x01;             //∂¡ ˝æ› 
		else
			dat =dat;             //∂¡ ˝æ›            
       // dat |= SDA;             //∂¡ ˝æ›               
        SCL_LOW();                //¿≠µÕ ±÷”œﬂ
        Delay_5us();             //—” ±
    }
    return dat;
}

void I2C_WriteByte(uint8_t Val,uint8_t  Reg, uint8_t addr)
{
	QMC5883_Start();                  //∆ º–≈∫≈
	QMC5883_SendByte(addr);   //∑¢ÀÕ…Ë±∏µÿ÷∑+–¥–≈∫≈
	QMC5883_SendByte(Reg);    //ƒ⁄≤øºƒ¥Ê∆˜µÿ÷∑£¨«Î≤Œøº÷–Œƒpdf22“≥ 
	QMC5883_SendByte(Val);       //ƒ⁄≤øºƒ¥Ê∆˜ ˝æ›£¨«Î≤Œøº÷–Œƒpdf22“≥ 
	QMC5883_Stop();
}
 void  I2C_ReadByte(uint8_t *RegVal,uint8_t recv_cnt,uint8_t Reg,uint8_t addr)
{
	unsigned char i;
        QMC5883_Start();                          //∆ º–≈∫≈
	QMC5883_SendByte(addr);           //∑¢ÀÕ…Ë±∏µÿ÷∑+–¥–≈∫≈
	QMC5883_SendByte(Reg);                   //∑¢ÀÕ¥Ê¥¢µ•‘™µÿ÷∑£¨¥”0ø™ º	
	QMC5883_Start();                          //∆ º–≈∫≈
	QMC5883_SendByte(addr+1);         //∑¢ÀÕ…Ë±∏µÿ÷∑+∂¡–≈∫≈
	for(i=0;i<recv_cnt;i++)
	{
		RegVal[i]=QMC5883_RecvByte();              //∂¡≥ˆºƒ¥Ê∆˜ ˝æ›

		if(i==(recv_cnt-1))
			QMC5883_SendACK(0x01);
		else
			QMC5883_SendACK(0x00);
	}

	
	QMC5883_Stop();                           //Õ£÷π–≈∫≈
}
/* ÀΩ”–∫Ø ˝ -------------------------------------------------------------------*/
/******************************************************************************
* @∫Ø ˝√˚≥∆£∫void MPU6050_Init(void)
* @∫Ø ˝√Ë ˆ£∫MPU6050¥´∏–∆˜≥ı ºªØ
* @ ‰»Î≤Œ ˝£∫None
* @ ‰≥ˆ≤Œ ˝£∫None
*******************************************************************************/
uint8_t QMC5883_ReadReg(uint8_t Reg)
{
    u8 RegVal = 0;
    I2C_ReadByte(&RegVal,1,Reg,QMC5883_ADDR);
    return RegVal;
}
/******************************************************************************
* @∫Ø ˝√˚≥∆£∫void MPU6050_Init(void)
* @∫Ø ˝√Ë ˆ£∫MPU6050¥´∏–∆˜≥ı ºªØ
* @ ‰»Î≤Œ ˝£∫None
* @ ‰≥ˆ≤Œ ˝£∫None
*******************************************************************************/
void QMC5883_WriteReg(uint8_t Val,uint8_t Reg)
{
    I2C_WriteByte(Val,Reg, QMC5883_ADDR);
}
/******************************************************************************
* @∫Ø ˝√˚≥∆£∫void QMC5883_Init(void)
* @∫Ø ˝√Ë ˆ£∫≥Ã–Ú»Îø⁄
* @ ‰»Î≤Œ ˝£∫None
* @ ‰≥ˆ≤Œ ˝£∫None
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
* @∫Ø ˝√˚≥∆£∫void QMC5883_GetData(float *Magnet)
* @∫Ø ˝√Ë ˆ£∫ªÒ»°µÿ¥≈ ˝æ›
* @ ‰»Î≤Œ ˝£∫µÿ¥≈buff
* @ ‰≥ˆ≤Œ ˝£∫None
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
        Magnet[i] = (float)MagnetRawAd[i] / 120.f;//µ•ŒªuT
		
    }
}

/* Œƒº˛Ω· ¯ --------------------------------------------------------------------*/

