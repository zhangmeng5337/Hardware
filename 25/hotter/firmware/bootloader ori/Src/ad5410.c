/******************************************************************************
* 文 件  名 称:BspAD5410.c
* 文件功能概述:实现AD5410的接口
* 文 件  作 者:
* 版        本:V1.0.0.0
* 修 订  记 录:2017-6-30创建
******************************************************************************/
 
/***************************相关配置*******************************************
              该驱动对应于AD5410的菊花链模式
******************************************************************************/
 
#include "BspAD5410.h"
 
#define AD5410xDelayUs(x) DelayUs(x) 
#define USE_DAISY_CHAIN 0

extern SPI_HandleTypeDef hspi3;

/*
 * 数据类型-AD5410端口信息
 */
typedef struct AD5410CtrlStruc
{
  E_SPIx     AD5410xSPIx;
  S_GpioCtrl AD5410xLATCH;
}S_AD5410Ctrl;
 
/*
 * 用于记录AD5410的端口信息
 */
static S_AD5410Ctrl stAD5410Ctrl[E_AD5410_Max] = 
{
  /* AD5410_1 */
  {
    E_SPI_2,
    {RCC_AHB1Periph_GPIOG,GPIOG,GPIO_Pin_6}    //LATCH
  },
  /* AD5410_2 */
  {
    E_SPI_2,
    {RCC_AHB1Periph_GPIOG,GPIOG,GPIO_Pin_6}    //LATCH
  }
};
 
/*
 * 拉高5410的LATCH端口
 */
static __inline void AD5410_LATCH_HIGH(E_AD5410 eAD5410)
{
  HAL_GPIO_WritePin(LATCH_AO_MCU_GPIO_Port, LATCH_AO_MCU_Pin, GPIO_PIN_SET);
}
/*
 * 拉低5410的LATCH端口
 */
static __inline void AD5410_LATCH_LOW(E_AD5410 eAD5410)
{
  //GPIOxWriteBit(stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOx, stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOxPinx , 0);
  HAL_GPIO_WritePin(LATCH_AO_MCU_GPIO_Port, LATCH_AO_MCU_Pin, GPIO_PIN_RESET);
}

/*
 *SPI总线上挂接的设备类型不一样，每次使用前均需对总线进行初始化
 */
static void AD5410InitSPIModule(E_AD5410 eAD5410)
{

}
 
/*******************************************************************************
 * 函 数 名:static sint8 AD5410xEnableOperate(void)
 * 参    数:
 * 返    回:0
 * 创 建 人:
 * 创建时间:2017-6-30
 * 详    述:在LATCH信号线产生一个下降沿
 * 修改记录:2017-6-30创建
*******************************************************************************/
static sint8 AD5410xEnableOperate(E_AD5410 eAD5410)
{
  
  //拉高LATCH
  AD5410_LATCH_HIGH(eAD5410);
 
  AD5410xDelayUs(10);
  
  //拉低LATCH
  AD5410_LATCH_LOW(eAD5410);
  
  return 0;
}
 
/*******************************************************************************
 * 函 数 名:static sint8 AD5410xDisableOperate(void)
 * 参    数:void
 * 返    回:0
 * 创 建 人:
 * 创建时间:2017-6-30
 * 详    述:在LATCH信号线产生一个上升沿，该信号线的上升沿会锁存数据
 * 修改记录:2017-6-30创建
*******************************************************************************/
static sint8 AD5410xDisableOperate(E_AD5410 eAD5410)
{
  
  //拉低LATCH
  AD5410_LATCH_LOW(eAD5410);
  
  AD5410xDelayUs(10);
  
  //拉高LATCH
  AD5410_LATCH_HIGH(eAD5410);
  
  return 0;
}
 
