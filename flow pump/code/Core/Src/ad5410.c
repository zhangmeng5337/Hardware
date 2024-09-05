/******************************************************************************
* �� ��  �� ��:BspAD5410.c
* �ļ����ܸ���:ʵ��AD5410�Ľӿ�
* �� ��  �� ��:
* ��        ��:V1.0.0.0
* �� ��  �� ¼:2017-6-30����
******************************************************************************/
 
/***************************�������*******************************************
              ��������Ӧ��AD5410�ľջ���ģʽ
******************************************************************************/
#include "AD5410.h" 
#include "BspAD5410.h"
 extern SPI_HandleTypeDef hspi3;
#define AD5410xDelayUs(x) HAL_Delay(x) 
#define USE_DAISY_CHAIN 0
 
/*
 * ��������-AD5410�˿���Ϣ
 */
//typedef struct AD5410CtrlStruc
//{
//  E_SPIx     AD5410xSPIx;
//  S_GpioCtrl AD5410xLATCH;
//}S_AD5410Ctrl;
 
/*
 * ���ڼ�¼AD5410�Ķ˿���Ϣ
 */
//static S_AD5410Ctrl stAD5410Ctrl[E_AD5410_Max] = 
//{
//  /* AD5410_1 */
//  {
//    E_SPI_2,
//    {RCC_AHB1Periph_GPIOG,GPIOG,GPIO_Pin_6}    //LATCH
//  },
//  /* AD5410_2 */
//  {
//    E_SPI_2,
//    {RCC_AHB1Periph_GPIOG,GPIOG,GPIO_Pin_6}    //LATCH
//  }
//};
 
/*
 * ����5410��LATCH�˿�
 */
static __inline void AD5410_LATCH_HIGH( )
{
//  GPIOxWriteBit(stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOx, stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOxPinx , 1);
	HAL_GPIO_WritePin(LATCH_AO_GPIO_Port,LATCH_AO_Pin,GPIO_PIN_SET);


}
/*
 * ����5410��LATCH�˿�
 */
static __inline void AD5410_LATCH_LOW( )
{
//  GPIOxWriteBit(stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOx, stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOxPinx , 0);
	HAL_GPIO_WritePin(LATCH_AO_GPIO_Port,LATCH_AO_Pin,GPIO_PIN_RESET);

}
 
/*
 *SPI�����Ϲҽӵ��豸���Ͳ�һ����ÿ��ʹ��ǰ��������߽��г�ʼ��
 */
//static void AD5410InitSPIModule(E_AD5410 eAD5410)
//{
//	SPI_InitTypeDef SPIxInitStruct;  
//    
//  /* Initialize the SPI_Direction member */
//  SPIxInitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//  /* initialize the SPI_Mode member */
//  SPIxInitStruct.SPI_Mode = SPI_Mode_Master;
//  /* initialize the SPI_DataSize member */
//  SPIxInitStruct.SPI_DataSize = SPI_DataSize_8b;
//  /* Initialize the SPI_CPOL member */
//  SPIxInitStruct.SPI_CPOL = SPI_CPOL_Low;
//  /* Initialize the SPI_CPHA member */
//  SPIxInitStruct.SPI_CPHA = SPI_CPHA_1Edge;
//  /* Initialize the SPI_NSS member */
//  SPIxInitStruct.SPI_NSS = SPI_NSS_Soft;    
//  /* Initialize the SPI_FirstBit member */
//  SPIxInitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
//  /* Initialize the SPI_CRCPolynomial member */
//  SPIxInitStruct.SPI_CRCPolynomial = 7;
//	
//	SPIxUpdateModule(stAD5410Ctrl[eAD5410].AD5410xSPIx,&SPIxInitStruct);
//}
 
