/******************************************************************************

                  版权所有 (C), 2019, 北京迪文科技有限公司

 ******************************************************************************
  文 件 名   : sys.c
  版 本 号   : V2.0
  作    者   : LiuYanZhou
  生成日期   : 2019年7月6日
  功能描述   : 
  修改历史   :
  1.日    期   : 
    作    者   : 
    修改内容   : 
******************************************************************************/

#include "sys.h"
#include "Uart.h"
#include  "Function_H.h"
#include  "i2c.h"
static u16 data SysTick=0;

u16  xdata  T_number_arr[TIMER_NUMBER]={0};
u8   xdata  T_number_en[TIMER_NUMBER]={0};
u16  xdata  T_NUMBER=0;
u16  xdata  T_NUMBER_1=0;
u16  xdata  number_one=0;
/*****************************************************************************
	函 数 名  : void INIT_CPU(void)
	功能描述  : CPU初始化函数
							根据实际使用外设修改或单独配置
	输入参数  :	
	输出参数  : 
	修改历史  :
	1.日    期   : 2019年4月1日
		作    者   : LiuYanZhou
		修改内容   : 创建
*****************************************************************************/ 
void INIT_CPU(){	
	
	IEN0 = 0x00;      //关闭所有中断
	IEN1 = 0x00;
	IEN2 = 0x00;
	IP0 = 0x00;				//中断优先级用默认值
	IP1 = 0x00;
	
	P0 = 0x00;				//IO配置，输出关闭，是不是输出取决于P0MDOUT   
	P1 = 0x00;
	P2 = 0x00;
	P3 = 0x00;
	
	TMOD = 0x11;			//T0 T1 16bit定时器
	TCON = 0x05;			//EX0 EX1是下跳沿中断
}

/*****************************************************************************
 函 数 名  : void PORT_Init(void)
 功能描述  : 端口初始化函数
 输入参数  :	
 输出参数  : 
 修改历史  :
  1.日    期   : 2019年4月1日
    作    者   : LiuYanZhou
    修改内容   : 创建
*****************************************************************************/ 
void PORT_Init(void){
	MUX_SEL = 0x00;	//不引出uart2 不引出 uart3 为IO口
	PORTDRV = 0x01;   //IO驱动强度 +/-8mA
	P0MDOUT = 0x73;		//P0 0111 0000   P0^4 强推  P0^5 强推   P0^6 强推    为IO口输出模式P0^0和P0^1为485方向控制
	P1MDOUT = 0x00;		//P1 0000 0000
	P2MDOUT = 0x00;		//P2 0000 0000
	P3MDOUT = 0x0d;		//P3 0000 1111
}

/*****************************************************************************
 函 数 名  : void T0_Init(void)
 功能描述  : 定时器0初始化	定时间隔1ms
 输入参数  :	 
 输出参数  : 
 修改历史  :
  1.日    期   : 2019年7月6日
    作    者   : LiuYanZhou
    修改内容   : 创建
*****************************************************************************/
void T0_Init(void){
	TMOD |= 0x01;
	TH0 = T1MS>>8;        //1ms定时器
	TL0 = T1MS;
	ET0 = 1;              //开启定时器0中断
	TR0 = 1;              //开启定时器
	EA = 1;               //开总中断
}

#if 0
/*****************************************************************************
 函 数 名  : void T1_Init(void)
 功能描述  : 定时器1初始化	定时间隔1ms
 输入参数  :	 
 输出参数  : 
 修改历史  :
  1.日    期   : 2019年7月6日
    作    者   : LiuYanZhou
    修改内容   : 创建
*****************************************************************************/
void T1_Init(void){
	TMOD |= 0x10;
	TH1 = 0;        			//1ms定时器
	TL1 = 0;
	ET1 = 1;              //开启定时器0中断
	TR1 = 1;              //开启定时器
	EA = 1;               //开总中断
}
#endif

/*****************************************************************************
 函 数 名  : void T2_Init(void)
 功能描述  : 定时器2初始化	定时间隔1ms
 输入参数  :	 
 输出参数  : 
 修改历史  :
  1.日    期   : 2019年7月6日
    作    者   : LiuYanZhou
    修改内容   : 创建
*****************************************************************************/
void T2_Init(void){
	T2CON = 0x70;
	TH2 = 0x00;
	TL2 = 0x00;
	TRL2H = 0xBC;			//1ms的定时器
	TRL2L = 0xCD;
	ET2 = 1;					//开启定时器
	TR2 = 1;
	EA=1;
}

