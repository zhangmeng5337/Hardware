#ifndef KEY_H
#define KEY_H

#include "gpio.h"

//key gpio port
#define KEY_UP_PORT          3    //key7
#define KEY_DOWN_PORT        3    // key4
#define KEY_EAST_PORT        2   // key2
#define KEY_WEST_PORT        1   // key6  
#define KEY_SOUTH_PORT       3   // key1
#define KEY_NORTH_PORT       3   // key8
#define KEY_START_PORT       3   // key5
#define KEY_STOP_PORT        0   // key3
#define KEY_BREAK_PORT       0

#define KEY_UP_PORT_IRQ            PORT3_IRQn
#define KEY_DOWN_PORT_IRQ          PORT3_IRQn
#define KEY_EAST_PORT_IRQ          PORT2_IRQn
#define KEY_WEST_PORT_IRQ          PORT1_IRQn
#define KEY_SOUTH_PORT_IRQ         PORT3_IRQn
#define KEY_NORTH_PORT_IRQ         PORT3_IRQn
#define KEY_START_PORT_IRQ         PORT3_IRQn
#define KEY_STOP_PORT_IRQ          PORT0_IRQn
#define KEY_BREAK_PORT_IRQ         PORT0_IRQn

//key gpio pin
#define KEY_UP_PIN          3
#define KEY_DOWN_PIN        1
#define KEY_EAST_PIN        7
#define KEY_WEST_PIN        4
#define KEY_SOUTH_PIN       6
#define KEY_NORTH_PIN       4
#define KEY_START_PIN       5
#define KEY_STOP_PIN        2
#define KEY_BREAK_PIN       1


#define KEY_UP_BIT               	0x8000
#define KEY_DOWN_BIT         	0x4000
#define KEY_EAST_BIT           	0x2000
#define KEY_WEST_BIT          	0x1000
#define KEY_SOUTH_BIT        	0x0800
#define KEY_NORTH_BIT        	0x0400
#define KEY_START_BIT         	0x0200
#define KEY_STOP_BIT           	0x0100

#define KEY_OUT1_BIT         	0x80
#define KEY_OUT2_BIT         	0x40
#define KEY_OUT3_BIT         	0x20
#define KEY_OUT4_BIT         	0x10
#define KEY_BREAK_BIT        	0x08

#define KEY_UP_DOWN_INCH_BIT           		0x80
#define KEY_EAST_WEST_INCH_BIT         	0x40
#define KEY_SOUTH_NORTH_INCH_BIT       	0x20
#define KEY_OUT1_OUT2_INCH_BIT         	0x10
#define KEY_OUT3_OUT4_INCH_BIT         	0x08






#define INCH					0X01   //点动
#define SELFLOCK				0X02 //自锁
#define INHIBITION			0X03//相互拟制
#define NOINHIBITION		0X04//非拟制
uint16_t *KeyStaus(void);//return key status 
void KEY_Init(void);
boolean_t key_interrupt_trigger(uint16_t inch_status,uint16_t key_num);
#endif
