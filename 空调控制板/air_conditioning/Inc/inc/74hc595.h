
#ifndef __74HC595_H__
#define	__74HC595_H__

#include "main.h"

#define M74HC595_PORT		M74HC595_SI_GPIO_Port
#define M74HC595_PORT_RCC	RCC_AHBPeriph_GPIOA
//#define M74HC595_SCLR_NOT	GPIO_Pin_0
#define M74HC595_SCK		M74HC595_SCK_Pin

#define M74HC595_RCK_PORT		M74HC595_RCK_GPIO_Port
#define M74HC595_RCK		M74HC595_RCK_Pin
//#define M74HC595_G_NOT		GPIO_Pin_3
#define M74HC595_SI			M74HC595_SI_Pin


void M74HC595_Init(void);
void M74HC595_ClearData(void);
void M74HC595_EnableData(void);
void M74HC595_WriteData(const unsigned char data);
void seg_select(unsigned char num);
#endif


