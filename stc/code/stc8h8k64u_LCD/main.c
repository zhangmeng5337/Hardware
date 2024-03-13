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
	//初始化
		uchar i,j;
//		P1M1=0x00;
//		P1M0=0x00;


//		P3M1=0x00;
//		P3M0=0x00;
//    P1M0 &= ~0x03;
//  	P1M1 &= ~0x03; 
//    P3M0 &= ~0x80; 
//	  P3M1 &= ~0x80; 

  P1M0 = 0x03; 
	P1M1 = 0x00; 
  P3M0 = 0x80; 
	P3M1 = 0x00; 

}

//主函数开始
main() 
{
   init();
   lcd_init();                             //初始化		
	
	while(1)
	{
test2();
		;//disp_proc();
	}
}

