/*******************************************************************

*******************************************************************/
#ifndef _ADS1118_H
#define _ADS1118_H
#include "stdint.h"
#include "main.h"
#define ADS1118_CS_PIN    HAL_GPIO_WritePin(EXTPROC_SPI2_ADC_CS3_GPIO_Port, EXTPROC_SPI2_ADC_CS3_Pin, GPIO_PIN_RESET)
#define ADS1118_CS_RELEASE HAL_GPIO_WritePin(EXTPROC_SPI2_ADC_CS3_GPIO_Port,EXTPROC_SPI2_ADC_CS3_Pin, GPIO_PIN_SET)

// 寄存器地址
#define ADS_CONFIG_REG    0x01
#define ADS_CONVERSION_REG 0x00


/**单次转换启动**/

#define ADS1118_SS_NONE  0 // 无效

#define ADS1118_SS_ONCE  1 // 启动单次转换



/**输入多路复用器配置**/

#define ADS1118_MUX_01 0 // 000 = AINP 为 AIN0 且 AINN 为 AIN1（默认）

#define ADS1118_MUX_03 1 // 001 = AINP 为 AIN0 且 AINN 为 AIN3

#define ADS1118_MUX_13 2 // 010 = AINP 为 AIN1 且 AINN 为 AIN3

#define ADS1118_MUX_23 3 // 011 = AINP 为 AIN2 且 AINN 为 AIN3

#define ADS1118_MUX_0G 4 // 100 = AINP 为 AIN0 且 AINN 为 GND

#define ADS1118_MUX_1G 5 // 101 = AINP 为 AIN1 且 AINN 为 GND

#define ADS1118_MUX_2G 6 // 110 = AINP 为 AIN2 且 AINN 为 GND

#define ADS1118_MUX_3G 7 // 111 = AINP 为 AIN3 且 AINN 为 GND



/**可编程增益放大器配置**/

#define ADS1118_PGA_61  0 // 000 = FSR 为 ±6.144V

#define ADS1118_PGA_40  1 // 001 = FSR 为 ±4.096V

#define ADS1118_PGA_20  2 // 010 = FSR 为 ±2.048V（默认）

#define ADS1118_PGA_10  3 // 011 = FSR 为 ±1.024V

#define ADS1118_PGA_05  4 // 100 = FSR 为 ±0.512V

#define ADS1118_PGA_02  5 // 101 = FSR 为 ±0.256V



/**器件工作模式配置**/

#define ADS1118_MODE_LX  0 // 连续转换模式

#define ADS1118_MODE_DC  1 // 断电并采用单次转换模式（默认）



/**数据传输速率**/

#define ADS1118_DR_8      0 // 000 = 8SPS

#define ADS1118_DR_16     1 // 001 = 16SPS

#define ADS1118_DR_32     2 // 010 = 32SPS

#define ADS1118_DR_64     3 // 011 = 64SPS

#define ADS1118_DR_128    4 // 100 = 128SPS（默认）

#define ADS1118_DR_250    5 // 101 = 250SPS

#define ADS1118_DR_475    6 // 110 = 475SPS

#define ADS1118_DR_860    7 // 111 = 860SPS



/**温度传感器模式**/

#define ADS1118_TS_MODE_ADC 0 // 0 = ADC 模式（默认）

#define ADS1118_TS_MODE_T 1 // 1 = 温度传感器模式



/**上拉使能**/

#define ADS1118_PULL_UP_EN_N 0 // 禁用 DOUT/DRDY 引脚的上拉电阻

#define ADS1118_PULL_UP_EN_E 1 // 使能 DOUT/DRDY 引脚的上拉电阻（默认）



/**控制数据是否写入配置寄存器**/

#define ADS1118_NOP_N 0 // 00 = 无效数据， 不更新配置寄存器内容

#define ADS1118_NOP_W 1 // 01 = 有效数据， 更新配置寄存器（默认）



/**保留**/

#define ADS1118_CNV_RDY_FL    1 // 始终写入 1h

typedef struct
{
	int16_t adcdat;
	float voltage;
	float current;
}adc_stru;

void ADS1118_Init(void);
void adcProc(void);
#endif





