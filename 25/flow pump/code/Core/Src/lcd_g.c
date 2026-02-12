
#include "sys.h"
#include "lcd.h"
#include <stdio.h>
/****************************************
12864的驱动采用串行接法
****************************************/


void LCD_Init(void)
{

    LCD_WriteCMD(0x30);
    delay_us(10000);
    LCD_WriteCMD(LCD_AWAIT_MODE);
    delay_us(10000);
    LCD_WriteCMD(LCD_IRAMADDRESS_ON);
    delay_us(10000);
    LCD_WriteCMD(0x06);
    delay_us(10000);
    LCD_WriteCMD(0x0c);
    delay_us(10000);
    LCD_WriteCMD(LCD_GRAPH_OFF); //图形开启（不显示）
    delay_us(10000);
    Lcd_Fill(0x00);    //清空屏内RAM
    delay_us(10000);
    LCD_WriteCMD(LCD_GRAPH_ON); //图形开启（开显示）
    delay_us(10000);
}

/*  以串行方式发送一字节数据 */ 
static void SendByte(u8 sendData)
{
  u8 i;
  for (i = 0; i < 8; i++)
  {
    if (sendData&0x80)
      LCD_SID_HIGH;
    else
      LCD_SID_LOW;
    LCD_CLK_LOW;
     delay_us(10); //延时10个us等待上一条指令执行完毕
    LCD_CLK_HIGH;
     sendData=sendData<<1;
   }
}


/* 串行接受一个字节的数据  */
static u8 ReceiveByte(void)
{
   u8 i,d1,d2;
   for (i = 0; i < 8; i++)
   {
    LCD_CLK_LOW;
    delay_us(10);
    LCD_CLK_HIGH;
    
    if (LCD_SID) d1++;
    d1 = d1<<1;
   }
   
   for (i = 0; i < 8; i++)
   {
    LCD_CLK_LOW;
     delay_us(1);
    LCD_CLK_HIGH;   
    if (LCD_SID) d2++;
    d2 = d2<<1;
   }
   return (d1&0xf0 + d2&0x0f);
}

//检测数据寄存器的最高位，判断忙标志
 static u8 ReadBF(void)
{
  SendByte(READ_STATE);
  u8 data = ReceiveByte();
  if (data > 0x7f) return 1;
  else  return 0;
}

void LCD_WriteCMD(u8 data)
{
  LCD_CS_HIGH;
  while (ReadBF());
  SendByte(writecommand); //发送串口控制格式
  SendByte(data&0xf0);  //高四位
  SendByte((data&0x0f)<<4); //低四位
  while (ReadBF());
  LCD_CS_LOW;
}
      
void LCD_WriteDATA(u8 data)
{
  LCD_CS_HIGH;
  while (ReadBF());
  SendByte(writedata); //发送串口控制格式
  SendByte(data&0xf0);   //发送高4位
  SendByte((data&0x0f)<<4); //发送低4位
  while (ReadBF());
  LCD_CS_LOW;
 }

//void LCD_WriteCMD(uchar Cbyte )
//{
//    //LCD_RS_1;
//    SendByte(0xf8);              //11111,RW(0),RS(0),0
//    SendByte(0xf0&Cbyte);
//    SendByte(0xf0&Cbyte<<4);
//    //LCD_RS_0;
//}
//
//void LCD_WriteDATA(uchar Dbyte )
//{
//    //LCD_RS_1;
//    SendByte(0xfa);              //11111,RW(0),RS(1),0
//    SendByte(0xf0&Dbyte);
//    SendByte(0xf0&Dbyte<<4);
//    //LCD_RS_0;
//}

void LCD_Set_XY(uchar x,uchar y)
{
    unsigned char k = 0;
    switch(x)
      {
        case 1:k  = 0x80 + y;break;
        case 2:k  = 0x90 + y;break;
        case 3:k  = 0x88 + y;break;
        case 4:k  = 0x98 + y;break;
        default:k = 0x80 + y;
      }
    LCD_WriteCMD(k);
}


