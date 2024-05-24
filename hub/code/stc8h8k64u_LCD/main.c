/***********************************
*包括4个按键,LED显示
*3路电压,3路电流输入
*4路开关量输入 4路开关量输出
*创建:xielungui
*日期:2018-04-02 09:28
************************************/

#include "main.h"
#include "com.h"

void init()
{

    P1M0 = 0x03;
    P1M1 = 0x00;
    P3M0 = 0x80;
    P3M1 = 0x00;                             //启动定时器

}
void Timer0_Isr(void) interrupt 1
{
    com_recv_proc();
}

void Timer0_Init(void)      //1微秒@11.0592MHz
{
    AUXR |= 0x80;           //定时器时钟1T模式
    TMOD &= 0xF0;           //设置定时器模式
    TL0 = 0xF5;             //设置定时初始值
    TH0 = 0xFF;             //设置定时初始值
    TF0 = 0;                //清除TF0标志
    TR0 = 1;                //定时器0开始计时
    ET0 = 1;                //使能定时器0中断
}




void INT1_Isr() interrupt 2
{
    unsigned char port_num;

   
    com_recv_proc();
}
void io_init()
{
    P0M0 = 0x00;
    P0M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P5M0 &= ~0x01;
    P5M1 &= ~0x01;
}
void main()
{
    io_init();

    IT1 = 1;                                    //使能INT1下降沿中断
    EX1 = 1;                                    //使能INT1中断

    Timer0_Init();      //1微秒@11.0592MHz
    EA = 1;

    while (1)
    {
        com_tx_to_master();
        com_tx_slaver();
	    P04 = !P04; 								//测试端口

    }
}



