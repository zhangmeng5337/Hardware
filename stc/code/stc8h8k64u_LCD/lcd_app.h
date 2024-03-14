#ifndef LCD_APP_H_
#define LCD_APP_H_
#include "main.h" 

#define ON 0
#define OFF 1


void disp_proc(unsigned char seg_num,unsigned char dat,unsigned char dot_status);
void lcd_init(void);
void lcd_disp(void);


#endif