/*****************************************************************************
 函 数 名  : void T0_ISR_PC(void)    interrupt 1
 功能描述  : 定时器0处理函数，1毫秒增加
 输入参数  :	 
 输出参数  : 
 修改历史  :
  1.日    期   : 2019年7月6日
    作    者   : LiuYanZhou
    修改内容   : 创建
*****************************************************************************/
void T0_ISR_PC(void)    interrupt 1
{
	EA=0;	
	TH0 = T1MS >> 8;
	TL0 = T1MS;

	Send_Page_Time_Count++;		//定时发送页面计数
		
	if(Pop_Time_Count && Pop_Time_Count != 0xFFFF){
		Pop_Time_Count--;
	}
	EA=1;
}



#if 0
/*****************************************************************************
 函 数 名  : void T1_ISR_PC(void)    interrupt 3
 功能描述  : 定时器1处理函数，提供RTC计时和处理
 输入参数  :	 
 输出参数  : 
 修改历史  :
  1.日    期   : 2019年7月6日
    作    者   : LiuYanZhou
    修改内容   : 创建
*****************************************************************************/
void T1_ISR_PC(void)    interrupt 3
{
	EA = 0;
	TH1 = T1MS>>8;
	TL1 = T1MS;
	SysTick_RTC++;
	if(SysTick_RTC == 1000){
		real_time.sec++;
		if(real_time.sec > 59){
			real_time.sec = 0;
			real_time.min++;
			if(real_time.min > 59){
				real_time.min = 0;
				real_time.hour++;
				if(real_time.hour>23){
					real_time.hour = 0;
					real_time.day++;
					if((real_time.month == 2) && (Is_Leap_Year((u16)real_time.year + 2000))){
						if(real_time.day > (mon_table[real_time.month - 1] + 1)){
							real_time.day = 1;
							real_time.month++;
						}
					}else{
						if(real_time.day > mon_table[real_time.month - 1]){
							real_time.day = 1;
							real_time.month++;
						}
					}
					if(real_time.month > 12){
						real_time.month = 1;
						real_time.year++;
					}
				}
			}
		}
		SysTick_RTC = 0;
		Second_Updata_Flag = 1;		
	}
	EA = 1;
}
#endif

/*****************************************************************************
 函 数 名  : void T2_ISR_PC(void)    interrupt 5
 功能描述  : 定时器2中断处理函数，提供延时函数计数和喂狗
 输入参数  :	 
 输出参数  : 
 修改历史  :
  1.日    期   : 2019年7月6日
    作    者   : LiuYanZhou
    修改内容   : 创建
*****************************************************************************/
void T2_ISR_PC(void)    interrupt 5
{
	u8 i=0;
	TF2 = 0;
	if(SysTick)	SysTick--;
	if(T_NUMBER)	T_NUMBER--;
  if(T_NUMBER_1)	T_NUMBER_1--;
	for(i=0;i<TIMER_NUMBER;i++)
	{
	    if(T_number_arr[i]&&T_number_en[i])	T_number_arr[i]--;
	}
	WDT_RST();
	 SysTick_RTC++;
	
}

#if 0
/*****************************************************************************
 函 数 名  : void start_timer(u8 ID, u16 nTime)
 功能描述  : 启动一个软定时器，
 输入参数  :	 u8 ID         定时器ID
               u16 nTime     定时ms数

  1.日    期   : 2022-06-17
    作    者   : cuijia
    修改内容   : 创建
*****************************************************************************/
void start_timer(u8 ID, u16 nTime)
{
       if(ID<TIMER_NUMBER) 
			 {
			      T_number_en[ID]= 1;   //定时器使能
				    T_number_arr[ID]= nTime; 
			 }  
}

