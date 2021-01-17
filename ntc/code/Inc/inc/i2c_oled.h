#ifndef	_I2C_OLED_H_
#define	_I2C_OLED_H_

#include "main.h"

/*#define I2C_OLED_SCL_GPIO_CLK			RCC_APB2Periph_GPIOB
#define I2C_OLED_CLK							RCC_APB1Periph_I2C1

#define I2C_OLED_SCL_GPIO_PIN			GPIO_Pin_6
#define I2C_OLED_SCL_GPIO_PORT		GPIOB

#define I2C_OLED_SDA_GPIO_PIN			GPIO_Pin_7
#define I2C_OLED_SDA_GPIO_PORT		GPIOB

#define I2C_Speed									400000
#define I2C_SLAVE_LEDGEADDR		  	I2C_AcknowledgedAddress_7bit*/
#define I2C_OWN_ADDR							0x3a		
//#define I2C_x									  	I2C1

#define	OLED_ADDRESS							0x78		//0111 1000 最有一位为读写控制为，此处可以随意设置，函数内部会处理
#define I2CT_LONG_TIMEOUT				  ((uint32_t)0x1000)

/*#define OLED_Show_Run()						OLED_DrawBMP(100,0,127,3,BMP_RUN);
#define OLED_Show_STOP()					OLED_DrawBMP(100,0,127,3,BMP_STOP);
#define OLED_Show_Alarm()					OLED_DrawBMP(100,0,127,3,BMP_ALARM);
#define OLED_Show_Hot()						OLED_DrawBMP(79,0,99,3,BMP_HOT);
#define OLED_Show_Cold()					OLED_DrawBMP(79,0,99,3,BMP_COLD);
#define OLED_Show_NO()						OLED_DrawBMP(79,0,99,3,BMP_MODE_NO);
#define OLED_Show_Value(x)				    OLED_ShowData(47,0,x);*/
void OLED_Show_Unit(void);
void OLED_Show_STOP(void);
void OLED_Show_Alarm(void);
void OLED_Show_Hot(void)	;					
void OLED_Show_Cold(void);					
void OLED_Show_NO(void)	;					
void OLED_Show_Value(uint16_t x);				    
void OLED_Show_Gear(uint16_t x);


void OLED_GPIO_Config(void);						//I2C配置
void OLED_Init(void);										//OLED初始化
uint8_t I2C_OLED_WriteByte(uint8_t control,uint8_t data);
void Delay_s(uint32_t time);
void OLED_Fill(unsigned char fill_Data);//全屏填充
void WriteData(uint8_t I2C_Data);				//写数据
void WriteCmd(uint8_t I2C_Command);			//写命令
void OLED_SetStartPoint(unsigned char x,unsigned char y);		//设置起点
void OLED_ON(void);											//唤醒
void OLED_OFF(void);										//休眠
void OLED_Clear(void);									//清屏
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_ShowStr(unsigned char x, unsigned char y, char ch[], unsigned char TextSize);
void OLED_ShowData(uint8_t x,uint8_t y,uint16_t data);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void oled_test(void);
void oled_process(unsigned char num);

#endif
