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
 
#include "BspAD5410.h"
 
#define AD5410xDelayUs(x) DelayUs(x) 
#define USE_DAISY_CHAIN 0

extern SPI_HandleTypeDef hspi3;

/*
 * ��������-AD5410�˿���Ϣ
 */
typedef struct AD5410CtrlStruc
{
  E_SPIx     AD5410xSPIx;
  S_GpioCtrl AD5410xLATCH;
}S_AD5410Ctrl;
 
/*
 * ���ڼ�¼AD5410�Ķ˿���Ϣ
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
 * ����5410��LATCH�˿�
 */
static __inline void AD5410_LATCH_HIGH(E_AD5410 eAD5410)
{
  HAL_GPIO_WritePin(LATCH_AO_MCU_GPIO_Port, LATCH_AO_MCU_Pin, GPIO_PIN_SET);
}
/*
 * ����5410��LATCH�˿�
 */
static __inline void AD5410_LATCH_LOW(E_AD5410 eAD5410)
{
  //GPIOxWriteBit(stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOx, stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOxPinx , 0);
  HAL_GPIO_WritePin(LATCH_AO_MCU_GPIO_Port, LATCH_AO_MCU_Pin, GPIO_PIN_RESET);
}

/*
 *SPI�����Ϲҽӵ��豸���Ͳ�һ����ÿ��ʹ��ǰ��������߽��г�ʼ��
 */
static void AD5410InitSPIModule(E_AD5410 eAD5410)
{

}
 
/*******************************************************************************
 * �� �� ��:static sint8 AD5410xEnableOperate(void)
 * ��    ��:
 * ��    ��:0
 * �� �� ��:
 * ����ʱ��:2017-6-30
 * ��    ��:��LATCH�ź��߲���һ���½���
 * �޸ļ�¼:2017-6-30����
*******************************************************************************/
static sint8 AD5410xEnableOperate(E_AD5410 eAD5410)
{
  
  //����LATCH
  AD5410_LATCH_HIGH(eAD5410);
 
  AD5410xDelayUs(10);
  
  //����LATCH
  AD5410_LATCH_LOW(eAD5410);
  
  return 0;
}
 
/*******************************************************************************
 * �� �� ��:static sint8 AD5410xDisableOperate(void)
 * ��    ��:void
 * ��    ��:0
 * �� �� ��:
 * ����ʱ��:2017-6-30
 * ��    ��:��LATCH�ź��߲���һ�������أ����ź��ߵ������ػ���������
 * �޸ļ�¼:2017-6-30����
*******************************************************************************/
static sint8 AD5410xDisableOperate(E_AD5410 eAD5410)
{
  
  //����LATCH
  AD5410_LATCH_LOW(eAD5410);
  
  AD5410xDelayUs(10);
  
  //����LATCH
  AD5410_LATCH_HIGH(eAD5410);
  
  return 0;
}
 
/*******************************************************************************
 * �� �� ��:static sint8 AD5410xWriteReg(E_AD5410 eAD5410, uint8 RegAddr, uint16 value)
 * ��    ��:E_AD5410 eAD5410 : E_AD5410_1��E_AD5410_2
            uint8 RegAddr      : �Ĵ�����ַ�ֽ�
             uint16 value      ������
 * ��    ��:0
 * �� �� ��:
 * ����ʱ��:2017-6-30
 * ��    ��:AD5410д�Ĵ���
 * �޸ļ�¼:2017-6-30����
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
 * �� �� ��:static sint8 AD5410xReadReg(E_AD5410 eAD5410,uint8 RegAddr, uint16 *value)
 * ��    ��:E_AD5410 eAD5410 : E_AD5410_1��E_AD5410_2
            uint8 RegAddr  :�Ĵ�����ַ
            uint16 *value  ������ָ��
 * ��    ��:0
 * �� �� ��:
 * ����ʱ��:2017-6-30
 * ��    ��:AD5410���Ĵ���������ֻ���ж�ȡ��������ȡ����ķ��Ͳ��ֶ��⴦��
 * �޸ļ�¼:2017-6-30����
*******************************************************************************/
static sint8 AD5410xReadReg(E_AD5410 eAD5410,uint8 RegAddr, uint16 *value)
{
	uint8 SendData[3*E_AD5410_Max]={0};
  uint8 RecvData[3*E_AD5410_Max]={0};
  uint8 i = 0;
	
	/*������Ϊ�ض�ģʽ*/
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
  /*���Ϳ�ָ���֣�ʹ�����ݴ�SDO�����*/
	SPIxWriteReadByte(stAD5410Ctrl[eAD5410].AD5410xSPIx, &SendData[0], &RecvData[0]);
	SPIxWriteReadByte(stAD5410Ctrl[eAD5410].AD5410xSPIx, &SendData[1], &RecvData[1]);
	SPIxWriteReadByte(stAD5410Ctrl[eAD5410].AD5410xSPIx, &SendData[2], &RecvData[2]);  
  
  *value = (RecvData[1]<<8) | RecvData[2];
#endif
  
	AD5410xDisableOperate(eAD5410);
	
 
  
  return 0;
}
 
