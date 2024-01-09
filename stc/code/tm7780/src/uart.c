#include "uart.h"
#include "intrins.h"
#include "stdio.h"

#define FOSC        12000000UL
#define BRT         (65536 - FOSC / 9600 / 4)
unsigned char cmd[] = {0x01, 0x03, 0x00, 0x39, 0x00, 0x34, 0x94, 0x10};
bit busy;
char wptr;
char rptr;
char buffer[16];

uart_stru uart_usr;

void UartSend(char dat);

char putchar(char c)
{
    while (busy);
    busy = 1;
    SBUF = c;
	return c;	
}




void UartIsr() interrupt 4
{
    if (TI)
    {
        TI = 0;
        busy = 0;
    }
    if (RI)
    {
        RI = 0;
		
        uart_usr.recv_buf[uart_usr.wptr++] = SBUF;
		//SBUF  = uart_usr.recv_buf[uart_usr.wptr];  
        uart_usr.recv_flag = 1;
        uart_usr.wptr &= 0x0f;
    }
}

void UartInit()
{
//    SCON = 0x50;
//    T2L = BRT;
//    T2H = BRT >> 8;
//    AUXR = 0x15;
//    wptr = 0x00;
//    rptr = 0x00;
//    busy = 0;


	SCON = 0x50;		//8位数�?可变波特�?
	AUXR |= 0x01;		//串口1选择定时�?为波特率发生�?
	AUXR |= 0x04;		//定时器时�?T模式
	T2L = 0xC7;			//设置定时初始�?
	T2H = 0xFE;			//设置定时初始�?
	AUXR |= 0x10;		//定时�?开始计�?
	busy = 0;
    ES = 1;
    EA = 1;


	
}

void UartSend(char dat)
{
    while (busy);
    busy = 1;
    SBUF = dat;
}

void UartSendStr(char *p)
{
    while (*p)
    {
        UartSend(*p++);
    }
}
void UartSendbyte(char *p,unsigned int len)
{
    while (len--)
    {
        UartSend(*p++);
    }
}

uart_stru *get_uart(void)
{
 return &uart_usr;
}


