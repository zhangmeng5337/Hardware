/******************************************************************************

                  ��Ȩ���� (C), 2019, �������ĿƼ����޹�˾

 ******************************************************************************
  �� �� ��   : sys.c
  �� �� ��   : V2.0
  ��    ��   : LiuYanZhou
  ��������   : 2019��7��6��
  ��������   : 
  �޸���ʷ   :
  1.��    ��   : 
    ��    ��   : 
    �޸�����   : 
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
	�� �� ��  : void INIT_CPU(void)
	��������  : CPU��ʼ������
							����ʵ��ʹ�������޸Ļ򵥶�����
	�������  :	
	�������  : 
	�޸���ʷ  :
	1.��    ��   : 2019��4��1��
		��    ��   : LiuYanZhou
		�޸�����   : ����
*****************************************************************************/ 
void INIT_CPU(){	
	
	IEN0 = 0x00;      //�ر������ж�
	IEN1 = 0x00;
	IEN2 = 0x00;
	IP0 = 0x00;				//�ж����ȼ���Ĭ��ֵ
	IP1 = 0x00;
	
	P0 = 0x00;				//IO���ã�����رգ��ǲ������ȡ����P0MDOUT   
	P1 = 0x00;
	P2 = 0x00;
	P3 = 0x00;
	
	TMOD = 0x11;			//T0 T1 16bit��ʱ��
	TCON = 0x05;			//EX0 EX1���������ж�
}

/*****************************************************************************
 �� �� ��  : void PORT_Init(void)
 ��������  : �˿ڳ�ʼ������
 �������  :	
 �������  : 
 �޸���ʷ  :
  1.��    ��   : 2019��4��1��
    ��    ��   : LiuYanZhou
    �޸�����   : ����
*****************************************************************************/ 
void PORT_Init(void){
	MUX_SEL = 0x00;	//������uart2 ������ uart3 ΪIO��
	PORTDRV = 0x01;   //IO����ǿ�� +/-8mA
	P0MDOUT = 0x73;		//P0 0111 0000   P0^4 ǿ��  P0^5 ǿ��   P0^6 ǿ��    ΪIO�����ģʽP0^0��P0^1Ϊ485�������
	P1MDOUT = 0x00;		//P1 0000 0000
	P2MDOUT = 0x00;		//P2 0000 0000
	P3MDOUT = 0x0d;		//P3 0000 1111
}

/*****************************************************************************
 �� �� ��  : void T0_Init(void)
 ��������  : ��ʱ��0��ʼ��	��ʱ���1ms
 �������  :	 
 �������  : 
 �޸���ʷ  :
  1.��    ��   : 2019��7��6��
    ��    ��   : LiuYanZhou
    �޸�����   : ����
*****************************************************************************/
void T0_Init(void){
	TMOD |= 0x01;
	TH0 = T1MS>>8;        //1ms��ʱ��
	TL0 = T1MS;
	ET0 = 1;              //������ʱ��0�ж�
	TR0 = 1;              //������ʱ��
	EA = 1;               //�����ж�
}

#if 0
/*****************************************************************************
 �� �� ��  : void T1_Init(void)
 ��������  : ��ʱ��1��ʼ��	��ʱ���1ms
 �������  :	 
 �������  : 
 �޸���ʷ  :
  1.��    ��   : 2019��7��6��
    ��    ��   : LiuYanZhou
    �޸�����   : ����
*****************************************************************************/
void T1_Init(void){
	TMOD |= 0x10;
	TH1 = 0;        			//1ms��ʱ��
	TL1 = 0;
	ET1 = 1;              //������ʱ��0�ж�
	TR1 = 1;              //������ʱ��
	EA = 1;               //�����ж�
}
#endif

/*****************************************************************************
 �� �� ��  : void T2_Init(void)
 ��������  : ��ʱ��2��ʼ��	��ʱ���1ms
 �������  :	 
 �������  : 
 �޸���ʷ  :
  1.��    ��   : 2019��7��6��
    ��    ��   : LiuYanZhou
    �޸�����   : ����
*****************************************************************************/
void T2_Init(void){
	T2CON = 0x70;
	TH2 = 0x00;
	TL2 = 0x00;
	TRL2H = 0xBC;			//1ms�Ķ�ʱ��
	TRL2L = 0xCD;
	ET2 = 1;					//������ʱ��
	TR2 = 1;
	EA=1;
}

/*****************************************************************************
 �� �� ��  : void T0_ISR_PC(void)    interrupt 1
 ��������  : ��ʱ��0��������1��������
 �������  :	 
 �������  : 
 �޸���ʷ  :
  1.��    ��   : 2019��7��6��
    ��    ��   : LiuYanZhou
    �޸�����   : ����
*****************************************************************************/
void T0_ISR_PC(void)    interrupt 1
{
	EA=0;	
	TH0 = T1MS >> 8;
	TL0 = T1MS;

	Send_Page_Time_Count++;		//��ʱ����ҳ�����
		
	if(Pop_Time_Count && Pop_Time_Count != 0xFFFF){
		Pop_Time_Count--;
	}
	EA=1;
}



