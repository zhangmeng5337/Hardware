/*****************************************
*        CS1237 designed by Robert Lee           *
*                                        *
*****************************************/
#include "cs1237.h"
#include "stdio.h"
#include "main.h"


//#ifdef USER_CS1237  
#define ADC_Bit  24 //ADC有效位数，带符号位 最高24位
#define CS1237_SCL_H  SetGpio(10, 0)
#define CS1237_SCL_L  ClearGpio(10, 0)
#define CS1237_SDA_H  SetGpio(10, 1)
#define CS1237_SDA_L  ClearGpio(10, 1)
#define        NOP_5()                __asm {nop};__asm {nop};__asm {nop}
#define        NOP30()                NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();
#define        NOP40()                NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();
#define One_CLK  CS1237_SCL_H;delay_us(1);CS1237_SCL_L;delay_us(1);
#define CS_CON  0X00   //芯片地配置 内部REF 输出10HZ PGA=1 通道A 0X00  
#define Lv_Bo 0.01  //滤波系数 小于1

//sbit DOUT = P3^7;//数据对应IO口
//sbit SCLK = P3^5;//时钟对应IO口
static long AD_Res_Last=0;//上一轮的ADC数值保存

//延时500US 5.5296MHZ
void delay_500us(volatile unsigned char a)
{        
        volatile unsigned char i,j,b;
        for(b=0;b<a;b++)
        {
                i = 3;
                j = 137;
                do
                {
                        while (--j);
                } while (--i);
        }
}
//CS1237进入低功耗模式
void CS1237_Power_Down(void)
{
        //SCLK = 1;
        CS1237_SCL_H;
        delay_500us(100);
        //SCLK = 1;
        CS1237_SCL_H;
        //SCLK = 1;
        CS1237_SCL_H;
        delay_500us(100);
}
//配置CS1237芯片
void Con_CS1237(void)
{
        unsigned char i;
        unsigned char dat;
        unsigned short count_i=0;//溢出计时器

        SetPinMux(10, 0, 0);//SCK
        SetPinMux(10, 1, 0);//SDA

        SetPinDir(10,0,1);
        SetPinDir(10,1,0);
        
        dat = CS_CON;// 0100 1000
        CS1237_SCL_L;//时钟拉低
        while(CS1237_SDA_READ ==1)//芯片准备好数据输出  时钟已经为0，数据也需要等CS1237全部拉低为0才算都准备好
        {
                delay_500us(10);
                count_i++;
                if(count_i > 15000)
                {
                        CS1237_SCL_H;
                        CS1237_SDA_H;
                        printf("here error\r\n");
                        return;//超时，则直接退出程序
                }
        }
        for(i=0;i<29;i++)// 1 - 29
        {
                One_CLK;
        }
        CS1237_SCL_H;NOP30();CS1237_SDA_H;CS1237_SCL_L;NOP30();//30
        CS1237_SCL_H;NOP30();CS1237_SDA_H;CS1237_SCL_L;NOP30();//31
        CS1237_SCL_H;NOP30();CS1237_SDA_L;CS1237_SCL_L;NOP30();//32
        CS1237_SCL_H;NOP30();CS1237_SDA_L;CS1237_SCL_L;NOP30();//33
        CS1237_SCL_H;NOP30();CS1237_SDA_H;CS1237_SCL_L;NOP30();//34
        CS1237_SCL_H;NOP30();CS1237_SDA_L;CS1237_SCL_L;NOP30();//35
        CS1237_SCL_H;NOP30();CS1237_SDA_H;CS1237_SCL_L;NOP30();//36
        One_CLK;//37     写入了0x65
        for(i=0;i<8;i++)// 38 - 45个脉冲了，写8位数据
        {
                CS1237_SCL_H;NOP40();
                if(dat&0x80)
                        CS1237_SDA_H;
                else
                        CS1237_SDA_L;
                dat <<= 1;
                CS1237_SCL_L;NOP40();
        }
        One_CLK;//46个脉冲拉高数据引脚
}
//读取芯片的配置数据
unsigned char Read_CON(void)
{
        unsigned char i;
        unsigned char dat=0;//读取到的数据
        unsigned short count_i=0;//溢出计时器
        unsigned char k=0,j=0;//中间变量
        CS1237_SCL_L;//时钟拉低
        while(CS1237_SDA_READ ==1)//芯片准备好数据输出  时钟已经为0，数据也需要等CS1237全部拉低为0才算都准备好
        {
                delay_500us(10);
                count_i++;
                if(count_i > 15000)
                {
                        CS1237_SCL_H;
                        CS1237_SDA_H;
                        printf("here outtime error\r\n");
                        return 1;//超时，则直接退出程序
                }
        }
        for(i=0;i<29;i++)// 1 - 29
        {
                One_CLK;
        }
        CS1237_SCL_H;NOP30();CS1237_SDA_H;CS1237_SCL_L;NOP30();//30
        CS1237_SCL_H;NOP30();CS1237_SDA_L;CS1237_SCL_L;NOP30();//31
        CS1237_SCL_H;NOP30();CS1237_SDA_H;CS1237_SCL_L;NOP30();//32
        CS1237_SCL_H;NOP30();CS1237_SDA_L;CS1237_SCL_L;NOP30();//33
        CS1237_SCL_H;NOP30();CS1237_SDA_H;CS1237_SCL_L;NOP30();//34
        CS1237_SCL_H;NOP30();CS1237_SDA_H;CS1237_SCL_L;NOP30();//35
        CS1237_SCL_H;NOP30();CS1237_SDA_L;CS1237_SCL_L;NOP30();//36
        CS1237_SDA_H;
        One_CLK;//37     写入了0x56
        dat=0;
        for(i=0;i<8;i++)// 38 - 45个脉冲了，读取数据
        {
                One_CLK;
                dat <<= 1;
                if(CS1237_SDA_READ ==1)
                        dat++;
        }
        One_CLK;//46个脉冲拉高数据引脚
        return dat;
}
//读取ADC数据，返回的是一个有符号数据
long Read_CS1237(void)
{
        unsigned char i;
        long dat=0;//读取到的数据
        unsigned char count_i=0;//溢出计时器
//        DOUT = 1;//端口锁存1，51必备
        CS1237_SCL_L;//时钟拉低
        while(CS1237_SDA_READ ==1)//芯片准备好数据输出  时钟已经为0，数据也需要等CS1237拉低为0才算都准备好
        {
                delay_500us(10);
                count_i++;
                if(count_i > 300)
                {
                        CS1237_SCL_H;
                        CS1237_SDA_H;
                        printf("Read_CS1237 outtime error\r\n");
                        return 0;//超时，则直接退出程序
                }
        }
//        DOUT = 1;//端口锁存1，51必备
        dat=0;
        for(i=0;i<24;i++)//获取24位有效转换
        {
                CS1237_SCL_H;
                NOP40();
                dat <<= 1;
                if(CS1237_SDA_READ ==1)
                        dat ++;
                CS1237_SCL_L;
                NOP40();        
        }
        for(i=0;i<3;i++)//一共输入27个脉冲
        {
                One_CLK;
        }
        //CS1237_SDA_H;
        //先根据宏定义里面的有效位，丢弃一些数据
        i = 24 - ADC_Bit;//i表示将要丢弃的位数
        dat >>= i;//丢弃多余的位数
        return dat;
}
//初始化ADC相关参数
void Init_CS1237(void)
{
        Con_CS1237();//配置CS1237
        if(Read_CON() != CS_CON)//如果读取的ADC配置出错，则重启
        {
                printf("Init_CS1237 error\r\n");
        }
                //IAP_CONTR = 0x20;
        //AD_Res_Last = Read_CS1237();
        //AD_Res_Last = Read_CS1237();
        //AD_Res_Last = Read_CS1237();
        //printf("AD_Res_Last 0x%x\r\n",AD_Res_Last);
}
//数字一阶滤波器 滤波系数A，小于1。上一次数值B，本次数值C out = b*A + C*(1-A)
//下面这个程序负责读取出最终ADC数据
long Read_12Bit_AD(void)
{
        float out,c;
        
        out = AD_Res_Last;
        c = Read_CS1237();
        if(out !=0 && (out<c*0.8 || out>c*1.2))
          c = out;
        if(c!=0) // 读到正确数据
        {
                out = out*Lv_Bo + c*(1-Lv_Bo);
                AD_Res_Last = out;//把这次的计算结果放到全局变量里面保护
        }
        //printf("AD_Res_Last 0x%x\r\n",AD_Res_Last);
        return AD_Res_Last;
}
//#endif

