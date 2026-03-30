#ifndef _MAX31856_H_
#define _MAX31856_H_ 
#include "stdint.h"

//定义参数
#define   Cold_Junction_Resolution    0.015625
#define   TC_Resolution               0.0078125   

//转换
#define  Stop_Conversion_Bit        (uint8_t) 0x3F
#define  One_Shot_Conversion        (uint8_t) 0x40
#define  Automatic_Conversion       (uint8_t) 0x80 
   
#define  OC_Fault_Disable_Bit       (uint8_t) 0xCF 
#define  OC_Fault_Enable_1          (uint8_t) 0x10
#define  OC_Fault_Enable_2          (uint8_t) 0x20
#define  OC_Fault_Enable_3          (uint8_t) 0x30
   
#define  CJ_Sensor_Enable_Bit       (uint8_t) 0xF7
#define  CJ_Sensor_Disable          (uint8_t) 0x08
   
#define  Comparator_Mode_Bit        (uint8_t) 0xFB
#define  Interrupt_Mode             (uint8_t) 0x04

#define  Fault_Clear                (uint8_t) 0x02

#define  NRF_60Hz_Bit               (uint8_t) 0xFE
#define  NRF_50Hz                   (uint8_t) 0x01
   
#define Average_1_Bit               (uint8_t) 0x0F
#define Average_2                   (uint8_t) 0x10
#define Average_4                   (uint8_t) 0x20
#define Average_8                   (uint8_t) 0x30
#define Average_16                  (uint8_t) 0x40  
   
#define TC_TypeB_Bit                (uint8_t) 0xF0
#define TC_TypeE                    (uint8_t) 0x01
#define TC_TypeJ                    (uint8_t) 0x02
#define TC_TypeK                    (uint8_t) 0x03
#define TC_TypeN                    (uint8_t) 0x04
#define TC_TypeR                    (uint8_t) 0x05
#define TC_TypeS                    (uint8_t) 0x06
#define TC_TypeT                    (uint8_t) 0x07
#define VM_Gain8                    (uint8_t) 0x80
#define VM_Gain16                   (uint8_t) 0xC0
   
#define CJ_High_Fault_Mask          (uint8_t)0x20
#define CJ_Low_Fault_Mask           (uint8_t)0x10
#define TC_High_Fault_Mask          (uint8_t)0x08
#define TC_Low_Fault_Mask           (uint8_t)0x04
#define OVUV_Fault_Mask             (uint8_t)0x02
#define Open_Fault_Mask             (uint8_t)0x01
#define ALL_Fault_Mask              (uint8_t)0x3F
#define No_Fault_Mask               (uint8_t)0x00
   
#define CJ_Range_Fault              (uint8_t)0x80
#define TC_Range_Fault              (uint8_t)0x40
#define CJHIGH_Fault                (uint8_t)0x20
#define CJLOW_Fault                 (uint8_t)0x10
#define TCHIGH_Fault                (uint8_t)0x08
#define TCLOW_Fault                 (uint8_t)0x04
#define OVUV_Fault                  (uint8_t)0x02
#define OPEN_Fault                  (uint8_t)0x01
#define NO_Fault                    (uint8_t)0x00   

typedef enum//定义测温芯片/片选序号
{
	cs1= 1,
	cs2=2,
	cs3=3,
	cs4=4
 }GPIO_cs;

typedef struct  
{										    
	uint8_t mode[2];			//配置1	模式配置
	float temp_tc;				//温度
	float temp_cold;			//冷端温度
	uint8_t mask;			//掩码
	uint8_t fault;			//错误掩码
}MAX31856_CS;

extern MAX31856_CS TEM1;
extern MAX31856_CS TEM2;
extern MAX31856_CS TEM3;
extern MAX31856_CS TEM4;

//温度读取后存储重要参数集
typedef struct  
{										    
	float temp1;			//读取模块1温度后存储
	float temp2;			//读取模块2温度后存储
	float temp3;			//读取模块3温度后存储
	float temp4;			//读取模块4温度后存储
}_temp_data; 
 
//void  Max31856_Init(void);
float   Get_Max31856(GPIO_cs cspin);//获取温度
//float Get_Max31856_12average(GPIO_cs cspin);//读取12次的温度后，温度按从小到大进行排序，出去最大值和最小值，其余数求和后算出平均值传送出去
void SPIReadRegister(uint16_t GPIO_Pin,uint8_t regAddr,uint8_t *rData,uint8_t rSize);
void SPIWriteRegister(uint16_t GPIO_Pin,uint8_t regAddr,uint8_t value);
void Max31856_SET_TYPE(GPIO_cs cspin,uint8_t type);
void Max31856_SET_MODE(GPIO_cs cspin,uint8_t mode);
void Max31856_READ_MODE(GPIO_cs cspin,uint8_t *rData);
uint8_t Max31856_READ_Fault(GPIO_cs cspin);
float Get_Max31856_TC(GPIO_cs cspin);
float Get_Max31856_Cold(GPIO_cs cspin);
#endif

