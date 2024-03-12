#include "lcd_driver.h"
#include "lcd_app.h"

#include "sys.h"
uchar code tab[10] =
{
    // 数字显示0-9，任何显示数字的区域，都是调用这里的数据
    0Xf5, // 0
    0X60, // 1
    0Xb6, // 2
    0XF2, // 3
    0X63, // 4
    0XD3, // 5
    0XD7, // 6
    0X70, // 7
    0XF7, // 8
    0XF3  // 9
};
void lcd_clear(void)
{
    unsigned char i;
    for (i = 0; i < 32; i++)
        Write_dat(0x00, i);                   //RAM清零

}
void lcd_init(void)
{
	unsigned char i;
    RD = 1;
    delay_ms(100);
    Write_cmd(0x01);                    //开启系统时钟
    Write_cmd(0X18);                    //内部RC震荡
//  Write_cmd(0X14);                    //晶体振荡
////    Write_cmd(0X07);          //允许WDT暂停标志输出
////    Write_cmd(0X88);          //允许IRQ输出
////    Write_cmd(0X60);          //蜂鸣频率：2KHz
    Write_cmd(0x29);                    //1/3偏置，4COM
    for (i = 0; i < 32; i++)
        Write_dat(0x00, i);                   //RAM清零
    Write_cmd(0x03);                    //开显示
}
void disp_dat(unsigned char seg_num, unsigned char dat,
              unsigned char dot_status)
{
    unsigned char i;
    unsigned char tmp;
    if (seg_num > 0)
    {


        tmp = seg_num - 1; //h4
        tmp = tmp * 2;      //l4
        if (dot_status == ON)
        {
            if (seg_num == 6)
            {
                Write_dat(tab[dat] << 4, tmp);      //写数据
                Write_dat(tab[dat], tmp + 1);  //写数据
                Write_dat(0x01, tmp + 2);  //写数据\
            }

        }
        else
        {
            Write_dat(tab[dat], tmp);       //写数据
            Write_dat(tab[dat] << 4, tmp + 1);  //写数据\
        }


        for (i = 0; i < ((seg_num - 1) * 2); i++)
        {
            Write_dat(0x00, i);
        }
        for (i = (seg_num * 2); i < 16; i++)
        {
            Write_dat(0x00, i);
        }

    }
}
}

}
void disp_proc(void)
{
	disp_dat(1, 0,OFF);//1代表第一个数码管，0为要显示的数据，OFF代表小数点关闭
	delay_ms(1000);
	
	disp_dat(2, 1,OFF);
	delay_ms(1000);
	
	disp_dat(3, 2,OFF);
	delay_ms(1000);
	
	disp_dat(4, 3,OFF);
	delay_ms(1000);
	
	disp_dat(5, 4,OFF);
	delay_ms(1000);

	
	disp_dat(6, 5,ON);
	delay_ms(1000);
	
	disp_dat(7, 6,OFF);
	delay_ms(1000);
	
	disp_dat(8, 7,OFF);
	delay_ms(1000);

}

