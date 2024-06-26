#include  "i2c.h"

u8  Select_rtc=SELECT_RTC_TYPE; //选择RTC类型
bit                 RTC_Flog=0;
u16    xdata        SysTick_RTC=0;
#if SELECT_RTC_TYPE		
//===========================
void Delayus() {u8 i;for(i=0;i<80;i++);}  //50
//===========================
void SDA_IN()  {P3MDOUT=P3MDOUT&0xF7;}
//===========================
void SDA_OUT() {P3MDOUT=P3MDOUT|0x08;}
//===========================
void Rx8130_Start()
{	
						SDA_OUT();
						RTC_SDA=1;
						RTC_SCL=1;
						Delayus();  //15
						RTC_SDA=0;
						Delayus();  //15
						RTC_SCL=0;
						Delayus();  //15
}
//================================
void Rx8130_Stop()
{	
						SDA_OUT();
						RTC_SDA=0;
						RTC_SCL=1;
						Delayus();//15
						RTC_SDA=1;
						Delayus();//15
						SDA_IN();
}
//================================
void Rx8130_Mack()
{	
						SDA_OUT();
						RTC_SDA=0;
						Delayus();
						RTC_SCL=1;
						Delayus();
						RTC_SCL=0;
						Delayus();
}
//=================================
void Rx8130_Mnak()
{	
						SDA_OUT();
						RTC_SDA=1;
						Delayus();
						RTC_SCL=1;
						Delayus();
						RTC_SCL=0;
						Delayus();
}
//=================================
void Rx8130_Cack()
{	
						u8 i;
						SDA_IN();
						RTC_SDA=1;
						Delayus();
						RTC_SCL=1;
						Delayus();
						for(i=0;i<50;i++)
						{   
										if(!RTC_SDA) break;
										Delayus();
						}
						RTC_SCL=0;
						Delayus();
						SDA_OUT();
}
//=========================================
void Rx8130_Writ_Byte(u8 dat)
{	
						u8 i;
						SDA_OUT();
						for(i=0;i<8;i++)
						{	
									    if(dat&0x80)   RTC_SDA=1;
										else           RTC_SDA=0;
										dat=(dat<<1);
										Delayus();
										RTC_SCL=1;
										Delayus();
										RTC_SCL=0;
										Delayus();
						}
						Rx8130_Cack();
}
//====================================================
u8 Rx8130_Read_Byte()
{	
						u8 i;
						u8 Dat;
						SDA_IN();
						for(i=0;i<8;i++)
						{	
										Delayus();
										RTC_SCL=1;
										Delayus();
										Dat=(Dat<<1);
										if(RTC_SDA)  Dat=Dat|0x01;
										else         Dat=Dat&0xFE;
										RTC_SCL=0;
										Delayus();
						}
						return(Dat);
}
//====================================================
u8 RTC_Get_Week(u8 years,u8 month,u8 day)
{	
	u8 xdata weak;
	u16 xdata year;
	year= years+2000;
  if(month==1||month==2)
	{
		month += 12, year--;
	}
  if((year<1752)||(year==1752&&month<9)||(year==1752&&month==9&&day<3))
	{
		weak = (day + 2*month + 3*(month+1)/5 + year + year/4 + 6)%7;
	}
  else
	{
		weak = (day +1 + 2*month + 3*(month+1)/5 + year + year/4 -year/100 + year/400)%7;
	}
   return weak;
}
//把RTC读取并处理，写到DGUS对应的变量空间，主程序中每0.5秒调用一次
void Read_Time()
{	
						u8 i,N,M; 
	          u8 Rtcdata[]={0,0,0,0,0,0,0,0};
						Rx8130_Start();
						Rx8130_Writ_Byte(0x64);
						Rx8130_Writ_Byte(0x10);
						Rx8130_Stop();
						Rx8130_Start();
						Rx8130_Writ_Byte(0x65);
						for(i=6;i>0;i--)
						{	
													Rtcdata[i]=Rx8130_Read_Byte();
													Rx8130_Mack();
						}
						Rtcdata[0]=Rx8130_Read_Byte(); Rx8130_Mnak();
						Rx8130_Stop();
						for(i=0;i<3;i++)	//年月日转换成HEX
						{	
							            N=Rtcdata[i]/16;
													M=Rtcdata[i]%16;
													Rtcdata[i]=N*10+M;
						}
						for(i=4;i<7;i++)	//时分秒转换成HEX
						{	
													N=Rtcdata[i]/16;
													M=Rtcdata[i]%16;
													Rtcdata[i]=N*10+M;
						}
						Rtcdata[3]=RTC_Get_Week(Rtcdata[0],Rtcdata[1],Rtcdata[2]);//周						
						Write_Dgus(0X0010,(Rtcdata[0]<<8)+Rtcdata[1]);
						Write_Dgus(0X0011,(Rtcdata[2]<<8)+Rtcdata[3]);
						Write_Dgus(0X0012,(Rtcdata[4]<<8)+Rtcdata[5]);
						Write_Dgus(0X0013,(Rtcdata[6]<<8)+Rtcdata[7]);
}
//====================================================================
void Init_Rtc()
{	
					   u8 i;
	           P3MDOUT = 0x0d;		//P3 0000 1111
				  	   Rx8130_Start();
					   Rx8130_Writ_Byte(0x64);
					   Rx8130_Writ_Byte(0x1d);
					   Rx8130_Stop();
					   Rx8130_Start();
					   Rx8130_Writ_Byte(0x65);
					   i=Rx8130_Read_Byte();
					   Rx8130_Mack();
					   Rx8130_Read_Byte();
					   Rx8130_Mnak();
					   Rx8130_Stop();
					   if((i&0x02)==0x02)
				       {	
									//重新配置时间
									Rx8130_Start();		//30=00
									Rx8130_Writ_Byte(0x64);
									Rx8130_Writ_Byte(0x30);
									Rx8130_Writ_Byte(0x00);
									Rx8130_Stop();
									Rx8130_Start();		//1C-1F=48 00 40 10
									Rx8130_Writ_Byte(0x64);
									Rx8130_Writ_Byte(0x1C);
									Rx8130_Writ_Byte(0x48);
									Rx8130_Writ_Byte(0x00);
									Rx8130_Writ_Byte(0x40);
									Rx8130_Writ_Byte(0x10);
									Rx8130_Stop();
									Rx8130_Start();		//10-16=RTC设置值 BCD格式
									Rx8130_Writ_Byte(0x64);
									Rx8130_Writ_Byte(0x10);
									Rx8130_Writ_Byte(0x00);	//秒
									Rx8130_Writ_Byte(0x00);	//分
									Rx8130_Writ_Byte(0x00);	//时
									Rx8130_Writ_Byte(0x01);	//星期
									Rx8130_Writ_Byte(0x01);	//日
									Rx8130_Writ_Byte(0x01);	//月
									Rx8130_Writ_Byte(0x17);	//年
									Rx8130_Stop();
									Rx8130_Start();		//1E-1F 00 10
									Rx8130_Writ_Byte(0x64);
									Rx8130_Writ_Byte(0x1E);
									Rx8130_Writ_Byte(0x00);	
									Rx8130_Writ_Byte(0x10);			
									Rx8130_Stop();
				       }
}
//=========================================
void RTC_Set_Time()
{					    
						u16  Va=Read_Dgus(0x009C); //0x5AA5
						if(Va==0x5AA5)
						{			u8 N=0,S=0;bit  Flog=1;
									u8 TimeS[]={0,0,0,0,0,0,0};
									u16  V1=Read_Dgus(0x009D); //年，月
									N=(u8)(V1>>8);TimeS[0]=((N/10)*16)+(N%10);
									N=(u8)(V1);   TimeS[1]=((N/10)*16)+(N%10);
									V1=Read_Dgus(0x009E); //日，时
									N=(u8)(V1>>8);TimeS[2]=((N/10)*16)+(N%10);
									N=(u8)(V1);   TimeS[4]=((N/10)*16)+(N%10);
									V1=Read_Dgus(0x009F); //分，秒
									N=(u8)(V1>>8);TimeS[5]=((N/10)*16)+(N%10);
									N=(u8)(V1);   TimeS[6]=((N/10)*16)+(N%10);
//						            V1=Read_Dgus(0x00A0); //秒，x  
//						            N=(u8)(V1>>8);TimeS[6]=((N/10)*16)+(N%10);
									Write_Dgus(0x009c,0);	
									while(Flog)
									{	
										        Rx8130_Start();		//30=00
												Rx8130_Writ_Byte(0x64);
												Rx8130_Writ_Byte(0x30);
												Rx8130_Writ_Byte(0x00);
												Rx8130_Stop();
												Rx8130_Start();		//1C-1F=48 00 40 10
												Rx8130_Writ_Byte(0x64);
												Rx8130_Writ_Byte(0x1C);
												Rx8130_Writ_Byte(0x48);
												Rx8130_Writ_Byte(0x00);
												Rx8130_Writ_Byte(0x40);
												Rx8130_Writ_Byte(0x10);
												Rx8130_Stop();
												Rx8130_Start();		//10-16=RTC设置值 BCD格式
												Rx8130_Writ_Byte(0x64);
												Rx8130_Writ_Byte(0x10);
												Rx8130_Writ_Byte(TimeS[6]);	//秒
												Rx8130_Writ_Byte(TimeS[5]);	//分
												Rx8130_Writ_Byte(TimeS[4]);	//时
												Rx8130_Writ_Byte(TimeS[3]);	//星期
												Rx8130_Writ_Byte(TimeS[2]);	//日
												Rx8130_Writ_Byte(TimeS[1]);	//月
												Rx8130_Writ_Byte(TimeS[0]);	//年
												Rx8130_Stop();
												Rx8130_Start();		//1E-1F 00 10
												Rx8130_Writ_Byte(0x64);
												Rx8130_Writ_Byte(0x1E);
												Rx8130_Writ_Byte(0x00);	
												Rx8130_Writ_Byte(0x10);			
												Rx8130_Stop(); 
												Flog=0;
								   }
						}
}
//========================================================================================================================================
//**********************SD2058接口程序，SDA 10K上拉到3.3V**************
		



