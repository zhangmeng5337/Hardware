/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/* --- BBS: www.STCAIMCU.com  -----------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����            */
/*---------------------------------------------------------------------*/

/*************  ����˵��    **************

�����̻���STC32GΪ����оƬ��ʵ������б�д���ԡ�

ʹ��Keil C251��������Memory Model�Ƽ�����XSmallģʽ��Ĭ�϶��������edata����ʱ�Ӵ�ȡ�����ٶȿ졣

edata���鱣��1K����ջʹ�ã��ռ䲻��ʱ�ɽ������顢�����ñ�����xdata�ؼ��ֶ��嵽xdata�ռ䡣

�߼�PWM��ʱ�� PWM1P/PWM1N,PWM2P/PWM2N,PWM3P/PWM3N,PWM4P/PWM4N ÿ��ͨ�����ɶ���ʵ��PWM������������������Գ����.

8��ͨ��PWM���ö�ӦP6��8���˿�.

ͨ��P6�������ӵ�8��LED�ƣ�����PWMʵ�ֺ�����Ч��.

PWM���ں�ռ�ձȿ��Ը�����Ҫ�������ã���߿ɴ�65535.

����ʱ, ѡ��ʱ�� 24MHz (�û��������޸�Ƶ��).

******************************************/

#include "..\..\comm\STC32G.h"

#include "stdio.h"
#include "intrins.h"

typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

#define MAIN_Fosc        33000000UL

/****************************** �û������ ***********************************/

#define Timer0_Reload   (65536UL -(MAIN_Fosc / 1000))       //Timer 0 �ж�Ƶ��, 1000��/��

#define PWM1_0      0x00	//P:P1.0  N:P1.1
#define PWM1_1      0x01	//P:P2.0  N:P2.1
#define PWM1_2      0x02	//P:P6.0  N:P6.1

#define PWM2_0      0x00	//P:P1.2/P5.4  N:P1.3
#define PWM2_1      0x04	//P:P2.2  N:P2.3
#define PWM2_2      0x08	//P:P6.2  N:P6.3

#define PWM3_0      0x00	//P:P1.4  N:P1.5
#define PWM3_1      0x10	//P:P2.4  N:P2.5
#define PWM3_2      0x20	//P:P6.4  N:P6.5

#define PWM4_0      0x00	//P:P1.6  N:P1.7
#define PWM4_1      0x40	//P:P2.6  N:P2.7
#define PWM4_2      0x80	//P:P6.6  N:P6.7
#define PWM4_3      0xC0	//P:P3.4  N:P3.3

#define ENO1P       0x01
#define ENO1N       0x02
#define ENO2P       0x04
#define ENO2N       0x08
#define ENO3P       0x10
#define ENO3N       0x20
#define ENO4P       0x40
#define ENO4N       0x80

/*****************************************************************************/


/*************  ���س�������    **************/

#define PWM_PERIOD  1023    //��������ֵ

/*************  ���ر�������    **************/

bit B_1ms;          //1ms��־

u16 PWM1_Duty;
u16 PWM2_Duty;
u16 PWM3_Duty;
u16 PWM4_Duty;

bit PWM1_Flag;
bit PWM2_Flag;
bit PWM3_Flag;
bit PWM4_Flag;

void UpdatePwm(void);