/*****************************************************************************
 函 数 名  : void stop_timer(u8 ID)
 功能描述  : 关闭一个软定时器，
 输入参数  :	 u8 ID         定时器ID
  1.日    期   : 2022-06-17
    作    者   : cuijia
    修改内容   : 创建
*****************************************************************************/
void stop_timer(u8 ID)
{
	   if(ID<TIMER_NUMBER) 
        T_number_en[ID]= 0;   //定时器关闭
}
/*****************************************************************************
 函 数 名  : u8 get_time_out_flag(u8 ID)
 功能描述  : 获得定时器是否超时
 输入参数  :	u8 ID  定时器ID
 输出参数  :  0  未超时    非0  超时
 修改历史  :
  1.日    期   : 2022-06-17
    作    者   : cuijia
    修改内容   : 创建
*****************************************************************************/
u8 get_time_out_flag(u8 ID)
{
  if(T_number_en[ID])
	{
	   	if(T_number_arr[ID]==0)
			{
				stop_timer(ID);
        return 1;
			}
      else	 
	      return 0;	
	}
   return 0;	
}
#endif

#if 0
/*****************************************************************************
 函 数 名  : void delay_ms(u16 n)
 功能描述  : 延时函数，使用定时器2硬件延时
 输入参数  :	 
 输出参数  : 
 修改历史  :
  1.日    期   : 2019年7月6日
    作    者   : LiuYanZhou
    修改内容   : 创建
*****************************************************************************/
void delay_ms(u16 n){
	SysTick = n;
	while(SysTick);
}

void delay_ms(u16 n){
    int data mi,mj;
    for(mi=0;mi<n;mi++)
        for(mj=0;mj<8600;mj++);
}

void delay_us(u16 n){
    int data ui,uj;
    for(ui=0;ui<n;ui++)
        for(uj=0;uj<5;uj++);
}
#endif

/*****************************************************************************
 函 数 名  :void Read_Dgusii_Vp(unsigned int addr,unsigned char *buf,unsigned int len)
 功能描述  :读DGUS数据
 输入参数  :	 
 输出参数  : 
 修改历史  :
  1.日    期   : 2019年7月20日
    作    者   : DWIN
    修改内容   : LiuYanZhou
*****************************************************************************/
void Read_Dgusii_Vp(unsigned int addr,unsigned char *buf,unsigned int len){
	unsigned char i;
	i = (unsigned char)(addr & 0x01);
	addr = addr / 2;
	ADR_H = 0x00;
	ADR_M = (unsigned char)(addr >> 8);
	ADR_L = (unsigned char)(addr);
	ADR_INC = 0x01;
	RAMMODE = 0xAF;
	while(APP_ACK == 0);
	while(len > 0){
		APP_EN = 1;
		while(APP_EN == 1);
		if((i==0)&&(len>0)){
			*buf++ = DATA3;
			*buf++ = DATA2;
			i = 1;
			len--;
		}
		if((i == 1) && (len > 0)){
			*buf++ = DATA1;
			*buf++ = DATA0;
			i = 0;
			len--;
		}
	}
	RAMMODE=0x00;
}



