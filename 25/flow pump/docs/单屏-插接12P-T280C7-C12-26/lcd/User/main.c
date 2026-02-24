/********** 深圳市金逸晨电子有限公司 *************************************
********* ShenZhen GoldenMorning Electronics Co., Ltd. ****************
******************************************************************************
//  文 件 名   : main.c
//  版 本 号   : v1.0
//  作    者   : zy
//  生成日期   : 2025-08-01
//  模组类型   : 深超2.8IPS ZH028IA-01A + ST7789	
//  分 辨 率   : 240*320
//  功能描述   : 4线SPI 演示例程(STM32F103系列)
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   3.3v电源
//              SCL   PB5
//              SDA   PB6
//              RST   PB7
//              DC    PB8
//              CS    PB9
//              ----------------------------------------------------------------
// Copyright(C) 金逸晨电子
******************************************************************************/
#include "stm32f10x.h"
#include "led.h"
#include "key.h"  
#include "bmp.h"  

#define USE_HORIZONTAL  1

#define TFT_COLUMN_NUMBER 240
#define TFT_LINE_NUMBER 320

#define TFT_COLUMN_OFFSET 0
#define TFT_LINE_OFFSET 0

/**********SPI引脚分配，连接TFT屏，更具实际情况修改*********/


//#define TFT_SCK_0  GPIOB->BRR=0X0020       // 设置sck接口到PB5
//#define TFT_SCK_1  GPIOB->BSRR=0X0020       //

//#define TFT_SDA_0  GPIOB->BRR=0X0040       // 设置SDA接口到PB6
//#define TFT_SDA_1  GPIOB->BSRR=0X0040

//#define TFT_RST_0  GPIOB->BRR=0X0080       // 设置DC接口到PB7
//#define TFT_RST_1  GPIOB->BSRR=0X0080

//#define TFT_DC_0   GPIOB->BRR=0X0100      // 设置CS接口到PB8
//#define TFT_DC_1   GPIOB->BSRR=0X0100

//#define TFT_CS_0   GPIOB->BRR=0X0200    // 设置RST接口到PB9
//#define TFT_CS_1   GPIOB->BSRR=0X0200



#define TFT_SCK_0  GPIOB->BRR=0X0020       // 设置sck接口到PB5
#define TFT_SCK_1  GPIOB->BSRR=0X0020       //

#define TFT_SDA_0  GPIOB->BRR=0X0040       // 设置SDA接口到PB6
#define TFT_SDA_1  GPIOB->BSRR=0X0040

#define TFT_RST_0  GPIOB->BRR=0X0080       // 设置DC接口到PB7
#define TFT_RST_1  GPIOB->BSRR=0X0080

#define TFT_DC_0   GPIOB->BRR=0X0100      // 设置CS接口到PB8
#define TFT_DC_1   GPIOB->BSRR=0X0100

#define TFT_CS_0   GPIOB->BRR=0X0200    // 设置RST接口到PB9
#define TFT_CS_1   GPIOB->BSRR=0X0200

#define TFT_BL_0  GPIOB->BRR=0x0010       //PB4
#define TFT_BL_1  GPIOB->BSRR=0x0010

/*
#define TFT_BL_0  GPIOB->BRR=0x0010       //PB4
#define TFT_BL_1  GPIOB->BSRR=0x0010

#define TFT_CS_0  GPIOB->BRR=0X0020      //PB5
#define TFT_CS_1  GPIOB->BSRR=0X0020

#define TFT_DC_0  GPIOB->BRR=0X0040      //PB6
#define TFT_DC_1  GPIOB->BSRR=0X0040 

#define TFT_RST_0  GPIOB->BRR=0x0080      //PB7
#define TFT_RST_1  GPIOB->BSRR=0x0080

#define TFT_SDA_0   GPIOB->BRR=0X0100      //PB8
#define TFT_SDA_1   GPIOB->BSRR=0X0100

#define TFT_SCK_0   GPIOB->BRR=0X0200      //PB9
#define TFT_SCK_1   GPIOB->BSRR=0X0200
*/


/*****************颜色数据**************************/
#define WHITE            0xFFFF
#define BLACK            0x0000   
#define BLUE             0x001F  
#define BRED             0XF81F
#define GRED             0XFFE0
#define GBLUE            0X07FF
#define RED              0xF800
#define GREEN            0x07E0

unsigned int page = 0;
unsigned int autoplay = 0;

