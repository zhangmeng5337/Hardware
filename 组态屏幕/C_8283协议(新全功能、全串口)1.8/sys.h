#ifndef __SYS_H__
#define __SYS_H__

#include "t5los8051.h"
#include "sys.h"

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned long   u32;
typedef char            s8;
typedef short           s16;
typedef long            s32;



//宏定义
#define	WDT_ON()	MUX_SEL|=0x02		/******开启看门狗*********/
#define	WDT_OFF()	MUX_SEL&=0xFD		/******关闭看门狗*********/
#define	WDT_RST()	MUX_SEL|=0x01		/******喂狗*********/


#define FOSC     206438400UL
#define T1MS    (65536-FOSC/12/1000)

#define NULL ((void *)0)
#define    TIMER_NUMBER   8  

extern u16 Send_Page_Time_Count, Pop_Time_Count;


/******************************************************************************
函数声明
******************************************************************************/
void INIT_CPU(void);
void PORT_Init(void);
void T0_Init(void);
void T1_Init(void);
void T2_Init(void);

void delay_ms(u16 n);
void delay_us(u16 n);

void Read_Dgusii_Vp(unsigned int addr,unsigned char *buf,unsigned int len);
void Write_Dgusii_Vp(unsigned int addr,unsigned char *buf,unsigned int len);
void Write_Dgusii_Vp_byChar(unsigned int addr,unsigned char *buf,unsigned int len);
void T5L_Flash(unsigned char mod,unsigned int addr,long addr_flash,unsigned int len);
u16  Read_Dgus(u16 Dgus_Addr);
void Write_Dgus(u16 Dgus_Addr,u16 Val);
void start_timer(u8 ID, u16 nTime); //
u8   get_time_out_flag(u8 ID);//
void page_change(u16 Page);  //切页
u16 get_ad_value(u16 adIdx);  //ad配置
void pwm_init(u8 id);      //pwm初始化配置
void pwm_set(u16 Pec,u8 id);//pwm配置占空比
void SetPinOut(u8 Port,u8 Pin);//IO口输出设置 Port=0，Pin=1表示P0^1
void SetPinIn(u8 Port,u8 Pin);//IO口输入设置 Port=1，Pin=2表示P1^2
u8 GetPinIn(u8 Port,u8 Pin); //获取IO口状态
void TouchSwitch(u16 PageID, u8 TouchType, u8 TouchID, u8 Status);//开启关闭触控
#endif