/*****************************************************************************
 函 数 名  :void Write_Dgusii_Vp(unsigned int addr,unsigned char *buf,unsigned int len)
 功能描述  :写DGUS数据 //写DGUS变量数据 addr=DGUS变量地址  *buf=写入数据指针  len=写入数据字长度
 输入参数  :	 
 输出参数  : 
 修改历史  :
  1.日    期   : 2019年7月20日
    作    者   : DWIN
    修改内容   : LiuYanZhou
*****************************************************************************
void Write_Dgusii_Vp(unsigned int addr,unsigned char *buf,unsigned int len){
	unsigned char i;
//	bit EA_Bak;
//	EA_Bak = EA;
//	EA = 0;
	i = (unsigned char)(addr & 0x01);
	addr = addr / 2;
	ADR_H = 0x00;
	ADR_M = (unsigned char)(addr >> 8);
	ADR_L = (unsigned char)(addr);
	ADR_INC = 0x01;
	RAMMODE = 0x8F;
	while(APP_ACK == 0);
	if(i && len > 0){
		RAMMODE = 0x83;
		DATA1 = *buf++;
		DATA0 = *buf++;
		APP_EN = 1;
		while(APP_EN == 1);
		len--;
	}
	RAMMODE = 0x8F;
	while(len >= 2){
		DATA3 =* buf++;
		DATA2 =* buf++;
		DATA1 =* buf++;
		DATA0 =* buf++;
		APP_EN = 1;
		while(APP_EN == 1);
		len = len - 2;
	}
	if(len){
		RAMMODE = 0x8C;
		DATA3 =* buf++;
		DATA2 =* buf++;
		APP_EN = 1;
		while(APP_EN == 1);
	}
	RAMMODE = 0x00;
//	EA = EA_Bak;
}
*****************************************************************************/
void Write_Dgusii_Vp_byChar(unsigned int addr,unsigned char *buf,unsigned int len){
	unsigned char i;
//	bit EA_Bak;
//	EA_Bak = EA;
//	EA = 0;
	i = (unsigned char)(addr & 0x01);
	addr = addr / 2;
	ADR_H = 0x00;
	ADR_M = (unsigned char)(addr >> 8);
	ADR_L = (unsigned char)(addr);
	ADR_INC = 0x01;
	RAMMODE = 0x8F;
	while(APP_ACK == 0);
	if(i && len > 0){
		if(len == 1){
			RAMMODE = 0x82;
			DATA1 = *buf++;
			APP_EN = 1;
			while(APP_EN == 1);
			len--;
		} else {
			RAMMODE = 0x83;
			DATA1 = *buf++;
			DATA0 = *buf++;
			APP_EN = 1;
			while(APP_EN == 1);
			len -= 2;
		}
	}
	RAMMODE = 0x8F;
	while(len >= 4){
		DATA3 = *buf++;
		DATA2 = *buf++;
		DATA1 = *buf++;
		DATA0 = *buf++;
		APP_EN = 1;
		while(APP_EN == 1);
		len -= 4;
	}
	
	if(len == 3){
		RAMMODE = 0x8E;
		DATA3 = *buf++;
		DATA2 = *buf++;
		DATA1 = *buf++;
		APP_EN = 1;
		while(APP_EN == 1);
	} else if(len == 2){
		RAMMODE = 0x8C;
		DATA3 =* buf++;
		DATA2 =* buf++;
		APP_EN = 1;
		while(APP_EN == 1);
	} else if(len == 1){
		RAMMODE = 0x88;
		DATA3 = *buf++;
		APP_EN = 1;
		while(APP_EN == 1);
	}
	RAMMODE = 0x00;
//	EA = EA_Bak;
}


/*****************************************************************************
 函 数 名  :u16 Read_Dgus(u16 Dgus_Addr)、void Write_Dgus(u16 Dgus_Addr,u16 Val)
 功能描述  :读DGUS数据 //写DGUS变量数据 Dgus_Addr=DGUS变量地址    Val=写入数据字长度
 输入参数  :	 
 输出参数  : 
 修改历史  :
  1.日    期   : 2021年9月30日
    作    者   : DWIN
    修改内容   : wenyu
*****************************************************************************/
u16 Read_Dgus(u16 Dgus_Addr)
{	   	   	u16 R_Dgus=0;

			ADR_H = 0x00;
			ADR_M = (u8)((Dgus_Addr/2)>>8);
			ADR_L = (u8)(Dgus_Addr/2);
			RAMMODE = 0xAF;		//读操作
			while(!APP_ACK);
			APP_EN = 1;
			while(APP_EN);
			if(Dgus_Addr%2)	R_Dgus = (DATA1<<8)+DATA0; 
			else   R_Dgus=(DATA3<<8)+DATA2;
			RAMMODE = 0;

			return   R_Dgus;		
}
/*****************************************************************************/
void Write_Dgus(u16 Dgus_Addr,u16 Val)
{         
			ADR_H = 0x00;
			ADR_M = (u8)((Dgus_Addr/2)>>8);
			ADR_L = (u8)(Dgus_Addr/2);
			RAMMODE = 0xAF;		//读操作
			while(!APP_ACK);
			APP_EN = 1;
			while(APP_EN);
			RAMMODE = 0;
		   	ADR_M = (u8)((Dgus_Addr/2)>>8);
			ADR_L = (u8)(Dgus_Addr/2);
			RAMMODE = 0x8F;		//写操作
		    while(!APP_ACK);
			if(Dgus_Addr%2){DATA1=(u8)(Val>>8); DATA0=(u8)(Val); }
			else { DATA3 =(u8)(Val>>8);	DATA2 =(u8)(Val);}
		    APP_EN = 1;
			while(APP_EN);
			RAMMODE = 0;
		
}

