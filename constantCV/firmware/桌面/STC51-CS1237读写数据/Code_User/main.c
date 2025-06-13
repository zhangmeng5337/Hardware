//�ڲ�ʱ�� 30MHZ
#include<STC15W.h>
#include<intrins.h>
#include<Uart_1.h>
#include<T4_Key.h>
#include<CS1237.h>     //20bit ADC


extern bit Key_1,Key_2,Key_3;//����������״̬��ȫ�ֱ��� 1��ʾ���£�ÿ���ù�֮����Ҫ�ֶ�����

//����һ�������壬���ڰ����ֽڵ� long ���ݷ��ͳ�ȥ
union
{
	unsigned char a[4];
	signed long   b;
}Read_Data;


void main(void)
{
	P0M1=0;P0M0=0;P1M1=0;P1M0=0;
	P2M1=0;P2M0=0;P3M1=0;P3M0=0;
	P4M1=0;P4M0=0;P5M1=0;P5M0=0;//�ϵ��ʼ������IO��Ϊ��ͨIO
	Init_Uart1();//��ʼ������1��9600bps
	Init_T4();//��ʼ����ʱ��T4 
	EA = 1;//�򿪵�Ƭ��ȫ���ж�
	Con_CS1237();//����CS1237оƬ��������֤��дʱ��
	Delay1ms(10);
	while(1)
	{
		
		if(Key_1)//��⵽ KEY1 ����
		{
			Key_1 = 0;
			Send_Data1(Read_CON());//����CS1237���������ݵ����� ����֤��ȡʱ��
		}
		if(Key_2)//��⵽ KEY2 ����
		{
			Key_2 = 0;
			Read_Data.b = Read_CS1237();//��ȡCS1237������
			
			Send_Data1(Read_Data.a[0]);//���Ͷ�ȡ�������ݵ����ڣ��Ӹ�8λ��ʼ����
			Send_Data1(Read_Data.a[1]);
			Send_Data1(Read_Data.a[2]);
			Send_Data1(Read_Data.a[3]);
			
		}
		if(Key_3)//��⵽ KEY3 ����
		{
			Key_3 = 0;
			Send_Str1("��С�»�ӭ�㣡 3");
		}
		
		
	}
}