/*******时钟初始化******************/
void SYS_init(unsigned char PLL)
{
	RCC->APB1RSTR = 0x00000000;//复位结束			 
	RCC->APB2RSTR = 0x00000000; 	  
	RCC->AHBENR = 0x00000014;  //睡眠模式闪存和SRAM时钟使能.其他关闭.	  
	RCC->APB2ENR = 0x00000000; //外设时钟关闭.			   
	RCC->APB1ENR = 0x00000000;   
	RCC->CR |= 0x00000001;     //使能内部高速时钟HSION
	
	RCC->CFGR &= 0xF8FF0000;   //复位SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
	RCC->CR &= 0xFEF6FFFF;     //复位HSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;     //复位HSEBYP	   	  
	RCC->CFGR &= 0xFF80FFFF;   //复位PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE
	while(((RCC->CFGR>>2)& 0x03 )!=0x00); 	
	RCC->CIR = 0x00000000;     //关闭所有中断		 
	//配置向量表				  
  
//	SCB->VTOR = 0x08000000|(0x0 & (u32)0x1FFFFF80);//设置NVIC的向量表偏移寄存器

 	RCC->CR|=0x00010000;  //外部高速时钟使能HSEON
	while(((RCC->CR>>17)&0x00000001)==0);//等待外部时钟就绪
	RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
	PLL-=2;//抵消2个单位
	RCC->CFGR|=PLL<<18;   //设置PLL值 2~16
	RCC->CFGR|=1<<16;	  //PLLSRC ON 
	

	RCC->CR|=0x01000000;  //PLLON
	while(!(RCC->CR>>25));//等待PLL锁定
	RCC->CFGR|=0x00000002;//PLL作为系统时钟	 
	while(((RCC->CFGR>>2)&0x03)!=0x02);   //等待PLL作为系统时钟设置成功
}

void IO_init(void)
{
	/*
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟 
	RCC->APB2ENR|=1<<4;    //使能PORTC时钟

	GPIOB->CRL	&=  0X00000000;				//将B34567口配置为通用推挽输出,最大50MH
	GPIOB->CRL	|=	0X33300000;				
	GPIOB->CRH	&=	0X00000000;				//将PB口配置为通用推挽输出,最大50MH
	GPIOB->CRH	|=	0X00033333;
	GPIOB->ODR	=0XFFFF;
	*/
	RCC->APB2ENR|=1<<2;    //??PORTA??
	GPIOA->CRL&=0X00000000;				//?B567??????????,??50MH
	GPIOA->CRL|=0X33333333;				
	GPIOA->CRH&=0X00000000;				//?B89??????????,??50MH
	GPIOA->CRH|=0X33333333;
	GPIOA->ODR=0XFFFF;

	RCC->APB2ENR|=1<<3;    //??PORTB?? 
	GPIOB->CRL&=0X00000000;				//?B567??????????,??50MH
	GPIOB->CRL|=0X33333333;				
	GPIOB->CRH&=0X00000000;				//?B89??????????,??50MH
	GPIOB->CRH|=0X33333333;
	GPIOB->ODR=0XFFFF;

	RCC->APB2ENR|=1<<4;    //??PORTC?? 
	GPIOC->CRL&=0X00000000;				//?B567??????????,??50MH
	GPIOC->CRL|=0X33333333;				
	GPIOC->CRH&=0X00000000;				//?B89??????????,??50MH
	GPIOC->CRH|=0X33333333;
	GPIOC->ODR=0XFFFF;
}

void delay_us(unsigned int _us_time)
{       
	unsigned char x=0;
	for(;_us_time>0;_us_time--)
	{
		x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;
	}
}

void delay_ms(unsigned int _ms_time)
{
	unsigned int x, i, j;
	for(i=0;i<_ms_time;i++)
	{
		for(j=0;j<1000;j++)
		{
			x++;//x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;
		}
	}
}

/*************SPI配置函数*******************
，SCL空闲时低电平，第一个上升沿采样
模拟SPI
******************************************/

/**************************SPI模块发送函数************************************************

 *************************************************************************/
void SPI_SendByte(unsigned char byte)
{
	unsigned char counter;

	for(counter=0;counter<8;counter++)
	{ 
		TFT_SCK_0;
	  
		if((byte&0x80)==0)
		{
			TFT_SDA_0;
		}else{
			TFT_SDA_1;
		}
	  
		byte=byte<<1;
	
		TFT_SCK_1;	
	} 
	TFT_SCK_0;
}

void TFT_SEND_CMD(unsigned char o_command)
{
  TFT_CS_0;
	TFT_DC_0;
	SPI_SendByte(o_command);
  TFT_CS_1;
	//TFT_DC_1;
}