/*******************************************************************************
 * �� �� ��:static uint8_t AD5410xEnableOperate(void)
 * ��    ��:
 * ��    ��:0
 * �� �� ��:
 * ����ʱ��:2017-6-30
 * ��    ��:��LATCH�ź��߲���һ���½���
 * �޸ļ�¼:2017-6-30����
*******************************************************************************/
static uint8_t AD5410xEnableOperate( )
{
  
  //����LATCH
  AD5410_LATCH_HIGH();
 
  AD5410xDelayUs(10);
  
  //����LATCH
  AD5410_LATCH_LOW();
  
  return 0;
}
 
/*******************************************************************************
 * �� �� ��:static uint8_t AD5410xDisableOperate(void)
 * ��    ��:void
 * ��    ��:0
 * �� �� ��:
 * ����ʱ��:2017-6-30
 * ��    ��:��LATCH�ź��߲���һ�������أ����ź��ߵ������ػ���������
 * �޸ļ�¼:2017-6-30����
*******************************************************************************/
static uint8_t AD5410xDisableOperate( )
{
  
  //����LATCH
  AD5410_LATCH_LOW();
  
  AD5410xDelayUs(10);
  
  //����LATCH
  AD5410_LATCH_HIGH();
  
  return 0;
}

void SPIxWriteBytes(uint8_t DataTx[], uint8_t DataRx[], uint8_t byteLength)
{
    
    HAL_SPI_TransmitReceive(&hspi3, DataTx,DataRx, byteLength,2000);

}
/*******************************************************************************
 * �� �� ��:static uint8_t AD5410xWriteReg(E_AD5410 eAD5410, uint8_t RegAddr, uint16_t value)
 * ��    ��:E_AD5410 eAD5410 : E_AD5410_1��E_AD5410_2
            uint8_t RegAddr      : �Ĵ�����ַ�ֽ�
             uint16_t value      ������
 * ��    ��:0
 * �� �� ��:
 * ����ʱ��:2017-6-30
 * ��    ��:AD5410д�Ĵ���
 * �޸ļ�¼:2017-6-30����
*******************************************************************************/
static uint8_t AD5410xWriteReg( uint8_t RegAddr, uint16_t value)
{
  
#if USE_DAISY_CHAIN
  uint8_t SendData[1*E_AD5410_Max]={0};
 
  memset(SendData,0,sizeof(SendData));
  
  AD5410xEnableOperate(eAD5410);
  
  SendData[(E_AD5410_Max-1-)*3 + 0] = RegAddr;
  SendData[(E_AD5410_Max-1-)*3 + 1] = value>>8;
  SendData[(E_AD5410_Max-1-)*3 + 2] = value&0xff;
  
  SPIxWriteBytes(SendData, 1*E_AD5410_Max);
   
  AD5410xDisableOperate();
#else
  uint8_t SendData[3]={0x00, 0x00, 0x00};
 uint8_t DataRx[3];
  AD5410xEnableOperate();
   
  SendData[0] = RegAddr;
  SendData[1] = value>>8;
  SendData[2] = value&0xff;
  
  SPIxWriteBytes(SendData, DataRx,3);  
  
  AD5410xDisableOperate();
#endif
     
  return 0;
}
 