/********************* ������ *************************/
void main(void)
{
    WTST = 0;  //���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
    EAXFR = 1; //��չ�Ĵ���(XFR)����ʹ��
    CKCON = 0; //��߷���XRAM�ٶ�

    P0M1 = 0x30;   P0M0 = 0x30;   //����P0.4��P0.5Ϊ©����·(ʵ��������������赽3.3V)
    P1M1 = 0x30;   P1M0 = 0x30;   //����P1.4��P1.5Ϊ©����·(ʵ��������������赽3.3V)
    P2M1 = 0x3c;   P2M0 = 0x3c;   //����P2.2~P2.5Ϊ©����·(ʵ��������������赽3.3V)
    P3M1 = 0x50;   P3M0 = 0x50;   //����P3.4��P3.6Ϊ©����·(ʵ��������������赽3.3V)
    P4M1 = 0x3c;   P4M0 = 0x3c;   //����P4.2~P4.5Ϊ©����·(ʵ��������������赽3.3V)
    P5M1 = 0x0c;   P5M0 = 0x0c;   //����P5.2��P5.3Ϊ©����·(ʵ��������������赽3.3V)
    P6M1 = 0xff;   P6M0 = 0xff;   //����Ϊ©����·(ʵ��������������赽3.3V)
    P7M1 = 0x00;   P7M0 = 0x00;   //����Ϊ׼˫���

    PWM1_Flag = 0;
    PWM2_Flag = 0;
    PWM3_Flag = 0;
    PWM4_Flag = 0;
    
    PWM1_Duty = 0;
    PWM2_Duty = 256;
    PWM3_Duty = 512;
    PWM4_Duty = 1024;

    //  Timer0��ʼ��
    AUXR = 0x80;    //Timer0 set as 1T, 16 bits timer auto-reload, 
    TH0 = (u8)(Timer0_Reload / 256);
    TL0 = (u8)(Timer0_Reload % 256);
    ET0 = 1;    //Timer0 interrupt enable
    TR0 = 1;    //Tiner0 run

    PWMA_CCER1 = 0x00; //д CCMRx ǰ���������� CCxE �ر�ͨ��
    PWMA_CCER2 = 0x00;
    PWMA_CCMR1 = 0x60; //ͨ��ģʽ����
    PWMA_CCMR2 = 0x60;
    PWMA_CCMR3 = 0x60;
    PWMA_CCMR4 = 0x60;
    PWMA_CCER1 = 0x55; //����ͨ�����ʹ�ܺͼ���
    PWMA_CCER2 = 0x55;

    PWMA_CCMR1 |= 0x08; //����PWMA_CCRxԤת�ع���(��ҪCCxE=1�ſ�д)
    PWMA_CCMR2 |= 0x08;
    PWMA_CCMR3 |= 0x08;
    PWMA_CCMR4 |= 0x08;

    PWMA_ARRH = (u8)(PWM_PERIOD >> 8); //��������ʱ��
    PWMA_ARRL = (u8)PWM_PERIOD;

    PWMA_ENO = 0x00;
    PWMA_ENO |= ENO1P; //ʹ�����
    PWMA_ENO |= ENO1N; //ʹ�����
    PWMA_ENO |= ENO2P; //ʹ�����
    PWMA_ENO |= ENO2N; //ʹ�����
    PWMA_ENO |= ENO3P; //ʹ�����
    PWMA_ENO |= ENO3N; //ʹ�����
    PWMA_ENO |= ENO4P; //ʹ�����
    PWMA_ENO |= ENO4N; //ʹ�����

    PWMA_PS = 0x00;  //�߼� PWM ͨ�������ѡ��λ
    PWMA_PS |= PWM1_2; //ѡ�� PWM1_2 ͨ��
    PWMA_PS |= PWM2_2; //ѡ�� PWM2_2 ͨ��
    PWMA_PS |= PWM3_2; //ѡ�� PWM3_2 ͨ��
    PWMA_PS |= PWM4_2; //ѡ�� PWM4_2 ͨ��

    PWMA_BKR = 0x80;  //ʹ�������
    PWMA_CR1 |= 0x81; //ʹ��ARRԤװ��,��ʼ��ʱ

    P40 = 0;	//��LED����
    EA = 1;     //�����ж�

    while (1);
}

/********************** Timer0 1ms�жϺ��� ************************/
void timer0(void) interrupt 1
{
    if(!PWM1_Flag)
    {
        PWM1_Duty++;
        if(PWM1_Duty > PWM_PERIOD) PWM1_Flag = 1;
    }
    else
    {
        PWM1_Duty--;
        if(PWM1_Duty <= 0) PWM1_Flag = 0;
    }

    if(!PWM2_Flag)
    {
        PWM2_Duty++;
        if(PWM2_Duty > PWM_PERIOD) PWM2_Flag = 1;
    }
    else
    {
        PWM2_Duty--;
        if(PWM2_Duty <= 0) PWM2_Flag = 0;
    }

    if(!PWM3_Flag)
    {
        PWM3_Duty++;
        if(PWM3_Duty > PWM_PERIOD) PWM3_Flag = 1;
    }
    else
    {
        PWM3_Duty--;
        if(PWM3_Duty <= 0) PWM3_Flag = 0;
    }

    if(!PWM4_Flag)
    {
        PWM4_Duty++;
        if(PWM4_Duty > PWM_PERIOD) PWM4_Flag = 1;
    }
    else
    {
        PWM4_Duty--;
        if(PWM4_Duty <= 0) PWM4_Flag = 0;
    }
    
    UpdatePwm();
}

//========================================================================
// ����: UpdatePwm(void)
// ����: ����PWMռ�ձ�. 
// ����: none.
// ����: none.
// �汾: V1.0, 2012-11-22
//========================================================================
void UpdatePwm(void)
{
    PWMA_CCR1H = (u8)(PWM1_Duty >> 8); //����ռ�ձ�ʱ��
    PWMA_CCR1L = (u8)(PWM1_Duty);
    PWMA_CCR2H = (u8)(PWM2_Duty >> 8); //����ռ�ձ�ʱ��
    PWMA_CCR2L = (u8)(PWM2_Duty);
    PWMA_CCR3H = (u8)(PWM3_Duty >> 8); //����ռ�ձ�ʱ��
    PWMA_CCR3L = (u8)(PWM3_Duty);
    PWMA_CCR4H = (u8)(PWM4_Duty >> 8); //����ռ�ձ�ʱ��
    PWMA_CCR4L = (u8)(PWM4_Duty);
}
