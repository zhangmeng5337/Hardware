//内部时钟 30MHZ
#include<STC15W.h>
#include<intrins.h>
#include<Uart_1.h>
#include<T4_Key.h>
#include<CS1237.h>     //20bit ADC


extern bit Key_1,Key_2,Key_3;//三个按键的状态，全局变量 1表示按下，每次用过之后需要手动置零

//定义一个共用体，用于把四字节的 long 数据发送出去
union
{
	unsigned char a[4];
	signed long   b;
}Read_Data;


void main(void)
{
	P0M1=0;P0M0=0;P1M1=0;P1M0=0;
	P2M1=0;P2M0=0;P3M1=0;P3M0=0;
	P4M1=0;P4M0=0;P5M1=0;P5M0=0;//上电初始化所有IO口为普通IO
	Init_Uart1();//初始化串口1，9600bps
	Init_T4();//初始化定时器T4 
	EA = 1;//打开单片机全局中断
	Con_CS1237();//配置CS1237芯片，这里验证了写时序
	Delay1ms(10);
	while(1)
	{
		
		if(Key_1)//检测到 KEY1 按下
		{
			Key_1 = 0;
			Send_Data1(Read_CON());//发送CS1237的配置数据到串口 ，验证读取时序
		}
		if(Key_2)//检测到 KEY2 按下
		{
			Key_2 = 0;
			Read_Data.b = Read_CS1237();//读取CS1237的数据
			
			Send_Data1(Read_Data.a[0]);//发送读取到的数据到串口，从高8位开始发送
			Send_Data1(Read_Data.a[1]);
			Send_Data1(Read_Data.a[2]);
			Send_Data1(Read_Data.a[3]);
			
		}
		if(Key_3)//检测到 KEY3 按下
		{
			Key_3 = 0;
			Send_Str1("技小新欢迎你！ 3");
		}
		
		
	}
}