/****************************************************************************
本程序适用于飞拓电子网提供的mag_3d_3100评估板测试，更新升级请关注淘宝店 flyarm.taobao.com
******************************************************************************/

#include "main.h"

#include "mag3d3100.h"


extern SPI_HandleTypeDef hspi1;

#define CS_L			HAL_GPIO_WritePin(RM_CS_GPIO_Port, RM_CS_Pin, GPIO_PIN_RESET)
#define CS_H			HAL_GPIO_WritePin(RM_CS_GPIO_Port, RM_CS_Pin, GPIO_PIN_SET)

extern uint8_t Tx_Update_Enable;


void SPI2_Init(void)
{
//	GPIO_InitTypeDef  GPIO_InitStructure; 
//	SPI_InitTypeDef   SPI_InitStructure; 
//  	SPI_Cmd(SPI2,DISABLE); 
// 	//SPI2 Periph clock enable 
//  	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );

//  	//Configure SPI2 pins: SCK, MISO and MOSI 
//  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_13; 
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     
//  	GPIO_Init(GPIOB,&GPIO_InitStructure);  

//  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       
//  	GPIO_Init(GPIOB,&GPIO_InitStructure); 
//	
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 		
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;       
//  	GPIO_Init(GPIOB,&GPIO_InitStructure); 

//  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //推挽输出
//  	GPIO_Init(GPIOB,&GPIO_InitStructure); 
// 
//  /* SPI2 configuration */ 
// SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //SPI1设置为两线全双工
// SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                    //设置SPI2为主模式
// SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                  //SPI发送接收8位帧结构
// SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                   //串行时钟在不操作时，时钟为低电平
// SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                 //第一个时钟沿开始采样数据
// SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                  //NSS信号由软件（使用SSI位）管理
// SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; //定义波特率预分频的值:波特率预分频值为8
// SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;       //数据传输从MSB位开始
// SPI_InitStructure.SPI_CRCPolynomial = 7;         //CRC值计算的多项式
// SPI_Init(SPI2, &SPI_InitStructure);
// /* Enable SPI2  */
// SPI_Cmd(SPI2, ENABLE);  
} 

/** 定义结构 */
struct config {
    unsigned int cycle_count;
    float sample_rate;
    float max_data_rate;
    float gain;
};
/* Default Values c99*/
struct config rm = {
    .cycle_count   = 200,
    .sample_rate   = 37,
    .max_data_rate = 440,
    .gain          = 75
};

u8 SPI_WriteByte(SPI_TypeDef* SPIx,uint8_t data) 
{ 
  u8 Data = 0;
 //HAL_SPI_Transmit(&hspi1, &data, 1,50);	
HAL_SPI_TransmitReceive(&hspi1, &data, &Data, 1,5);

 /*HAL_SPI_Transmit(&hspi1, &data, 1, 10);
  //Wait until the transmit buffer is empty 
  while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_TXE)==RESET); 
  // Send the byte  
  SPI_I2S_SendData(SPIx,data); 

  //Wait until a data is received 
  while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE)==RESET); 
  // Get the received data 
  Data = SPI_I2S_ReceiveData(SPIx); 

  // Return the shifted data */
  return Data; 

}  
//多字节写入寄存器
u8 SPI_Write_M3D3100(SPI_TypeDef* SPIx,u8 reg_addr,u8 *datain,u8 lenth)
{
	u8 bytecount=0;
	u8 temp=0;
 
	CS_L ;
	HAL_Delay(10);	
	temp=0x00+(reg_addr&0x3f); //MSB=0  read reg
	SPI_WriteByte(SPIx,temp);

	//send lest bytes
	for(bytecount=0;bytecount<lenth;bytecount++)
	{
		temp=*(datain+bytecount);
		SPI_WriteByte(SPIx,temp);
	}
	CS_H;HAL_Delay(10);
	return 0;
}

u8 SPI_Read_M3D3100(SPI_TypeDef* SPIx,u8 reg_addr,u8 *dataout,u8 lenth)
{
	u8 bytecount=0;
	u8 temp=0;	
	
	CS_L;
	HAL_Delay(10);
	temp=0x80+(reg_addr&0x3f); //MSB=1 rw=1-->addr auto increase
	SPI_WriteByte(SPIx,temp);

	for(bytecount=0;bytecount<lenth;bytecount++)
	{
		temp=SPI_WriteByte(SPIx,0);
		*(dataout+bytecount) = temp;
	}
	CS_H ;HAL_Delay(10);
	return 0;	
}
/**
 *  @brief  Sets cycle count and updates gain and max_data_rate values
 *  @param[in]  desire value PNI recomends values between 30 and 400 Hz
 *  @return     1 if successful, 0 otherwise.
 */