void delayus(unsigned char t)
{	
	char i;
	while(t)
	{	
		for(i=0;i<50;i++)
		{
			i=i;
		}
		t--;
	}
}


//void SDA_IN(void)
//{	
//	P3MDOUT=P3MDOUT&0xF7;
//}

//void SDA_OUT(void)
//{   
//	P3MDOUT=P3MDOUT|0x08;
//}





void i2cstart(void)
{	
	SDA_OUT();
	RTC_SDA=1;
	RTC_SCL=1;
	delayus(15);
	RTC_SDA=0;
	delayus(15);
	RTC_SCL=0;
	delayus(15);
}

void i2cstop(void)
{	
	SDA_OUT();
	RTC_SDA=0;
	RTC_SCL=1;
	delayus(15);
	RTC_SDA=1;
	delayus(15);
	SDA_IN();
}

void mack(void)
{	
	SDA_OUT();
	RTC_SDA=0;
	delayus(5);
	RTC_SCL=1;
	delayus(5);
	RTC_SCL=0;
	delayus(5);
}

void mnak(void)
{	
	SDA_OUT();
	RTC_SDA=1;
	delayus(5);
	RTC_SCL=1;
	delayus(5);
	RTC_SCL=0;
	delayus(5);
}

void cack(void)
{	
	unsigned char i;
	SDA_IN();
	RTC_SDA=1;
	delayus(5);
	RTC_SCL=1;
	delayus(5);
	for(i=0;i<50;i++)
	{   
		if(!RTC_SDA) 
		{
			break;
		}
		delayus(5);
	}
	RTC_SCL=0;
	delayus(5);
	SDA_OUT();
}

