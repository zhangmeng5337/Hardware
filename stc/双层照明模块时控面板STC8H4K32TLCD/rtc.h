


//sfr     AUXR    =   0x8E;
//sfr     P_SW2   =   0xBA;


#define RTCCR       (*(unsigned char volatile xdata *)0xfe60)
#define RTCCFG      (*(unsigned char volatile xdata *)0xfe61)
#define RTCIEN      (*(unsigned char volatile xdata *)0xfe62)
#define RTCIF       (*(unsigned char volatile xdata *)0xfe63)
#define ALAHOUR     (*(unsigned char volatile xdata *)0xfe64)
#define ALAMIN      (*(unsigned char volatile xdata *)0xfe65)
#define ALASEC      (*(unsigned char volatile xdata *)0xfe66)
#define ALASSEC     (*(unsigned char volatile xdata *)0xfe67)
#define INIYEAR     (*(unsigned char volatile xdata *)0xfe68)
#define INIMONTH    (*(unsigned char volatile xdata *)0xfe69)
#define INIDAY      (*(unsigned char volatile xdata *)0xfe6a)
#define INIHOUR     (*(unsigned char volatile xdata *)0xfe6b)
#define INIMIN      (*(unsigned char volatile xdata *)0xfe6c)
#define INISEC      (*(unsigned char volatile xdata *)0xfe6d)
#define INISSEC     (*(unsigned char volatile xdata *)0xfe6e)
#define YEAR        (*(unsigned char volatile xdata *)0xfe70)
#define MONTH       (*(unsigned char volatile xdata *)0xfe71)
#define DAY         (*(unsigned char volatile xdata *)0xfe72)
#define HOUR        (*(unsigned char volatile xdata *)0xfe73)
#define MIN         (*(unsigned char volatile xdata *)0xfe74)
#define SEC         (*(unsigned char volatile xdata *)0xfe75)
#define SSEC        (*(unsigned char volatile xdata *)0xfe76)
#define WEEK        (*(unsigned char volatile xdata *)0xfe6f)



#define MAIN_Fosc   22118400L
#define Baudrate    115200L
#define TM          (65536 -(MAIN_Fosc/Baudrate/4))

bit B1S_Flag;

void RTC_config(void);





void I2CWriteDate(unsigned char miao_s,unsigned char fen_s,unsigned char shi_s,unsigned char zhou_s,unsigned char ri_s,unsigned char yue_s,unsigned char nian_s)
{
    P_SW2 |= 0x80;                              //使能XFR访问 
	  WEEK = zhou_s;
    INIYEAR = nian_s;                               //Y:2021
    INIMONTH = yue_s;                              //M:12
    INIDAY = ri_s;                                //D:31
    INIHOUR = shi_s;                               //H:23
    INIMIN = fen_s;                                //M:59
    INISEC = miao_s;                                //S:50
    INISSEC = 0;                                //S/128:0
   
    RTCCFG |= 0x01;                             //触发RTC寄存器初始化

    RTCIF = 0;                                  //清中断标志
    RTCIEN = 0x00;                              //禁止RTC秒中断
    RTCCR = 0x01;                               // RTC使能
    P_SW2 &= ~0x80;                             //禁止XFR访问
}


bit I2CReadDate(void)
{

	miao=SEC;
	fen=MIN;
	shi=HOUR;
	zhou=WEEK;
	ri=DAY;
	yue=MONTH;
	nian=YEAR;
	shi=shi&0x3f;
	return	1;
}

/*

;将以下代码保存为ASM格式文件，一起加载到项目里，例如：isr.asm

    CSEG    AT  0123H
    JMP     006BH
    END

*/