/*******************************************************************************
 * �� �� ��:static uint8_t AD5410xReadReg(E_AD5410 eAD5410,uint8_t RegAddr, uint16_t *value)
 * ��    ��:E_AD5410 eAD5410 : E_AD5410_1��E_AD5410_2
            uint8_t RegAddr  :�Ĵ�����ַ
            uint16_t *value  ������ָ��
 * ��    ��:0
 * �� �� ��:
 * ����ʱ��:2017-6-30
 * ��    ��:AD5410���Ĵ���������ֻ���ж�ȡ��������ȡ����ķ��Ͳ��ֶ��⴦��
 * �޸ļ�¼:2017-6-30����
*******************************************************************************/
 uint8_t AD5410xReadReg(uint8_t RegAddr, uint16_t *value)
{
	uint8_t SendData[3*E_AD5410_Max]={0};
  uint8_t RecvData[3*E_AD5410_Max]={0};
  uint8_t i = 0;
	
	/*������Ϊ�ض�ģʽ*/
  AD5410xEnableOperate();
  
	AD5410xWriteReg(ADDR_FUNC_READBACK,RegAddr);
	
	AD5410xDisableOperate();
	
	AD5410xDelayUs(10);
	
	AD5410xEnableOperate();
	
#if USE_DAISY_CHAIN
  
  memset(SendData, 0, sizeof(SendData));
  memset(RecvData, 0, sizeof(RecvData));
  
  for(i=0;i<E_AD5410_Max*3;i++)
  {
    SPIxWriteReadByte(stAD5410Ctrl[eAD5410].AD5410xSPIx, &SendData[i], &RecvData[i]);
  }
  
  {
    uint16_t Temp[2]={0,0};
    
    Temp[0] = RecvData[(E_AD5410_Max-1-eAD5410)*3 + 1];
    Temp[1] = RecvData[(E_AD5410_Max-1-eAD5410)*3 + 2];
    
    *value = (Temp[0]<<8) | Temp[1];
  }
  
#else
  /*���Ϳ�ָ���֣�ʹ�����ݴ�SDO�����*/
	SPIxWriteBytes( SendData, RecvData,3);
	//SPIxWriteReadByte( &SendData[1], &RecvData[1]);
	//SPIxWriteReadByte(&SendData[2], &RecvData[2]);  
  
  *value = (RecvData[1]<<8) | RecvData[2];
#endif
  
	AD5410xDisableOperate();
	
 
  
  return 0;
}
 
/*******************************************************************************
 * �� �� ��:static uint8_t AD5410xSoftReset(E_AD5410 eAD5410)
 * ��    ��:E_AD5410 eAD5410 : E_AD5410_1��E_AD5410_2
 * ��    ��:0
 * �� �� ��:
 * ����ʱ��:2017-6-30
 * ��    ��:AD5410�����λ         
 * �޸ļ�¼:2017-6-30����
*******************************************************************************/
static uint8_t AD5410xSoftReset( )
{
  uint8_t SendValidData[3]={0x56, 0x00, 0x00};
  uint8_t SendInvalidData[3]={0x00, 0x00, 0x00};
	uint8_t RecvData[3*E_AD5410_Max]={0};
	AD5410xEnableOperate();
	
#if USE_DAISY_CHAIN
  {
    uint8_t i = 0;
    for(i=E_AD5410_Max-1;i>=0;i--)
    {
      if(eAD5410 == ((E_AD5410)i))
      {
        SPIxWriteBytes(stAD5410Ctrl[eAD5410].AD5410xSPIx, SendValidData, 3);
      }
      else
      {
        /*���Ϳ�ָ���֣�������һ��5410������*/
		   SPIxWriteBytes(stAD5410Ctrl[eAD5410].AD5410xSPIx, SendInvalidData, 3);  
      }
    }
  }
#else
  SPIxWriteBytes(SendValidData, RecvData,3);
#endif
 
  
	AD5410xDisableOperate();
  
  return 0;
}
 
/*******************************************************************************
 * �� �� ��:uint8_t AD5410xInit(E_AD5410 eAD5410)
 * ��    ��:E_AD5410 eAD5410:���E_AD5410
 * ��    ��: 0  : �������� 
             -1 ��SPI��ʼ��ʧ��        -2 ��AD5410��λʧ��
 * �� �� ��:
 * ����ʱ��:2017-6-30
 * ��    ��:AD5410��ʼ��      
 * �޸ļ�¼:2017-6-30����
*******************************************************************************/
uint8_t AD5410xInit(unsigned char rangeC)
{
  uint8_t ret = 0;
  
 // if(eAD5410 < E_AD5410_Max)

//    GPIO_InitTypeDef GPIO_InitStructure;
//    
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;


    
    AD5410_LATCH_LOW();		
       
    AD5410xSoftReset();
	if(rangeC == 0)
    
    /******************����AD5410Ϊ0-20mA��Χ��ʹ�ܾջ���ģʽ��ʹ�������ѡ���ⲿ15K����************************************/
	  AD5410xWriteReg( ADDR_FUNC_CTRL_REG, CTRL_0_20_RANG | CTRL_DCEN_ENABLE | CTRL_OUTEN_ENABLE | CTRL_REXT_ENABLE);
    else
    /******************����AD5410Ϊ0-20mA��Χ��ʹ�ܾջ���ģʽ��ʹ�������ѡ���ⲿ15K����************************************/
	  AD5410xWriteReg( ADDR_FUNC_CTRL_REG, CTRL_4_20_RANG | CTRL_DCEN_ENABLE | CTRL_OUTEN_ENABLE | CTRL_REXT_ENABLE);
  		
 
  return ret;
}
 
