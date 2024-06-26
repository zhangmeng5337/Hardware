/******************************************************************************

                  ��Ȩ���� (C), 2019, �������ĿƼ����޹�˾

 ******************************************************************************
  �� �� ��   : main.c
  �� �� ��   : V2.0
  ��    ��   : LiuYanZhou
  ��������   : 2019��10��6��
  ��������   : ��������������ʼ������ѭ������Ҫ���ܺ�����ڡ�
  �޸���ʷ   :
  1.��    ��   : 
    ��    ��   : 
    �޸�����   : 
******************************************************************************/

#include "t5los8051.h"
#include "sys.h"
#include "stdlib.h"
#include "uart.h"
#include  "led.h"
#include "save_data_dgus.h"
#include  "i2c.h"

#define Dgus_Send_Page_Time_Set 0x7000
u16 Send_Page_Time_Count = 0, Pop_Time_Count = 0xFFFF;

u8   use_sed[4]={0x12,0x13,0x23,0x1}; 


void main(){
	INIT_CPU();  //ϵͳ��ʼ��
	PORT_Init(); //IO�ڳ�ʼ��
		
	T0_Init();//��ʱ��0��ʼ��
//	T1_Init();
	T2_Init();//��ʱ��2��ʼ��
//	WDT_ON();		//�򿪿��Ź�	ι���ڶ�ʱ��T2��	 
	uart_init();// ���ڳ�ʼ��
   ws2812_writ_byte(8);	//LED��ʼ��
	data_save_init();//���ݸı��Զ������ʼ��
	Uart2_Tx_write2buff(0x78);	//����
	uart_send_str(4,use_sed,4);
	Write_Dgus(0x0084,0x01);
	while(1){
		Uart_Handle_all();
    Read_0xF00();//�����Զ��ϴ�
		data_change_sava();//���ݸı��Զ�����
		 Clock();
	}
}
