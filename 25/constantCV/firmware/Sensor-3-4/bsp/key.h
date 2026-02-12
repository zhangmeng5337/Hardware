#ifndef KEY_H_
#define KEY_H_
#include "main.h"
#define KEY1_NUM   1
#define KEY2_NUM   2
#define KEY3_NUM  3

#define BIT_SEL  4
#define KEY_FILTER 5
#define SHORT_PRES 1000
#define LONG_PRES 5000
#define KEY_TIMEOUT 60000    //key press time out 
#define NORMAL_MENU  		0
#define PASS_MENU    		2
#define ADDR_MENU  		    4
#define BAUD_MENU  		    6
#define PAR_MENU  		    8
#define SPD_MENU  		    10
#define UNIT_MENU  		    12
#define DOT_MENU  		    14
#define OFT_MENU  		    16
#define COE_MENU  		    18
#define SAVE_MENU  		    20
#define YES_MENU  		    22
#define NO_MENU  		    24
#define OUT_MENU  		    26


#define NO_PRE      0x00
#define INC     	0x01
#define MOVE     	0x02
#define LONG_PRE    0x03
#define SHORT_PRE   0x04

#define A 0x77
#define B 0x7C
#define C 0x58
#define D 0x5e
#define E 0x79
#define F 0x71
#define H 0x76
#define I 0x06
#define L 0x38
#define N 0x37
#define U 0x3e
#define O 0x5c
#define P 0x73
#define R 0x50
#define S 0x6d
#define T 0x78
#define Y 0x6e
#define V 0x62


typedef struct
{
	unsigned char menu_num;
	unsigned char enter;
	unsigned char next;
	unsigned char retur;
	unsigned char move;//¹â±ê
	void (*current_operation)(void);
}meun_task_stru;

typedef struct
{
	unsigned int addr;
	unsigned int baud;
	unsigned char par;
	unsigned char spd;
	unsigned char unit;
	unsigned char dot;
	float oft;

	float coe;
	unsigned char passw[4];
	unsigned char status;//1:yes  2:no
	//

}input_params_stru;
typedef struct
{
	unsigned char key_irq_status;
	unsigned char update;
	//unsigned char mode;//0:
	uint32_t delay_tick;
	unsigned char double_check;//0: 1:inc 2:dec 3:double long pre 4:double short pre
	input_params_stru *indat;	
	unsigned char bit_sel;
		unsigned char state;


}key_irq_stru;
void KeyProc(void);
void key_init(void);
key_irq_stru *GetKeyStatus(void);
void display_menu(void);
key_irq_stru *getKey(void);
unsigned char getTaskIndex(void);

#endif

