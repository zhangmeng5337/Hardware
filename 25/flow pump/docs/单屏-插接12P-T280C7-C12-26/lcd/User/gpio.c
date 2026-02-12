#include"stm32f10x.h"															//包含芯片寄存器头文件
#include"gpio.h"																	//包含引脚宏定义头文件

/*初始化引脚函数*/
void Gpio_Init(void)
{
	GPIO_InitTypeDef iic_pin;												//用于初始化IIC接口引脚
	/*开启端口A的时钟*/
	RCC_APB2PeriphClockCmd(Port_Clock,ENABLE);			//使能IIC所在端口的时钟信号
	/*设置工作模式参数*/
	iic_pin.GPIO_Pin=(CS|AS|RST|SCL|SDA);						//初始化显示屏的相关引脚
	iic_pin.GPIO_Mode=GPIO_Mode_Out_PP;							//将引脚设置为“通用推挽输出”模式
	iic_pin.GPIO_Speed=GPIO_Speed_50MHz;						//设定引脚工作速度最高50MHz
	/*写入引脚设置参数*/
	GPIO_Init(Port,&iic_pin);												//写入引脚设置参数
	/*初始化完毕后设置为高电平*/
	CS_1();																					//片选=1
	AS_1();																					//指令数据=1
	RST_1();																				//复位=1
	SCL_1();																				//时钟=1
	SDA_1();																				//数据=1
}


