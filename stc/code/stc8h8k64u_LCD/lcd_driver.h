#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_
#include "main.h" 

void write_byte(uchar byte,uchar length);
void Write_dat(uchar dat,uchar addr);		//д����
void Write_cmd(uchar cmd);		//дָ��
void write_byte2(uchar byte,uchar length);
void Write_dat2(uchar *dat,uchar addr);		//д����;		//д����;
#endif

