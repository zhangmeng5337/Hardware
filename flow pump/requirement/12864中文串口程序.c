
//***************************************************************************
//连线表: CPU=89C52  SystemClock=12Mhz                                      *
//CS=P1.0            SCLK=P1.2          SID=P1.1       Reset=RC in Board    *
//***************************************************************************

#include<reg52.h>
#include <math.h>

sbit CS =P1^0;
sbit SCK=P1^2;
sbit SID=P1^1;
//sbit Key=P1^4;

unsigned char code AC_TABLE[]={
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,      //第一行汉字位置
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,      //第二行汉字位置
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,      //第三行汉字位置
0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,      //第四行汉字位置
};
unsigned char code str2[]="欢迎光临sonsan!!";
unsigned char code str1[]="黄河远上白云间，一片孤城万仞山。羌笛何须怨杨柳，春风不度玉门关。";
unsigned char code bmp1[];

//串口发送一个字节
void SendByte(unsigned char Dbyte)
{
     unsigned char i;
     for(i=0;i<8;i++)
     {
           SCK = 0;
           Dbyte=Dbyte<<1;      //左移一位
           SID = CY;            //移出的位给SID
           SCK = 1;
           SCK = 0;
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
           SCK = 0;
           SCK = 1;            
           SCK = 0;
           if(SID) temp1++;
     }
     for(i=0;i<8;i++)
     {
           temp2=temp2<<1;
           SCK = 0;
           SCK = 1;
           SCK = 0;
           if(SID) temp2++;
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
     CS = 1;
     CheckBusy();
     SendByte(0xf8);            //11111,RW(0),RS(0),0
     SendByte(0xf0&Cbyte);      //高四位
     SendByte(0xf0&Cbyte<<4);//低四位(先执行<<)
     CS = 0;
}

void WriteData( unsigned char Dbyte )
{
     CS = 1;
     CheckBusy();
     SendByte(0xfa);            //11111,RW(0),RS(1),0
     SendByte(0xf0&Dbyte);      //高四位
     SendByte(0xf0&Dbyte<<4);//低四位(先执行<<)
     CS = 0;
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

void main( void )
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

//********************************************************//
//***** 转换的文件：E:\!Program\!BmpSample\25632.bmp
//***** 源图形宽度 * 高度：256 * 32
//***** 调整后宽度 * 高度：256 * 32
//***** 字模格式：横向取模,冗余模式,字节正序,正色取模,
//***** 点阵转换时间：2005/03/ 2   16:4:32
//***** 位图点阵占用1024字节
//********************************************************//
unsigned char code bmp1[]=
{
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0x80,0x00,0x0F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x70,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x3F,0xC0,0x00,0x07,0x07,0x00,0x04,0x01,0x00,0x00,0x38,0x60,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x78,0xE0,0x00,0x3F,0x83,0x80,0x06,0x03,0x80,0x00,0x1C,0x70,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x02,0x38,0x70,0x00,0xFF,0x07,0x80,0x06,0x39,0x80,0x00,0x1F,0x70,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x02,0x1C,0x30,0x00,0x7C,0x0E,0x00,0x06,0x19,0xC0,0x00,0x1F,0x80,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x02,0x1C,0x30,0x00,0x3E,0x1C,0x00,0x06,0x1F,0xC0,0x00,0x7F,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x03,0x3E,0x30,0x00,0x62,0x1A,0x00,0x0F,0x79,0x00,0x03,0xFE,0x00,0x00,0x01,
0x80,0x00,0x00,0x60,0x0F,0x00,0x3F,0x00,0x00,0x00,0x3C,0x00,0xC0,0x00,0x00,0x01,
0x80,0x07,0x76,0x18,0x00,0xC3,0xC1,0x00,0x1F,0xFB,0x00,0x03,0xFE,0x00,0x00,0x01,
0x80,0x00,0x01,0xE0,0x3F,0xC0,0xF1,0xC0,0x00,0x00,0xF0,0x01,0xC0,0x00,0x00,0x01,
0x80,0x07,0x6E,0x18,0x00,0xCF,0xC1,0x80,0x7E,0x3F,0x80,0x01,0xE2,0x00,0x00,0x01,
0x80,0x00,0x07,0xE0,0x7F,0xE0,0xE0,0xE0,0x00,0x03,0xC0,0x03,0xC0,0x00,0x00,0x01,
0x80,0x07,0x7F,0x98,0x00,0xFE,0x83,0x81,0xFC,0x1E,0xC0,0x00,0x83,0x00,0x00,0x01,
0x80,0x00,0x0F,0xE0,0x63,0xE1,0xE0,0xE0,0x00,0x07,0x00,0x03,0xC0,0x00,0x00,0x01,
0x80,0x06,0xFF,0xD8,0x01,0xFA,0x87,0x00,0xFD,0xF8,0xC0,0x01,0x73,0x00,0x00,0x01,
0x80,0x00,0x01,0xE0,0xC1,0xE1,0xE0,0xE0,0x00,0x0F,0x00,0x07,0xC0,0x00,0x00,0x01,
0x80,0x0F,0xD9,0xD8,0x03,0xE3,0x0E,0x80,0xFE,0x7E,0xC0,0x03,0xF9,0x80,0x00,0x01,
0x80,0x00,0x01,0xE0,0x01,0xE1,0xF0,0xE0,0x00,0x1E,0x00,0x0B,0xC0,0x00,0x00,0x01,
0x80,0x0D,0xB8,0x18,0x1F,0xC3,0x08,0xC0,0x0E,0x6D,0x80,0x03,0xF1,0x80,0x00,0x01,
0x80,0x00,0x01,0xE0,0x01,0xE0,0xFD,0xC7,0xEF,0x9E,0x00,0x13,0xC0,0x00,0x00,0x01,
0x80,0x0D,0xFE,0x18,0x0F,0xC2,0x10,0xE0,0x1C,0x3F,0x00,0x03,0xE0,0xC0,0x00,0x01,
0x80,0x00,0x01,0xE0,0x01,0xC0,0xFF,0x83,0xC6,0x1F,0xE0,0x13,0xC0,0x00,0x00,0x01,
0x80,0x0D,0xCE,0x18,0x06,0xC2,0x01,0xC0,0x38,0x3E,0x00,0x00,0xE8,0xC0,0x00,0x01,
0x80,0x00,0x01,0xE0,0x01,0x80,0x3F,0x81,0xE4,0x3C,0x78,0x23,0xC0,0x00,0x00,0x01,
0x80,0x0C,0x4C,0x18,0x00,0xC2,0x03,0x80,0x78,0x1F,0xF0,0x00,0xFC,0x60,0x00,0x01,
0x80,0x00,0x01,0xE0,0x03,0x80,0x3F,0xC1,0xE8,0x3C,0x78,0x43,0xC0,0x00,0x00,0x01,
0x80,0x0C,0x5E,0x18,0x00,0xC2,0x07,0x00,0x68,0x3F,0xF8,0x03,0xFC,0x22,0x00,0x01,
0x80,0x00,0x01,0xE0,0x03,0x00,0x6F,0xC0,0xF0,0x3C,0x3C,0x83,0xC0,0x00,0x00,0x01,
0x80,0x06,0x3F,0x38,0x01,0x82,0x0E,0x00,0x09,0xF8,0x00,0x03,0xC0,0x32,0x00,0x01,
0x80,0x00,0x01,0xE0,0x06,0x00,0xC3,0xE0,0x78,0x3C,0x3C,0xFF,0xF0,0x00,0x00,0x01,
0x80,0x07,0x10,0x38,0x00,0x82,0x1C,0x00,0x0B,0x99,0xC0,0x03,0x00,0x1A,0x00,0x01,
0x80,0x00,0x01,0xE0,0x0C,0x11,0xC3,0xE0,0x78,0x3C,0x3C,0xFF,0xF0,0x00,0x00,0x01,
0x80,0x03,0xFF,0xF0,0x00,0x02,0x38,0x00,0x08,0x30,0xE0,0x02,0x00,0x1E,0x00,0x01,
0x80,0x00,0x01,0xE0,0x08,0x11,0xC1,0xE0,0x7C,0x3C,0x3C,0xFF,0xF0,0x00,0x00,0x01,
0x80,0x01,0xEF,0xF0,0x00,0x00,0xE0,0x00,0x08,0xE0,0x60,0x00,0x00,0x0E,0x00,0x01,
0x80,0x00,0x01,0xE0,0x1F,0xF1,0xC1,0xE0,0xBC,0x1C,0x3C,0x03,0xC0,0x00,0x00,0x01,
0x80,0x00,0x03,0xE0,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x07,0x00,0x01,
0x80,0x00,0x01,0xE0,0x3F,0xE1,0xC1,0xC0,0x9E,0x1E,0x38,0x03,0xC0,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01,
0x80,0x00,0x01,0xF0,0x7F,0xE0,0xE3,0x83,0x1F,0x0E,0x70,0x03,0xC0,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x0F,0xFC,0xFF,0xE0,0x3F,0x07,0xDF,0x83,0xE0,0x03,0xC0,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x00,0x01,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
};
