/***********************************
*����4������,LED��ʾ
*3·��ѹ,3·��������
*4·���������� 4·���������
*����:xielungui
*����:2018-04-02 09:28
************************************/

#include "main.h"
#include "lcd_app.h"
#include "lcd_driver.h"
void init()
{

  P1M0 = 0x03; 
	P1M1 = 0x00; 
  P3M0 = 0x80; 
	P3M1 = 0x00;                             //������ʱ��

}
void Timer0_Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x80;			//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0xCD;				//���ö�ʱ��ʼֵ
	TH0 = 0xD4;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0 = 1;                                    //ʹ�ܶ�ʱ���ж�
  EA = 1;
}
unsigned char disp_flag;
void TM0_Isr() interrupt 1
{
	  static unsigned char tick=0;
	  if(tick<50)
			tick++;
		else
		{ 
			tick = 0;
			disp_flag=1;                                //���Զ˿�		
		}

}
//��������ʼ
main() 
{
   init();

   Ht1621_Init();                             //��ʼ��		
	 Timer0_Init();	
	//	test2();
	while(1)
	{
		 disp_proc();
    if(disp_flag ==1)
		{
		
			//disp_flag = 0;			
		}
	}
}

