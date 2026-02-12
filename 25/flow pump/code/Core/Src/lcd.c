// file code :SYSTEM

 /******************************************************************************* 
【产品型号】：BJ12864M-1
【点 阵 数】：128X64
【外型尺寸】：93X70
【视域尺寸】：72X40
【定位尺寸】：88X64
【点 间 距】：0.52X0.52
【点 大 小】：0.48X0.48
【控制器类型】：ST7920
【简单介绍】：带字库，SPI串并口，3.3V或5V

  2015-7-12 程序更新了!
更新记录：
1.增加了24*32大字体显示功能；
2.增加了16*32大字体显示功能；
3.优化了读LCD忙的代码；
4.新增了是sprintf的任意位数数字直接显示；
5.新增了CGRAM的写入，读出操作函数；
6.优化了绘图GDRAM函数的操作，绘图速度提高；
 控制器 : ST7920  
 
1、ST7920 内部固化了8192 个16×16 点阵的中文字型在CGROM里。
2、固化有126 个16×8 点阵的西文字符在HCGROM里。
3、提供4个16×16点阵自造字符的存储空间CGRAM。
4、提供128×64的点阵绘图共1024个字节的存储空间GDRAM。
5、提供1个16×15点阵图标的存储空间IRAM（ICON RAM）。

DDRAM地址
0x80  0x81  0x82  0x83  0x84  0x85  0x86  0x87    //第一行汉字位置
0x90  0x91  0x92  0x93  0x94  0x95  0x96  0x97    //第二行汉字位置
0x88  0x89  0x8a  0x8b  0x8c  0x8d  0x8e  0x8f     //第三行汉字位置
0x98  0x99  0x9a  0x9b  0x9c  0x9d  0x9e  0x9f     //第四行汉字位置

*******************************************************************************/  
#include "lcd.h"
#include "sys.h"




/*  以串行方式发送一字节数据 */ 
static void SendByte(u8 sendData)
{
  u8 i;
  for (i = 0; i < 8; i++)
  {
    if (sendData&0x80)
      LCD_SID_HIGH;
    else
      LCD_SID_LOW;
    LCD_CLK_LOW;
     delay_us(10); //延时10个us等待上一条指令执行完毕
    LCD_CLK_HIGH;
     sendData=sendData<<1;
   }
}


/* 串行接受一个字节的数据  */
static u8 ReceiveByte(void)
{
   u8 i,d1,d2;
   for (i = 0; i < 8; i++)
   {
    LCD_CLK_LOW;
    delay_us(10);
    LCD_CLK_HIGH;
    
    if (LCD_SID) d1++;
    d1 = d1<<1;
   }
   
   for (i = 0; i < 8; i++)
   {
    LCD_CLK_LOW;
     delay_us(1);
    LCD_CLK_HIGH;   
    if (LCD_SID) d2++;
    d2 = d2<<1;
   }
   return (d1&0xf0 + d2&0x0f);
}

//检测数据寄存器的最高位，判断忙标志
 static u8 ReadBF(void)
{
  SendByte(READ_STATE);
  u8 data = ReceiveByte();
  if (data > 0x7f) return 1;
  else  return 0;
}

void LCD_WriteCMD(u8 data)
{
  LCD_CS_HIGH;
  while (ReadBF());
  SendByte(writecommand); //发送串口控制格式
  SendByte(data&0xf0);  //高四位
  SendByte((data&0x0f)<<4); //低四位
  while (ReadBF());
  LCD_CS_LOW;
}
      
void LCD_WriteDATA(u8 data)
{
  LCD_CS_HIGH;
  while (ReadBF());
  SendByte(writedata); //发送串口控制格式
  SendByte(data&0xf0);   //发送高4位
  SendByte((data&0x0f)<<4); //发送低4位
  while (ReadBF());
  LCD_CS_LOW;
 }

/*******************************************************************************
清屏函数，清除整屏的文字显示，非绘图
出口参数: 无
********************************************************************************/
void LCD_Clear_DDRAM(void)
{
  LCD_WriteCMD(0x01);  //基本指令集
  HAL_Delay(2);        //datasheet >=1.4ms 
}

//一种算法，测试OK xianhuitu test OK
// 测试发现整个执行完大概需要2~3S
 void LCD_Clear_GDRAM(void)  //test OK
{    
  u8 j, i;
  LCD_WriteCMD(0x36);
  for(j=0;j<32;j++) //上半屏32点行
  {
    LCD_WriteCMD(0x80+j);
    LCD_WriteCMD(0x80);//X坐标
    for(i=0;i<32;i++)//32表示如1行写完,自动写第3行
    {
      LCD_WriteDATA(0x00);  
    }
  }
  LCD_WriteCMD(0x30);     //基本指令 
}

void LCD_Init(void)
{  
  //配置与LCD相连接的GPIO引脚
  
// PA6--MISO--(not used) // you omit this IO
// PB13--SCK--CLK--E
// PB14--NSS--CS--RS
// PB15--MOSI--SDIO--R/W

// PB1--BLK
  

  //  1>芯片上电；
  //2>延时40ms以上；
  //3>复位操作：RST出现一个上升沿（RST=1;RST=0;RST=1;）
     HAL_Delay(50);  // this delay is necessary !
     LCD_WriteCMD(0x30);//功能设置，一次送8位数据，基本指令集
     LCD_WriteCMD(0x0C);//0000,1100  显示状态开关：整体显示ON，光标显示OFF，光标显示反白OFF
     //LCD_WriteCMD(0x0f);//整体显示 ON C=1； 游标ON(方块闪烁) B=1； 游标位置(游标下划线)ON
     //如果有按键输入数字，则开光标，左右移动：LCD_WriteCMD(0x10); LCD_WriteCMD(0x10);  
     LCD_WriteCMD(0x01);//0000,0001 清除显示DDRAM
     LCD_WriteCMD(0x02);//0000,0010 DDRAM地址归位
//   LCD_WriteCMD(0x80);//1000,0000 设定DDRAM 7位地址000，0000到地址计数器AC
//   LCD_WriteCMD(0x04);//点设定，显示字符/光标从左到右移位，DDRAM地址加1
     LCD_WriteCMD(0x06); //点设定，画面不移动,游标右移，DDRAM位地址加1
     HAL_Delay(10);  // this delay is necessary !
}

