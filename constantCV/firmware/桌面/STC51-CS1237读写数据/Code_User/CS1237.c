#include<CS1237.h>     //20bit ADC
//网址：WWW.JIXIN.PRO


#define ADC_Bit  20 //ADC有效位数，带符号位 最高24位
#define SCK_1  SCLK = 1
#define SCK_0  SCLK = 0
#define DAT_1  DOUT = 1
#define DAT_0  DOUT = 0
#define	NOP_5()		_nop_();_nop_()
#define	NOP30()		NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();
#define	NOP40()		NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();
#define One_CLK  SCK_1;NOP40();SCK_0;NOP40();
#define CS_CON  0X0C   //芯片地配置 内部REF 输出40HZ PGA=128 通道A 0X1C   

sbit DOUT = P3^7;//数据对应IO口
sbit SCLK = P3^5;//时钟对应IO口

//延时500US 25MHZ
void delay_500us(unsigned char a)
{	
	unsigned char i,j,b;
	for(b=0;b<a;b++)
	{
		i = 13;
		j = 37;
		do
		{
			while (--j);
		} while (--i);
	}
}
//配置CS1237芯片
void Con_CS1237(void)
{
	unsigned char i;
	unsigned char dat;
	unsigned char count_i=0;//溢出计时器
	dat = CS_CON;// 0100 1000
	SCK_0;//时钟拉低
	while(DOUT)//芯片准备好数据输出  时钟已经为0，数据也需要等CS1237全部拉低为0才算都准备好
	{
		delay_500us(10);
		count_i++;
		if(count_i > 150)
		{
			SCK_1;
			DAT_1;
			return;//超时，则直接退出程序
		}
	}
	for(i=0;i<29;i++)// 1 - 29
	{
		One_CLK;
	}
	SCK_1;NOP30();DAT_1;SCK_0;NOP30();//30
	SCK_1;NOP30();DAT_1;SCK_0;NOP30();//31
	SCK_1;NOP30();DAT_0;SCK_0;NOP30();//32
	SCK_1;NOP30();DAT_0;SCK_0;NOP30();//33
	SCK_1;NOP30();DAT_1;SCK_0;NOP30();//34
	SCK_1;NOP30();DAT_0;SCK_0;NOP30();//35
	SCK_1;NOP30();DAT_1;SCK_0;NOP30();//36
	One_CLK;//37     写入了0x65
	for(i=0;i<8;i++)// 38 - 45个脉冲了，写8位数据
	{
		SCK_1;NOP40();
		if(dat&0x80)
			DAT_1;
		else
			DAT_0;
		dat <<= 1;
		SCK_0;NOP40();
	}
	One_CLK;//46个脉冲拉高数据引脚
}
//读取芯片的配置数据
unsigned char Read_CON(void)
{
	unsigned char i;
	unsigned char dat=0;//读取到的数据
	unsigned char count_i=0;//溢出计时器
	unsigned char k=0,j=0;//中间变量
	SCK_0;//时钟拉低
	while(DOUT)//芯片准备好数据输出  时钟已经为0，数据也需要等CS1237全部拉低为0才算都准备好
	{
		delay_500us(10);
		count_i++;
		if(count_i > 150)
		{
			SCK_1;
			DAT_1;
			return 1;//超时，则直接退出程序
		}
	}
	for(i=0;i<29;i++)// 1 - 29
	{
		One_CLK;
	}
	SCK_1;NOP30();DAT_1;SCK_0;NOP30();//30
	SCK_1;NOP30();DAT_0;SCK_0;NOP30();//31
	SCK_1;NOP30();DAT_1;SCK_0;NOP30();//32
	SCK_1;NOP30();DAT_0;SCK_0;NOP30();//33
	SCK_1;NOP30();DAT_1;SCK_0;NOP30();//34
	SCK_1;NOP30();DAT_1;SCK_0;NOP30();//35
	SCK_1;NOP30();DAT_0;SCK_0;NOP30();//36
	DAT_1;
	One_CLK;//37     写入了0x56
	dat=0;
	for(i=0;i<8;i++)// 38 - 45个脉冲了，读取数据
	{
		One_CLK;
		dat <<= 1;
		if(DOUT)
			dat++;
	}
	One_CLK;//46个脉冲拉高数据引脚
	return dat;
}
//读取ADC数据，返回的是一个有符号数据
long Read_CS1237(void)
{
	unsigned char i;
	long dat=0;//读取到的数据
	unsigned char count_i=0;//溢出计时器
	DOUT = 1;//端口锁存1，51必备
	SCK_0;//时钟拉低
	while(DOUT)//芯片准备好数据输出  时钟已经为0，数据也需要等CS1237拉低为0才算都准备好
	{
		delay_500us(10);
		count_i++;
		if(count_i > 300)
		{
			SCK_1;
			DAT_1;
			return 0;//超时，则直接退出程序
		}
	}
	DOUT = 1;//端口锁存1，51必备
	dat=0;
	for(i=0;i<24;i++)//获取24位有效转换
	{
		SCK_1;
		NOP40();
		dat <<= 1;
		if(DOUT)
			dat ++;
		SCK_0;
		NOP40();	
	}
	for(i=0;i<3;i++)//一共输入27个脉冲
	{
		One_CLK;
	}
	DAT_1;
	//先根据宏定义里面的有效位，丢弃一些数据
	i = 24 - ADC_Bit;//i表示将要丢弃的位数
	dat >>= i;//丢弃多余的位数
	return dat;
}
//
