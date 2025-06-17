#ifndef DO_H_
#define DO_H_
#include "main.h"

#define  DO_ctrl_7 		0
#define  DO_ctrl_6 		1
#define  DO_ctrl_5 		2
#define  DO_ctrl_4 		3
#define  DO_ctrl_3 		4
#define  DO_ctrl_2 		5
#define  DO_ctrl_1 		6
#define  DO_ctrl_0 		7
#define  DO_ctrl_15 	8
#define  DO_ctrl_14 	9
#define  DO_ctrl_13 	10
#define  DO_ctrl_12 	11
#define  DO_ctrl_11 	12
#define  DO_ctrl_10 	13
#define  DO_ctrl_9 		14
#define  DO_ctrl_8 		15

#define  DO_ctrl_19 	16
#define  DO_ctrl_18 	17
#define  DO_ctrl_17 	18
#define  DO_ctrl_16 	19
#define  DO_ctrl_20 	20
#define  DO_ctrl_21 	21
#define  DO_ctrl_22 	22
#define  DO_ctrl_23 	23

//*****do ctrl*********************


#define RELAY_ON		0
#define RELAY_OFF     1

#define HC595_SHCP_Low()   	   HAL_GPIO_WritePin(IN_SCK_GPIO_Port,IN_SCK_Pin,GPIO_PIN_RESET)
#define HC595_SHCP_High()      HAL_GPIO_WritePin(IN_SCK_GPIO_Port,IN_SCK_Pin,GPIO_PIN_SET)
#define HC595_STCP_Low() 			 HAL_GPIO_WritePin(IN_RCK_GPIO_Port,IN_RCK_Pin,GPIO_PIN_RESET)
#define HC595_STCP_High() 		 HAL_GPIO_WritePin(IN_RCK_GPIO_Port,IN_RCK_Pin,GPIO_PIN_SET)
#define HC595_Data_Low()   	   HAL_GPIO_WritePin(IN_SI_GPIO_Port,IN_SI_Pin,GPIO_PIN_RESET)
#define HC595_Data_High()      HAL_GPIO_WritePin(IN_SI_GPIO_Port,IN_SI_Pin,GPIO_PIN_SET)
#define DO_SIZE  15

void HC595_SendByte(uint8_t byte);
void XL74HC595_MultiWrite(uint8_t *data, uint8_t Length);//»ìºÏÐ´Êý¾Ý;
 

typedef struct
{
	//unsigned do_status;
	unsigned char do_No_out[3];
	unsigned char do_No_buf[DO_SIZE];
} stru_do_stru;

void do_ctrl_proc(unsigned int do_NO_sel,unsigned char bit_set);
void do_off(void);

#endif

