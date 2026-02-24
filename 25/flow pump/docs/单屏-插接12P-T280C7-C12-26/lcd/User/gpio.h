#ifndef __GPIO_H
#define __GPIO_H

#define Port					GPIOA																				//定义iic总线所在的端口
#define Port_Clock    RCC_APB2Periph_GPIOA												//定义iic总线所在的端口的时钟

#define CS						GPIO_Pin_0																	//定义显示屏的片选引脚
#define AS						GPIO_Pin_1																	//定义显示屏指令/数据选择引脚
#define RST						GPIO_Pin_2																	//定义显示屏复位引脚
#define SCL 					GPIO_Pin_3																	//定义数据总线时钟引脚
#define SDA 					GPIO_Pin_4																	//定义数据总线数据引脚

#define CS_1()  			GPIO_SetBits(Port,CS)												//片选至 1
#define CS_0()  			GPIO_ResetBits(Port,CS)											//片选至 0

#define AS_1()  			GPIO_SetBits(Port,AS)												//指令/数据至 1
#define AS_0()  			GPIO_ResetBits(Port,AS)											//指令/数据至 0

#define RST_1()  			GPIO_SetBits(Port,RST)											//复位至 1
#define RST_0()  			GPIO_ResetBits(Port,RST)										//复位至 0

#define SCL_1()  			GPIO_SetBits(Port,SCL)											//时钟至 1
#define SCL_0()  			GPIO_ResetBits(Port,SCL)										//时钟至 0

#define SDA_1()				GPIO_SetBits(Port,SDA)											//数据至 1
#define SDA_0()				GPIO_ResetBits(Port,SDA)										//数据至 0


void Gpio_Init(void);																							//初始化引脚函数

#endif

