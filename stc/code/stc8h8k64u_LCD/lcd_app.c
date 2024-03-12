#include "lcd_driver.h"
#include "lcd_app.h"

#include "sys.h"
uchar code tab[10] =
{
    // ������ʾ0-9���κ���ʾ���ֵ����򣬶��ǵ������������
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
        Write_dat(0x00, i);                   //RAM����

}
void lcd_init(void)
{
	unsigned char i;
    RD = 1;
    delay_ms(100);
    Write_cmd(0x01);                    //����ϵͳʱ��
    Write_cmd(0X18);                    //�ڲ�RC��
//  Write_cmd(0X14);                    //������
////    Write_cmd(0X07);          //����WDT��ͣ��־���
////    Write_cmd(0X88);          //����IRQ���
////    Write_cmd(0X60);          //����Ƶ�ʣ�2KHz
    Write_cmd(0x29);                    //1/3ƫ�ã�4COM
    for (i = 0; i < 32; i++)
        Write_dat(0x00, i);                   //RAM����
    Write_cmd(0x03);                    //����ʾ
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
                Write_dat(tab[dat] << 4, tmp);      //д����
                Write_dat(tab[dat], tmp + 1);  //д����
                Write_dat(0x01, tmp + 2);  //д����\
            }

        }
        else
        {
            Write_dat(tab[dat], tmp);       //д����
            Write_dat(tab[dat] << 4, tmp + 1);  //д����\
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
	disp_dat(1, 0,OFF);//1�����һ������ܣ�0ΪҪ��ʾ�����ݣ�OFF����С����ر�
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

