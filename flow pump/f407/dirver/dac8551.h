#ifndef __DAC8552_H_
#define __DAC8552_H_
#include "main.h"
extern uint16_t Zero_Voltage_OutPut;
extern uint16_t LOAD_Channel;

#define SUB 85
//------------------------------------------------------------------------------
//IO口宏定义：
//------------------------------------------------------------------------------


#define SYNC_1  HAL_GPIO_WritePin(LATCH_AO_GPIO_Port, LATCH_AO_Pin, GPIO_PIN_SET)
#define SYNC_0  HAL_GPIO_WritePin(LATCH_AO_GPIO_Port, LATCH_AO_Pin, GPIO_PIN_RESET)
//------------------------------------------------------------------------------
//写寄存器宏定义：
//参考（低、中、高）；通道（A、B）；输出阻抗(低、高)
//------------------------------------------------------------------------------
#define No_Impedance       0x00
#define Low_Impedance      0x01  //低四位可以是0000、    0001、  0010、    0011
#define Middle_Impedance   0x02  //分别代表    普通模式  1K阻抗 100K阻抗  高阻抗
#define High_Impedance     0x03  //阻抗为输出阻抗


#define DAC_4To20MA   0
#define DAC_0To20MA   1
typedef struct
{
	uint32_t dacDat;
	float dacVoltage;
	float vref;
	uint8_t mode;//0:4-20ma   1:0-20ma
	
}dac_stru;

//------------------------------------------------------------------------------
//函数定义：
//------------------------------------------------------------------------------
void dacInit(void);

void DAC8552_Out_Put_Voltage(float volf);
#endif


