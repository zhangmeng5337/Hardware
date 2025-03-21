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

����4ȫ˫���жϷ�ʽ�շ�ͨѶ����

ͨ��PC��MCU��������, MCU���յ��������Զ�����DMA�ռ�.

��DMA�ռ�������ô�С�����ݺ�ͨ������4��DMA�Զ����͹��ܰѴ洢�ռ������ԭ������.

���ô��ڽ����жϽ��г�ʱ�жϣ���ʱû���յ��µ����ݣ���ʾһ�������Ѿ�������ϣ����յ�������ԭ������.

�ö�ʱ���������ʷ�����������ʹ��1Tģʽ(���ǵͲ�������12T)����ѡ��ɱ�������������ʱ��Ƶ�ʣ�����߾��ȡ�

����ʱ, ѡ��ʱ�� 22.1184MHz (�û��������޸�Ƶ��).

******************************************/

#include "..\..\comm\STC32G.h"

#include "stdio.h"
#include "intrins.h"
#include <string.h>

typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

#define MAIN_Fosc   22118400L   //������ʱ�ӣ���ȷ����115200�����ʣ�
#define Baudrate4   115200L

#define Timer0_Reload   (65536UL -(MAIN_Fosc / 1000))       //Timer 0 �ж�Ƶ��, 1000��/��

bit B_1ms;          //1ms��־
bit Tx4SendFlag;
bit	DmaTx4Flag;
bit	DmaRx4Flag;
u8  RX4_TimeOut;
u16 RX4_Cnt;    //���ռ���
u8 xdata DmaBuffer[256];

u8 xdata TemBuffer[256];

void UART4_config(u8 brt);   // ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer4��������.
void DMA_Config(void);
void UART4_DMA_Transmit(u8 *pData, u16 Size);
void UART4_DMA_Receive(u8 *pData, u16 Size);