//检查指定DGUS变量地址的DATA3清零的话就退出
void wait_ok(unsigned int addr)
{	
	ADR_H=0x00;
	ADR_M=(unsigned char)(addr>>8);
	ADR_L=(unsigned char)(addr);
	ADR_INC=0x00;
	do
	{ 
		for(addr=0;addr<1000;addr++)	//释放变量空间一段时间
			DATA2=DATA1;
		RAMMODE=0xAF;
		while(APP_ACK==0);
		APP_EN=1;
		while(APP_EN==1);
		RAMMODE=0x00;
	}	
	while(DATA3!=0);
}

/*****************************************************************************/
	//读写T5L片内256KW Flash，mod=0x5A 为读取，mod=0xA5为写入
	//addr=DGUS变量地址，必须是偶数；addr_flash=flash读取地址，必须是偶数；len=读取字长度，必须是偶数。
void T5L_Flash(unsigned char mod,unsigned int addr,long addr_flash,unsigned int len)
{	
	ADR_H=0x00;
	ADR_M=0x00;
	ADR_L=0x04;
	ADR_INC=0x01;
	RAMMODE=0x8F;		//启动读Flash
	while(APP_ACK==0);
	DATA3=mod;
	DATA2=(unsigned char)(addr_flash>>16);
	DATA1=(unsigned char)(addr_flash>>8);
	DATA0=(unsigned char)(addr_flash&0xFE);
	APP_EN=1;
	while(APP_EN==1);	
	DATA3=(unsigned char)(addr>>8);
	DATA2=(unsigned char)(addr&0xFE);
	DATA1=(unsigned char)(len>>8);
	DATA0=(unsigned char)(len&0xFE);
	APP_EN=1;
	while(APP_EN==1);
	RAMMODE=0x00;
	wait_ok(0x0004);
}	//等待数据读取OK
#if 0
/*****************************************************************************
 函 数 名  : hdMAC(u8 *a, u8 *b, u8 *c) ; hdDIV(u8 *a, u8 *b, u8 *c)
 功能描述  : 乘法，除法函数，利用T5L数学运算单元（MDU）计算
 输入参数  :	 
 输出参数  : 
 修改历史  :
  1.日    期   : 2022年4月15日
    作    者   : cuijia
    修改内容   : 创建
*****************************************************************************/

void hdMAC(u8 *a, u8 *b, u8 *c)
{
    u8 i;

    EXADR = 0;
    for (i = 0; i < 8; i++)
        EXDATA = a[i];
    for (i = 0; i < 8; i++)
        EXDATA = b[i];
    for (i = 0; i < 8; i++)
        EXDATA = c[i];
    MAC_MODE = 0XD8;//执行一次c=a*b+c的64位带符号乘法计算
	  while(MAC_MODE&0x80);//等待硬件执行完成
    EXADR = 0X10;
    for (i = 0; i < 8; i++)
        c[i] = EXDATA;//计算结果
}

void hdDIV(u8 *a, u8 *b, u8 *c)
{
    u8 i;

    EXADR = 0;
    for (i = 0; i < 8; i++)
        EXDATA = a[i];
    for (i = 0; i < 8; i++)
        EXDATA = b[i];
    for (i = 0; i < 8; i++)
        EXDATA = c[i];
    DIV_MODE = 0X88;//执行一次c/a = a余b的64位带符号除法计算,不进行四舍五入
	  while(DIV_MODE&0x80);//等待硬件执行完成
    EXADR = 0X00;
    for (i = 0; i < 8; i++)
        a[i] = EXDATA;//计算结果
    for (i = 0; i < 8; i++)
        b[i] = EXDATA;//计算结果
}

/****************************************
名称：void page_change(u16 Page)
功能：切页
***/
void page_change(u16 Page)
{
   Write_Dgus(0x85,Page); 
   Write_Dgus(0x84,0x5A01); 
	    do{
      delay_us(500);
   } while(Read_Dgus(0x84)&0xFF00);
}
#endif

