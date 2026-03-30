#include "stdio.h"
#include "main.h"
#include "spi.h"
#include "max31856.h"
 
MAX31856_CS TEM1;
MAX31856_CS TEM2;
MAX31856_CS TEM3;
MAX31856_CS TEM4;

/*读寄存器操作*/
void SPIReadRegister(uint16_t GPIO_Pin,uint8_t regAddr,uint8_t *rData,uint8_t rSize)//
{
    uint8_t wData=regAddr;

    if(rSize<1)
    {
        return;
    }
		HAL_GPIO_WritePin(GPIOB,GPIO_Pin,GPIO_PIN_RESET);//拉低片选信号
		HAL_SPI_Transmit(&hspi1, &wData,1, 0xff);//
		HAL_SPI_Receive(&hspi1,rData,rSize,0xff);
		HAL_GPIO_WritePin(GPIOB,GPIO_Pin,GPIO_PIN_SET);//拉高片选信号
}

/*写寄存器操作*/
void SPIWriteRegister(uint16_t GPIO_Pin,uint8_t regAddr,uint8_t value)
{
    uint8_t wData[2];
    
    if(regAddr>0x8B || regAddr<0x80)
    {
        return;
    }  
		wData[0]=regAddr;
		wData[1]=value;
		HAL_GPIO_WritePin(GPIOB,GPIO_Pin,GPIO_PIN_RESET);//拉低片选信号
		HAL_SPI_Transmit(&hspi1,wData,2, 0xff);
		HAL_GPIO_WritePin(GPIOB,GPIO_Pin,GPIO_PIN_SET);//拉高片选信号
}

/*写寄存器操作*/
void SPIWriteRegister_4(uint16_t GPIO_Pin,uint8_t regAddr,uint8_t value)
{
    uint8_t wData[2];
    
    if(regAddr>0x8B || regAddr<0x80)
    {
        return;
    }  
		wData[0]=regAddr;
		wData[1]=value;
		HAL_GPIO_WritePin(GPIOE,GPIO_Pin,GPIO_PIN_RESET);//拉低片选信号
		HAL_SPI_Transmit(&hspi1,wData,2, 0xff);//
		HAL_GPIO_WritePin(GPIOE,GPIO_Pin,GPIO_PIN_SET);//拉高片选信号
}

/*读寄存器操作*/
void SPIReadRegister_4(uint16_t GPIO_Pin,uint8_t regAddr,uint8_t *rData,uint8_t rSize)//
{
    uint8_t wData=regAddr;

    if(rSize<1)
    {
        return;
    }
		HAL_GPIO_WritePin(GPIOE,GPIO_Pin,GPIO_PIN_RESET);//拉低片选信号
		HAL_SPI_Transmit(&hspi1, &wData,1, 0xff);//
		HAL_SPI_Receive(&hspi1,rData,rSize,0xff);
		HAL_GPIO_WritePin(GPIOE,GPIO_Pin,GPIO_PIN_SET);//拉高片选信号
}

void Max31856_SET_TYPE(GPIO_cs cspin,uint8_t type)
{  
	switch(cspin)//选择使能哪一个芯片：0使能，1失能
	{
	  case cs1: SPIWriteRegister(SPI1_CS1_Pin,0x81,type);break;//片选CS1设置
	  case cs2: SPIWriteRegister(SPI1_CS2_Pin,0x81,type);break;//片选CS2设置
	  case cs3: SPIWriteRegister(SPI1_CS3_Pin,0x81,type);break;//片选CS3设置
	  case cs4: SPIWriteRegister_4(SPI1_CS4_Pin,0x81,type);break;//片选CS4设置
	}	
}

void Max31856_SET_MODE(GPIO_cs cspin,uint8_t mode)
{  
	switch(cspin)//选择使能哪一个芯片：0使能，1失能
	{
	  case cs1: SPIWriteRegister(SPI1_CS1_Pin,0x80,mode);break;//片选CS1设置
	  case cs2: SPIWriteRegister(SPI1_CS2_Pin,0x80,mode);break;//片选CS2设置
	  case cs3: SPIWriteRegister(SPI1_CS3_Pin,0x80,mode);break;//片选CS3设置
	  case cs4: SPIWriteRegister_4(SPI1_CS4_Pin,0x80,mode);break;//片选CS4设置
	}	
}

