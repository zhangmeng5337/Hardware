/***********************************
*����6������, 12864LCD��ʾ
*1·ttlͨ��9600
*������xielungui
*���ڣ�2018-10-28 10:28
************************************/
#include "stc8h.h"       //������ͷ�ļ��󣬲���Ҫ�ٰ���"reg51.h"ͷ�ļ�
#include "hardware.h"

//#include "reg51.h"
#include "intrins.h"
#include "stdio.h"
#include <math.h>




//��������ʼ
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
