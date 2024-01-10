#include <STC8F.h>

//SPI
sbit CS   =P3^7;		//选通 1
sbit SCLK =P3^4;		//时钟 1
sbit DIN  =P3^6;		//输入 1
sbit DOUT =P3^5;		//输出 1   AT7053BU

#define uchar  unsigned char
#define bchar bdata unsigned char
#define uint   unsigned int
#define ulong  unsigned long
#define xchar  xdata unsigned char
#define xint   xdata unsigned int
#define xlong  unsigned long

void write_7053c(uchar add,ulong date);

//从ATT7053c的指定地址读出数据
ulong read_7053c(uchar add);

//初始化att7053c
void in_att7053c(void);


