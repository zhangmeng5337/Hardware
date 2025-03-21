/*
 * @Description: 鍗″皵鏇兼护娉唬�? * @Author: TOTHTOT
 * @Date: 2023-03-28 10:14:16
 * @LastEditTime: 2023-03-28 10:33:27
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Work\MCU\stm32\read_cs1237_STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\KALMAN\kalman.h
 */
#ifndef ____DISPLAY_H____
#define __DISPLAY_H__

#include "main.h"
#define NUM_MODE  		0
#define SETTING_MODE  	1

typedef struct
{
	unsigned char dis_buf[4];
	unsigned char update;
	unsigned char dot_pos;
	uint32_t update_cycle;//ms
	unsigned char signedFlag;//0:positvie
	unsigned char dat_bits;
	float dis_val;
	uint32_t dis_val_u;
	unsigned char cusor;
	unsigned char mode;  // 0:number 1:setting mode 
}dis_stru;
void display(void);
void display_init(void);
dis_stru *GetDisp(void);
void display_led(unsigned char Cusor,unsigned BitSel);
unsigned char GetSeg(unsigned char dat);

#endif /* __KALMAN_H__ */