void TFT_SEND_DATA(unsigned char o_data)
{ 
	TFT_CS_0;
	TFT_DC_1;
	SPI_SendByte(o_data);
	TFT_CS_1;
}

void TFT_SET_ADD(unsigned short int X,unsigned short int Y,unsigned short int X_END,unsigned short int Y_END)
{	
	TFT_SEND_CMD(0x2A);     //窗口地址横向                   
    TFT_SEND_DATA(X>>8);     
    TFT_SEND_DATA(X);//start column
	TFT_SEND_DATA(X_END>>8);     
    TFT_SEND_DATA(X_END);//start column
	TFT_SEND_CMD(0x2B);     //窗口地址纵向                   
    TFT_SEND_DATA(Y>>8);     
    TFT_SEND_DATA(Y);		//start row
    TFT_SEND_DATA(Y_END>>8);     
    TFT_SEND_DATA(Y_END);		//start row
    TFT_SEND_CMD(0x2C);     //写入数据   
}

void TFT_init(void)	//ST7789
{
	TFT_RST_0;//复位 
	delay_ms(100); 
	TFT_RST_1; 
	delay_ms(100); 

TFT_SEND_CMD(0xB2);     
TFT_SEND_DATA(0x0C);   
TFT_SEND_DATA(0x0C);   
TFT_SEND_DATA(0x00);   
TFT_SEND_DATA(0x33);   
TFT_SEND_DATA(0x33);   

//TFT_SEND_CMD(0x35);     
//TFT_SEND_DATA(0x00);   

TFT_SEND_CMD(0x36);     
TFT_SEND_DATA(0x00);   

TFT_SEND_CMD(0x3A);     
TFT_SEND_DATA(0x55); //06  

TFT_SEND_CMD(0xB7);     
TFT_SEND_DATA(0x55);   

TFT_SEND_CMD(0xBB);     
TFT_SEND_DATA(0x1A);   

TFT_SEND_CMD(0xC0);     
TFT_SEND_DATA(0x2C);   

TFT_SEND_CMD(0xC2);     
TFT_SEND_DATA(0x01);   

TFT_SEND_CMD(0xC3);     
TFT_SEND_DATA(0x19);   

TFT_SEND_CMD(0xC6);     
TFT_SEND_DATA(0x0F);   

TFT_SEND_CMD(0xD0);     
TFT_SEND_DATA(0xA7);   

TFT_SEND_CMD(0xD0);     
TFT_SEND_DATA(0xA4);   
TFT_SEND_DATA(0xA1);   

TFT_SEND_CMD(0xD6);     
TFT_SEND_DATA(0xA1);   

TFT_SEND_CMD(0xE0);
TFT_SEND_DATA(0xF0);
TFT_SEND_DATA(0x03);
TFT_SEND_DATA(0x09);
TFT_SEND_DATA(0x0B);
TFT_SEND_DATA(0x0A);
TFT_SEND_DATA(0x16);
TFT_SEND_DATA(0x2B);
TFT_SEND_DATA(0x33);
TFT_SEND_DATA(0x41);
TFT_SEND_DATA(0x38);
TFT_SEND_DATA(0x14);
TFT_SEND_DATA(0x14);
TFT_SEND_DATA(0x29);
TFT_SEND_DATA(0x2F);

TFT_SEND_CMD(0xE1);
TFT_SEND_DATA(0xF0);
TFT_SEND_DATA(0x04);
TFT_SEND_DATA(0x06);
TFT_SEND_DATA(0x09);
TFT_SEND_DATA(0x08);
TFT_SEND_DATA(0x04);
TFT_SEND_DATA(0x2B);
TFT_SEND_DATA(0x32);
TFT_SEND_DATA(0x41);
TFT_SEND_DATA(0x36);
TFT_SEND_DATA(0x12);
TFT_SEND_DATA(0x12);
TFT_SEND_DATA(0x2A);
TFT_SEND_DATA(0x30);

TFT_SEND_CMD(0x21); 

   
TFT_SEND_CMD(0x2A);     //Column Address Set
TFT_SEND_DATA(0x00);   
TFT_SEND_DATA(0x00);   //0
TFT_SEND_DATA(0x00);   
TFT_SEND_DATA(0xEF);   //239

TFT_SEND_CMD(0x2B);     //Row Address Set
TFT_SEND_DATA(0x00);   
TFT_SEND_DATA(0x00);   //0
TFT_SEND_DATA(0x01);   
TFT_SEND_DATA(0x3F);   //319

TFT_SEND_CMD(0x11);     
delay_ms(120); 
TFT_SEND_CMD(0x29);  
TFT_SEND_CMD(0x2C);     
      
	
}