#if 0
/*****************************************************************************
 �� �� ��  : void T1_ISR_PC(void)    interrupt 3
 ��������  : ��ʱ��1���������ṩRTC��ʱ�ʹ���
 �������  :	 
 �������  : 
 �޸���ʷ  :
  1.��    ��   : 2019��7��6��
    ��    ��   : LiuYanZhou
    �޸�����   : ����
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
 �� �� ��  : void T2_ISR_PC(void)    interrupt 5
 ��������  : ��ʱ��2�жϴ��������ṩ��ʱ����������ι��
 �������  :	 
 �������  : 
 �޸���ʷ  :
  1.��    ��   : 2019��7��6��
    ��    ��   : LiuYanZhou
    �޸�����   : ����
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
 �� �� ��  : void start_timer(u8 ID, u16 nTime)
 ��������  : ����һ����ʱ����
 �������  :	 u8 ID         ��ʱ��ID
               u16 nTime     ��ʱms��

  1.��    ��   : 2022-06-17
    ��    ��   : cuijia
    �޸�����   : ����
*****************************************************************************/
void start_timer(u8 ID, u16 nTime)
{
       if(ID<TIMER_NUMBER) 
			 {
			      T_number_en[ID]= 1;   //��ʱ��ʹ��
				    T_number_arr[ID]= nTime; 
			 }  
}

/*****************************************************************************
 �� �� ��  : void stop_timer(u8 ID)
 ��������  : �ر�һ����ʱ����
 �������  :	 u8 ID         ��ʱ��ID
  1.��    ��   : 2022-06-17
    ��    ��   : cuijia
    �޸�����   : ����
*****************************************************************************/
void stop_timer(u8 ID)
{
	   if(ID<TIMER_NUMBER) 
        T_number_en[ID]= 0;   //��ʱ���ر�
}
/*****************************************************************************
 �� �� ��  : u8 get_time_out_flag(u8 ID)
 ��������  : ��ö�ʱ���Ƿ�ʱ
 �������  :	u8 ID  ��ʱ��ID
 �������  :  0  δ��ʱ    ��0  ��ʱ
 �޸���ʷ  :
  1.��    ��   : 2022-06-17
    ��    ��   : cuijia
    �޸�����   : ����
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
 �� �� ��  : void delay_ms(u16 n)
 ��������  : ��ʱ������ʹ�ö�ʱ��2Ӳ����ʱ
 �������  :	 
 �������  : 
 �޸���ʷ  :
  1.��    ��   : 2019��7��6��
    ��    ��   : LiuYanZhou
    �޸�����   : ����
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
 �� �� ��  :void Read_Dgusii_Vp(unsigned int addr,unsigned char *buf,unsigned int len)
 ��������  :��DGUS����
 �������  :	 
 �������  : 
 �޸���ʷ  :
  1.��    ��   : 2019��7��20��
    ��    ��   : DWIN
    �޸�����   : LiuYanZhou
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
 �� �� ��  :void Write_Dgusii_Vp(unsigned int addr,unsigned char *buf,unsigned int len)
 ��������  :дDGUS���� //дDGUS�������� addr=DGUS������ַ  *buf=д������ָ��  len=д�������ֳ���
 �������  :	 
 �������  : 
 �޸���ʷ  :
  1.��    ��   : 2019��7��20��
    ��    ��   : DWIN
    �޸�����   : LiuYanZhou
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
 �� �� ��  :u16 Read_Dgus(u16 Dgus_Addr)��void Write_Dgus(u16 Dgus_Addr,u16 Val)
 ��������  :��DGUS���� //дDGUS�������� Dgus_Addr=DGUS������ַ    Val=д�������ֳ���
 �������  :	 
 �������  : 
 �޸���ʷ  :
  1.��    ��   : 2021��9��30��
    ��    ��   : DWIN
    �޸�����   : wenyu
*****************************************************************************/
u16 Read_Dgus(u16 Dgus_Addr)
{	   	   	u16 R_Dgus=0;

			ADR_H = 0x00;
			ADR_M = (u8)((Dgus_Addr/2)>>8);
			ADR_L = (u8)(Dgus_Addr/2);
			RAMMODE = 0xAF;		//������
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
			RAMMODE = 0xAF;		//������
			while(!APP_ACK);
			APP_EN = 1;
			while(APP_EN);
			RAMMODE = 0;
		   	ADR_M = (u8)((Dgus_Addr/2)>>8);
			ADR_L = (u8)(Dgus_Addr/2);
			RAMMODE = 0x8F;		//д����
		    while(!APP_ACK);
			if(Dgus_Addr%2){DATA1=(u8)(Val>>8); DATA0=(u8)(Val); }
			else { DATA3 =(u8)(Val>>8);	DATA2 =(u8)(Val);}
		    APP_EN = 1;
			while(APP_EN);
			RAMMODE = 0;
		
}

