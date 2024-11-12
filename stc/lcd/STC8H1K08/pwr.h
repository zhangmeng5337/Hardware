//#include "reg51.h"
//#include "intrins.h"

#include "stc8h.h"
//========================================================================
// 函数: void Ext_Vcc_Det(void)
// 描述: 外部电源检测函数。
// 参数: 无.
// 返回: 无.
// 版本: V1.0, 2022-10-10
//========================================================================
extern  unsigned char result;
void pwr_init(void)
{
	EA=0;

	INTCLKO=EX2;
	EA=1;
}


void Ext_Vcc_Det(void)
{
    P35 = 0; //比较时，对外输出 0，做比较电路的地线
  //  CMPCR1 |= 0x80; //使能比较器模块
    _nop_();
    _nop_();
    _nop_();

	        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    if(CMPCR1 & 0x01) //判断是否 CMP+电平高于 CMP-，外部电源连接
    {
        CMPCR1 |= ~0x80; //使能比较器模块
        P35 = 0;
    }
    else
    {
        

        CMPCR1 &= ~0x80; //关闭比较器模块
        P35 = 1; //不比较时，对外设置为 1，I/O 口浮空，省电
        _nop_();
        _nop_();
		com_data_tx(0x11);
        sleep();
		lcd_pwr();
		//pwr_init();
        PCON = 0x02; //STC8H8K64U B 版本芯片使用内部 32K 时钟，
        //IAP_CONTR = IAP_CONTR|0x20;
         
		//EA=0;
		
		//INTCLKO=0;
		//EA=1;

        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
		result=1;
    }
}
void int2_Isr() interrupt 10
{

}

/*****************************************************************************/
//如果开启了比较器中断就需要编写对应的中断函数
void CMP_Isr() interrupt 21
{
    CMPCR1 &= ~0x40; //清中断标志
// P10 = CMPCR1 & 0x01; //中断方式读取比较器比较结果
}

// 函数: void CMP_config(void)
// 描述: 比较器初始化函数。
// 参数: 无.
// 返回: 无.
// 版本: V1.0, 2022-10-10
//========================================================================
void CMP_config(void)
{
	  P_SW2 |= 0x80;  //扩展寄存器(XFR)访问使能

	  CMPEXCFG = 0x00;
	//	CMPEXCFG |= 0x40;							//比较器DC迟滞输入选择，0:0mV; 0x40:10mV; 0x80:20mV; 0xc0:30mV
	
	//	CMPEXCFG &= ~0x04;							//P3.6为CMP-输入脚
		CMPEXCFG |= 0x04;							//内部1.19V参考电压为CMP-输入脚
	
		CMPEXCFG &= ~0x03;							//P3.7为CMP+输入脚
	//	CMPEXCFG |= 0x01;							//P5.0为CMP+输入脚
	//	CMPEXCFG |= 0x02;							//P5.1为CMP+输入脚
	//	CMPEXCFG |= 0x03;							//ADC输入脚为CMP+输入脚
	
	
		CMPCR2 = 0x00;
		CMPCR2 &= ~0x80;							//比较器正向输出
	//	CMPCR2 |= 0x80; 							//比较器反向输出
		CMPCR2 &= ~0x40;							//使能0.1us滤波
	//	CMPCR2 |= 0x40; 							//禁止0.1us滤波
	//	CMPCR2 &= ~0x3f;							//比较器结果直接输出
		CMPCR2 |= 0x10; 							//比较器结果经过16个去抖时钟后输出
	
		CMPCR1 = 0x00;
	   // CMPCR1 |= 0x30;							  //使能比较器边沿中断
	//	CMPCR1 &= ~0x20;							//禁止比较器上升沿中断
	//	CMPCR1 |= 0x20; 							//使能比较器上升沿中断
	//	CMPCR1 &= ~0x10;							//禁止比较器下降沿中断
	//	CMPCR1 |= 0x10; 							//使能比较器下降沿中断
	
	//	CMPCR1 &= ~0x02;							//禁止比较器输出
		CMPCR1 |= 0x02; 							//使能比较器输出
	
		P_SW2 &= ~0x08; 							//选择P3.4作为比较器输出脚
	//	P_SW2 |= 0x08;								//选择P4.1作为比较器输出脚
		CMPCR1 |= 0x80; 							//使能比较器模块
    //IAP_CONTR = IAP_CONTR&0xd0;

}