/*******************************************************************************
 * 函 数 名:static sint8 AD5410xWriteReg(E_AD5410 eAD5410, uint8 RegAddr, uint16 value)
 * 参    数:E_AD5410 eAD5410 : E_AD5410_1或E_AD5410_2
            uint8 RegAddr      : 寄存器地址字节
             uint16 value      ：数据
 * 返    回:0
 * 创 建 人:
 * 创建时间:2017-6-30
 * 详    述:AD5410写寄存器
 * 修改记录:2017-6-30创建
*******************************************************************************/
static sint8 AD5410xWriteReg(E_AD5410 eAD5410, uint8 RegAddr, uint16 value)
{
  
#if USE_DAISY_CHAIN
  uint8 SendData[3*E_AD5410_Max]={0};
 
  memset(SendData,0,sizeof(SendData));
  
  AD5410xEnableOperate(eAD5410);
  
  SendData[(E_AD5410_Max-1-eAD5410)*3 + 0] = RegAddr;
  SendData[(E_AD5410_Max-1-eAD5410)*3 + 1] = value>>8;
  SendData[(E_AD5410_Max-1-eAD5410)*3 + 2] = value&0xff;
  
  SPIxWriteBytes(stAD5410Ctrl[eAD5410].AD5410xSPIx, SendData, 3*E_AD5410_Max);
   HAL_SPI_TransmitReceive(&hspi3, SendData, pRxData,Size, 200);
  AD5410xDisableOperate(eAD5410);
#else
  uint8 SendData[3]={0x00, 0x00, 0x00};
 
  AD5410xEnableOperate(eAD5410);
   
  SendData[0] = RegAddr;
  SendData[1] = value>>8;
  SendData[2] = value&0xff;
  
  SPIxWriteBytes(stAD5410Ctrl[eAD5410].AD5410xSPIx, SendData, 3);  
  
  AD5410xDisableOperate(eAD5410);
#endif
     
  return 0;
}
 
/*******************************************************************************
 * 函 数 名:static sint8 AD5410xReadReg(E_AD5410 eAD5410,uint8 RegAddr, uint16 *value)
 * 参    数:E_AD5410 eAD5410 : E_AD5410_1或E_AD5410_2
            uint8 RegAddr  :寄存器地址
            uint16 *value  ：数据指针
 * 返    回:0
 * 创 建 人:
 * 创建时间:2017-6-30
 * 详    述:AD5410读寄存器操作，只进行读取操作，读取命令的发送部分额外处理
 * 修改记录:2017-6-30创建
*******************************************************************************/
static sint8 AD5410xReadReg(E_AD5410 eAD5410,uint8 RegAddr, uint16 *value)
{
	uint8 SendData[3*E_AD5410_Max]={0};
  uint8 RecvData[3*E_AD5410_Max]={0};
  uint8 i = 0;
	
	/*先配置为回读模式*/
  AD5410xEnableOperate(eAD5410);
  
	AD5410xWriteReg(eAD5410,ADDR_FUNC_READBACK,RegAddr);
	
	AD5410xDisableOperate(eAD5410);
	
	AD5410xDelayUs(10);
	
	AD5410xEnableOperate(eAD5410);
	
#if USE_DAISY_CHAIN
  
  memset(SendData, 0, sizeof(SendData));
  memset(RecvData, 0, sizeof(RecvData));
  
  for(i=0;i<E_AD5410_Max*3;i++)
  {
    SPIxWriteReadByte(stAD5410Ctrl[eAD5410].AD5410xSPIx, &SendData[i], &RecvData[i]);
  }
  
  {
    uint16 Temp[2]={0,0};
    
    Temp[0] = RecvData[(E_AD5410_Max-1-eAD5410)*3 + 1];
    Temp[1] = RecvData[(E_AD5410_Max-1-eAD5410)*3 + 2];
    
    *value = (Temp[0]<<8) | Temp[1];
  }
  
#else
  /*发送空指令字，使得数据从SDO上输出*/
	SPIxWriteReadByte(stAD5410Ctrl[eAD5410].AD5410xSPIx, &SendData[0], &RecvData[0]);
	SPIxWriteReadByte(stAD5410Ctrl[eAD5410].AD5410xSPIx, &SendData[1], &RecvData[1]);
	SPIxWriteReadByte(stAD5410Ctrl[eAD5410].AD5410xSPIx, &SendData[2], &RecvData[2]);  
  
  *value = (RecvData[1]<<8) | RecvData[2];
#endif
  
	AD5410xDisableOperate(eAD5410);
	
 
  
  return 0;
}
 
