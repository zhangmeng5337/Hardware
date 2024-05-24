/***********************************
*����4������,LED��ʾ
*3·��ѹ,3·��������
*4·���������� 4·���������
*����:xielungui
*����:2018-04-02 09:28
************************************/

#include "main.h"
#include "com.h"

void init()
{

    P1M0 = 0x03;
    P1M1 = 0x00;
    P3M0 = 0x80;
    P3M1 = 0x00;                             //������ʱ��

}
void Timer0_Isr(void) interrupt 1
{
    com_recv_proc();
}

void Timer0_Init(void)      //1΢��@11.0592MHz
{
    AUXR |= 0x80;           //��ʱ��ʱ��1Tģʽ
    TMOD &= 0xF0;           //���ö�ʱ��ģʽ
    TL0 = 0xF5;             //���ö�ʱ��ʼֵ
    TH0 = 0xFF;             //���ö�ʱ��ʼֵ
    TF0 = 0;                //���TF0��־
    TR0 = 1;                //��ʱ��0��ʼ��ʱ
    ET0 = 1;                //ʹ�ܶ�ʱ��0�ж�
}




void INT1_Isr() interrupt 2
{
    unsigned char port_num;

   
    com_recv_proc();
}
void io_init()
{
    P0M0 = 0x00;
    P0M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P5M0 &= ~0x01;
    P5M1 &= ~0x01;
}
void main()
{
    io_init();

    IT1 = 1;                                    //ʹ��INT1�½����ж�
    EX1 = 1;                                    //ʹ��INT1�ж�

    Timer0_Init();      //1΢��@11.0592MHz
    EA = 1;

    while (1)
    {
        com_tx_to_master();
        com_tx_slaver();
	    P04 = !P04; 								//���Զ˿�

    }
}



