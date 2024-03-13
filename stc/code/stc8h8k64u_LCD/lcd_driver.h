#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_
#include "main.h" 

void write_byte(uchar byte,uchar length);
void Write_dat(uchar dat,uchar addr);		//写数据
void Write_cmd(uchar cmd);		//写指令
void write_byte2(uchar byte,uchar length);
void Write_dat2(uchar *dat,uchar addr);		//写数据;		//写数据;
#endif