/*******************************************************************************
 * 函 数 名:static sint8 AD5410xSoftReset(E_AD5410 eAD5410)
 * 参    数:E_AD5410 eAD5410 : E_AD5410_1或E_AD5410_2
 * 返    回:0
 * 创 建 人:
 * 创建时间:2017-6-30
 * 详    述:AD5410软件复位         
 * 修改记录:2017-6-30创建
*******************************************************************************/
static sint8 AD5410xSoftReset(E_AD5410 eAD5410)
{
  uint8 SendValidData[3]={0x56, 0x00, 0x00};
  uint8 SendInvalidData[3]={0x00, 0x00, 0x00};
	
	AD5410xEnableOperate(eAD5410);
	
#if USE_DAISY_CHAIN
  {
    sint8 i = 0;
    for(i=E_AD5410_Max-1;i>=0;i--)
    {
      if(eAD5410 == ((E_AD5410)i))
      {
        SPIxWriteBytes(stAD5410Ctrl[eAD5410].AD5410xSPIx, SendValidData, 3);
      }
      else
      {
        /*发送空指令字，跳过后一级5410的数据*/
		   SPIxWriteBytes(stAD5410Ctrl[eAD5410].AD5410xSPIx, SendInvalidData, 3);  
      }
    }
  }
#else
  SPIxWriteBytes(stAD5410Ctrl[eAD5410].AD5410xSPIx, SendValidData, 3);
#endif
 
  
	AD5410xDisableOperate(eAD5410);
  
  return 0;
}
 
