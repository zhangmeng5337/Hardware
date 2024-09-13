
//***************************************************************************
//连线表: CPU=89C52  SystemClock=12Mhz                                      *
//CS=P1.0            SCLK=P1.2          SID=P1.1       Reset=RC in Board    *
//***************************************************************************


#include "lcd.h"
#include "lcd_string.h"


//sbit Key=P1^4;



//串口发送一个字节
void SendByte(unsigned char Dbyte)
{
     unsigned char i,tmp;
	 tmp = Dbyte;
     for(i=0;i<8;i++)
     {
           LCD_CLK_L
		   	if(tmp&0x80)
				LCD_SID_H
			else
				LCD_SID_L
           tmp=tmp<<1;      //左移一位
           LCD_CLK_H
           LCD_CLK_L
     }
}

//串口接收一个字节
//仅在读取数据的时候用到
//而读出的数据是一次只能读出4bit的
unsigned char ReceiveByte(void)
{
     unsigned char i,temp1,temp2;
     temp1=temp2=0;
     for(i=0;i<8;i++)
     {
           temp1=temp1<<1;
            LCD_CLK_L 
		  	LCD_CLK_H
            LCD_CLK_L
           if(LCD_SID_R)
		   	 temp1++;
     }
     for(i=0;i<8;i++)
     {
           temp2=temp2<<1;
		   LCD_CLK_L 
		   LCD_CLK_H
		   LCD_CLK_L
           if(LCD_SID_R) temp2++;
     }
     return ((0xf0&temp1)+(0x0f&temp2));
}

void CheckBusy( void )
{
     do   SendByte(0xfc);      //11111,RW(1),RS(0),0
     while(0x80&ReceiveByte());      //BF(.7)=1 Busy
}

void WriteCommand( unsigned char Cbyte )
{
     LCD_CS_H
     CheckBusy();
     SendByte(0xf8);            //11111,RW(0),RS(0),0
     SendByte(0xf0&Cbyte);      //高四位
     SendByte(0xf0&Cbyte<<4);//低四位(先执行<<)
     LCD_CS_L
}

void WriteData( unsigned char Dbyte )
{
     LCD_CS_H
     CheckBusy();
     SendByte(0xfa);            //11111,RW(0),RS(1),0
     SendByte(0xf0&Dbyte);      //高四位
     SendByte(0xf0&Dbyte<<4);//低四位(先执行<<)
    LCD_CS_L
}

unsigned char ReadData( void )
{
     CheckBusy();
     SendByte(0xfe);            //11111,RW(1),RS(1),0
     return ReceiveByte();
}

void Delay(unsigned int MS)
{
     unsigned char us,usn;
     while(MS!=0)            //for 12M
           { usn = 2;
                 while(usn!=0)
                       {
                             us=0xf5;
                             while (us!=0){us--;};
                             usn--;
                       }
                 MS--;
           }
}

//维捷登测试架专用延时函数
void DelayKey(unsigned int Second , unsigned int MS100)
{                              //输入精确到0.1S,是用","
     unsigned int i;
     for(i=0;i<Second*100+MS100*10;i++)
     {
          // if(Key==0)
          // {
                 Delay(20);
          //       while(Key==0) {Delay(20);}
           //      break;
          // }
          // else Delay(10);
     }
}

void LcmInit( void )
{
     WriteCommand(0x30);      //8BitMCU,基本指令集合
     WriteCommand(0x03);      //AC归0,不改变DDRAM内容
     WriteCommand(0x0C);      //显示ON,游标OFF,游标位反白OFF
     WriteCommand(0x01);      //清屏,AC归0
     WriteCommand(0x06);      //写入时,游标右移动
}

//文本区清RAM函数
void LcmClearTXT( void )
{
     unsigned char i;
     WriteCommand(0x30);      //8BitMCU,基本指令集合
     WriteCommand(0x80);      //AC归起始位
     for(i=0;i<64;i++)
        WriteData(0x20);
}

//图形区和文本区显示在两个不同的RAM区
//图形区清RAM函数
void LcmClearBMP( void )
{
     unsigned char i,j;
     WriteCommand(0x34);      //8Bit扩充指令集,即使是36H也要写两次
     WriteCommand(0x36);      //绘图ON,基本指令集里面36H不能开绘图
     for(i=0;i<32;i++)            //12864实际为256x32
     {
           WriteCommand(0x80|i);      //行位置
           WriteCommand(0x80);      //列位置
           for(j=0;j<32;j++)            //256/8=32 byte
                WriteData(0);
     }
}

void PutStr(unsigned char row,unsigned char col,unsigned char *puts)
{
     WriteCommand(0x30);      //8BitMCU,基本指令集合
     WriteCommand(AC_TABLE[8*row+col]);      //起始位置
     while(*puts != '\0')      //判断字符串是否显示完毕
     {
           if(col==8)            //判断换行
           {            //若不判断,则自动从第一行到第三行
                 col=0;
                 row++;
           }
           if(row==4) row=0;      //一屏显示完,回到屏左上角
           WriteCommand(AC_TABLE[8*row+col]);
           WriteData(*puts);      //一个汉字要写两次
           puts++;
           WriteData(*puts);
           puts++;
           col++;
     }
}

void PutBMP(unsigned char *puts)
{
     unsigned int x=0;
     unsigned char i,j;
     WriteCommand(0x34);      //8Bit扩充指令集,即使是36H也要写两次
     WriteCommand(0x36);      //绘图ON,基本指令集里面36H不能开绘图
     for(i=0;i<32;i++)            //12864实际为256x32
     {
           WriteCommand(0x80|i);      //行位置
           WriteCommand(0x80);      //列位置
           for(j=0;j<32;j++)      //256/8=32 byte
           {            //列位置每行自动增加
                 WriteData(puts[x]);
                 x++;
           }
     }
}

//维捷登测试用点阵显示
void DisplayDots(unsigned char DotByte)
{
     unsigned char i,j;
     WriteCommand(0x34);      //8Bit扩充指令集,即使是36H也要写两次
     WriteCommand(0x36);      //绘图ON,基本指令集里面36H不能开绘图
     for(i=0;i<32;i++)            //12864实际为256x32
     {
           WriteCommand(0x80|i);      //行位置
           WriteCommand(0x80);      //列位置
           for(j=0;j<32;j++)      //256/8=32 byte
           {            //列位置每行自动增加
                 WriteData(DotByte);                  
           }
           DotByte=~DotByte;
     }
}

void lcd_proc_test( void )
{
     Delay(100);      //等待复位
     LcmInit();
     LcmClearTXT();
     LcmClearBMP();
     while(1)
     {
           LcmClearTXT();
           PutBMP(bmp1);
           DelayKey(1,5);
           
           DisplayDots(0x55);
           DelayKey(1,5);
           
           LcmClearBMP();
           PutStr(0,0,str1);
           DelayKey(1,5);
                       
           LcmClearTXT();
           DisplayDots(0xaa);
           DelayKey(1,5);
     }
}



