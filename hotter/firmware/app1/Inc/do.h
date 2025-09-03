#ifndef DO_H_
#define DO_H_
#include "main.h"


//*****do ctrl*********************
#define MACHINE_PWR   		0
#define HEATER_PWR   		1
#define SUPPLE_PUMP_PWR   	2
#define BASIC_PUMP_PWR		3
#define SECONDARY_PUMP_PWR	4
#define HEATER_INDOOR_PWR	5

#define RELAY_ON		0
#define RELAY_OFF     1

#define HC595_SHCP_Low()   	   HAL_GPIO_WritePin(IN_SCK_GPIO_Port,IN_SCK_Pin,GPIO_PIN_RESET)
#define HC595_SHCP_High()      HAL_GPIO_WritePin(IN_SCK_GPIO_Port,IN_SCK_Pin,GPIO_PIN_SET)
#define HC595_STCP_Low() 			 HAL_GPIO_WritePin(IN_RCK_GPIO_Port,IN_RCK_Pin,GPIO_PIN_RESET)
#define HC595_STCP_High() 		 HAL_GPIO_WritePin(IN_RCK_GPIO_Port,IN_RCK_Pin,GPIO_PIN_SET)
#define HC595_Data_Low()   	   HAL_GPIO_WritePin(IN_SI_GPIO_Port,IN_SI_Pin,GPIO_PIN_RESET)
#define HC595_Data_High()      HAL_GPIO_WritePin(IN_SI_GPIO_Port,IN_SI_Pin,GPIO_PIN_SET)
#define DO_SIZE  16


void HC595_SendByte(uint8_t byte);
void XL74HC595_MultiWrite(uint8_t *data, uint8_t Length);//»ìºÏÐ´Êý¾Ý;

typedef enum
{
	dq1 = 14,
	dq2 = 13,
	dq3 = 12,
	dq4 = 11,
	dq5 = 10,
	dq6 = 9,
	dq7 = 8,
	dq8 = 7,
	dq9 = 6,
	dq10 = 4,
	dq11 = 5,
	dq12 = 3,
	dq13 = 2,
	dq14 = 1,
	dq15 = 0
}do_table;

typedef struct
{
	 unsigned int do_status;
	unsigned char do_No_out[3];
	unsigned char do_update;
	short int do_num;
	short int do_out;
} stru_do_stru;

void do_ctrl_proc(unsigned int do_NO_sel,unsigned char bit_set);
void do_off(void);
long unsigned int get_do_status(void);
stru_do_stru *set_do_out(void);
void do_proc(void);

#endif