float Get_Max31856_TC(GPIO_cs cspin) //GPIO_cs枚举名称
{
	uint8_t rData[3];
	__IO  int32_t raw_temp = 0;
	float temp = 0;
	
	switch(cspin)//选择使能哪一个芯片：0使能，1失能
	{
		case cs1: 
			SPIReadRegister(SPI1_CS1_Pin,0x0C,rData,3);//片选CS1设置
			break;
		case cs2: 
			SPIReadRegister(SPI1_CS2_Pin,0x0C,rData,3);//片选CS1设置
			break;//片选CS2设置
		case cs3: 
			SPIReadRegister(SPI1_CS3_Pin,0x0C,rData,3);//片选CS1设置
			break;//片选CS3设置
		case cs4: 
			SPIReadRegister(SPI1_CS4_Pin,0x0C,rData,3);//片选CS1设置
			break;//片选CS4设置
	}	
	if((rData[0] & 0x80)==0x80)
		raw_temp = (rData[0]<<16) | (rData[1]<<8) | rData[2] | (0xFF<<24);
	else
		raw_temp = (uint32_t)(rData[0]<<16) + (uint32_t)(rData[1]<<8) + (uint32_t)rData[2];
	raw_temp >>= 5;
	temp = (float)raw_temp/128;

	return (temp);
} 

//读冷端温度
float Get_Max31856_Cold(GPIO_cs cspin) //GPIO_cs枚举名称
{
	uint8_t rData[2] = {0};
	__IO int16_t raw_temp1 = 0;
	__IO float temp1 = 0;
	
	switch(cspin)//选择使能哪一个芯片：0使能，1失能
	{
		case cs1: 
			SPIReadRegister(SPI1_CS1_Pin,0x0A,rData,2);//片选CS1设置
			break;
		case cs2: 
			SPIReadRegister(SPI1_CS2_Pin,0x0A,rData,2);//片选CS1设置
			break;//片选CS2设置
		case cs3: 
			SPIReadRegister(SPI1_CS3_Pin,0x0A,rData,2);//片选CS1设置
			break;//片选CS3设置
		case cs4: 
			SPIReadRegister_4(SPI1_CS4_Pin,0x0A,rData,2);//片选CS1设置
			break;//片选CS4设置
	}	
	raw_temp1 = ((rData[0]<<8) | rData[1])>>2;
	if((rData[0] & 0x80)==0x80)
	{
		raw_temp1=0x3FFF-raw_temp1+1;
		temp1=0-(float)raw_temp1/64;
	}
	else
	{
		temp1=(float)raw_temp1/64;
	}
	
	return (temp1);
} 

//读错误信息
uint8_t Max31856_READ_Fault(GPIO_cs cspin)
{  
	uint8_t fault;
	switch(cspin)//选择使能哪一个芯片：0使能，1失能
	{
	  case cs1: SPIReadRegister(SPI1_CS1_Pin,0x0F,&fault,1);break;//片选CS1设置
	  case cs2: SPIReadRegister(SPI1_CS2_Pin,0x0F,&fault,1);break;//片选CS2设置
	  case cs3: SPIReadRegister(SPI1_CS3_Pin,0x0F,&fault,1);break;//片选CS3设置
	  case cs4: SPIReadRegister_4(SPI1_CS4_Pin,0x0F,&fault,1);break;//片选CS4设置
	}	
	return fault;
}

//读配置
void Max31856_READ_MODE(GPIO_cs cspin,uint8_t *rData)
{  
	
	switch(cspin)//选择使能哪一个芯片：0使能，1失能
	{
	  case cs1: SPIReadRegister(SPI1_CS1_Pin,0x00,rData,2);break;//片选CS1设置
	  case cs2: SPIReadRegister(SPI1_CS2_Pin,0x00,rData,2);break;//片选CS2设置
	  case cs3: SPIReadRegister(SPI1_CS3_Pin,0x00,rData,2);break;//片选CS3设置
	  case cs4: SPIReadRegister_4(SPI1_CS4_Pin,0x00,rData,2);break;//片选CS4设置
	}	
}

//float Get_Max31856_12average(GPIO_cs cspin) //GPIO_cs枚举名称,读取12次数据，去除最小值和最大值然后其余数求和计算出平均值传送出去
//{ 
//	static float temp12[12]={0};
//	int i,k, j, isSorted;
//  static float temp;
//	float num1=0;
//	for(u8 i=0;i<12;i++)      //读取12个温度
//	   {
//	    temp12[i]=Get_Max31855(cspin);
//			delay_ms(5); 
//		 }
//	   
//    //优化算法：最多进行 n-1 轮比较
//	for(k=0; k<12-1; k++)//排序算法，对素组进行从小到大依次排序
//			{
//						isSorted = 1;  //假设剩下的元素已经排序好了
//						for(j=0; j<12-1-k; j++)
//								{
//									if(temp12[j] > temp12[j+1])
//										{
//											temp = temp12[j];
//											temp12[j] =temp12[j+1];
//											temp12[j+1] = temp;
//											isSorted = 0;  //一旦需要交换数组元素，就说明剩下的元素没有排序好
//										 }
//								}
//						if(isSorted) break; //如果没有发生交换，说明剩下的元素已经排序好了
//			}
//	
//    for(i=1;i<12-1;i++)//除去最大数和最小数，对其余数组进行求和
//       {
//	     num1+=temp12[i];
//	     }
//      //num1=(float)num1/10;	
//			 return (float)num1/10;	//计算出10组平均值传送出去		 
//}
