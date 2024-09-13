/*
SPI2: PB13,PB14,PB15
CS  : PB12
*/
#include "AD_SPI2\AD_SPI2.h"

#include "includes.h"

#define MyDelayms          OSTimeDly
//===AD7705相关寄存器命令,前四位选择寄存器==
#define REG_COM         0x00
#define REG_SETUP       0x10
#define REG_CLOCK       0x20
#define REG_DATA        0x30
#define REG_TEST        0x40
#define REG_NO          0x50
#define REG_OFFSET      0x60
#define REG_GAIN        0x70
#define WRITE_COMMAND   0x00
#define READ_COMMAND    0x08

#define STBY_POWERDOWN  0x04
#define STBY_NORMAL     0x00
#define CHANNEL0        0x00
#define CHANNEL1        0x01
//=====:设置寄存器===========
#define MODE_NORMAL     0x00
#define MODE_SELF       0x40
#define MODE_ZERO       0x80
#define MODE_FULL       0xC0
#define GAIN_X1         0x00
#define GAIN_X2         0x08
#define GAIN_X4         0x10
#define GAIN_X8         0x18
#define GAIN_X16        0x20
#define GAIN_X32        0x28
#define GAIN_X64        0x30
#define GAIN_X128       0x38
#define BIPOLAR         0x00
#define UNPOLAR         0x04
#define INPUTBUF        0x02
#define INPUTNOBUF      0x00

#define   DATA_NORMAL             0x06
#define   DATA_OVER               0x05
#define   DATA_ERRO               0x09
#define   DATA_TIME               0x04

//.............................................
void         ADSPI2Reset(       void);
unsigned int ADSPI2WaitReady(   unsigned char ADChannel);
void         ADSPI2WriteOneByte(unsigned char Command,unsigned char Commanddata);
unsigned char ADSPI2ReadOneByte(unsigned char Command);
/*===============================================================================
WriteDate:
EditDate :
Author   :
Function : 
===============================================================================*/
void ADSPI2Intial(void)
{
	  unsigned short mReg;
//..IO Inital   PF2 PE7 PB6
    GPIO_InitTypeDef GPIO_InitStructure;
	   SPI_InitTypeDef  SPI_InitStructure;
    /* Enable SPI2 and GPIO clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
	  mReg = 1000;
	  while(mReg--)
		{
			 if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) == Bit_SET ) break;
			 MyDelayms(1);
		}
		if( mReg == 0 ) mSystemData.mSPIFlag = mSystemData.mSPIFlag | 0x20; 
	
    /* Configure SPI pins: SCK, MISO and MOSI */
	  //GPIO_Mode_IN_FLOATING
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13  | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	  //GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	  //GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    /* Configure I/O for AD7705 Chip select */
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    AD_SPI2_CS_H;
	  		   
    // SP2 configuration 
		SPI_Cmd(SPI2, DISABLE); 
    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial      = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
    //
    SPI_Cmd(SPI2,ENABLE);
//..................................................................................
    //=====AD7705 intial
    ADSPI2Reset();
    MyDelayms(100);
    //.......................................
		mReg = ADSPI2ReadOneByte(REG_CLOCK|READ_COMMAND|CHANNEL0);
		if( mReg != 0x05  )  mSystemData.mSPIFlag = mSystemData.mSPIFlag | 0x02;
    //.......................................
    //AD Number2设置时钟寄存器，通道0
		ADSPI2WaitReady(CHANNEL0); 
		
		
    ADSPI2WriteOneByte(REG_CLOCK|WRITE_COMMAND|CHANNEL0,0x05);
    //AD Number2设置设置寄存器，通道0:自校准、增益1、单极性、输入无缓冲器
		ADSPI2WaitReady(CHANNEL0);  
    ADSPI2WriteOneByte(REG_SETUP|WRITE_COMMAND|CHANNEL0,MODE_NORMAL|GAIN_X1|UNPOLAR|INPUTNOBUF);		
		ADSPI2WaitReady(CHANNEL0);  
    ADSPI2WriteOneByte(REG_SETUP|WRITE_COMMAND|CHANNEL0,MODE_SELF  |GAIN_X1|UNPOLAR|INPUTNOBUF);
    
		//AD Number2设置时钟寄存器，通道1
		//ADSPI2WaitReady(CHANNEL1); 
    //ADSPI2WriteOneByte(REG_CLOCK|WRITE_COMMAND|CHANNEL1,0x05);
    //AD Number2设置设置寄存器，通道0:自校准、增益1、单极性、输入无缓冲器
		//ADSPI2WaitReady(CHANNEL1);  
    //ADSPI2WriteOneByte(REG_SETUP|WRITE_COMMAND|CHANNEL1,MODE_NORMAL|GAIN_X1|UNPOLAR|INPUTNOBUF);		
		//ADSPI2WaitReady(CHANNEL1);  
    //ADSPI2WriteOneByte(REG_SETUP|WRITE_COMMAND|CHANNEL1,MODE_SELF  |GAIN_X1|UNPOLAR|INPUTNOBUF);
}
/*==============================================================================
*           硬件SPI写和读 器件
* 硬件SPI写:SCK,MOSI起作用，此时数据从MOSI进入从机，而从机MISO此时为高阻，因而
*           主机接受的数据为无效数据。
* 硬件SPI写:SCk,MISO起作用，此时数据从MISO进入主机，而从机此时不关心MOSI的数据
* 对于SPI函数:写入数据，返回参数不影响
* 对于SPI函数:读取数据，入口参数不影响
* Parameter:  data,Master unsigned Data write to Slave
* return   :  Slave Data Out to Master
* Date     :  2007.12.10
* EDIT Data:  2007.12.10
* note     :  datasheet page 28
* Writer   :  Wu Qingsong
==============================================================================
*/
unsigned char ADSPI2Swap(unsigned char data)
{

    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET);    
    SPI_I2S_SendData(SPI2,data);    
    while( SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET );    
    return SPI_I2S_ReceiveData(SPI2);
}
/*
*/
void ADSPI2SendByte(unsigned char Data)
{
//..CS EN
    AD_SPI2_CS_L;
    ADSPI2Swap(Data);
    AD_SPI2_CS_H;
}
/*
*/
unsigned char ADSPI2ReadByte(void)
{
    unsigned char Data;
    //..CS EN
    AD_SPI2_CS_L;
    Data = ADSPI2Swap(0xff);
    AD_SPI2_CS_H;
    return Data;
}
/*
==============================================================================
*   Send 1 for 32 clk
==============================================================================
*/
void ADSPI2Reset(void)
{
    AD_SPI2_CS_L;
    ADSPI2Swap(0xff);
    ADSPI2Swap(0xff);
    ADSPI2Swap(0xff);
    ADSPI2Swap(0xff);
    AD_SPI2_CS_H;  
}
/*
==============================================================================
*
==============================================================================
*/
void ADSPI2WriteOneByte(unsigned char Command,unsigned char Commanddata)
{
    ADSPI2SendByte(Command);            //..send cmand
    ADSPI2SendByte(Commanddata);        //..send cmand  Data
}
/*
==============================================================================
*
==============================================================================
*/
unsigned char ADSPI2ReadOneByte(unsigned char Command)
{
    unsigned char Data;
    ADSPI2SendByte(Command);            //..send cmand
    Data = ADSPI2ReadByte();            //..send cmand  Data;
    return Data;
}
/*
==============================================================================
*
==============================================================================
*/
unsigned int ADSPI2ReadTwoByte(unsigned char Command)
{
    unsigned char ByteH,ByteL;
    unsigned int  Data;
    //cli();
    //..send cmand
    ADSPI2SendByte(Command);
    ByteH = ADSPI2ReadByte();
    ByteL = ADSPI2ReadByte();
    //sei();
    Data = (((unsigned int)ByteH)<<8 ) + ByteL;
    return Data;
}