//I2C 写入1个字节
void i2cbw(unsigned char dat)
{	
	char i;
	SDA_OUT();
	for(i=0;i<8;i++)
	{	
		if(dat&0x80)
		{
			RTC_SDA=1;
		}
		else 
		{
			RTC_SDA=0;
		}
		dat=(dat<<1);
		delayus(5);
		RTC_SCL=1;
		delayus(5);
		RTC_SCL=0;
		delayus(5);
	}
	cack();
}

//i2c 读取1个字节数据
unsigned char i2cbr(void)
{	
	char i;
	unsigned char dat;
	SDA_IN();
	for(i=0;i<8;i++)
	{	
		delayus(5);
		RTC_SCL=1;
		delayus(5);
		dat=(dat<<1);
		if(RTC_SDA) 
		{
			dat=dat|0x01;
		}
		else 
		{
			dat=dat&0xFE;
		}
		//dat=(dat<<1);
		RTC_SCL=0;
		delayus(5);
	}
	return(dat);
}

//检查SD2058有没有掉电，掉电则初始化SD2058，设置时间为2021.01.01 星期五 00:00:00
void init_rtc_1(void)
{	
	unsigned char dat1,dat2;
	//检查有没有掉电
	P3MDOUT = 0x0d;		//P3 0000 1111
	i2cstart();
	i2cbw(0x64);
	i2cbw(0x0f);		//0x0f最低位
	i2cstart();
	i2cbw(0x65);
	dat2 = i2cbr();
	mack();
	dat1 = i2cbr();
	mnak();
	i2cstop();
	if(dat2&0x01)
	{
		if(dat2&0x84)		//WRTC2 WRTC3是否为0，是的话，写0
		{
			dat2 &= ~0x84;
			i2cstart();		
			i2cbw(0x64);
			i2cbw(0x0f);
			i2cbw(dat2);
			i2cstop();
		}
		dat2 &= ~0x01;
		//读WRTC1是否为0
		if(dat1 & 0x80)		//WRTC1是否为1
		{
			dat1 &= ~0x80;
			i2cstart();		
			i2cbw(0x64);
			i2cbw(0x10);
			i2cbw(dat1);
			i2cstop();
		}
		
		//写使能
		dat1 |= 0x80;
		i2cstart();		
		i2cbw(0x64);
		i2cbw(0x10);
		i2cbw(dat1);
		i2cstop();
		dat2 |= 0x84;
		i2cstart();		
		i2cbw(0x64);
		i2cbw(0x0f);
		i2cbw(dat2);
		i2cstop();
		
		
//		//重新配置时间
//		i2cstart();		//10-16=RTC设置值 BCD格式
//		i2cbw(0x64);
//		i2cbw(0x00);
//		i2cbw(0x00);	//秒
//		i2cbw(0x00);	//分
//		i2cbw(0x80);	//时，小时最高位是24-12小时配置位
//		i2cbw(0x05);	//星期
//		i2cbw(0x01);	//日
//		i2cbw(0x01);	//月
//		i2cbw(0x21);	//年
//		i2cstop();
		dat2 &= ~0x84;
		dat1 &= ~0x80;
		i2cstart();		
		i2cbw(0x64);
		i2cbw(0x10);
		i2cbw(dat1);
		i2cstop();
		i2cstart();		
		i2cbw(0x64);
		i2cbw(0x0f);
		i2cbw(dat2);
		i2cstop();
	}
}