/*******************************************************************************
 * �� �� ��:static sint8 AD5410xSoftReset(E_AD5410 eAD5410)
 * ��    ��:E_AD5410 eAD5410 : E_AD5410_1��E_AD5410_2
 * ��    ��:0
 * �� �� ��:
 * ����ʱ��:2017-6-30
 * ��    ��:AD5410�����λ         
 * �޸ļ�¼:2017-6-30����
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
        /*���Ϳ�ָ���֣�������һ��5410������*/
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
 * �� �� ��:sint8 AD5410xInit(E_AD5410 eAD5410)
 * ��    ��:E_AD5410 eAD5410:���E_AD5410
 * ��    ��: 0  : �������� 
             -1 ��SPI��ʼ��ʧ��        -2 ��AD5410��λʧ��
 * �� �� ��:
 * ����ʱ��:2017-6-30
 * ��    ��:AD5410��ʼ��      
 * �޸ļ�¼:2017-6-30����
*******************************************************************************/
sint8 AD5410xInit(E_AD5410 eAD5410)
{
  sint8 ret = 0;
  
  if(eAD5410 < E_AD5410_Max)
  {

#if USE_DAISY_CHAIN
    /******** �ջ���ģʽ������һ��LATCH���� ********/
    
    /* ����LATCH���� */
    RCC_AHB1PeriphClockCmd(stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOxSource, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOxPinx;
    GPIO_Init(stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOx, &GPIO_InitStructure);  
    
    AD5410_LATCH_LOW(eAD5410);	
    
    AD5410InitSPIModule(eAD5410);//SPI�����Ϲҽӵ��豸���Ͳ�һ����ÿ��ʹ��ǰ��������߽��г�ʼ��
    
    for(eAD5410=E_AD5410_1;eAD5410<E_AD5410_Max;eAD5410++)
    {
      AD5410xSoftReset(eAD5410);
    }
    
    for(eAD5410=E_AD5410_1;eAD5410<E_AD5410_Max;eAD5410++)
    {  
      /******************����AD5410Ϊ0-20mA��Χ��ʹ�ܾջ���ģʽ��ʹ�������ѡ���ⲿ15K����************************************/
	    AD5410xWriteReg(eAD5410, ADDR_FUNC_CTRL_REG, CTRL_0_20_RANG | CTRL_DCEN_ENABLE | CTRL_OUTEN_ENABLE | CTRL_REXT_ENABLE);  
    }
#else    
    /*����LATCH����*/
    RCC_AHB1PeriphClockCmd(stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOxSource, ENABLE);
    
    /*����LATCH����*/
    GPIO_InitStructure.GPIO_Pin = stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOxPinx;
    GPIO_Init(stAD5410Ctrl[eAD5410].AD5410xLATCH.GPIOx, &GPIO_InitStructure);  
    
    AD5410_LATCH_LOW(eAD5410);		
    
	  AD5410InitSPIModule(eAD5410);//SPI�����Ϲҽӵ��豸���Ͳ�һ����ÿ��ʹ��ǰ��������߽��г�ʼ��
    
    AD5410xSoftReset(eAD5410);
    
    /******************����AD5410Ϊ0-20mA��Χ��ʹ�ܾջ���ģʽ��ʹ�������ѡ���ⲿ15K����************************************/
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
 * �� �� ��:sint8 AD5410xWriteDataReg(E_AD5410 eAD5410, uint16 data)
 * ��    ��:E_AD5410 eAD5410 : E_AD5410_1��E_AD5410_2
             uint16 data       ������
 * ��    ��: 0  : �������� 
 * �� �� ��:
 * ����ʱ��:2017-6-30
 * ��    ��:AD5410д���ݼĴ�������   
 * �޸ļ�¼:2017-6-30����
*******************************************************************************/
sint8 AD5410xWriteDataReg(E_AD5410 eAD5410, uint16 data)
{
  uint16 SendData = 0x0000;
	
	AD5410InitSPIModule(eAD5410);//SPI�����Ϲҽӵ��豸���Ͳ�һ����ÿ��ʹ��ǰ��������߽��г�ʼ��
	
  SendData = (data<<4)&0xFFF0 ;  
	    
  AD5410xWriteReg(eAD5410, ADDR_FUNC_DATA_REG, SendData);
   
  return 0;
}
 
/*******************************************************************************
 * �� �� ��:sint8 AD5410xRead(E_AD5410 eAD5410, E_AD5410_REG_ADDR RegAddr, uint16 *data)
 * ��    ��:E_AD5410 eAD5410         : E_AD5410_1��E_AD5410_2
             E_AD5410_REG_ADDR RegAddr : �Ĵ�����ַ
             uint16 *data              ������
 * ��    ��: 0  : �������� 
 * �� �� ��:
 * ����ʱ��:2017-6-30
 * ��    ��:AD5410�����ݼĴ�������   
 * �޸ļ�¼:2017-6-30����
*******************************************************************************/
sint8 AD5410xReadDataReg(E_AD5410 eAD5410, uint16 *data)
{
  uint16 RecvData = 0x0000;  
  
	AD5410InitSPIModule(eAD5410);//SPI�����Ϲҽӵ��豸���Ͳ�һ����ÿ��ʹ��ǰ��������߽��г�ʼ��
	
  AD5410xReadReg(eAD5410,ADDR_FUNC_DATA_REG,&RecvData);
  
  RecvData = ((RecvData>>4)&0x0FFF);
  
  *data = RecvData;
    
  return 0;
}
 
/*******************************************************************************
 * �� �� ��:uint16 AD5410xValueToCurrent(float value, float MaxValue)
 * ��    ��:float value     ���ϲ�Ҫ���������
            float MaxValue ���ϲ㴫������ֵ
 * ��    ��: 0  : �������� 
 * �� �� ��:
 * ����ʱ��:2017-6-30
 * ��    ��:���ݴ������ֵ�����������õ�����AD5410��Χ������
 * �޸ļ�¼:2017-6-30����
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