BOOL setCycleCount ( u16 value ) {

    char to_reg[6];

    if (value > 65535 || value<0)
        return FALSE;
    else{

        if (value > 400)
            value=400;
        else if (value < 30)
            value=30;

        to_reg[0] = value>>8;
        to_reg[1] = value;
        to_reg[2] = to_reg[0];
        to_reg[3] = to_reg[1];
        to_reg[4] = to_reg[0];
        to_reg[5] = to_reg[1];

        if (SPI_Write_M3D3100(SPI2, M3D_3100_CCX, to_reg,6))
            return FALSE;

    }
    return TRUE;
}


/**
 *  @brief  Sets data rate in Continuous Measurement Mode.
 *  Fails if desire datarate is higher than the max data rate recommended by PNI.
 *  @param[in]  Data rate configuration BYTE
 *  @return     1 if successful, 0 otherwise.
 */
BOOL setCMMdatarate ( char conf ) {

    if (conf < CMM_UPDATERATE_600 || conf > CMM_UPDATERATE_0_075)
        return FALSE;
    else{
        char *ptr;
        float temp = 1000;

        switch (conf){
            case CMM_UPDATERATE_600: temp=600;
                                    break;
            case CMM_UPDATERATE_300: temp=300;
                                    break;
            case CMM_UPDATERATE_150: temp=150;
                                    break;
            case CMM_UPDATERATE_75 : temp=75;
                                    break;
            case CMM_UPDATERATE_37 : temp=37;
                                    break;
            case CMM_UPDATERATE_18 : temp=18;
                                    break;
            case CMM_UPDATERATE_9  : temp=9;
                                    break;
            case CMM_UPDATERATE_4_5: temp=4.5;
                                    break;
            case CMM_UPDATERATE_2_3: temp=2.3;
                                    break;
            case CMM_UPDATERATE_1_2: temp=1.2;
                                    break;
            case CMM_UPDATERATE_0_6: temp=0.6;
                                    break;
            case CMM_UPDATERATE_0_3: temp=0.3;
                                    break;
            case CMM_UPDATERATE_0_15: temp=0.15;
                                    break;
            case CMM_UPDATERATE_0_075: temp=0.075;
                                    break;
        }
        if (temp > rm.max_data_rate)				
            return FALSE;

        ptr = &conf ;
        if (SPI_Write_M3D3100(SPI2, M3D_3100_TMRC, ptr,1))
            return FALSE;
        else{
            rm.sample_rate = temp;
        }
    }
    return TRUE;
}


void setCMMdtr(char dtr)
{
	char temp;
   switch (dtr){
		case 14:
			temp=CMM_UPDATERATE_600;
         break;
		case 13:
			temp=CMM_UPDATERATE_300;
         break;
		case 12:
			temp=CMM_UPDATERATE_150;
         break;
		case 11:
			temp=CMM_UPDATERATE_75;
         break;
		case 10:
			temp=CMM_UPDATERATE_37;
         break;
		case 9:
			temp=CMM_UPDATERATE_18;
         break;
		case 8:
			temp=CMM_UPDATERATE_9;
         break;
		case 7:
			temp=CMM_UPDATERATE_4_5;
         break;		
		case 6:
			temp=CMM_UPDATERATE_2_3;
         break;
		case 5:
			temp=CMM_UPDATERATE_1_2;
         break;
		case 4:
			temp=CMM_UPDATERATE_0_6;
         break;
		case 3:
			temp=CMM_UPDATERATE_0_3;
         break;
		case 2:
			temp=CMM_UPDATERATE_0_15;
         break;
		case 1:
			temp=CMM_UPDATERATE_0_075;
         break;
      }

	setCMMdatarate ( temp );
}
/**
 *  @brief  Continuous Measurement Mode (CMM) Register Configuration.
 *  @param[in]  CMM configuration BYTE：
 *  @return     1 if successful, 0 otherwise.
 */
BOOL continuousModeConfig ( char conf ) {

    char *ptr;

    ptr = &conf ;
    if (SPI_Write_M3D3100(SPI2, M3D_3100_CMM, ptr,1))
        return FALSE;

    return TRUE;
}