void rtc_config(u8* prtc_set)
{
	u8 dat,dat1;
	i2cstart();
	i2cbw(0x64);
	i2cbw(0x0f);		//0x10
	i2cstart();
	i2cbw(0x65);
	dat = i2cbr();
	mack();
	dat1 = i2cbr();
	mnak();
	i2cstop();
	dat1 |= 0x80;
	i2cstart();		
	i2cbw(0x64);
	i2cbw(0x10);
	i2cbw(dat1);
	i2cstop();
	dat |= 0x84;
	i2cstart();		
	i2cbw(0x64);
	i2cbw(0x0f);
	i2cbw(dat);
	i2cstop();
	

	i2cstart();		//10-16=RTC设置值 BCD格式
	i2cbw(0x64);
	i2cbw(0x00);
	i2cbw(prtc_set[6]);	//秒
	i2cbw(prtc_set[5]);	//分
	i2cbw(prtc_set[4]);	//时
	i2cbw(prtc_set[3]);	//星期
	i2cbw(prtc_set[2]);	//日
	i2cbw(prtc_set[1]);	//月
	i2cbw(prtc_set[0]);	//年
	i2cstop();
	dat &= ~0x84;
	dat1 &= ~0x80;
	i2cstart();		
	i2cbw(0x64);
	i2cbw(0x10);
	i2cbw(dat1);
	i2cstop();
	i2cstart();		
	i2cbw(0x64);
	i2cbw(0x0f);
	i2cbw(dat);
	i2cstop();
}