//一种算法，测试OK xianhuitu test OK
// 测试发现整个执行完大概需要2~3S
 void LCD_Clear_GDRAM(void)  //test OK
{    
  u8 j, i;
  LCD_WriteCMD(0x36);
  for(j=0;j<32;j++) //上半屏32点行
  {
    LCD_WriteCMD(0x80+j);
    LCD_WriteCMD(0x80);//X坐标
    for(i=0;i<32;i++)//32表示如1行写完,自动写第3行
    {
      LCD_WriteDATA(0x00);  
    }
  }
  LCD_WriteCMD(0x30);     //基本指令 
}

void LCD_Clear_DDRAM()
{
	LCD12864_Write_SuperString(1,0,"                      ");
	LCD12864_Write_SuperString(2,0,"                      ");
	LCD12864_Write_SuperString(3,0,"                      ");
	LCD12864_Write_SuperString(4,0,"                      ");
  LCD_WriteCMD(0x01);
  delay_us(10000);
}
void LCD12864_Write_SuperString(uchar x,uchar y,uchar *s)
{
    LCD_WriteCMD(0x30); //进入标准模式
    LCD_Set_XY(x,y);
    while (*s)
        {
            LCD_WriteDATA(*s);
            s++;
        }
    LCD_WriteCMD(0x36); //返回图形模式
}
void LCD12864_Write_Numberd(unsigned char X,unsigned char Y,int number)
{
	char sz[8];//字符串
	sprintf(sz, "%d", number);//这句需要头文件#include <stdio.h>
  LCD12864_Write_SuperString(X,Y,sz);
}
void LCD12864_Write_Numberf(unsigned char X,unsigned char Y,float number)
{
	char sz[8];//字符串
	sprintf(sz, "%.1f", number);//这句需要头文件#include <stdio.h>
  LCD12864_Write_SuperString(X,Y,sz);
}


void Lcd_Fill(uchar color) //填充屏内RAM
{
    uchar x,y,i;
    LCD_WriteCMD(0x34);
    for(i=0; i<9; i=i+8)
        for(y=0; y<0x20; y++)
            {
                for(x=0; x<8; x++)
                    {
                        LCD_WriteCMD(y+0x80);
                        LCD_WriteCMD(x+0x80+i);
                        LCD_WriteDATA(color);
                        LCD_WriteDATA(color);
                    }
            }
    LCD_WriteCMD(0x36);
}
/*******************************************************************************
LCD12864接口定义如下：
并口模式如下：
 
第1脚： VSS为地电源
第2脚： VDD接5V正电源
第3脚： V0为液晶显示器对比度调整端，接正电源时对比度最弱，接地电源时对比度最高
第4脚： RS为寄存器选择，高电平时选择数据寄存器、低电平时选择指令寄存器。
第5脚： R/W  当R/W和E都为高电平时，数据被读到DB0～DB7；当R/W为低电平，E从高电平变为低电平，DB0～DB7的数据被写到IR或DR寄存器。
第6脚： E  使能信号
第7~14脚：DB0～DB7  为8位双向数据线。
第15脚：PSB  并行/串行选择。当PSB为高电平时是并行模式；当PSB为低电平时，是串行模式。。
第16脚：NC  空脚。
第17脚：/RESET 复位脚。低电平有郊。
第18脚：VOUT  LCD驱动电压输出端，可不接。
第19脚：A  背光灯电源正极，接+5V。
第20脚：K  背光灯电源负极，接地。

串口模式如下：
 
第1脚： VSS为地电源
第2脚： VDD接5V正电源
第3脚： V0为液晶显示器对比度调整端，接正电源时对比度最弱，接地电源时对比度最高
第4脚： CS  片选信号，高电平有效。
第5脚： SID  串行数据输入端。
第6脚： CLK  串行同步时钟。
第15脚：PSB  并行/串行选择。当PSB为高电平时是并行模式；当PSB为低电平时，是串行模式。
第17脚：/RESET 复位脚。低电平有郊。
第19脚：A  背光灯电源正极，接+5V。
第20脚：K  背光灯电源负极，接地?
*******************************************************************************/
