#ifndef _LCD_H
#define _LCD_H
#include "main.h"
typedef struct  
{
	unsigned char page_id;
	unsigned char page_button;
	unsigned char update;
	unsigned char buf[16];
}
void lcd_proc(void);


#endif