/*******************************************************************************
 * 函 数 名:sint8 AD5410xInit(E_AD5410 eAD5410)
 * 参    数:E_AD5410 eAD5410:详见E_AD5410
 * 返    回: 0  : 正常返回 
             -1 ：SPI初始化失败        -2 ：AD5410复位失败
 * 创 建 人:
 * 创建时间:2017-6-30
 * 详    述:AD5410初始化      
 * 修改记录:2017-6-30创建
*******************************************************************************/
sint8 AD5410xInit(E_AD5410 eAD5410)
{
  sint8 ret = 0;
  
  if(eAD5410 < E_AD5410_Max)
  {

#if USE_DAISY_CHAIN
    /******** 菊花链模式，公用一个LATCH引脚 ********/
    
    /* 配置LATCH引脚 */
    RCC_AHB1PeriphClockCmd(stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOxSource, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOxPinx;
    GPIO_Init(stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOx, &GPIO_InitStructure);  
    
    AD5410_LATCH_LOW(eAD5410);	
    
    AD5410InitSPIModule(eAD5410);//SPI总线上挂接的设备类型不一样，每次使用前均需对总线进行初始化
    
    for(eAD5410=E_AD5410_1;eAD5410<E_AD5410_Max;eAD5410++)
    {
      AD5410xSoftReset(eAD5410);
    }
    
    for(eAD5410=E_AD5410_1;eAD5410<E_AD5410_Max;eAD5410++)
    {  
      /******************配置AD5410为0-20mA范围，使能菊花链模式，使能输出，选择外部15K电阻************************************/
	    AD5410xWriteReg(eAD5410, ADDR_FUNC_CTRL_REG, CTRL_0_20_RANG | CTRL_DCEN_ENABLE | CTRL_OUTEN_ENABLE | CTRL_REXT_ENABLE);  
    }
#else    
    /*配置LATCH引脚*/
    RCC_AHB1PeriphClockCmd(stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOxSource, ENABLE);
    
    /*配置LATCH引脚*/
    GPIO_InitStructure.GPIO_Pin = stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOxPinx;
    GPIO_Init(stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOx, &GPIO_InitStructure);  
    
    AD5410_LATCH_LOW(eAD5410);		
    
	  AD5410InitSPIModule(eAD5410);//SPI总线上挂接的设备类型不一样，每次使用前均需对总线进行初始化
    
    AD5410xSoftReset(eAD5410);
    
    /******************配置AD5410为0-20mA范围，使能菊花链模式，使能输出，选择外部15K电阻************************************/
	  AD5410xWriteReg(eAD5410, ADDR_FUNC_CTRL_REG, CTRL_0_20_RANG | CTRL_DCEN_ENABLE | CTRL_OUTEN_ENABLE | CTRL_REXT_ENABLE);
#endif
  }
  else
  {
    ret = -1;
  }
 
  return ret;
}
 
/*******************************************************************************
 * 函 数 名:sint8 AD5410xWriteDataReg(E_AD5410 eAD5410, uint16 data)
 * 参    数:E_AD5410 eAD5410 : E_AD5410_1或E_AD5410_2
             uint16 data       ：数据
 * 返    回: 0  : 正常返回 
 * 创 建 人:
 * 创建时间:2017-6-30
 * 详    述:AD5410写数据寄存器操作   
 * 修改记录:2017-6-30创建
*******************************************************************************/
sint8 AD5410xWriteDataReg(E_AD5410 eAD5410, uint16 data)
{
  uint16 SendData = 0x0000;
	
	AD5410InitSPIModule(eAD5410);//SPI总线上挂接的设备类型不一样，每次使用前均需对总线进行初始化
	
  SendData = (data<<4)&0xFFF0 ;  
	    
  AD5410xWriteReg(eAD5410, ADDR_FUNC_DATA_REG, SendData);
   
  return 0;
}
 
/*******************************************************************************
 * 函 数 名:sint8 AD5410xRead(E_AD5410 eAD5410, E_AD5410_REG_ADDR RegAddr, uint16 *data)
 * 参    数:E_AD5410 eAD5410         : E_AD5410_1或E_AD5410_2
             E_AD5410_REG_ADDR RegAddr : 寄存器地址
             uint16 *data              ：数据
 * 返    回: 0  : 正常返回 
 * 创 建 人:
 * 创建时间:2017-6-30
 * 详    述:AD5410读数据寄存器操作   
 * 修改记录:2017-6-30创建
*******************************************************************************/
sint8 AD5410xReadDataReg(E_AD5410 eAD5410, uint16 *data)
{
  uint16 RecvData = 0x0000;  
  
	AD5410InitSPIModule(eAD5410);//SPI总线上挂接的设备类型不一样，每次使用前均需对总线进行初始化
	
  AD5410xReadReg(eAD5410,ADDR_FUNC_DATA_REG,&RecvData);
  
  RecvData = ((RecvData>>4)&0x0FFF);
  
  *data = RecvData;
    
  return 0;
}
 
/*******************************************************************************
 * 函 数 名:uint16 AD5410xValueToCurrent(float value, float MaxValue)
 * 参    数:float value     ：上层要输出的数据
            float MaxValue ：上层传入的最大值
 * 返    回: 0  : 正常返回 
 * 创 建 人:
 * 创建时间:2017-6-30
 * 详    述:根据传入最大值，计算量化得到符合AD5410范围的数据
 * 修改记录:2017-6-30创建
*******************************************************************************/
uint16 AD5410xValueToCurrent(float value, float MaxValue)
{
  if(value >= MaxValue)
  {
    value = MaxValue;
  }
  else
  {}
    
  return (uint16)roundf((float)(value * AD5410_RATIO / MaxValue));
}
 
#if 1
void AD5410xTestSPI(void)
{
	uint8   SendData=0x55;
	uint16   RecvData[9]={0x00,0x00,0x00};
		
	AD5410InitSPIModule(E_AD5410_1);
	
	AD5410xWriteDataReg(E_AD5410_1,SendData);
	AD5410xWriteDataReg(E_AD5410_2,SendData);
	
	AD5410xReadDataReg(E_AD5410_1,RecvData);
	AD5410xReadDataReg(E_AD5410_2,&RecvData[1]);
	
 
	return;
}