/******************************************************************************* 
* 名称 :  LCD_SetPos
* 功能 : 设定显示位置,只适用于标准指令集 
* 输入 :  x (0~3)行
          y (0~127)列
* 输出 : 无 
*******************************************************************************/  
void LCD_SetPos(u8 row, u8 col)  
{  
    u8 pos;  
    if( row == 0)  
            row = 0x80;  
    else if(row == 1)  
            row = 0x90;  
    else if(row == 2)  
            row = 0x88;  
    else if(row == 3)  
            row = 0x98;     
    pos = row + col;  
    LCD_WriteCMD(pos);//在写入数据前先指定显示地址 
    delay_us(8);
}  


/******************************************************************** 
* 名称 : ShowCharPos 
* 功能 : 在当前坐标位置显示单个字符,col=0,1,2,每个col占16dot
* 输入 : u8 row, u8 col,u8 dat
* 输出 : 无 
***********************************************************************/ 
void ShowCharPos(u8 row, u8 col,u8 dat)
{    
  LCD_SetPos(row, col);
  LCD_WriteDATA(dat);
}

/******************************************************************************* 
* 名称 : ShowEnString(u8 *s)  
* 功能 : 显示英文字符串  col=0,1,2,1 col=16dot
* 输入 : *s 
* 输出 : 无 
*******************************************************************************/  
void ShowString(u8 *s)  
{    
    u8  i = 0;  
    while(s[i] != '\0')  
    {   
      LCD_WriteDATA(s[i++]);      
    }  
}  

void ShowStringPos(u8 row, u8 col,u8 *s)     //col is full char wide 
{  
     u8  i = 0;  
     LCD_SetPos(row, col);    
    while(s[i] != '\0')  
    {       
      LCD_WriteDATA(s[i++]);        
      if((2*col+i)%(16)==0)     //must 16, is OK
      {          
        LCD_SetPos(++row,0);     //display start at next row.
      }
   
    }  
}  


/******************************************************************************* 
* 名称 : ShowGB 
* 功能 : 显示单个汉字
* 输入 : i 
* 输出 : 无 
********************************************************************************/  
void ShowGB( unsigned char *HZ)
{
   LCD_WriteDATA(HZ[0]);                //写入汉字的高八位数据
   LCD_WriteDATA(HZ[1]);                //写入汉字的低八位数据 
}

void ShowGBPos(u8 row, u8 col, u8 *HZ)
{
   LCD_SetPos(row, col);
   ShowGB(HZ);
}


/******************************************************************** 
* 名称 : ShowGBStringPos 
* 功能 : 显示多个汉字
* 输入 : u8 row, u8 col :汉字串的起始地址
* 输出 : 无 
* 说明 : 自动换行:lcm默认换行顺序是乱的，0--2--1--3--0
***********************************************************************/  
void ShowGBStringPos(u8 row, u8 col, u8 *s)
{
  u8 i = 0;
  LCD_SetPos(row, col);
  while (s[i] != '\0')
  {
    ShowGB(s+i);
    i+=2;   
    if((2*col+i)%(16)==0)                 //如果满一行
    {            
       LCD_SetPos(++row,0);            //重新设置显示的起始地址
    }     
  } 
}



/******************************************************************** 
* 名称 : ShowBlankPos 
* 功能 : 
* 输入 : col = 0~7 1num =1半宽字符
* 功能 : 黑板擦功能，擦除不显示的内容
***********************************************************************/ 
void ShowBlankPos(u8 row, u8 col, u8 num)
{
  u8 i ;
  LCD_SetPos(row,col);   
  for (i = 0; i < num;i++) 
    LCD_WriteDATA(0x20); //写空格
}


/******************************************************************** 
* 名称 : ShowNumPos
* 功能 : 
* 输入 : u8 row, u8 col 起始地址 col=0,1,2,每个col占16dot
* 输出 : 无 
* 说明 : 自动换行:lcm默认换行顺序是乱的，0--2--1--3--0
***********************************************************************/ 
void ShowNumPos(u8 row, u8 col, u16 num,u8 DecOrHex)//col is full char wide 
{     
    char buf[16]; 
    if(DecOrHex==10)  // dex 
    sprintf(buf, "%d", num);
    else
    sprintf(buf,"%X",num);  //"0x%X"
    ShowStringPos(row,col,(u8 *)(buf));
}


/******************************************************************** 
* 名称 : Show2NumPos
* 功能 : 显示2位数字,十位不足补零,0~99
* 输入 : u8 row, u8 col 起始地址 col=0,1,2,每个col占16dot
* 输出 : 无 
* 说明 : 自动换行:lcm默认换行顺序是乱的，0--2--1--3--0
***********************************************************************/ 
void Show2NumPos(u8 row, u8 col, u8 num)//col is full char wide 
{     
    char buf[1]; 
	char buf1[2]="0";
    
	if(num <10)
	 {
		sprintf(buf, "%d", num);  	 
		strcat (buf1, buf);	//buf link to the buf1
		ShowStringPos(row,col,(u8 *)buf1);	//06
	 }
	else
	{
        sprintf(buf1, "%d", num);  
		ShowStringPos(row,col,(u8 *)buf1);	//06
	}
   
}



