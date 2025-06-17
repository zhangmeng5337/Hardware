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

����3ȫ˫���жϷ�ʽ�շ�ͨѶ����

ͨ��PC��MCU��������, MCU���յ��������Զ�����DMA�ռ�.

��DMA�ռ�������ô�С�����ݺ�ͨ������3��DMA�Զ����͹��ܰѴ洢�ռ������ԭ������.

�ö�ʱ���������ʷ�����������ʹ��1Tģʽ(���ǵͲ�������12T)����ѡ��ɱ�������������ʱ��Ƶ�ʣ�����߾��ȡ�

����ʱ, ѡ��ʱ�� 22.1184MHz (�û��������޸�Ƶ��).

******************************************/

#include "..\..\comm\STC32G.h"

#include "stdio.h"
#include "intrins.h"

typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

#define MAIN_Fosc   22118400L   //������ʱ�ӣ���ȷ����115200�����ʣ�
#define Baudrate3   115200L

bit	DmaTxFlag;
bit	DmaRxFlag;

u8 xdata DmaBuffer[256];

void UART3_config(u8 brt);   // ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer3��������.
void DMA_Config(void);


void UartPutc(unsigned char dat)
{
    S3BUF = dat; 
    while(S3TI == 0);
    S3TI = 0;
}

char putchar(char c)
{
	UartPutc(c);
	return c;
}

//========================================================================
// ����: void main(void)
// ����: ��������
// ����: none.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void main(void)
{
	u16 i;

    WTST = 0;  //���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
    EAXFR = 1; //��չ�Ĵ���(XFR)����ʹ��
    CKCON = 0; //��߷���XRAM�ٶ�

    P0M1 = 0x30;   P0M0 = 0x30;   //����P0.4��P0.5Ϊ©����·(ʵ��������������赽3.3V)
    P1M1 = 0x32;   P1M0 = 0x32;   //����P1.1��P1.4��P1.5Ϊ©����·(ʵ��������������赽3.3V), P1.1��PWM��DAC��·ͨ�����贮����P2.3
    P2M1 = 0x3c;   P2M0 = 0x3c;   //����P2.2~P2.5Ϊ©����·(ʵ��������������赽3.3V)
    P3M1 = 0x50;   P3M0 = 0x50;   //����P3.4��P3.6Ϊ©����·(ʵ��������������赽3.3V)
    P4M1 = 0x3c;   P4M0 = 0x3c;   //����P4.2~P4.5Ϊ©����·(ʵ��������������赽3.3V)
    P5M1 = 0x0c;   P5M0 = 0x0c;   //����P5.2��P5.3Ϊ©����·(ʵ��������������赽3.3V)
    P6M1 = 0xff;   P6M0 = 0xff;   //����Ϊ©����·(ʵ��������������赽3.3V)
    P7M1 = 0x00;   P7M0 = 0x00;   //����Ϊ׼˫���

	for(i=0; i<256; i++)
	{
		DmaBuffer[i] = i;
	}

	UART3_config(0);    // ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer3��������.
	printf("STC32G UART3 DMA Test Programme!\r\n");  //UART3����һ���ַ���
	
	DMA_Config();
	EA = 1; //�������ж�

	DmaTxFlag = 0;
	DmaRxFlag = 0;
	
	while(1)
	{
		if((DmaTxFlag) && (DmaRxFlag))
		{
			DmaTxFlag = 0;
			DMA_UR3T_CR = 0xc0;			//bit7 1:ʹ�� UART3_DMA, bit6 1:��ʼ UART3_DMA �Զ�����
			DmaRxFlag = 0;
			DMA_UR3R_CR = 0xa1;			//bit7 1:ʹ�� UART3_DMA, bit5 1:��ʼ UART3_DMA �Զ�����, bit0 1:��� FIFO
		}
	}
}

