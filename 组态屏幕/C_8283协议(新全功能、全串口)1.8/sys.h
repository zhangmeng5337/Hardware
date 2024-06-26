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



//�궨��
#define	WDT_ON()	MUX_SEL|=0x02		/******�������Ź�*********/
#define	WDT_OFF()	MUX_SEL&=0xFD		/******�رտ��Ź�*********/
#define	WDT_RST()	MUX_SEL|=0x01		/******ι��*********/


#define FOSC     206438400UL
#define T1MS    (65536-FOSC/12/1000)

#define NULL ((void *)0)
#define    TIMER_NUMBER   8  

extern u16 Send_Page_Time_Count, Pop_Time_Count;


/******************************************************************************
��������
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
void page_change(u16 Page);  //��ҳ
u16 get_ad_value(u16 adIdx);  //ad����
void pwm_init(u8 id);      //pwm��ʼ������
void pwm_set(u16 Pec,u8 id);//pwm����ռ�ձ�
void SetPinOut(u8 Port,u8 Pin);//IO��������� Port=0��Pin=1��ʾP0^1
void SetPinIn(u8 Port,u8 Pin);//IO���������� Port=1��Pin=2��ʾP1^2
u8 GetPinIn(u8 Port,u8 Pin); //��ȡIO��״̬
void TouchSwitch(u16 PageID, u8 TouchType, u8 TouchID, u8 Status);//�����رմ���
#endif

