#include "main.h"
#include "lcd_driver.h"
#define uchar unsigned char
#define uint unsigned int
 
#define _Nop() _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_()
#define BIAS 0x29 //0b1000 0101 0010 1/3duty 4com
 
#define SYSDIS 0X00 //0b1000 0000 0000 关振系统荡器和LCD偏压发生器
#define SYSEN 0X01 //0b1000 0000 0010 打开系统振荡器
 
#define LCDOFF 0X02 //0b1000 0000 0100 关LCD偏压
#define LCDON 0X03 //0b1000 0000 0110 打开LCD偏压
#define XTAL 0x1c //0b1000 0010 1000 外部接时钟
#define RC256 0X18 //0b1000 0011 0000 内部时钟
#define TONEON 0X12 //0b1000 0001 0010 打开声音输出
#define TONEOFF 0X10 //0b1000 0001 0000 关闭声音输出
#define WDTDIS 0X0A //0b1000 0000 1010 禁止看门狗
 




uchar code
Ht1621Tab[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
 uchar code seg_tab[] =
{
    // 数字显示0-9，任何显示数字的区域，都是调用这里的数据
    0X5f, // 0
    0X06, // 1
    0X6b, // 2
    0X2f, // 3
    0X36, // 4
    0X3D, // 5
    0X7D, // 6
    0X07, // 7
    0X7F, // 8
    0X3F,  // 9
	  0x80   //dot
};
//delay us
void Delay(uchar us) //5,7,9
{
 while(--us);
}
 
 
//delay ms
void DelayMS(uint iMs)
{
 uint i,j;
 for(i=0;i<iMs;i++)
 for(j=0;j<65;j++) Delay(1);
}
 
/******************************************************
写数据函数,cnt为传送数据位数,数据传送为低位在前
*******************************************************/
void Ht1621Wr_Data(uchar Data,uchar cnt)
{
 uchar i;
 for (i=0;i<cnt;i++)
      {
       HT1621_WR=0;
       Delay(1);
       HT1621_DAT=Data&0x80;
       Delay(1);
       HT1621_WR=1;
       Delay(1);
      Data<<=1;
     }
 
}
 
/********************************************************
函数名称：void Ht1621WrCmd(uchar Cmd)
功能描述: HT1621命令写入函数
全局变量：无
参数说明：Cmd为写入命令数据
返回说明：无
 
说 明：写入命令标识位100
********************************************************/
void Ht1621WrCmd(uchar Cmd)
{
 HT1621_CS=0;
       Delay(1);
 Ht1621Wr_Data(0x80,3); //写入命令标志100
 Ht1621Wr_Data(Cmd,9); //写入命令数据
 HT1621_CS=1;
       Delay(1);
}
/********************************************************
函数名称：void Ht1621WrOneData(uchar Addr,uchar Data)
功能描述: HT1621在指定地址写入数据函数
全局变量：无
参数说明：Addr为写入初始地址，Data为写入数据
返回说明：无
说 明：因为HT1621的数据位4位，所以实际写入数据为参数的后4位
********************************************************/
void Ht1621WrOneData(uchar Addr,uchar Data)
{
 HT1621_CS=0;
 Ht1621Wr_Data(0xa0,3); //写入数据标志101
 Ht1621Wr_Data(Addr<<2,6); //写入地址数据
 Ht1621Wr_Data(Data<<4,4); //写入数据
 HT1621_CS=1;
       Delay(1);
}
/********************************************************
HT1621测试程序，2008-2-13, 22:41:43
函数名称：void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt)
功能描述: HT1621连续写入方式函数
全局变量：无
参数说明：Addr为写入初始地址，*p为连续写入数据指针，
                  cnt为写入数据总数
返回说明：无
说 明：HT1621的数据位4位，此处每次数据为8位，写入数据
           总数按8位计算
********************************************************/
void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt)
{
 uchar i;
 HT1621_CS=0;
 Ht1621Wr_Data(0xa0,3); //写入数据标志101
 Ht1621Wr_Data(Addr<<2,6); //写入地址数据
 for (i=0;i<cnt;i++)
      {
        Ht1621Wr_Data(*p,8); //写入数据
        p++;
       }
 HT1621_CS=1;
       Delay(1);
}
/********************************************************
函数名称：void Ht1621_Init(void)
功能描述: HT1621初始化
全局变量：无
参数说明：无
返回说明：无
版 本：1.0
说 明：初始化后，液晶屏所有字段均显示
********************************************************/
void Ht1621_Init(void)
{
 HT1621_CS=1;
 HT1621_WR=1;
 HT1621_DAT=1;
 DelayMS(2000); //延时使LCD工作电压稳定
 Ht1621WrCmd(BIAS);
 Ht1621WrCmd(RC256); //使用内部振荡器
 Ht1621WrCmd(SYSDIS);
 Ht1621WrCmd(WDTDIS);
 Ht1621WrCmd(SYSEN);
 Ht1621WrCmd(LCDON);
	Ht1621WrAllData(0,Ht1621Tab,16);
}
 
/**********************************/
void test()
{
 uchar i,j,t;
 //Ht1621_Init(); //上电初始化LCD
 //DelayMS(1000); //延时一段时间
 while(1){
               //Ht1621WrAllData(0,Ht1621Tab,16); //清除1621寄存器数据，暨清屏
	             
	             Ht1621WrOneData(0,seg_tab[0]>>4);
	             Ht1621WrOneData(1,seg_tab[0]);
								DelayMS(5000);
	             Ht1621WrOneData(0,seg_tab[1]>>4);
	             Ht1621WrOneData(1,seg_tab[1]);
								DelayMS(5000);
		             Ht1621WrOneData(0,seg_tab[2]>>4);
	             Ht1621WrOneData(1,seg_tab[2]);
								DelayMS(5000);
	             Ht1621WrOneData(0,seg_tab[3]>>4);
	             Ht1621WrOneData(1,seg_tab[3]);
								DelayMS(5000); 
	 
	             Ht1621WrOneData(0,seg_tab[4]>>4);
	             Ht1621WrOneData(1,seg_tab[4]);
								DelayMS(5000);
	             Ht1621WrOneData(0,seg_tab[5]>>4);
	             Ht1621WrOneData(1,seg_tab[5]);
								DelayMS(5000);	
		             Ht1621WrOneData(0,seg_tab[6]>>4);
	             Ht1621WrOneData(1,seg_tab[6]);
								DelayMS(5000);
	             Ht1621WrOneData(0,seg_tab[7]>>4);
	             Ht1621WrOneData(1,seg_tab[7]);
								DelayMS(5000); 
	 
	             Ht1621WrOneData(0,seg_tab[8]>>4);
	             Ht1621WrOneData(1,seg_tab[8]);
								DelayMS(5000);
	             Ht1621WrOneData(0,seg_tab[9]>>4);
	             Ht1621WrOneData(1,seg_tab[9]);
								DelayMS(5000);	 								
  }
}