void TFT_clear()
{
	 TFT_SET_ADD(0,0,TFT_COLUMN_NUMBER-1,TFT_LINE_NUMBER-1);
   for(int i=0;i<76800;i++)             //Line loop
   { 
			TFT_SEND_DATA(0xF);
			TFT_SEND_DATA(0xF);
	 }
}
	
	
void TFT_full(unsigned int color)
  {
    unsigned long Line,column;
	  TFT_SET_ADD(0,0,TFT_COLUMN_NUMBER-1,TFT_LINE_NUMBER-1);
    for(Line=0;Line<TFT_LINE_NUMBER;Line++)             //Line loop
      { 
		  
		for(column=0;column<TFT_COLUMN_NUMBER;column++)	//column loop
          {
              TFT_SEND_DATA(color>>8);
			  TFT_SEND_DATA(color);
          }
      }
  }

	
void TFT_Fill(u16 x,u16 y,u16 length,u16 width,unsigned int color)
{
	u16 i,j;
	TFT_SET_ADD(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			TFT_SEND_DATA(color>>8);
			TFT_SEND_DATA(color);
		}
	}			
}

void Picture_display(u16 x,u16 y,u16 length,u16 width,const unsigned char *ptr_pic)
{
	u16 i,j;
	TFT_SET_ADD(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
      TFT_SEND_DATA(*ptr_pic++);
			TFT_SEND_DATA(*ptr_pic++);
		}
	}			
}



void Show_L127(u16 x,u16 y,u16 length,u16 width,const unsigned char *ptr_pic)
{
	u16 i,j,no;
	TFT_SET_ADD(x,y,x+length-1,y+width-1);
	for(i=0;i<320;i++)
	{
		no = 0;
		for(j=0;j<240;j++)
		{
      TFT_SEND_DATA(ptr_pic[no+1]);
      TFT_SEND_DATA(ptr_pic[no]);
			no = no + 2;
		}
	}			
}



void TFT_Page_bar()
{
	int col = TFT_COLUMN_NUMBER;
	int row = TFT_LINE_NUMBER;
	
	int c = 0;
	int r = 0;

	int c2 = 0;
	//int r2 = 0;

	int ex = 0;

	for(c=0;c<col;c++)
	{
		for(r=0;r<row;r++)
		{
			c2 = (int)(c/30);
			//r2 = (int)(r/30);
			
			ex = c2 % 8;
			//ex = r2 % 8;
			
			if(ex == 0){
				TFT_SEND_DATA(0xFF);
				TFT_SEND_DATA(0xFF);
			}else if(ex == 1){
				TFT_SEND_DATA(0x00);
				TFT_SEND_DATA(0x00);
			}else if(ex == 2){
				TFT_SEND_DATA(0x00);
				TFT_SEND_DATA(0x1F);
			}else if(ex == 3){
				TFT_SEND_DATA(0xF8);
				TFT_SEND_DATA(0x1F);
			}else if(ex == 4){
				TFT_SEND_DATA(0xFF);
				TFT_SEND_DATA(0xE0);
			}else if(ex == 5){
				TFT_SEND_DATA(0x07);
				TFT_SEND_DATA(0xFF);
			}else if(ex == 6){
				TFT_SEND_DATA(0xF8);
				TFT_SEND_DATA(0x00);
			}else if(ex == 7){
				TFT_SEND_DATA(0x07);
				TFT_SEND_DATA(0xE0);
			}
			//delay_ms(10);
		}
	}
}


void TFT_Page_border()
{
	int col = TFT_COLUMN_NUMBER;
	int row = TFT_LINE_NUMBER;
	
	int c, r;

  TFT_SET_ADD(0,0,TFT_COLUMN_NUMBER-1,TFT_LINE_NUMBER-1);
	
	
	for(r=0;r<row;r++)
	{
		for(c=0;c<col;c++)
		{
			if(c==0 || r==0 || c == col-1 || r == row-1)
			{
				TFT_SEND_DATA(0xFF);
				TFT_SEND_DATA(0xFF);
			}else{
				TFT_SEND_DATA(0x00);
				TFT_SEND_DATA(0x00);
			}
		}
	}
}
	

