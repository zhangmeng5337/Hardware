//#include "reg51.h"
//#include "intrins.h"

//sfr     RSTCFG      =   0xff;
//#define ENLVR           0x40                    //RSTCFG.6
//#define LVD2V0          0x00                    //LVD@2.0V
//#define LVD2V4          0x01                    //LVD@2.4V
//#define LVD2V7          0x02                    //LVD@2.7V
//#define LVD3V0          0x03                    //LVD@3.0V
//sbit    ELVD        =   IE^6;
//#define LVDF            0x20                    //PCON.5
//sbit    P32         =   P3^2;
//
//sfr     P0M1        =   0x93;
//sfr     P0M0        =   0x94;
//sfr     P1M1        =   0x91;
//sfr     P1M0        =   0x92;
//sfr     P2M1        =   0x95;
//sfr     P2M0        =   0x96;
//sfr     P3M1        =   0xb1;
//sfr     P3M0        =   0xb2;
//sfr     P4M1        =   0xb3;
//sfr     P4M0        =   0xb4;
//sfr     P5M1        =   0xc9;
//sfr     P5M0        =   0xca;

void lvd_proc()
{
    if (PCON & LVDF)
    {

    sleep:   
		RSTCFG = LVD3V0;
        delay();
        PCON &= ~LVDF;
        WKTCL = 0x0f;                               //设定掉电唤醒时钟约为1秒钟
        WKTCH = 0x27;
        PCON = 0x02;                            //MCU进入掉电模式
		PCON &= ~LVDF;
		RSTCFG = LVD3V0;
		delay();

        if (PCON & LVDF)
        { 
           IAP_CONTR = IAP_CONTR & 0xef;//disable reset 
           goto  sleep;
        }
		else
		{
			IAP_CONTR = IAP_CONTR | 0x10;//reset

		}
    }

}

void pwr_init()
{
    PCON &= ~LVDF;
    RSTCFG = LVD3V0;
    IAP_CONTR = IAP_CONTR & 0xef;//disable reset


//    while (1);
}