//把RTC读取并处理，写到DGUS对应的变量空间，主程序中每0.5秒调用一次
void rdtime(void)
{	
	unsigned char rtcdata[8];
	unsigned char i,n,m;
	i2cstart();
	i2cbw(0x64);
	i2cbw(0x00);
	i2cstart();
	i2cbw(0x65);
	for(i=6;i>0;i--)
	{	
		rtcdata[i]=i2cbr();
		mack();
	}
	rtcdata[0]=i2cbr();
	mnak();
	i2cstop();
	rtcdata[4] &= 0x7F;
	for(i=0;i<3;i++)	//年月日转换成HEX
	{	
		n=rtcdata[i]/16;
		m=rtcdata[i]%16;
		rtcdata[i]=n*10+m;
	}
	for(i=4;i<7;i++)	//时分秒转换成HEX
	{	
		n=rtcdata[i]/16;
		m=rtcdata[i]%16;
		rtcdata[i]=n*10+m;
	}
	//星期可以不用处理
	rtcdata[7]=0;
	Write_Dgusii_Vp_byChar(0x0010,(u8*)rtcdata,8);	//写入DGUS变量空间
}

//void btn_click(u16 x,u16 y)
//{
//	u16 btn_check[4];
//	btn_check[0]=0x5aa5;
//	btn_check[1]=0x0004;
//	btn_check[2]=x;
//	btn_check[3]=y;
//	Write_Dgusii_Vp_byChar(0x00D4,(u8*)&btn_check[0],4);
//}

u8 BCD(u8 dat)
{
	  return ((dat/10)<<4)|(dat%10);
}

//u8 IBCD(u8 dat)
//{
//	 return (dat>>4)*10+(dat&0x0f);
//}





u8 rtc_get_week_1(u8 year,u8 month,u8 day)
{	
	u16 tmp,mon,y;
	u8 week;
	if((month == 1) || (month == 2))
	{
		mon = month + 12;
		y = year - 1;
	}
	else 
	{
		mon = month;
		y = year;
	}
	tmp = y + (y / 4) +(((mon + 1) * 26) / 10) + day - 36;
	week = tmp % 7;
	return week;
}





void check_rtc_set(void)
{
	u8 rtc_parm[8],rtc_set[8];
	Read_Dgusii_Vp(0x9c,rtc_parm,4);
	if((rtc_parm[0] == 0x5A) && (rtc_parm[1] == 0xA5))		//启动配置
	{
		rtc_set[0] = BCD(rtc_parm[2]);
		rtc_set[1] = BCD(rtc_parm[3]);
		rtc_set[2] = BCD(rtc_parm[4]);
		rtc_set[3] = rtc_get_week_1(rtc_parm[2],rtc_parm[3],rtc_parm[4]);
		rtc_set[4] = BCD(rtc_parm[5]);
		rtc_set[4] |= 0x80;
		rtc_set[5] = BCD(rtc_parm[6]);
		rtc_set[6] = BCD(rtc_parm[7]);
		rtc_config(rtc_set);
		rtc_parm[0] = 0;
		rtc_parm[1] = 0;
		Write_Dgusii_Vp_byChar(0x9c,rtc_parm,2);
	}
}

void     Clock_1()//SD2058
{
          if(RTC_Flog==0){ RTC_Flog=1;  init_rtc_1(); }
				       if(SysTick_RTC >= 500) 
				       {
		                rdtime();								//更新硬件RTC时间
		              	SysTick_RTC = 0;
				       }
		        	check_rtc_set();				  
}



#endif

	   
//========================================================================================================================================
void        Clock()
{
       bit k=0;
	     k=1;
#if SELECT_RTC_TYPE		    
       if(Select_rtc==1)  //RX8130
			 {
          if(RTC_Flog==0){ RTC_Flog=1;  Init_Rtc(); }
				       if(SysTick_RTC >= 500) 
				       {
										Read_Time();								//更新硬件RTC时间
										SysTick_RTC = 0;
				       }
					   RTC_Set_Time();
			 }
			 else if(Select_rtc==2)  //SD2058
			 {
			      Clock_1();
			 }
#endif				 
}