#if 0
/******************************
名称：int get_ad_value(int adIdx)
功能：获取AD值 参数adIdx为通道号
*****/
u16 get_ad_value(u16 adIdx)
{
    if(adIdx<8)
		{
		   return Read_Dgus(0x32+adIdx);
		}
		return 0;
}

#define       PWM_ACCURACY	        0x2042         //PWM精度
/**************************
名称：void  pwm_init(u8 id)
功能：id=0或1，目前支持配置两路PWM
**/
void pwm_init(u8 id)
{
	if(id==0)
	{
	   Write_Dgus(0x87,PWM_ACCURACY);    // 如 PWM_ACCURACY= 0x2042 配置频率  100kHz=825.7536MHz/0x2042(8258)    
     Write_Dgus(0x86,0x5A01);   //配置分频系数 pwm0配置100khz/1=100khz
	}else if(id==1)
	{
		 Write_Dgus(0x89,PWM_ACCURACY);    // 如 PWM_ACCURACY= 0x2042 配置频率  100kHz=825.7536MHz/0x2042(8258)    
     Write_Dgus(0x88,0x5A01);  //配置分频系数 pwm0配置100khz/1=100khz
	}
}

/**************************
名称：void  pwm_set(u16 Pec,u8 id)
功能：pwm 设置 参数Pec为占空比,id=0或1，目前支持配置两路PWM
**/
void pwm_set(u16 Pec,u8 id)
{
	 u16 Val=0;
	if(id==0)
	{
	   Val=(u32)Pec*PWM_ACCURACY/100;
     Write_Dgus(0x92,Val);  	 //配置占空比		
	}else if(id==1)
	{
	   Val=(u32)Pec*PWM_ACCURACY/100;
     Write_Dgus(0x93,Val); 
	}
}

/**************************
名称：void SetPinOut(u8 Port,u8 Pin)
功能：IO口输出设置 Port=0，Pin=1表示P0^1
**/

void SetPinOut(u8 Port,u8 Pin)
{
 switch(Port)
 {
	case 0: P0MDOUT|=(1<<Pin);
			break;
	case 1: P1MDOUT|=(1<<Pin);
			break;
	case 2: P2MDOUT|=(1<<Pin);
			break;
	case 3: P3MDOUT|=(1<<Pin);
			break;	
	default:break;				
 }
}
/**************************
名称：void SetPinIn(u8 Port,u8 Pin)
功能：IO口输入设置 Port=1，Pin=2表示P1^2
**/
void SetPinIn(u8 Port,u8 Pin)
{
	switch(Port)
	{
		case 0: P0MDOUT&=~(1<<Pin);
				break;
		case 1: P1MDOUT&=~(1<<Pin);
				break;
		case 2: P2MDOUT&=~(1<<Pin);
				break;
		case 3: P3MDOUT&=~(1<<Pin);
				break;	
		default:break;				
	}
		
}
u8 GetPinIn(u8 Port,u8 Pin)
{	 
	u8 value;
	switch(Port)
	{
		case 0: value=P0&(1<<Pin);
				  break;
		case 1: value=P1&(1<<Pin);
				  break;
		case 2: value=P2&(1<<Pin);
				  break;
		case 3: value=P3&(1<<Pin);
				  break;	
		default:
				value=0;
				break;				
	}
	return value;
}
/********************************************************************************************
名称：TouchSwitch(u16 PageID, u8 TouchType, u8 TouchID, u8 Status)
功能：开启关闭触控 PageID页面号，TouchType触控类型，TouchID触控ID，Status开启1关闭触控=0
***/
void TouchSwitch(u16 PageID, u8 TouchType, u8 TouchID, u8 Status)
{
	u8 k_data[8];
	
	*(u16*)k_data = 0x5aa5;
	*(u16*)&k_data[2] = PageID;
	k_data[4] = TouchID;
	k_data[5] = TouchType;
	if(Status)
		*(u16*)&k_data[6] = 1;
	else
		*(u16*)&k_data[6] = 0;
	Write_Dgusii_Vp_byChar(0xb0,k_data,8);
	do
	{
		delay_us(500);
		Read_Dgusii_Vp(0xb0,k_data,1);
	}while(k_data[0]!=0);
}
#endif