/******************************************************************************

                  版权所有 (C), 2019, 北京迪文科技有限公司

 ******************************************************************************
  文 件 名   : main.c
  版 本 号   : V2.0
  作    者   : LiuYanZhou
  生成日期   : 2019年10月6日
  功能描述   : 主函数，参数初始化，主循环中主要功能函数入口。
  修改历史   :
  1.日    期   : 
    作    者   : 
    修改内容   : 
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
	INIT_CPU();  //系统初始化
	PORT_Init(); //IO口初始化
		
	T0_Init();//定时器0初始化
//	T1_Init();
	T2_Init();//定时器2初始化
//	WDT_ON();		//打开开门狗	喂狗在定时器T2中	 
	uart_init();// 串口初始化
   ws2812_writ_byte(8);	//LED初始化
	data_save_init();//数据改变自动保存初始化
	Uart2_Tx_write2buff(0x78);	//测试
	uart_send_str(4,use_sed,4);
	Write_Dgus(0x0084,0x01);
	while(1){
		Uart_Handle_all();
    Read_0xF00();//数据自动上传
		data_change_sava();//数据改变自动保存
		 Clock();
	}
}
