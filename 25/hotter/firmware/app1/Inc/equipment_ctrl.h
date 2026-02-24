#ifndef EQUIPMENT_CTRL_H_
#define EQUIPMENT_CTRL_H_

#define MACHINE_NUM			3
#define MACHINE_REG_COUNT   16

#define  MACHINE_REG_CTRL	0  //
#define  MACHINE_REG_MODE	1 
#define  MACHINE_REG_T   	3

#define  MACHINE_REG_FAULT	4 //


#define MACHINE_CTRL    0x0000
#define MACHINE_MODE    0x0000
#define MACHINE_FAULT   0x0051


typedef struct 
{
 unsigned int reg_data[MACHINE_NUM][MACHINE_REG_COUNT];
}machine_stru;
unsigned char equipment_control(void);
machine_stru *get_machine(void);

#endif


