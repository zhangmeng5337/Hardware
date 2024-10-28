#include "reg51.h"
#include "intrins.h"
#include "stdio.h"

sfr     TH2     =   0xD6;
sfr     TL2     =   0xD7;
sfr     AUXR    =   0x8E;
sfr     P_SW2   =   0xBA;

sfr     P1M1    =   0x91;
sfr     P1M0    =   0x92;
sfr     P0M1    =   0x93;
sfr     P0M0    =   0x94;
sfr     P2M1    =   0x95;
sfr     P2M0    =   0x96;
sfr     P3M1    =   0xb1;
sfr     P3M0    =   0xb2;
sfr     P4M1    =   0xb3;
sfr     P4M0    =   0xb4;
sfr     P5M1    =   0xc9;
sfr     P5M0    =   0xca;

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

#define MAIN_Fosc   22118400L
#define Baudrate    115200L
#define TM          (65536 -(MAIN_Fosc/Baudrate/4))

bit B1S_Flag;

void RTC_config(void);

void UartInit(void)
{
    SCON = (SCON & 0x3f) | 0x40;
    TL2  = TM;
    TH2  = TM>>8;
    AUXR |= 0x15;
}

void UartPutc(unsigned char dat)
{
    SBUF = dat;
    while(TI==0);
    TI = 0;
}

char putchar(char c)
{
    UartPutc(c);
    return c;
}

void RTC_Isr() interrupt 13
{
    char store;

    store = P_SW2;
    P_SW2 |= 0x80;                              //使能XFR访问

    if(RTCIF & 0x08)                            //判断是否秒中断
    {
        RTCIF &= ~0x08;                         //清中断标志
        B1S_Flag = 1;
    }

    P_SW2 = store;
}

void main(void)
{
    P0M1 = 0;   P0M0 = 0;                       //设置为准双向口
    P1M1 = 0;   P1M0 = 0;                       //设置为准双向口
    P2M1 = 0;   P2M0 = 0;                       //设置为准双向口
    P3M1 = 0;   P3M0 = 0;                       //设置为准双向口
    P4M1 = 0;   P4M0 = 0;                       //设置为准双向口
    P5M1 = 0;   P5M0 = 0;                       //设置为准双向口

    UartInit();
    RTC_config();
    EA = 1;
    printf("STC8H4K64TLR RTC Test Programme!\r\n"); //UART发送一个字符串

    while (1)
    {
        if(B1S_Flag)
        {
            B1S_Flag = 0;
            P_SW2 |= 0x80;                      //使能XFR访问

            printf("Year=20%bd  ", YEAR);
            printf("Month=%bd  ", MONTH);
            printf("Day=%bd  ", DAY);
            printf("Hour=%bd  ", HOUR);
            printf("Minute=%bd  ", MIN);
            printf("Second=%bd  ", SEC);
            printf("\r\n");

            P_SW2 &= ~0x80;                     //禁止XFR访问
        }
    }
}

void RTC_config(void)
{
    P_SW2 |= 0x80;                              //使能XFR访问
    INIYEAR = 21;                               //Y:2021
    INIMONTH = 12;                              //M:12
    INIDAY = 31;                                //D:31
    INIHOUR = 23;                               //H:23
    INIMIN = 59;                                //M:59
    INISEC = 50;                                //S:50
    INISSEC = 0;                                //S/128:0
    RTCCFG |= 0x01;                             //触发RTC寄存器初始化

    RTCIF = 0;                                  //清中断标志
    RTCIEN = 0x08;                              //使能RTC秒中断
    RTCCR = 0x01;                               // RTC使能
    P_SW2 &= ~0x80;                             //禁止XFR访问
}

/*

;将以下代码保存为ASM格式文件，一起加载到项目里，例如：isr.asm

    CSEG    AT  0123H
    JMP     006BH
    END

*/