void UartPutc(unsigned char dat)
{
    Tx4SendFlag = 1;
    S4BUF = dat; 
    while(Tx4SendFlag); 
//	while(S4TI == 0);
//	S4TI = 0;
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

	AUXR = 0x80;    //Timer0 set as 1T, 16 bits timer auto-reload, 
	TH0 = (u8)(Timer0_Reload / 256);
	TL0 = (u8)(Timer0_Reload % 256);
	ET0 = 1;    //Timer0 interrupt enable
	TR0 = 1;    //Tiner0 run
	
	UART4_config(0);    // ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer4��������.

	DMA_Config();
	EA = 1; //����ȫ���ж�

	DmaTx4Flag = 0;
	DmaRx4Flag = 0;
	printf("STC32G UART4 Test Programme!\r\n");  //UART4����һ���ַ���
	
	while (1)
	{
		if((DmaTx4Flag) && (DmaRx4Flag))
		{
			DmaTx4Flag = 0;
			DMA_UR4T_CR = 0xc0;			//bit7 1:ʹ�� UART4_DMA, bit6 1:��ʼ UART4_DMA �Զ�����
			DmaRx4Flag = 0;
			DMA_UR4R_CR = 0xa1;			//bit7 1:ʹ�� UART4_DMA, bit5 1:��ʼ UART4_DMA �Զ�����, bit0 1:��� FIFO
		}

        if(B_1ms)   //1ms��
        {
            B_1ms = 0;
            if(RX4_TimeOut > 0)     //��ʱ����
            {
                if(--RX4_TimeOut == 0)
                {
                    //�رս���DMA���´ν��յ��������´������ʼ��ַλ�ã������´ν������ݼ����������š�
                    DMA_UR4R_CR = 0x00;			//bit7 1:ʹ�� UART1_DMA, bit5 1:��ʼ UART1_DMA �Զ�����, bit0 1:��� FIFO

                    printf("\r\nUART4 Timeout!\r\n");  //UART4����һ���ַ���
                    memcpy(TemBuffer,DmaBuffer,RX4_Cnt);
                    UART4_DMA_Transmit(TemBuffer,RX4_Cnt);  //����DMA���ͻ����������ݳ��ȣ�����������
                    RX4_Cnt = 0;
                    DmaTx4Flag = 0;
                    DmaRx4Flag = 0;
                    UART4_DMA_Receive(DmaBuffer,256);  //����DMA���ջ����������ݳ��ȣ�����������
                }
            }
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
	DMA_UR4T_CFG = 0x80;		//bit7 1:Enable Interrupt
	DMA_UR4T_STA = 0x00;
	DMA_UR4T_AMT = 0xff;		//���ô������ֽ���(��8λ)��n+1
	DMA_UR4T_AMTH = 0x00;		//���ô������ֽ���(��8λ)��n+1
	DMA_UR4T_TXAH = (u8)((u16)&DmaBuffer >> 8);
	DMA_UR4T_TXAL = (u8)((u16)&DmaBuffer);
	DMA_UR4T_CR = 0xc0;			//bit7 1:ʹ�� UART4_DMA, bit6 1:��ʼ UART4_DMA �Զ�����

	DMA_UR4R_CFG = 0x80;		//bit7 1:Enable Interrupt
	DMA_UR4R_STA = 0x00;
	DMA_UR4R_AMT = 0xff;		//���ô������ֽ���(��8λ)��n+1
	DMA_UR4R_AMTH = 0x00;		//���ô������ֽ���(��8λ)��n+1
	DMA_UR4R_RXAH = (u8)((u16)&DmaBuffer >> 8);
	DMA_UR4R_RXAL = (u8)((u16)&DmaBuffer);
	DMA_UR4R_CR = 0xa1;			//bit7 1:ʹ�� UART4_DMA, bit5 1:��ʼ UART4_DMA �Զ�����, bit0 1:��� FIFO
}

void UART4_DMA_Transmit(u8 *pData, u16 Size)
{
    if(Size == 0) return;
    Size -= 1;
	DMA_UR4T_AMTH = (u8)(Size >> 8);    //���ô������ֽ���(��8λ)��n+1
	DMA_UR4T_AMT = (u8)Size;            //���ô������ֽ���(��8λ)��n+1
	DMA_UR4T_TXAH = (u8)((u16)pData >> 8);
	DMA_UR4T_TXAL = (u8)((u16)pData);
    DMA_UR4T_CR = 0xc0;     //bit7 1:ʹ�� UART4_DMA, bit6 1:��ʼ UART1_DMA �Զ�����
}

void UART4_DMA_Receive(u8 *pData, u16 Size)
{
    if(Size == 0) return;
    Size -= 1;
	DMA_UR4R_AMTH = (u8)(Size >> 8);    //���ô������ֽ���(��8λ)��n+1
	DMA_UR4R_AMT = (u8)Size;            //���ô������ֽ���(��8λ)��n+1
	DMA_UR4R_RXAH = (u8)((u16)pData >> 8);
	DMA_UR4R_RXAL = (u8)((u16)pData);
    DMA_UR4R_CR = 0xa1;     //bit7 1:ʹ�� UART4_DMA, bit5 1:��ʼ UART4_DMA �Զ�����, bit0 1:��� FIFO
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
// ����: void UART4_config(u8 brt)
// ����: UART4��ʼ��������
// ����: brt: ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer4��������.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void UART4_config(u8 brt)    // ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer4��������.
{
	if(brt == 2)
	{
		SetTimer2Baudraye((u16)(65536UL - (MAIN_Fosc / 4) / Baudrate4));
		S4CON = 0x10;       //8λ����, ʹ��Timer2�������ʷ�����, �������
	}
	else
	{
        T4R = 0;	//Timer stop
		S4CON = 0x50;       //8λ����, ʹ��Timer4�������ʷ�����, �������
		T4H = (65536UL - (MAIN_Fosc / 4) / Baudrate4) / 256;
		T4L = (65536UL - (MAIN_Fosc / 4) / Baudrate4) % 256;
        T4_CT = 0;	//Timer3 set As Timer
        T4x12 = 1;	//Timer3 set as 1T mode
        T4R = 1;	//Timer run enable
	}
    ES4  = 1;       //����UART4�ж�
    S4_S = 1;       //UART4 switch bit2 to: 0: P0.2 P0.3, 1: P5.2 P5.3

    RX4_Cnt = 0;
    RX4_TimeOut = 0;
}

//========================================================================
// ����: void timer0 (void) interrupt 1
// ����: ��ʱ��0�жϺ�����
// ����: nine.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void timer0 (void) interrupt 1
{
    B_1ms = 1;      //1ms��־
}

//========================================================================
// ����: void UART4_int (void) interrupt UART4_VECTOR
// ����: UART4�жϺ�����
// ����: nine.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void UART4_int (void) interrupt 18
{
    if(S4RI)
    {
        S4RI = 0;    //Clear Rx flag
        RX4_Cnt++;
        RX4_TimeOut = 5;
    }

    if(S4TI)
    {
        S4TI = 0;    //Clear Tx flag
        Tx4SendFlag = 0;
    }
}

//========================================================================
// ����: void UART4_DMA_Interrupt (void) interrupt 56/57
// ����: UART4 DMA�жϺ���
// ����: none.
// ����: none.
// �汾: VER1.0
// ����: 2021-5-8
// ��ע: 
//========================================================================
void UART4_DMA_Interrupt(void) interrupt 13
{
	if (DMA_UR4T_STA & 0x01)	//�������
	{
		DMA_UR4T_STA &= ~0x01;
		DmaTx4Flag = 1;
	}
	if (DMA_UR4T_STA & 0x04)	//���ݸ���
	{
		DMA_UR4T_STA &= ~0x04;
	}
	
	if (DMA_UR4R_STA & 0x01)	//�������
	{
		DMA_UR4R_STA &= ~0x01;
		DmaRx4Flag = 1;
	}
	if (DMA_UR4R_STA & 0x02)	//���ݶ���
	{
		DMA_UR4R_STA &= ~0x02;
	}
}
