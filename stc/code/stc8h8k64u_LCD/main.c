/***********************************
*包括4个按键,LED显示
*3路电压,3路电流输入
*4路开关量输入 4路开关量输出
*创建:xielungui
*日期:2018-04-02 09:28
************************************/

#include "main.h"
#include "lcd_app.h"
#include "lcd_driver.h"
void init()
{

    P1M0 = 0x03;
    P1M1 = 0x00;
    P3M0 = 0x80;
    P3M1 = 0x00;                             //启动定时器

}
void Timer0_Init(void)		//1毫秒@11.0592MHz
{
    AUXR |= 0x80;			//定时器时钟1T模式
    TMOD &= 0xF0;			//设置定时器模式
    TL0 = 0xCD;				//设置定时初始值
    TH0 = 0xD4;				//设置定时初始值
    TF0 = 0;				//清除TF0标志
    TR0 = 1;				//定时器0开始计时
    ET0 = 1;                                    //使能定时器中断
    EA = 1;
}


//主函数开始
main()
{
    init();

    Ht1621_Init();                             //初始化
    Timer0_Init();
    //	test2();
    while(1)
    {

        lcd_disp();

    }
}