/*******************************************************************************
 * �� �� ��:uint8_t AD5410xWriteDataReg(E_AD5410 eAD5410, uint16_t data)
 * ��    ��:E_AD5410 eAD5410 : E_AD5410_1��E_AD5410_2
             uint16_t data       ������
 * ��    ��: 0  : �������� 
 * �� �� ��:
 * ����ʱ��:2017-6-30
 * ��    ��:AD5410д���ݼĴ�������   
 * �޸ļ�¼:2017-6-30����
*******************************************************************************/
uint8_t AD5410xWriteDataReg(  uint16_t data)
{
  uint16_t SendData = 0x0000;
	

  SendData = (data<<4)&0xFFF0 ;  
	    
  AD5410xWriteReg( ADDR_FUNC_DATA_REG, SendData);
   
  return 0;
}
 
/*******************************************************************************
 * �� �� ��:uint8_t AD5410xRead(E_AD5410 eAD5410, E_AD5410_REG_ADDR RegAddr, uint16_t *data)
 * ��    ��:E_AD5410 eAD5410         : E_AD5410_1��E_AD5410_2
             E_AD5410_REG_ADDR RegAddr : �Ĵ�����ַ
             uint16_t *data              ������
 * ��    ��: 0  : �������� 
 * �� �� ��:
 * ����ʱ��:2017-6-30
 * ��    ��:AD5410�����ݼĴ�������   
 * �޸ļ�¼:2017-6-30����
*******************************************************************************/
uint8_t AD5410xReadDataReg( uint16_t *data)
{
  uint16_t RecvData = 0x0000;  
  
	//AD5410InitSPIModule(eAD5410);//SPI�����Ϲҽӵ��豸���Ͳ�һ����ÿ��ʹ��ǰ��������߽��г�ʼ��
	
  AD5410xReadReg(ADDR_FUNC_DATA_REG,&RecvData);
  
  RecvData = ((RecvData>>4)&0x0FFF);
  
  *data = RecvData;
    
  return 0;
}
 
/*******************************************************************************
 * �� �� ��:uint16_t AD5410xValueToCurrent(float value, float MaxValue)
 * ��    ��:float value     ���ϲ�Ҫ���������
            float MaxValue ���ϲ㴫������ֵ
 * ��    ��: 0  : �������� 
 * �� �� ��:
 * ����ʱ��:2017-6-30
 * ��    ��:���ݴ������ֵ�����������õ�����AD5410��Χ������
 * �޸ļ�¼:2017-6-30����
*******************************************************************************/
uint16_t AD5410xValueToCurrent(float value, float MaxValue)
{
  if(value >= MaxValue)
  {
    value = MaxValue;
  }
  else
  {}
    
  return (uint16_t)roundf((float)(value * AD5410_RATIO / MaxValue));
}
 
#if 1
void AD5410xTestSPI(void)
{
	uint8_t   SendData=0x55;
	uint16_t   RecvData[9]={0x00,0x00,0x00};
		

	
	AD5410xWriteDataReg(SendData);
	AD5410xWriteDataReg(SendData);
	
	AD5410xReadDataReg(RecvData);
	AD5410xReadDataReg(&RecvData[1]);
	
 
	return;
}

#endif
