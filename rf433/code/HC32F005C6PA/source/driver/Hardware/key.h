#ifndef KEY_H
#define KEY_H

#include "gpio.h"

//key gpio port
#define KEY_UP_PORT          3    //key7
#define KEY_DOWN_PORT        0    // key4
#define KEY_EAST_PORT        3   // key2
#define KEY_WEST_PORT        3   // key6  
#define KEY_SOUTH_PORT       3   // key1
#define KEY_NORTH_PORT       3   // key8
#define KEY_START_PORT       3   // key5
#define KEY_STOP_PORT        1   // key3
#define KEY_BREAK_PORT       0

#define KEY_UP_PORT_IRQ            PORT3_IRQn
#define KEY_DOWN_PORT_IRQ          PORT0_IRQn
#define KEY_EAST_PORT_IRQ          PORT3_IRQn
#define KEY_WEST_PORT_IRQ          PORT3_IRQn
#define KEY_SOUTH_PORT_IRQ         PORT3_IRQn
#define KEY_NORTH_PORT_IRQ         PORT3_IRQn
#define KEY_START_PORT_IRQ         PORT3_IRQn
#define KEY_STOP_PORT_IRQ          PORT1_IRQn
#define KEY_BREAK_PORT_IRQ         PORT0_IRQn

//key gpio pin
#define KEY_UP_PIN          4
#define KEY_DOWN_PIN        2
#define KEY_EAST_PIN        5
#define KEY_WEST_PIN        6
#define KEY_SOUTH_PIN       3
#define KEY_NORTH_PIN       1
#define KEY_START_PIN       7
#define KEY_STOP_PIN        4
#define KEY_BREAK_PIN       1



#define INCH					0X01   //点动
#define SELFLOCK				0X02 //自锁
#define INHIBITION			0X03//相互拟制
#define NOINHIBITION		0X04//非拟制
void KEY_Init(void);
#endif
