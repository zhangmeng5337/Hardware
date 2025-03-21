/*******************************************************************************
*
* 代码许可和免责信�?* 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源�?* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或�?* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件�?* 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失�?* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些�?* 全部上述排除或限制可能并不适用于您�?*
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __74HC595_H
#define __74HC595_H
#include "main.h"
#define HC595_SHCP_Low()   	   GPIO_WritePin(IN_SCK_GPIO_PORT,IN_SCK_GPIO_PIN,GPIO_Pin_RESET)
#define HC595_SHCP_High()      GPIO_WritePin(IN_SCK_GPIO_PORT,IN_SCK_GPIO_PIN,GPIO_Pin_SET)
#define HC595_STCP_Low() 			 GPIO_WritePin(IN_RCKGPIO_PORT,IN_RCK_GPIO_PIN,GPIO_Pin_RESET)
#define HC595_STCP_High() 		 GPIO_WritePin(IN_SCK_GPIO_PORT,IN_RCK_GPIO_PIN,GPIO_Pin_SET)
#define HC595_Data_Low()   	   GPIO_WritePin(IN_SI_GPIO_PORT,IN_SI_GPIO_PIN,GPIO_Pin_RESET)
#define HC595_Data_High()      GPIO_WritePin(IN_SI_GPIO_PORT,IN_SI_GPIO_PIN,GPIO_Pin_SET)

void XL74HC595_MultiWrite(uint8_t *data, uint8_t Length);//���д����

#endif /*__CW32L010_SPI_H */

