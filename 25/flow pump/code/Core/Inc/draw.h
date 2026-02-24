#ifndef __DRAW_H
#define __DRAW_H


#include "lcd.h"

#include <stdbool.h>

void LCD_Disp_Image(const u8 *str);

void LCD_Disp_Large_Num2432(const u8 *Font,u8 col,u8 Num);
void LCD_Disp_Large_Num1632(const u8 *Font,u8 col,u8 Num,bool reverse);

extern const unsigned char  Logo[];
extern const unsigned char  digitalFont1_24x32[];
extern const unsigned char  digitalFont2_24x32[10][96];
extern const u8  digitalFont_16x32[];
extern unsigned char const usr_char[];

void LCD_Write_USR_CGRAM(void);

void LCD_DISP_USR_Char(u8 row, u8 col,u8 index);

void LCD_Baddot_test(void);
void lcd_proc_test(void);
void lcd_init(void);
unsigned char *show_dat(float dat);
void LCD12864_Fixpicture(unsigned char row, unsigned char x_pos,
                         unsigned char y_pos,
                         unsigned char row_size, unsigned char col_size, unsigned char *gImage);

#endif