//========================================================================
// ����: void DMA_Config(void)
// ����: UART DMA ��������.
// ����: none.
// ����: none.
// �汾: V1.0, 2021-5-6
//========================================================================
void DMA_Config(void)
{
	DMA_UR3T_CFG = 0x80;		//bit7 1:Enable Interrupt
	DMA_UR3T_STA = 0x00;
	DMA_UR3T_AMT = 0xff;		//���ô������ֽ���(��8λ)��n+1
	DMA_UR3T_AMTH = 0x00;		//���ô������ֽ���(��8λ)��n+1
	DMA_UR3T_TXAH = (u8)((u16)&DmaBuffer >> 8);
	DMA_UR3T_TXAL = (u8)((u16)&DmaBuffer);
	DMA_UR3T_CR = 0xc0;			//bit7 1:ʹ�� UART3_DMA, bit6 1:��ʼ UART3_DMA �Զ�����

	DMA_UR3R_CFG = 0x80;		//bit7 1:Enable Interrupt
	DMA_UR3R_STA = 0x00;
	DMA_UR3R_AMT = 0xff;		//���ô������ֽ���(��8λ)��n+1
	DMA_UR3R_AMTH = 0x00;		//���ô������ֽ���(��8λ)��n+1
	DMA_UR3R_RXAH = (u8)((u16)&DmaBuffer >> 8);
	DMA_UR3R_RXAL = (u8)((u16)&DmaBuffer);
	DMA_UR3R_CR = 0xa1;			//bit7 1:ʹ�� UART3_DMA, bit5 1:��ʼ UART3_DMA �Զ�����, bit0 1:��� FIFO
}

//========================================================================
// ����: SetTimer2Baudraye(u16 dat)
// ����: ����Timer2�������ʷ�������
// ����: dat: Timer2����װֵ.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void SetTimer2Baudraye(u16 dat)  // ʹ��Timer2��������.
{
    T2R = 0;	//Timer stop
    T2_CT = 0;	//Timer2 set As Timer
    T2x12 = 1;	//Timer2 set as 1T mode
	T2H = dat / 256;
	T2L = dat % 256;
    ET2 = 0;    //��ֹ�ж�
    T2R = 1;	//Timer run enable
}

//========================================================================
// ����: void UART3_config(u8 brt)
// ����: UART3��ʼ��������
// ����: brt: ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer3��������.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void UART3_config(u8 brt)    // ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer3��������.
{
	if(brt == 2)
	{
		SetTimer2Baudraye((u16)(65536UL - (MAIN_Fosc / 4) / Baudrate3));
		S3CON = 0x10;       //8λ����, ʹ��Timer2�������ʷ�����, �������
	}
	else
	{
		S3CON = 0x50;       //8λ����, ʹ��Timer3�������ʷ�����, �������
		T3H = (65536UL - (MAIN_Fosc / 4) / Baudrate3) / 256;
		T3L = (65536UL - (MAIN_Fosc / 4) / Baudrate3) % 256;
        T3_CT = 0;	//Timer3 set As Timer
        T3x12 = 1;	//Timer3 set as 1T mode
        T3R = 1;	//Timer run enable
	}
//    ES3  = 1;       //����UART3�ж�
    S3_S = 0;       //UART3 switch bit1 to: 0: P0.0 P0.1,  1: P5.0 P5.1
}

//========================================================================
// ����: void UART3_int (void) interrupt UART3_VECTOR
// ����: UART3�жϺ�����
// ����: nine.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
//void UART3_int (void) interrupt 17
//{
//    if(S3RI)
//    {
//        S3RI = 0;    //Clear Rx flag
//        RX3_Buffer[RX3_Cnt] = S3BUF;
//        if(++RX3_Cnt >= UART3_BUF_LENGTH)   RX3_Cnt = 0;
//    }

//    if(S3TI)
//    {
//        S3TI = 0;   //Clear Tx flag
//        B_TX3_Busy = 0;
//    }
//}

//========================================================================
// ����: void UART3_DMA_Interrupt (void) interrupt 54/55
// ����: UART3 DMA�жϺ���
// ����: none.
// ����: none.
// �汾: VER1.0
// ����: 2021-5-8
// ��ע: 
//========================================================================
void UART3_DMA_Interrupt(void) interrupt 13
{
	if (DMA_UR3T_STA & 0x01)	//�������
	{
		DMA_UR3T_STA &= ~0x01;
		DmaTxFlag = 1;
	}
	if (DMA_UR3T_STA & 0x04)	//���ݸ���
	{
		DMA_UR3T_STA &= ~0x04;
	}
	
	if (DMA_UR3R_STA & 0x01)	//�������
	{
		DMA_UR3R_STA &= ~0x01;
		DmaRxFlag = 1;
	}
	if (DMA_UR3R_STA & 0x02)	//���ݶ���
	{
		DMA_UR3R_STA &= ~0x02;
	}
}
