/***********************************
*包括6个按键, 12864LCD显示
*1路ttl通信9600
*创建：xielungui
*日期：2018-10-28 10:28
************************************/
#include "stc8h.h"       //包含此头文件后，不需要再包含"reg51.h"头文件
#include "hardware.h"

//#include "reg51.h"
#include "intrins.h"
#include "stdio.h"
#include <math.h>




//主函数开始
main()
{
		P3M0 = 0x00;
    P3M1 = 0x00;

    while(1)
    {
        display(1,8,1);
        Delay200ms();	//@11.0592MHz
        display(1,2,2);
        Delay200ms();	//@11.0592MHz
        display(1,3,3);
        Delay200ms();	//@11.0592MHz
    }
}