//���ָ��DGUS������ַ��DATA3����Ļ����˳�
void wait_ok(unsigned int addr)
{	
	ADR_H=0x00;
	ADR_M=(unsigned char)(addr>>8);
	ADR_L=(unsigned char)(addr);
	ADR_INC=0x00;
	do
	{ 
		for(addr=0;addr<1000;addr++)	//�ͷű����ռ�һ��ʱ��
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
	//��дT5LƬ��256KW Flash��mod=0x5A Ϊ��ȡ��mod=0xA5Ϊд��
	//addr=DGUS������ַ��������ż����addr_flash=flash��ȡ��ַ��������ż����len=��ȡ�ֳ��ȣ�������ż����
void T5L_Flash(unsigned char mod,unsigned int addr,long addr_flash,unsigned int len)
{	
	ADR_H=0x00;
	ADR_M=0x00;
	ADR_L=0x04;
	ADR_INC=0x01;
	RAMMODE=0x8F;		//������Flash
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
}	//�ȴ����ݶ�ȡOK
#if 0
/*****************************************************************************
 �� �� ��  : hdMAC(u8 *a, u8 *b, u8 *c) ; hdDIV(u8 *a, u8 *b, u8 *c)
 ��������  : �˷�����������������T5L��ѧ���㵥Ԫ��MDU������
 �������  :	 
 �������  : 
 �޸���ʷ  :
  1.��    ��   : 2022��4��15��
    ��    ��   : cuijia
    �޸�����   : ����
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
    MAC_MODE = 0XD8;//ִ��һ��c=a*b+c��64λ�����ų˷�����
	  while(MAC_MODE&0x80);//�ȴ�Ӳ��ִ�����
    EXADR = 0X10;
    for (i = 0; i < 8; i++)
        c[i] = EXDATA;//������
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
    DIV_MODE = 0X88;//ִ��һ��c/a = a��b��64λ�����ų�������,��������������
	  while(DIV_MODE&0x80);//�ȴ�Ӳ��ִ�����
    EXADR = 0X00;
    for (i = 0; i < 8; i++)
        a[i] = EXDATA;//������
    for (i = 0; i < 8; i++)
        b[i] = EXDATA;//������
}

/****************************************
���ƣ�void page_change(u16 Page)
���ܣ���ҳ
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
���ƣ�int get_ad_value(int adIdx)
���ܣ���ȡADֵ ����adIdxΪͨ����
*****/
u16 get_ad_value(u16 adIdx)
{
    if(adIdx<8)
		{
		   return Read_Dgus(0x32+adIdx);
		}
		return 0;
}

#define       PWM_ACCURACY	        0x2042         //PWM����
/**************************
���ƣ�void  pwm_init(u8 id)
���ܣ�id=0��1��Ŀǰ֧��������·PWM
**/
void pwm_init(u8 id)
{
	if(id==0)
	{
	   Write_Dgus(0x87,PWM_ACCURACY);    // �� PWM_ACCURACY= 0x2042 ����Ƶ��  100kHz=825.7536MHz/0x2042(8258)    
     Write_Dgus(0x86,0x5A01);   //���÷�Ƶϵ�� pwm0����100khz/1=100khz
	}else if(id==1)
	{
		 Write_Dgus(0x89,PWM_ACCURACY);    // �� PWM_ACCURACY= 0x2042 ����Ƶ��  100kHz=825.7536MHz/0x2042(8258)    
     Write_Dgus(0x88,0x5A01);  //���÷�Ƶϵ�� pwm0����100khz/1=100khz
	}
}

/**************************
���ƣ�void  pwm_set(u16 Pec,u8 id)
���ܣ�pwm ���� ����PecΪռ�ձ�,id=0��1��Ŀǰ֧��������·PWM
**/
void pwm_set(u16 Pec,u8 id)
{
	 u16 Val=0;
	if(id==0)
	{
	   Val=(u32)Pec*PWM_ACCURACY/100;
     Write_Dgus(0x92,Val);  	 //����ռ�ձ�		
	}else if(id==1)
	{
	   Val=(u32)Pec*PWM_ACCURACY/100;
     Write_Dgus(0x93,Val); 
	}
}

/**************************
���ƣ�void SetPinOut(u8 Port,u8 Pin)
���ܣ�IO��������� Port=0��Pin=1��ʾP0^1
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
���ƣ�void SetPinIn(u8 Port,u8 Pin)
���ܣ�IO���������� Port=1��Pin=2��ʾP1^2
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
���ƣ�TouchSwitch(u16 PageID, u8 TouchType, u8 TouchID, u8 Status)
���ܣ������رմ��� PageIDҳ��ţ�TouchType�������ͣ�TouchID����ID��Status����1�رմ���=0
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