/*=================================================
【Function  】:	  
【WriteDate 】:2011.11.21
【EditDate  】:2011.11.21
【Autor     】:wuqingsong
【Descrption】:
=================================================*/
unsigned int ADSPI2WaitReady(unsigned char ADChannel)
{
    unsigned int WaitTimes;
	  unsigned char ReadySignal;
    WaitTimes = 0;
	  while(WaitTimes<200)
	  {
		   ReadySignal = ADSPI2ReadOneByte(REG_COM|READ_COMMAND|ADChannel);
		   if( ReadySignal<0x80)  break;		
		   MyDelayms(1);
		   WaitTimes++;
	  }
	  if( WaitTimes>=190)   return WaitTimes;	 
	  return 0;
}
/*
==============================================================================

==============================================================================
*/
unsigned int ADSPI2ReadADSignal(unsigned char Channel,unsigned int *ReturnTimes)
{
    unsigned int  ADData;	  
//....while wait for ready signal	
	  *ReturnTimes = ADSPI2WaitReady(Channel); 
	  ADData = ADSPI2ReadTwoByte(REG_DATA|READ_COMMAND|Channel);
    return ADData;   
}
/*=================================================================
Function name:collection
Descreption  :collect the data From the ad
Auther       :Wuqingsong
Write Date   :2007.03.07
Edit Date    :2007.03.07
Parameter    :gain,AD芯片增益
=================================================================*/
unsigned int ADSPI2collectionADSignal(unsigned char channel,unsigned char *erro)
{
    unsigned char i;
    unsigned short mAdbuf[10],mMax,mMin;
    unsigned int ever,Parameter;
    //uint32_t sum=0;
    *erro = DATA_NORMAL;

	
    //:设置增益
    //AD Number1设置时钟寄存器，通道0   GAIN_X1  : 3K电阻
	  ADSPI2WaitReady(channel);
	  ADSPI2WriteOneByte(REG_SETUP|WRITE_COMMAND|channel,MODE_SELF|GAIN_X1|UNPOLAR|INPUTNOBUF);	
	  ADSPI2WaitReady(channel);
	   
    //:两次空数据采集
    ADSPI2ReadADSignal(channel,&Parameter);
    ADSPI2ReadADSignal(channel,&Parameter);
    //............................................................
	  for(i =0,mMax=0,mMin= 0xffff,ever = 0; i< 10;i++ )
	  {
	      mAdbuf [i] = ADSPI2ReadADSignal(channel,&Parameter);
			  if( Parameter>=99)
        { //超时
           *erro = DATA_TIME;
        }
			  ever       = ever + mAdbuf [i];
			  if( mAdbuf [i] > mMax ) mMax = mAdbuf [i];
			  if( mAdbuf [i] < mMin ) mMin = mAdbuf [i];
				OSTimeDly(5); 
		}
    ever = (ever - mMax - mMin)>>3 ;// /8    
    return ever;
}
//$
