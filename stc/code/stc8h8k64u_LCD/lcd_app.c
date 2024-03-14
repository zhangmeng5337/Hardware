#include "lcd_driver.h"
#include "lcd_app.h"

#include "sys.h"
uchar code seg_num_tab[]={
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
//0,1,//1
//2,3,//2
//4,5,//3
//13,14,//4
//15,16,//5
//17,18,//6
//19,26,//7
//27,28,//8
//29,30};//9

//void lcd_clear(void)
//{ //0-5 13-19 26-30
//    unsigned char i;
//    for (i = 0; i < 32; i++)
//        Write_dat2(0x00, i);                   //RAM清零

//}
//void lcd_init(void)
//{
//	unsigned char i;
//  //  RD = 1;
//    delay_ms(100);
//    Write_cmd(0x01);                    //开启系统时钟
//    Write_cmd(0X18);                    //内部RC震荡
////  Write_cmd(0X14);                    //晶体振荡
//////    Write_cmd(0X07);          //允许WDT暂停标志输出
//////    Write_cmd(0X88);          //允许IRQ输出
//////    Write_cmd(0X60);          //蜂鸣频率：2KHz
//    Write_cmd(0x29);                    //1/3偏置，4COM
//    for (i = 0; i < 32; i++)
//	 {
//		Write_dat2(0,i);		//写数据
//	 }
//    Write_cmd(0x03);                    //开显示
//}


void disp_dat(unsigned char seg_num, unsigned char dat,
              unsigned char dot_status)
{
    unsigned char i;
    unsigned char tmp;
    if (seg_num > 0)
    {
        if (dot_status == ON)
        {
					
					  tmp = seg_tab2[dat];

            if(seg_num == 7)
						{
	              Ht1621WrOneData(seg_num_tab[seg_num-1],(tmp|0x80)>>4);
	              Ht1621WrOneData(26,tmp);
						}
						else
						{
	             Ht1621WrOneData(seg_num_tab[seg_num-1],tmp>>4);
	             Ht1621WrOneData(seg_num_tab[seg_num],tmp);				
						}
        }
        else
        {
					  tmp = seg_tab2[dat];

            if(seg_num == 7)
						{

	              Ht1621WrOneData(seg_num_tab[seg_num-1],tmp>>4);
	              Ht1621WrOneData(26,tmp);	
						}
						else
						{
	             Ht1621WrOneData(seg_num_tab[seg_num-1],tmp>>4);
	             Ht1621WrOneData(seg_num_tab[seg_num],tmp);	      //写数据						
						}
        }
    }
}



//test2()
//{
////	static unsigned char tick_disp=0,i=1;
////	if(tick_disp <=20)
////		tick_disp++;
////	else 
////	{
////		tick_disp = 0;
////		i ++;
////		if(i>=8)
////			i=1;
////	}
// disp_dat(1, 1,OFF);
// disp_dat(2, 1,OFF);
// disp_dat(3, 1,OFF);
// disp_dat(4, 1,OFF);
// disp_dat(5, 1,OFF);
// disp_dat(6, 1,OFF);
//// disp_dat(7, 1,OFF);
//// disp_dat(8, 1,OFF);	
//	
//	 //Write_dat2(tmp|0x80, seg_tab[seg_num-1]);      //写数据
//   //Write_dat2(tmp<<4, 26);  //写数据
//	
//}
void disp_proc()
{ 
	unsigned char i,j;
	for(i=0;i<10;i++)
	{	
		for(j=1;j<9;j++)
		{
			disp_dat(j, i,OFF);
		}	
	  DelayMS(5000);
	}
	
	
//	disp_dat(1, 1,OFF);
//	
//	disp_dat(2, 1,OFF);
//	
//	disp_dat(2, 1,OFF);

//	
//	disp_dat(1, 1,OFF);
//	DelayMS(5000);
//	
//	disp_dat(1, 1,OFF);
//	DelayMS(5000);
//	
//	disp_dat(1, 1,OFF);
//	DelayMS(5000);
//	
//	disp_dat(1, 1,OFF);
//	DelayMS(5000);
//	
//	disp_dat(1, 1,OFF);
//	DelayMS(5000);
}