/**
 *  @brief      Request Single Measurement to PNI ASIC.
 *  @param[in]  none：0*** 0000  ：
 *  @return     1 if successful, 0 otherwise.
 */
BOOL requestSingleMeasurement ( void ) {

    char to_reg = SM_ALL_AXIS;
    char *ptr;

    ptr = &to_reg;
    if (SPI_Write_M3D3100(SPI2, M3D_3100_POLL, ptr,1))
        return FALSE;

    return TRUE;
}

/**
 *  @brief      Get data ready Status
 *  @param[in]  none
 *  @return     1 if data ready, 0 otherwise.
 */
BOOL getDataReadyStatus ( void ) {

    char data[1];

    SPI_Read_M3D3100(SPI2, M3D_3100_STATUS,  data,1);

    return (data[0] & STATUS_MASK);
}

/**
 *  @brief      Self test to Ev Board.
 *  @param[in]  none  缺省值0x1b
 *  @return     1 if all axis OK, 0 otherwise.
 */
BOOL getRM3100Status ( void ) {

    char data[1];
    char to_reg = STE_ON | BW_11 | BP_11;
    char *ptr;

    ptr = &to_reg;
    if (SPI_Write_M3D3100(SPI2, M3D_3100_BIST, ptr,1))
        return FALSE;

    if (requestSingleMeasurement ()==FALSE)
        return FALSE;

    while(!getDataReadyStatus());
    
    if (SPI_Read_M3D3100(SPI2, M3D_3100_BIST,  data,1))
        return FALSE;

    if (data[0] & STE_ON){
        to_reg = STE_OFF;
        ptr = &to_reg;
        if (SPI_Write_M3D3100(SPI2, M3D_3100_BIST, ptr,1))
            return FALSE;

        return (data[0] & BIST_MASK);
    }
    return FALSE;
}

/**
 *  @brief      Request RM3100 Ev. Board Revision
 *  @param[in]  none
 *  @return     Revision value.
 */
char getRM3100revision ( void ) {

   char data[1];

	SPI_Read_M3D3100(SPI2, M3D_3100_REVID,  data,1);
   return data[0];
}


void ThreeD3100_magic_init(void)
{
u8  i=0;
u8 cycle_count_read_back[6]={0};

SPI2_Init();
HAL_Delay(100);

continuousModeConfig(CMM_ALL_AXIS_ON|DRDY_WHEN_ANY_AXIS_MEASURED|CM_START); 			//设置测试轴,drdy 开启连续模式
setCycleCount(200);												//设置CCR寄存器200
setCMMdatarate(12);
HAL_Delay(100);	

SPI_Read_M3D3100(SPI2,M3D_3100_CCX,cycle_count_read_back,6);								//for verification purpose only.   回读 验证是否写入
}

/**************************实现函数********************************************
*函数原型:	  void MagL_Start_Calib(void)
*功　　能:	   进入磁力计标定
输入参数：     	
输出参数：  无
*******************************************************************************/

void ThreeD3100_magic_GetData(MagData_t* buff)
{
 u8 i;
 u8 temp[9]={0};

 s32 Mag_Data[3]={0};
 char err[5]={0xaa,0x00,0xa1,0xa2,0xa3};
// Spi2_Init(0);
// Delay(1000);

while(M3D_DRDY==0);

 CS_H ;
 SPI_Read_M3D3100(SPI2,0x24,temp,9);
 Mag_Data[0]=temp[0]<<16 | temp[1]<<8 | temp[2];			//ned坐标系--前提是所有焊接都是标记位焊接到N
 Mag_Data[1]=temp[3]<<16 | temp[4]<<8 | temp[5];
 Mag_Data[2]=temp[6]<<16 | temp[7]<<8 | temp[8];

 for(i=0;i<3;i++)
 {
 if(Mag_Data[i]&0x00800000)
	Mag_Data[i]|=0xff000000;
 }
 
 buff->MAG_X=Mag_Data[0]/75;
 buff->MAG_Y=Mag_Data[1]/7;
 buff->MAG_Z=Mag_Data[2]/75; 
}






/****************************************************************************
本程序适用于飞拓电子网提供的mag_3d_3100评估板测试，更新升级请关注淘宝店 flyarm.taobao.com
******************************************************************************/



