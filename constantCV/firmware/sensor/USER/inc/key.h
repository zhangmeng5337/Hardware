#ifndef KEY_H_
#define KEY_H_
#include "main.h"
#define KEY1_NUM   1
#define KEY2_NUM   2
#define KEY3_NUM  3

typedef struct
{
	unsigned char key_irq_status;
	unsigned char update;
}key_irq_stru;
void KeyProc(void);

#endif