void TFT_Page_net()
{
	int col = TFT_COLUMN_NUMBER;
	int row = TFT_LINE_NUMBER;
	
	int c, r;
	
	for(c=0;c<col;c++)
	{
		for(r=0;r<row;r++)
		{
			if(r%2 == 0)
			{
				TFT_SEND_DATA(0xFF);
				TFT_SEND_DATA(0xFF);
			}else{
				TFT_SEND_DATA(0x00);
				TFT_SEND_DATA(0x00);
			}
		}
	}
}	


void TFT_Page_cell()
{
  TFT_SET_ADD(0,0,239,319);

	int col = TFT_COLUMN_NUMBER;
	int row = TFT_LINE_NUMBER;
	
	int c = 0;
	int r = 0;

	int c2 = 0;
	int r2 = 0;

	int c3 = 0;
	int r3 = 0;

	for(c=0;c<col;c++)
	{
		for(r=0;r<row;r++)
		{
			c2 = (int)(c/5);
			r2 = (int)(r/5);
			c3 = c2 % 2;
			r3 = r2 % 2;

			if(c3 == r3){
				TFT_SEND_DATA(0xFF);
				TFT_SEND_DATA(0xFF);
			}else{
				TFT_SEND_DATA(0x00);
				TFT_SEND_DATA(0x00);
			}			
		}
	}
}



//静态残影测试，PB12手动下一页
int main2(void)
{
	delay_ms(1);
	
	SYS_init(4);
  IO_init();
	LED_GPIO_Config(); //LED 端口初始化   	
  Key_GPIO_Config();//按键端口初始化

  TFT_init();
	
	int speeds = 0;

	page = 0;

  while(1)
  {
		if( Key_Scan(GPIOB,GPIO_Pin_2) == KEY_ON  )	 //判断KEY1是否按下
		{
			LED1( ON );
			page++;
			if(page > 2){ page = 1; }

			if(page == 1){
				Picture_display(0,0,240,80, gImage_IPS);
				Picture_display(0,80,240,80, gImage_IPS);
				Picture_display(0,160,240,80, gImage_IPS);
				Picture_display(0,240,240,80, gImage_IPS);
			}else if(page == 2){
				TFT_full(BLACK);
			}else{
				page = 1;
			}
			delay_ms(speeds);
		}else{
			
			LED1( OFF );
		}
	}

}

//常规自动测试PB12暂停
int main(void)
{
	delay_ms(1);
	
	SYS_init(4);
  IO_init();

  TFT_init();
	
	int speeds = 0;

	LED_GPIO_Config(); //LED 端口初始化   	
  Key_GPIO_Config();//按键端口初始化
	
	TFT_SEND_CMD(0x29);     

	TFT_Page_cell();

//				Picture_display(0,0,240,160, gImage_IPS);
//				Picture_display(0,80,240,80, gImage_IPS);
//				Picture_display(0,160,240,80, gImage_IPS);
//				Picture_display(0,240,240,80, gImage_IPS);
	//return 0;

	page = 0;

  while(1)
  {

		if( Key_Scan(GPIOB,GPIO_Pin_2) == KEY_OFF  )	 //判断KEY1是否按下
		{
			//TFT_SEND_CMD(0x20);     
		}else{
			//TFT_SEND_CMD(0x21);     
		}

		if( Key_Scan(GPIOB,GPIO_Pin_2) == KEY_OFF  )	 //判断KEY1是否按下
		{
			LED1( OFF );
			page++;
			if(page > 2){ page = 1; }

			if(page == 1){
				TFT_Page_cell();
				 //TFT_init();
				//TFT_Page_border();
			}else if(page == 2){
				Show_L127(0, 0, 240,320, gImage_L127);
				 //TFT_init();
				//TFT_full(RED);
				//TFT_full(BLACK);
			}else if(page == 3){
				 //TFT_init();
				TFT_full(WHITE);
				
			}else if(page == 4){
					TFT_full(BLACK);
			}else if(page == 5){
				TFT_full(BLUE);
			}else if(page == 6){
				TFT_full(GREEN);
				//TFT_Page_bar();
			}else if(page == 7){
				TFT_Page_cell();
			}else if(page == 8){
				Picture_display(0,0,240,160, gImage_IPS);
				Picture_display(0,80,240,80, gImage_IPS);
				Picture_display(0,160,240,80, gImage_IPS);
				Picture_display(0,240,240,80, gImage_IPS);

				//				TFT_full(GREEN);
//				Picture_display(0,0,240,16,gImage_xk_1_1);
			}else{
				page = 1;
			}
			delay_ms(speeds);
		}else{
			
			LED1( ON );
		}
	}

}



