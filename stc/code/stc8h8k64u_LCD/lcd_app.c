#include "lcd_driver.h"
#include "lcd_app.h"

#include "sys.h"
uchar code seg_num_tab[]=
{
    0,//1
    2,//2
    4,//3
    13,//4
    15,//5
    17,//6
    19,//7
    27,//8
};//9

uchar code seg_tab2[] =
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

unsigned char disp_flag;
void disp_proc(unsigned char seg_num,unsigned char dat,unsigned char dot_status)
{
    unsigned char tmp;

    tmp = seg_tab2[dat];
    switch(seg_num)
    {
    case 1:
        Ht1621WrOneData(0,tmp>>4);
        Ht1621WrOneData(1,tmp);	      //写数据
        break;
    case 2:
        Ht1621WrOneData(2,tmp>>4);
        Ht1621WrOneData(3,tmp);	      //写数据
        break;
    case 3:
        Ht1621WrOneData(4,tmp>>4);
        Ht1621WrOneData(5,tmp);	      //写数据
        break;
    case 4:
        Ht1621WrOneData(13,tmp>>4);
        Ht1621WrOneData(14,tmp);	      //写数据
        break;
    case 5:
        Ht1621WrOneData(15,tmp>>4);
        Ht1621WrOneData(16,tmp);	      //写数据
        break;
    case 6:
        Ht1621WrOneData(17,tmp>>4);
        Ht1621WrOneData(18,tmp);	      //写数据
        break;
    case 7:
        if(dot_status == ON)
            Ht1621WrOneData(19,(tmp|0x80)>>4);
        else
            Ht1621WrOneData(19,(tmp)>>4);
        Ht1621WrOneData(26,tmp);
        break;
    case 8:
        Ht1621WrOneData(27,tmp>>4);
        Ht1621WrOneData(28,tmp);	      //写数据
        break;
    }

}
void lcd_disp()
{
    static unsigned int tick=0;
    static unsigned int i=0,j;
    unsigned char buf[8];
    if(disp_flag == 1)
    {
//        i=i+1;
//        buf[0] = i/10000000;

//        j=i%10000000;
//        buf[1] = j/1000000;

//        j=i%1000000;
//        buf[2] = j/100000;
//        j=i%100000;
//        buf[3] = j/10000;
//        j=i%10000;
//        buf[4] = j/1000;
//        j=i%1000;
//        buf[5] = j/100;
//        j=i%100;
//        buf[6] = j/10;
//        j=i%10;
//        buf[7] = j/1;

<<<<<<< HEAD
        if(tick>=20)//1s刷新一次，20*50=1000ms
=======
        if(tick>=20)//1s刷新一次
>>>>>>> c72cc78ddfaeba6c56efd87b908343bc1063eea2
        {
            disp_proc(1,0,ON);//1代表数码管1,0位要显示的数字（使用时替换成想要显示的数字就行了），ON代表显示小数点，OFF代表不显示小数点
            disp_proc(2,1,ON);
            disp_proc(3,2,ON);
            disp_proc(4,3,ON);
            disp_proc(5,4,ON);
            disp_proc(6,5,ON);
            disp_proc(7,6,ON);
            disp_proc(8,7,ON);
            i=i+1;
        }
        else
            tick++;


        disp_flag = 0;

    }
}
void TM0_Isr() interrupt 1
{
    static unsigned char tick=0;
    if(tick<50) //50ms定时，外部晶振为11.0592MHZ
        tick++;
    else
    {
        tick = 0;
        disp_flag=1;                                //测试端口
    }

}

