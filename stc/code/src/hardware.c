#include "hardware.h"
#include "tm7780.h"

void mcu_init()
{
    /*********************************************
    			P1.0：为输出模式
    			P3.4：为输出模式
    			P3.5：为输入模式
    			P3.6：为输入模式
    			P3.7：为输出模式

    ***********************************************/
    irq_init();
    P1M0 = 0x01;                                //设置P1.0为输出模式
    P1M1 = 0x00;

    P3M0 = 0x90;                                 //设置P3.4~P3.7为输出模式
    P3M1 = 0x00;
	cd4052_driver(0);


}
void irq_init(void)
{
    P_SW2 = P_SW2 | 0x80;
    P3IM0 = 0x60;//上升沿触发
    P3IM1 = 0x00;
    P3INTE = 0x60;
    //EA = 1;
}

void gpio_Isr() interrupt 13
{
    unsigned char intf;
    intf = P3INTF;
    if (intf)
    {
        if (intf & 0x20) //p3.5
        {
            VI_Interrupt();
        }
        if (intf & 0x40) //p3.6
        {
            P_Interrupt();
        }
    }
}

/*************************************************
00：va1 va2
01：vb1 vb2
10：vc1 vc2

***********************************************/
void cd4052_driver(unsigned char channel)
{
    CD4052_A1 = channel & 0x02;
    CD4052_A0 = channel & 0x01;
}

//void tm7780_measure_ctrl(unsigned char ctrl)
//{
//    if (ctrl == CURRENT_M)
//    {
//        TM7780_SET = 0;  //输出电流有效值
//    }
//    else if (ctrl == VOL_M)
//    {
//        TM7780_SET = 1;//输出电压有效值
//    }
//    else
//        TM7780_SET = 1;
//}

